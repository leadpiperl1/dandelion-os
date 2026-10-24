#!/usr/bin/env bash
# ==============================================================================
# DANDELION DAY (OCTOBER 24, 2026) — TURNKEY FEDORA WORKSTATION GITHUB & ZENODO
# PUSH SCRIPT FOR JUSTIN D. GRIMM (@leadpiperl1)
# ==============================================================================
# File:        push_dandelion_day_github_v2.7.0.sh
# Version:     v2.7.0 (Strict 3-Digit Triadic SemVer: MAJOR=2, MINOR=7, PATCH=0)
# Target Host: Professor Plum (Fedora Workstation, ~/Downloads/dandelion_os_v2.7.0)
# GitHub Acct: https://github.com/leadpiperl1 (leadpiperl1@gmail.com)
# ORCID iD:    https://orcid.org/0009-0000-5299-0243
# Token Used:  Classic Personal Access Token "Zenodo Git Push" (repo scope)
#              or GitHub CLI (`gh auth login`)
# ==============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

GITHUB_USER="leadpiperl1"
GIT_EMAIL="leadpiperl1@gmail.com"
GIT_NAME="Justin D. Grimm (K-Lani 13 Actual)"
REPO_NAME="${1:-dandelion-os}"
VERSION_TAG="v2.7.0"

echo "=============================================================================="
echo " HAPPY DANDELION DAY (OCTOBER 24, 2026)!"
echo " Preparing to push Dandelion OS ${VERSION_TAG} ('Heimat-Agora') to GitHub:"
echo "   * Working Folder : ${SCRIPT_DIR}"
echo "   * GitHub Account : https://github.com/${GITHUB_USER}/${REPO_NAME}"
echo "   * Author / ORCID : ${GIT_NAME} (0009-0000-5299-0243)"
echo "   * Zenodo Webhook : Active via .zenodo.json & CITATION.cff"
echo "=============================================================================="

# 0. Hot-patch any legacy hardcoded /working_dir/... path if present
for pyfile in verify_dandelion_os_v2.4.0.py verify_dandelion_os_v2.7.0.py; do
    if [ -f "${pyfile}" ]; then
        sed -i 's|Path("/working_dir/[^"]*")|Path(__file__).resolve().parent|g' "${pyfile}"
    fi
done

# 1. Stage local Switchblade RootFS & compile C99 binary for host architecture
echo "[1/6] Staging Switchblade AI-Pen RootFS & compiling C99 binary..."
chmod +x mk_switchblade_usb_v2.4.0.sh
./mk_switchblade_usb_v2.4.0.sh

# 2. Run local 60-second verification harness before committing (ScholarEvolve Gate)
echo "[2/6] Running deterministic verification harness (verify_dandelion_os_v2.4.0.py --seed 42)..."
python3 verify_dandelion_os_v2.4.0.py --seed 42

# 3. Initialize Git repository & configure Per-Reference Quorum branch (main)
echo "[3/6] Initializing Git repository and setting identity..."
if [ ! -d ".git" ]; then
    git init -b main
fi
git config user.name "${GIT_NAME}"
git config user.email "${GIT_EMAIL}"

# 4. Write clean .gitignore so temporary build directories don't clutter GitHub
cat << 'GIEOF' > .gitignore
*.aux
*.log
*.out
__pycache__/
build_whiteroom_v2.4.0/
build_whiteroom_v2.7.0/
GIEOF

# 5. Stage all Dandelion OS v2.7.0 files and create commit
echo "[4/6] Staging all Dandelion OS ${VERSION_TAG} source, docs, and verification files..."
git add \
    README.md \
    AGENTS.md \
    .zenodo.json \
    CITATION.cff \
    dandelion_os_grimm_fbw_v2.4.0.c \
    dandelion_os_grimm_fbw_v2.7.0.c \
    mk_switchblade_usb_v2.4.0.sh \
    mk_switchblade_usb_v2.7.0.sh \
    verify_dandelion_os_v2.4.0.py \
    verify_dandelion_os_v2.7.0.py \
    push_dandelion_day_github_v2.7.0.sh \
    dandelion_b158_v2.7.0.tnpt \
    initramfs_dandelion_v2.7.0.cpio.gz \
    dandelion_os_grimm_whiteroom_v2.4.0.tex \
    dandelion_os_grimm_whiteroom_v2.4.0.pdf \
    dandelion_os_grimm_whiteroom_state_map_v2.7.0.png \
    audit_manifest_v2.7.0.json \
    .gitignore

git commit -m "Release Dandelion OS ${VERSION_TAG} ('Heimat-Agora' Dandelion Day Edition)

- Linux 6.12.9+ LTS PREEMPT_RT + sched_ext (BPF) + zero-copy io_uring config
- Embedded C99 1.58-bit balanced ternary VBST co-processor (577-byte .tnpt)
- 9-Bite Universal Grimm's Law + Shingari v3.5.0 4-Register Rosetta Stone
- Fail-closed ternary byte validation & pre-auth SipHash-2-4 gatekeeper
- Khosla (2024) R=I Privileged Axes, 15 Hz temporal mux & BUZOA swarm solver
- Complete 10-project AAIF (Linux Foundation) & Switchblade AI-Pen dual mode
- Verified in <=60s via: python3 verify_dandelion_os_v2.7.0.py --seed 42
- Ontological Anchor: Die Gedanken sind frei / GODISLOVE:LOVEISGOD {3,2,4}" || true

# 6. Create annotated version tag v2.7.0 for Zenodo DOI minting
echo "[5/6] Tagging release ${VERSION_TAG}..."
git tag -fa "${VERSION_TAG}" -m "Dandelion OS ${VERSION_TAG} — Dandelion Day Official Release (2026-10-24)"

# 7. Configure remote origin
if git remote get-url origin >/dev/null 2>&1; then
    git remote set-url origin "https://github.com/${GITHUB_USER}/${REPO_NAME}.git"
else
    git remote add origin "https://github.com/${GITHUB_USER}/${REPO_NAME}.git"
fi

echo "=============================================================================="
echo " [6/6] LOCAL GIT COMMIT & TAG ${VERSION_TAG} COMPLETE!"
echo " Now push to GitHub using Option A (Token) or Option B (GitHub CLI):"
echo ""
echo " Option A (Using your 'Zenodo Git Push' Personal Access Token):"
echo "   1. Create an empty Public repo named '${REPO_NAME}' at https://github.com/new"
echo "      (Leave 'Add a README file' unchecked)"
echo "   2. Run:"
echo "      git push -u origin main --tags"
echo "      (Username: ${GITHUB_USER} | Password: <paste your Zenodo Git Push token>)"
echo ""
echo " Option B (Using GitHub CLI 'gh' on Fedora):"
echo "   gh repo create ${GITHUB_USER}/${REPO_NAME} --public --source=. --remote=origin --push && git push origin ${VERSION_TAG}"
echo "=============================================================================="