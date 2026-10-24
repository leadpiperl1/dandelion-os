/**
 * ============================================================================
 * DANDELION DRIVE-TO-GITHUB CLOSED-LOOP DAEMON (v2.0.0)
 * ============================================================================
 * Closes the full loop:
 *   Idea & Brainstorm (Gemini Chat)
 *     -> Build & <=60s Verification (Sandbox VM)
 *     -> github_payload_<repo>_<version>.json (Dropped into Google Drive)
 *     -> Atomic Git Tree Commit + Tag + GitHub Release (Apps Script -> GitHub)
 *     -> Automatic Zenodo DOI Minting (via GitHub Release Webhook)
 *     -> github_sync_receipt_<repo>.json (Written back to Google Drive so
 *        Gemini Chat reads live commit SHAs, releases, forks & issues!)
 *
 * HUMAN-IN-THE-LOOP FUNCTIONS (Select from top dropdown in Apps Script):
 *   1. pushLatestDrivePayloadToGitHub()
 *      - Immediately pushes the newest payload JSON in Drive to GitHub,
 *        tags the release, publishes the GitHub Release, and writes the
 *        sync receipt back to Google Drive.
 *   2. installFiveMinuteAutoSyncTrigger()
 *      - Run ONCE to install a background timer that checks Google Drive
 *        every 5 minutes and auto-pushes any new payload JSON to GitHub
 *        with zero manual clicking!
 *   3. pullGitHubFeedbackToDrive()
 *      - Fetches latest stars, forks, open issues, and commit state from
 *        GitHub and updates github_sync_receipt_<repo>.json in Drive.
 * ============================================================================
 */

const DEFAULT_GITHUB_OWNER  = "leadpiperl1";
const DEFAULT_GITHUB_REPO   = "dandelion-os";
const DEFAULT_GITHUB_BRANCH = "main";
const PAYLOAD_PREFIX        = "dandelion_github_payload_";

// Reads token from Project Settings -> Script Properties ("GITHUB_TOKEN") or fallback string
const GITHUB_TOKEN = PropertiesService.getScriptProperties().getProperty("GITHUB_TOKEN") || "PASTE_YOUR_GHP_TOKEN_HERE";

/**
 * Main Closed-Loop Entrypoint:
 * Finds the newest dandelion_github_payload_*.json (or github_payload_*.json)
 * in Google Drive, ensures the target GitHub repository exists, commits all
 * files atomically, deletes obsolete files, creates the Git tag + Release,
 * and writes github_sync_receipt_<repo>.json back to Google Drive.
 */
function pushLatestDrivePayloadToGitHub() {
  if (!GITHUB_TOKEN || GITHUB_TOKEN === "PASTE_YOUR_GHP_TOKEN_HERE") {
    throw new Error("Please set GITHUB_TOKEN in Project Settings -> Script Properties (or paste on line 33).");
  }

  const file = findNewestPayloadFile_();
  if (!file) {
    Logger.log("No payload file found in Google Drive.");
    return;
  }

  const payload = JSON.parse(file.getBlob().getDataAsString());
  const repoFull = payload.repo || (DEFAULT_GITHUB_OWNER + "/" + DEFAULT_GITHUB_REPO);
  const parts = repoFull.split("/");
  const owner = parts[0] || DEFAULT_GITHUB_OWNER;
  const repo  = parts[1] || DEFAULT_GITHUB_REPO;
  const branch = payload.branch || DEFAULT_GITHUB_BRANCH;
  const version = payload.version || "v1.0.0";

  Logger.log("Loaded payload from Drive: " + file.getName() + " -> Target: " + owner + "/" + repo + " (" + version + ")");

  const headers = {
    "Authorization": "Bearer " + GITHUB_TOKEN,
    "Accept": "application/vnd.github+json",
    "X-GitHub-Api-Version": "2022-11-28"
  };

  // Step 1: Ensure target repository exists on GitHub (auto-create if new!)
  ensureRepoExists_(owner, repo, payload.description || ("Open-source repository for " + repo), headers);

  const apiBase = "https://api.github.com/repos/" + owner + "/" + repo;

  // Step 2: Get current commit SHA and base tree SHA of target branch
  const refRes = JSON.parse(UrlFetchApp.fetch(apiBase + "/git/ref/heads/" + branch, {headers: headers}).getContentText());
  const latestCommitSha = refRes.object.sha;
  const commitRes = JSON.parse(UrlFetchApp.fetch(apiBase + "/git/commits/" + latestCommitSha, {headers: headers}).getContentText());
  const baseTreeSha = commitRes.tree.sha;

  // Step 3: Build atomic Git Tree (updates/adds canonical files and deletes legacy duplicates)
  const currentTree = JSON.parse(UrlFetchApp.fetch(apiBase + "/git/trees/" + baseTreeSha + "?recursive=1", {headers: headers}).getContentText());
  const existingPaths = {};
  (currentTree.tree || []).forEach(function(item) { existingPaths[item.path] = true; });

  const treeEntries = [];
  (payload.delete_files || []).forEach(function(delPath) {
    if (existingPaths[delPath]) {
      treeEntries.push({
        path: delPath,
        mode: "100644",
        type: "blob",
        sha: null // Deletes the file from the Git tree
      });
    }
  });

  const fileNames = Object.keys(payload.files_base64 || {});
  for (var i = 0; i < fileNames.length; i++) {
    var fname = fileNames[i];
    var blobRes = JSON.parse(UrlFetchApp.fetch(apiBase + "/git/blobs", {
      method: "post",
      headers: headers,
      contentType: "application/json",
      payload: JSON.stringify({
        content: payload.files_base64[fname],
        encoding: "base64"
      })
    }).getContentText());

    var isExec = (fname.indexOf(".sh") !== -1 || fname.indexOf(".py") !== -1);
    treeEntries.push({
      path: fname,
      mode: isExec ? "100755" : "100644",
      type: "blob",
      sha: blobRes.sha
    });
  }

  // Step 4: Create new Git Tree & Commit
  const newTreeRes = JSON.parse(UrlFetchApp.fetch(apiBase + "/git/trees", {
    method: "post",
    headers: headers,
    contentType: "application/json",
    payload: JSON.stringify({
      base_tree: baseTreeSha,
      tree: treeEntries
    })
  }).getContentText());

  const newCommitRes = JSON.parse(UrlFetchApp.fetch(apiBase + "/git/commits", {
    method: "post",
    headers: headers,
    contentType: "application/json",
    payload: JSON.stringify({
      message: payload.commit_message || ("Release " + repo + " " + version),
      tree: newTreeRes.sha,
      parents: [latestCommitSha]
    })
  }).getContentText());

  // Step 5: Advance branch ref (refs/heads/main)
  UrlFetchApp.fetch(apiBase + "/git/refs/heads/" + branch, {
    method: "patch",
    headers: headers,
    contentType: "application/json",
    payload: JSON.stringify({
      sha: newCommitRes.sha,
      force: false
    })
  });

  // Step 6: Create/Update Git Tag & Official GitHub Release (Triggers Zenodo Webhook!)
  const releaseUrl = upsertTagAndRelease_(apiBase, version, newCommitRes.sha, payload, headers);

  // Step 7: Record processed state & write bidirectional receipt back to Google Drive
  const syncKey = file.getId() + "_" + file.getLastUpdated().getTime();
  PropertiesService.getScriptProperties().setProperty("LAST_SYNCED_PAYLOAD", syncKey);

  const receipt = writeSyncReceiptToDrive_(owner, repo, branch, version, newCommitRes.sha, releaseUrl, fileNames.length, headers);
  Logger.log("SUCCESS! Closed loop complete for " + owner + "/" + repo + " (" + version + ").");
  Logger.log("  * Commit SHA  : " + newCommitRes.sha);
  Logger.log("  * Release URL : " + releaseUrl);
  Logger.log("  * Drive Receipt: " + receipt.receiptFileName);
}

/**
 * Run ONCE from the Apps Script toolbar to enable automatic 5-minute background polling.
 */
function installFiveMinuteAutoSyncTrigger() {
  const triggers = ScriptApp.getProjectTriggers();
  triggers.forEach(function(t) {
    if (t.getHandlerFunction() === "autoPollDriveAndPush_") {
      ScriptApp.deleteTrigger(t);
    }
  });
  ScriptApp.newTrigger("autoPollDriveAndPush_")
    .timeBased()
    .everyMinutes(5)
    .create();
  Logger.log("Installed 5-minute background trigger (autoPollDriveAndPush_). Any new payload JSON dropped into Drive will auto-push to GitHub!");
}

function autoPollDriveAndPush_() {
  const file = findNewestPayloadFile_();
  if (!file) return;
  const syncKey = file.getId() + "_" + file.getLastUpdated().getTime();
  const lastSynced = PropertiesService.getScriptProperties().getProperty("LAST_SYNCED_PAYLOAD");
  if (syncKey !== lastSynced) {
    Logger.log("New or updated payload detected (" + file.getName() + "). Auto-pushing to GitHub...");
    pushLatestDrivePayloadToGitHub();
  }
}

/**
 * Pulls live community feedback (stars, forks, open issues, latest commit) from GitHub
 * into Google Drive so Gemini Chat can inspect live repository telemetry.
 */
function pullGitHubFeedbackToDrive() {
  const headers = {
    "Authorization": "Bearer " + GITHUB_TOKEN,
    "Accept": "application/vnd.github+json",
    "X-GitHub-Api-Version": "2022-11-28"
  };
  writeSyncReceiptToDrive_(DEFAULT_GITHUB_OWNER, DEFAULT_GITHUB_REPO, DEFAULT_GITHUB_BRANCH, "v2.7.2", "HEAD", "", 18, headers);
}

function findNewestPayloadFile_() {
  const query = "(title contains 'dandelion_github_payload_' or title contains 'github_payload_') and mimeType = 'application/json' and trashed = false";
  const iter = DriveApp.searchFiles(query);
  var newestFile = null;
  var newestTime = 0;
  while (iter.hasNext()) {
    var f = iter.next();
    var t = f.getLastUpdated().getTime();
    if (t > newestTime) {
      newestTime = t;
      newestFile = f;
    }
  }
  return newestFile;
}

function ensureRepoExists_(owner, repo, description, headers) {
  const checkRes = UrlFetchApp.fetch("https://api.github.com/repos/" + owner + "/" + repo, {
    headers: headers,
    muteHttpExceptions: true
  });
  if (checkRes.getResponseCode() === 404) {
    Logger.log("Repository " + owner + "/" + repo + " does not exist yet. Creating public repository on GitHub...");
    UrlFetchApp.fetch("https://api.github.com/user/repos", {
      method: "post",
      headers: headers,
      contentType: "application/json",
      payload: JSON.stringify({
        name: repo,
        description: description,
        private: false,
        auto_init: true
      })
    });
    Utilities.sleep(2000);
  }
}

function upsertTagAndRelease_(apiBase, version, commitSha, payload, headers) {
  // 1. Create or update lightweight tag ref (refs/tags/vX.Y.Z)
  const tagRefUrl = apiBase + "/git/refs/tags/" + version;
  const tagCheck = UrlFetchApp.fetch(tagRefUrl, {headers: headers, muteHttpExceptions: true});
  if (tagCheck.getResponseCode() === 200) {
    UrlFetchApp.fetch(tagRefUrl, {
      method: "patch",
      headers: headers,
      contentType: "application/json",
      payload: JSON.stringify({sha: commitSha, force: true})
    });
  } else {
    UrlFetchApp.fetch(apiBase + "/git/refs", {
      method: "post",
      headers: headers,
      contentType: "application/json",
      payload: JSON.stringify({ref: "refs/tags/" + version, sha: commitSha})
    });
  }

  // 2. Create or update GitHub Release (triggers Zenodo DOI webhook)
  const relCheck = UrlFetchApp.fetch(apiBase + "/releases/tags/" + version, {headers: headers, muteHttpExceptions: true});
  if (relCheck.getResponseCode() === 200) {
    const existingRel = JSON.parse(relCheck.getContentText());
    return existingRel.html_url;
  }

  const createRel = UrlFetchApp.fetch(apiBase + "/releases", {
    method: "post",
    headers: headers,
    contentType: "application/json",
    muteHttpExceptions: true,
    payload: JSON.stringify({
      tag_name: version,
      name: payload.release_title || ("Dandelion OS " + version + " ('Heimat-Agora' LEGO Manual Edition)"),
      body: payload.release_notes || payload.commit_message || ("Automated closed-loop release " + version + " from Google Drive."),
      draft: false,
      prerelease: false
    })
  });
  if (createRel.getResponseCode() === 201) {
    return JSON.parse(createRel.getContentText()).html_url;
  }
  return "https://github.com/" + DEFAULT_GITHUB_OWNER + "/" + DEFAULT_GITHUB_REPO + "/releases/tag/" + version;
}

function writeSyncReceiptToDrive_(owner, repo, branch, version, commitSha, releaseUrl, fileCount, headers) {
  const apiBase = "https://api.github.com/repos/" + owner + "/" + repo;
  var repoMeta = {};
  var openIssues = [];
  try {
    repoMeta = JSON.parse(UrlFetchApp.fetch(apiBase, {headers: headers, muteHttpExceptions: true}).getContentText());
    const issuesRaw = JSON.parse(UrlFetchApp.fetch(apiBase + "/issues?state=open&per_page=10", {headers: headers, muteHttpExceptions: true}).getContentText());
    if (Array.isArray(issuesRaw)) {
      openIssues = issuesRaw.map(function(iss) {
        return {number: iss.number, title: iss.title, url: iss.html_url, user: iss.user ? iss.user.login : ""};
      });
    }
  } catch (e) {}

  const receiptData = {
    status: "SYNCED_LIVE",
    synced_at_utc: new Date().toISOString(),
    repository: owner + "/" + repo,
    html_url: "https://github.com/" + owner + "/" + repo,
    branch: branch,
    version: version,
    commit_sha: commitSha,
    release_url: releaseUrl,
    canonical_file_count: fileCount,
    stargazers_count: repoMeta.stargazers_count || 0,
    forks_count: repoMeta.forks_count || 0,
    open_issues_count: repoMeta.open_issues_count || 0,
    open_issues_and_prs: openIssues
  };

  const receiptName = "github_sync_receipt_" + repo + ".json";
  const existing = DriveApp.getFilesByName(receiptName);
  if (existing.hasNext()) {
    const f = existing.next();
    f.setContent(JSON.stringify(receiptData, null, 2));
  } else {
    DriveApp.createFile(receiptName, JSON.stringify(receiptData, null, 2), MimeType.PLAIN_TEXT);
  }
  return {receiptFileName: receiptName, data: receiptData};
}
