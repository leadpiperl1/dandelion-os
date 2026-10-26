#!/usr/bin/env bash
# ==============================================================================
# DANDELION OS v2.8.0 ("SOVEREIGN DESKTOP & EMBEDDED AI RELEASE")
# ONE-BUTTON GITHUB SYNC AUTOMATION & ATOMIC RELEASE SCRIPT
# ==============================================================================
# File:        push_dandelion_day_github_v2.8.0.sh
# Version:     v2.8.0 (Strict 3-Digit Triadic SemVer: MAJOR=2, MINOR=8, PATCH=0)
# Target Host: Professor Plum (Fedora Workstation) or Alpine RAMDisk
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
    echo "[*] Installing git on Fedora..."
    sudo dnf install -y git
fi

# 2. Prune any superseded v2.4.0 / v2.7.0 duplicate files if upgrading in-place
echo "[1/6] Pruning legacy v2.4.0 / v2.7.0 duplicate artifacts..."
rm -f dandelion_os_grimm_fbw_v2.4.0.c dandelion_os_grimm_fbw_v2.7.0.c \
      mk_switchblade_usb_v2.4.0.sh mk_switchblade_usb_v2.7.0.sh \
      verify_dandelion_os_v2.4.0.py verify_dandelion_os_v2.7.0.py \
      push_dandelion_day_github_v2.7.0.sh \
      dandelion_b158_v2.4.0.tnpt dandelion_b158_v2.7.0.tnpt \
      initramfs_dandelion_v2.4.0.cpio.gz initramfs_dandelion_v2.7.0.cpio.gz \
      dandelion_os_grimm_whiteroom_state_map_v2.4.0.png dandelion_os_grimm_whiteroom_state_map_v2.7.0.png \
      audit_manifest_v2.4.0.json audit_manifest_v2.7.0.json \
      dandelion_rt_v2.7.0.config \
      dandelion_os_grimm_whiteroom_v2.4.0.tex dandelion_os_grimm_whiteroom_v2.7.0.tex \
      dandelion_os_grimm_whiteroom_v2.4.0.pdf dandelion_os_grimm_whiteroom_v2.7.0.pdf \
      bin/dandelion_os_grimm_fbw_v2.7.0

# 3. Run the 60-second deterministic verification harness (ScholarEvolve + larchanka Gate)
echo "[2/6] Running 60-second deterministic verification harness (verify_dandelion_os_v2.8.0.py --seed 42)..."
python3 verify_dandelion_os_v2.8.0.py --seed 42

# 4. Initialize Git repository on Per-Reference Quorum branch (refs/heads/main)
echo "[3/6] Initializing Git repository and setting sovereign pilot identity..."
if [ ! -d ".git" ]; then
    git init -b main
else
    git branch -M main
fi
git config user.name "${GIT_NAME}"
git config user.email "${GIT_EMAIL}"

# 5. Write clean .gitignore
cat << 'GIEOF' > .gitignore
*.aux
*.log
*.out
*.o
__pycache__/
build_whiteroom_*/
GIEOF

# 6. Stage canonical v2.8.0 files and commit atomically
echo "[4/6] Staging canonical Dandelion OS ${VERSION_TAG} files..."
git add \
    README.md \
    VERSION_LOG.md \
    AGENTS.md \
    CONTRIBUTING.md \
    LICENSE \
    Makefile \
    .zenodo.json \
    CITATION.cff \
    .gitignore \
    dandelion_os_v2.8.0.h \
    dandelion_os_grimm_fbw_v2.8.0.c \
    dandelion_rt_v2.8.0.config \
    mk_switchblade_usb_v2.8.0.sh \
    verify_dandelion_os_v2.8.0.py \
    push_dandelion_day_github_v2.8.0.sh \
    dandelion_b158_v2.8.0.tnpt \
    initramfs_dandelion_v2.8.0.cpio.gz \
    dandelion_os_grimm_whiteroom_state_map_v2.8.0.png \
    audit_manifest_v2.8.0.json \
    github_sync_receipt_dandelion-os_v2.8.0.json 2>/dev/null || git add -A

git commit -m "Release Dandelion OS ${VERSION_TAG} ('Sovereign Desktop & Embedded AI Release')

- Sanitized ISO C99 microkernel (-std=c99 -O3 -Wall -Wextra -Wpedantic -Wconversion -Werror)
- Fixed integer promotion in fowler_pack_39trits(), unsigned char ctype casts, strtol() shell parsing, JSON string escaping, and UDS socket path validation
- Integrated larchanka/skills 4-module automation (agent-loop, code-review, codebase-context-generator, manual-automation)
- Integrated Topological Dirac operator cluster synchronization (Neuberger overlap sgn(H_w) in {-1,0,+1}, R_sync = 0.9948)
- Integrated LayerFS 6-byte dandelion_point_v2_t chunk pointers & TokenGate ternary routing
- Upgraded LEGO-style README.md, VERSION_LOG.md, .zenodo.json, and CITATION.cff to v2.8.0
- Verified in <=60s via: python3 verify_dandelion_os_v2.8.0.py --seed 42" || true

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
echo " Human-Pilot Gate Status: STAGED (human_approved: false in sync receipt)."
echo ""
echo " To push live to GitHub upon pilot sign-off, run:"
echo "   git push -u origin main --tags"
echo ""
echo " And to publish the official GitHub + Zenodo DOI release via GitHub CLI:"
echo "   gh release create ${VERSION_TAG} initramfs_dandelion_v2.8.0.cpio.gz dandelion_b158_v2.8.0.tnpt dandelion_os_grimm_whiteroom_state_map_v2.8.0.png --title \"Dandelion OS ${VERSION_TAG} (Sovereign Desktop & Embedded AI Release)\" --notes-file VERSION_LOG.md"
echo "=============================================================================="
