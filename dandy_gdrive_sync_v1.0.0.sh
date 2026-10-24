#!/usr/bin/env bash
# ==============================================================================
# DANDY-SYNC (v1.0.0) — ZERO-DOWNLOAD GOOGLE DRIVE -> ~/dandelion-os -> GITHUB
# ==============================================================================
# Purpose: Eliminates clicking "Download" in the Gemini chat box and never touches
#          ~/Downloads! Pulls dandelion_github_payload_*.json directly from your
#          Fedora GNOME Google Drive mount (or rclone), unpacks exact canonical
#          filenames into ~/dandelion-os, runs `make check`, and pushes to GitHub.
#
# Install once on Fedora (Professor Plum):
#   mkdir -p ~/bin && cp dandy_gdrive_sync_v1.0.0.sh ~/bin/dandy-sync && chmod +x ~/bin/dandy-sync
# ==============================================================================

set -euo pipefail

REPO_DIR="${HOME}/dandelion-os"
GITHUB_REPO="https://github.com/leadpiperl1/dandelion-os.git"

echo "=============================================================================="
echo " [DANDY-SYNC v1.0.0] Zero-Download Google Drive -> GitHub Bridge"
echo "   * Dedicated Repo : ${REPO_DIR} (Isolated from ~/Downloads)"
echo "=============================================================================="

# 1. Ensure ~/dandelion-os exists as a clean Git clone outside ~/Downloads
if [ ! -d "${REPO_DIR}/.git" ]; then
    echo "[1/4] Cloning ${GITHUB_REPO} into ${REPO_DIR}..."
    rm -rf "${REPO_DIR}"
    git clone "${GITHUB_REPO}" "${REPO_DIR}"
fi

cd "${REPO_DIR}"
git pull --ff-only origin main 2>/dev/null || true

# 2. Locate latest dandelion_github_payload_*.json on Fedora Google Drive mount or rclone
PAYLOAD_FILE="${1:-}"
if [ -z "${PAYLOAD_FILE}" ]; then
    GVFS_GDRIVE=$(find "/run/user/$(id -u)/gvfs" -maxdepth 1 -name "google-drive*" 2>/dev/null | head -n 1 || true)
    if [ -n "${GVFS_GDRIVE}" ]; then
        PAYLOAD_FILE=$(find "${GVFS_GDRIVE}" -name "dandelion_github_payload_*.json" 2>/dev/null | sort -V | tail -n 1 || true)
    fi
fi

if [ -z "${PAYLOAD_FILE}" ] && command -v rclone >/dev/null 2>&1; then
    mkdir -p /tmp/dandy_gdrive_stage
    rclone copy "gdrive:" /tmp/dandy_gdrive_stage --include "dandelion_github_payload_*.json" 2>/dev/null || true
    PAYLOAD_FILE=$(find /tmp/dandy_gdrive_stage -name "dandelion_github_payload_*.json" 2>/dev/null | sort -V | tail -n 1 || true)
fi

if [ -n "${PAYLOAD_FILE}" ] && [ -f "${PAYLOAD_FILE}" ]; then
    echo "[2/4] Unpacking exact canonical filenames fromPayload: ${PAYLOAD_FILE}"
    python3 - "${PAYLOAD_FILE}" << 'PYEOF'
import base64, json, os, subprocess, sys
from pathlib import Path

payload = json.loads(Path(sys.argv[1]).read_text(encoding="utf-8"))
for del_f in payload.get("delete_files", []):
    subprocess.run(["git", "rm", "-f", "--ignore-unmatch", del_f], check=False)
    if Path(del_f).exists():
        Path(del_f).unlink()

for fname, b64 in payload.get("files_base64", {}).items():
    p = Path(fname)
    p.write_bytes(base64.b64decode(b64))
    if fname.endswith((".sh", ".py")):
        p.chmod(0o755)
    subprocess.run(["git", "add", fname], check=True)
print(f"[OK] Unpacked and staged {len(payload.get('files_base64', {}))} canonical files ({payload.get('version')}).")
PYEOF
fi

# 3. Run the 60-second verification gate
echo "[3/4] Running 'make check' verification gate..."
make check

# 4. Commit and push to GitHub
echo "[4/4] Committing and pushing to ${GITHUB_REPO}..."
git add -A
git commit -m "Sync Dandelion OS release via dandy-sync bridge" || true
git push origin main
echo "[SUCCESS] Live on GitHub: https://github.com/leadpiperl1/dandelion-os"