#!/usr/bin/env bash
# ==============================================================================
# DANDELION OS v2.8.0 ("SOVEREIGN DESKTOP & EMBEDDED AI RELEASE")
# ONE-BUTTON GITHUB SYNC AUTOMATION & ATOMIC RELEASE SCRIPT
# ==============================================================================
# File:        push_dandelion_day_github_v2.8.0.sh
# Version:     v2.8.0 (Strict 3-Digit Triadic SemVer: MAJOR=2, MINOR=8, PATCH=0)
# Target Host: Professor Plum (Fedora Workstation), Ubuntu CI, or Alpine RAMDisk
# GitHub Repo: https://github.com/leadpiperl1/dandelion-os
# Author:      Justin D. Grimm (K-Lani 13 Actual, ORCID: 0009-0000-5299-0243)
# ==============================================================================

set -euo pipefail

GITHUB_USER="leadpiperl1"
GIT_EMAIL="leadpiperl1@gmail.com"
GIT_NAME="Justin D. Grimm (K-Lani 13 Actual)"
REPO_NAME="${1:-dandelion-os}"
VERSION_TAG="v2.8.0"

echo "=============================================================================="
echo " DANDELION OS ${VERSION_TAG} ('Sovereign Desktop & Embedded AI Release')"
echo " One-Button GitHub Sync & Atomic Release Preparation:"
echo "   * Target Repo   : https://github.com/${GITHUB_USER}/${REPO_NAME}"
echo "   * Author/ORCID  : ${GIT_NAME} (0009-0000-5299-0243)"
echo "   * Zenodo Hook   : Active via .zenodo.json & CITATION.cff"
echo "=============================================================================="

# 1. Verify git is installed
if ! command -v git >/dev/null 2>&1; then
    echo "[*] Installing git..."
    sudo dnf install -y git || sudo apt-get update && sudo apt-get install -y git
fi

# 2. Prune any superseded v2.4.0 / v2.7.0 duplicate files if upgrading in-place
echo "[1/6] Pruning legacy v2.4.0 / v2.7.0 duplicate artifacts..."
rm -f dandelion_os_grimm_fbw_v2.4.0.c dandelion_os_grimm_fbw_v2.7.0.c       mk_switchblade_usb_v2.4.0.sh mk_switchblade_usb_v2.7.0.sh       verify_dandelion_os_v2.4.0.py verify_dandelion_os_v2.7.0.py       push_dandelion_day_github_v2.7.0.sh       dandelion_b158_v2.4.0.tnpt dandelion_b158_v2.7.0.tnpt       initramfs_dandelion_v2.4.0.cpio.gz initramfs_dandelion_v2.7.0.cpio.gz       dandelion_os_grimm_whiteroom_state_map_v2.4.0.png dandelion_os_grimm_whiteroom_state_map_v2.7.0.png       audit_manifest_v2.4.0.json audit_manifest_v2.7.0.json       dandelion_rt_v2.7.0.config       dandelion_os_grimm_whiteroom_v2.4.0.tex dandelion_os_grimm_whiteroom_v2.7.0.tex       dandelion_os_grimm_whiteroom_v2.4.0.pdf dandelion_os_grimm_whiteroom_v2.7.0.pdf       bin/dandelion_os_grimm_fbw_v2.7.0

# 3. Run the 60-second deterministic verification harness (Strict mode)
echo "[2/6] Running 60-second deterministic verification harness (verify_dandelion_os_v2.8.0.py --seed 42 --mode strict)..."
python3 verify_dandelion_os_v2.8.0.py --seed 42 --mode strict

# 4. Initialize Git repository on Per-Reference Quorum branch (refs/heads/main)
echo "[3/6] Initializing Git repository and setting sovereign pilot identity..."
if [ ! -d ".git" ]; then
    git init -b main
else
    if [ "$(git branch --show-current 2>/dev/null)" != "main" ]; then
        git branch -M main
    fi
fi
git config user.name "${GIT_NAME}"
git config user.email "${GIT_EMAIL}"

# 5. Write clean .gitignore (Copilot Point 7: Keep build/ and temporary artifacts out of source tree)
cat << 'GIEOF' > .gitignore
build/
out/
bin/
build_whiteroom_*/
__pycache__/
.venv/
*.aux
*.log
*.out
*.o
GIEOF

# 6. Stage canonical v2.8.0 files and commit atomically
echo "[4/6] Staging canonical Dandelion OS ${VERSION_TAG} files..."
git add -f     README.md     VERSION_LOG.md     AGENTS.md     CONTRIBUTING.md     LICENSE     Makefile     requirements.txt     .github/workflows/ci.yml     .zenodo.json     CITATION.cff     .gitignore     dandelion_os_v2.8.0.h     dandelion_os_grimm_fbw_v2.8.0.c     dandelion_rt_v2.8.0.config     mk_switchblade_usb_v2.8.0.sh     verify_dandelion_os_v2.8.0.py     push_dandelion_day_github_v2.8.0.sh     dandelion_b158_v2.8.0.tnpt     initramfs_dandelion_v2.8.0.cpio.gz     dandelion_os_grimm_whiteroom_state_map_v2.8.0.png     audit_manifest_v2.8.0.json     github_sync_receipt_dandelion-os_v2.8.0.json 2>/dev/null || git add -A

git commit -m "Release Dandelion OS ${VERSION_TAG} ('Sovereign Desktop & Embedded AI Release')

- Incorporated all 10 GitHub Copilot build portability, CI, and architecture recommendations
- Added .github/workflows/ci.yml (GCC & Clang matrix on ubuntu-latest) and pinned requirements.txt
- Centralized all magic constants, socket namespaces, and 9-Bite inodes into dandelion_os_v2.8.0.h
- Gated Wild linker behind explicit opt-in USE_WILD_LINKER=0 and directed build outputs to build/
- Added portable vs. strict verification modes (--mode portable / --mode strict)
- Documented Supported Environments Matrix, Verified vs. Experimental Scope, and Known Limitations" || true

# 7. Create annotated tag v2.8.0
echo "[5/6] Creating annotated release tag ${VERSION_TAG}..."
git tag -fa "${VERSION_TAG}" -m "Dandelion OS ${VERSION_TAG} — Sovereign Desktop & Embedded AI Release (2026-10-26)"

# 8. Configure remote origin
echo "[6/6] Configuring GitHub remote https://github.com/${GITHUB_USER}/${REPO_NAME}.git ..."
if git remote get-url origin >/dev/null 2>&1; then
    git remote set-url origin "https://github.com/${GITHUB_USER}/${REPO_NAME}.git"
else
    git remote add origin "https://github.com/${GITHUB_USER}/${REPO_NAME}.git"
fi

echo "=============================================================================="
echo " [READY] Local Git tree and tag ${VERSION_TAG} are locked and verified!"
echo " To push live to GitHub upon pilot sign-off, run:"
echo "   git push -u origin main --tags"
echo ""
echo " And to publish the official GitHub + Zenodo DOI release via GitHub CLI:"
echo "   gh release create ${VERSION_TAG} initramfs_dandelion_v2.8.0.cpio.gz dandelion_b158_v2.8.0.tnpt dandelion_os_grimm_whiteroom_state_map_v2.8.0.png --title \"Dandelion OS ${VERSION_TAG} (Sovereign Desktop & Embedded AI Release)\" --notes-file VERSION_LOG.md"
echo "=============================================================================="
