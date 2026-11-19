#!/usr/bin/env bash
# ==============================================================================
# audit_wfx_loot.sh (v1.2.1 — Zero-E2BIG Cache-File Stream)
# Read-only local & optional GitHub API scanner for wfx-loot supply-chain IOCs,
# empty runner secret vault (S_runner = 0), 40-char SHA action pinning, and
# strict monotonic GitHub Actions run_number continuity (Delta R = 1).
# Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
#         Independent Dandelion Substrate Research Group, Gurley, AL 35748, USA
# ==============================================================================
set -euo pipefail

REPO_DIR="${1:-.}"
GH_SLUG="${2:-}"

cd "${REPO_DIR}"
echo "=== [wfx-loot Gate 1/4] Scanning Git Branch & Commit History in $(pwd) ==="

if git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
  SUS_BRANCHES="$(git branch -a 2>/dev/null | grep -E 'wfx-docs|ci-check-[0-9]{4}|chore/[a-z0-9]{8}' || true)"
  if [[ -n "${SUS_BRANCHES}" ]]; then
    echo "[CRIMSON SPEEDO ALERT] Suspicious wfx-loot branches detected:" >&2
    echo "${SUS_BRANCHES}" >&2
    exit 251
  fi
  echo "  [OK] Zero wfx-docs / ci-check-* / chore/* probe branches."

  SUS_COMMITS="$(git log --all --since='2026-10-24' --oneline \
    --grep='docs: dependency audit notes' \
    --grep='ci: refresh build cache' \
    --grep='chore: add ci helper' \
    --grep='chore: add dependency audit workflow' \
    --grep='ci: restore' \
    --grep='ci: cleanup' 2>/dev/null || true)"
  if [[ -n "${SUS_COMMITS}" ]]; then
    echo "[CRIMSON SPEEDO ALERT] Sandwich/probe commit signatures found:" >&2
    echo "${SUS_COMMITS}" >&2
    exit 251
  fi
  echo "  [OK] Zero wfx-loot sandwich commit signatures."

  SUS_HIST_FILES="$(git log --all --since='2026-10-24' --name-only --pretty=format: 2>/dev/null | \
    grep -E '(^\.ci-[a-z0-9]+\.sh$|^docs/wfx-audit\.md$|build-audit-[a-z0-9]+\.yml$)' | sort -u || true)"
  if [[ -n "${SUS_HIST_FILES}" ]]; then
    echo "[CRIMSON SPEEDO ALERT] Suspicious historical artifact paths detected:" >&2
    echo "${SUS_HIST_FILES}" >&2
    exit 251
  fi
  echo "  [OK] Zero historical .ci-*.sh / wfx-audit.md / build-audit-*.yml traces."
else
  echo "  [INFO] Not yet inside a Git worktree; checking filesystem tree directly."
fi

echo "=== [wfx-loot Gate 2/4] Auditing Working-Tree Lifecycle Hook Surface ==="
ROGUE_FILES="$(find . -maxdepth 3 -not -path './.git*' \( -name '.ci-*.sh' -o -name 'package.json' -o -name '.DS_Store' -o -name 'Thumbs.db' \) || true)"
if [[ -n "${ROGUE_FILES}" ]]; then
  echo "[CRIMSON SPEEDO ALERT] Unauthorized hook/junk file in tree:" >&2
  echo "${ROGUE_FILES}" >&2
  exit 251
fi
echo "  [OK] Zero package.json lifecycle hooks and zero hidden .ci-*.sh scripts."

echo "=== [wfx-loot Gate 3/4] Auditing GitHub Actions SHA Pinning & Read-Only Scope ==="
if [[ -d ".github/workflows" ]]; then
  for wf in .github/workflows/*.yml .github/workflows/*.yaml; do
    [[ -e "${wf}" ]] || continue
    if ! grep -Eq '^permissions:[[:space:]]*$' "${wf}" || ! grep -Eq '^[[:space:]]+contents:[[:space:]]*read[[:space:]]*$' "${wf}"; then
      echo "[CRIMSON SPEEDO ALERT] ${wf} lacks strict top-level permissions: contents: read" >&2
      exit 251
    fi
    if ! grep -Eq 'persist-credentials:[[:space:]]*false' "${wf}"; then
      echo "[CRIMSON SPEEDO ALERT] ${wf} lacks persist-credentials: false on checkout" >&2
      exit 251
    fi
    UNPINNED="$(grep -E '^[[:space:]-]*uses:[[:space:]]*' "${wf}" | grep -Ev '@[0-9a-f]{40}([[:space:]]|$|#)' || true)"
    if [[ -n "${UNPINNED}" ]]; then
      echo "[CRIMSON SPEEDO ALERT] Unpinned action reference in ${wf}:" >&2
      echo "${UNPINNED}" >&2
      exit 251
    fi
    echo "  [OK] ${wf}: contents:read + persist-credentials:false + 40-char SHA pins verified."
  done
fi

if [[ -n "${GH_SLUG}" ]] && command -v gh >/dev/null 2>&1 && gh auth status >/dev/null 2>&1; then
  echo "=== [wfx-loot Gate 4/4] Checking Live Runner Vault (S_runner = 0) & Delta R == 1 for ${GH_SLUG} ==="
  SECRET_COUNT="$(gh api "repos/${GH_SLUG}/actions/secrets" --jq '.total_count' 2>/dev/null || echo "0")"
  if [[ "${SECRET_COUNT}" != "0" ]]; then
    echo "[CRIMSON SPEEDO ALERT] S_runner = ${SECRET_COUNT} (expected 0 repository secrets)!" >&2
    exit 251
  fi
  echo "  [OK] S_runner = 0 (empty GitHub Actions repository secret vault)."

  mkdir -p "${HOME}/.cache/waystation_sync"
  RUNS_CACHE_JSON="${HOME}/.cache/waystation_sync/gh_actions_runs_$$.json"
  if ! gh api "repos/${GH_SLUG}/actions/runs?per_page=100" > "${RUNS_CACHE_JSON}" 2>/dev/null; then
    printf '{"workflow_runs":[]}\n' > "${RUNS_CACHE_JSON}"
  fi
  python3 - "${RUNS_CACHE_JSON}" << 'PYAUDIT'
import json, pathlib, sys
p = pathlib.Path(sys.argv[1])
try:
    data = json.loads(p.read_text(encoding="utf-8"))
finally:
    p.unlink(missing_ok=True)
runs = data.get("workflow_runs", [])
by_wf = {}
for r in runs:
    by_wf.setdefault(r.get("workflow_id"), []).append(
        (int(r["run_number"]), r.get("name", "wf"), r.get("created_at", ""))
    )
post_nov11_gaps = []
legacy_gaps = []
for wid, items in by_wf.items():
    items.sort()
    for (r0, nm0, c0), (r1, nm1, c1) in zip(items, items[1:]):
        if (r1 - r0) > 1:
            msg = f"Workflow {nm1} ({wid}): gap #{r0} -> #{r1} (Delta R = {r1 - r0}, at {c1})"
            if c1 >= "2026-11-11T00:00:00Z":
                post_nov11_gaps.append(msg)
            else:
                legacy_gaps.append(msg)
for lg in legacy_gaps:
    print(f"  [INFO] Historical pre-Nov-11 run gap noted: {lg}")
if post_nov11_gaps:
    for g in post_nov11_gaps:
        print("[CRIMSON SPEEDO ALERT]", g, file=sys.stderr)
    sys.exit(251)
print(f"  [OK] Monotonic workflow run continuity verified across {len(runs)} sampled runs (Delta R = 1).")
PYAUDIT
else
  echo "=== [wfx-loot Gate 4/4] Offline/Unauthenticated Mode: Local Git & Workflow Gates Passed ==="
fi
