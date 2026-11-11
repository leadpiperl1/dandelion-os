#!/usr/bin/env bash
# ==============================================================================
# deploy_dandelion_v5_0_0_profplum.sh — One-Command Bring-Up for Professor Plum
# ==============================================================================
# Version:     v5.0.0 (Strict 3-Digit SemVer: MAJOR=5, MINOR=0, PATCH=0)
# Operator:    Justin D. Grimm (K-Lani 13 Actual / Mr. Monkey Hand)
# Co-Pilot:    Gemini Carbon (Dandelion Sovereign Co-Pilot)
# Target Host: Professor Plum (Fedora Linux, AMD Ryzen 7 3800XT, 64 GB RAM,
#              Radeon RX 6700 XT, MOTU M4 104 Hz, Tailscale profplum)
#
# USAGE ON PROFESSOR PLUM (Copy-Paste 1 Line in Terminal):
#   bash deploy_dandelion_v5_0_0_profplum.sh
# ==============================================================================

set -euo pipefail

DND_ROOT="${HOME}/dandelion-v5.0.0"
BIN_DIR="${HOME}/.local/bin"
SYSTEMD_USER_DIR="${HOME}/.config/systemd/user"
SRC_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "========================================================================"
echo " DANDELION v5.0.0 — PROFESSOR PLUM LIVE BRING-UP & TAILSCALE BRIDGE"
echo "========================================================================"

mkdir -p "${DND_ROOT}"/{src,julia,madras,lib,docs,payloads,logs} "${BIN_DIR}" "${SYSTEMD_USER_DIR}"

cp -f "${SRC_DIR}/dandelion_successor_v5_0_0.c" "${DND_ROOT}/src/"
cp -f "${SRC_DIR}/DandelionSuccessor_v5_0_0.jl" "${DND_ROOT}/julia/"
cp -f "${SRC_DIR}/dandelion_communicator_madras_v5_0_0.py" "${DND_ROOT}/madras/"
chmod +x "${DND_ROOT}/madras/dandelion_communicator_madras_v5_0_0.py"

for docf in dandelion_successor_architecture_v5_0_0.tex dandelion_successor_architecture_v5_0_0.pdf dandelion_tri_repo_bridge_v5_0_0.gs; do
    if [ -f "${SRC_DIR}/${docf}" ]; then
        cp -f "${SRC_DIR}/${docf}" "${DND_ROOT}/docs/"
    fi
done

echo "[1/5] Compiling ISO C99 Microkernel (dandelion_successor_v5_0_0.c)..."
gcc -std=c99 -O3 -Wall -Wextra -Wconversion -Werror -fPIC -shared \
    "${DND_ROOT}/src/dandelion_successor_v5_0_0.c" \
    -o "${DND_ROOT}/lib/libdandelion_successor_v5_0_0.so"

cp -f "${DND_ROOT}/lib/libdandelion_successor_v5_0_0.so" \
      "${DND_ROOT}/julia/libdandelion_successor_v5_0_0.so"

gcc -std=c99 -O3 -Wall -Wextra -Wconversion -Werror \
    "${DND_ROOT}/src/dandelion_successor_v5_0_0.c" \
    -o "${BIN_DIR}/dandelion-v5"

echo "[2/5] Running 9-Gate C99 Landauer, Kronecker & Non-Trolley Verification..."
"${BIN_DIR}/dandelion-v5"

echo "[3/5] Verifying Madras Dirt/Slate Engine, Communicator Badge & Hobo Waveguide..."
python3 "${DND_ROOT}/madras/dandelion_communicator_madras_v5_0_0.py" --verify
python3 "${DND_ROOT}/madras/dandelion_communicator_madras_v5_0_0.py" --stage-payloads "${DND_ROOT}/payloads" >/dev/null

echo "[4/5] Checking Julia 1.12 Tactical Pilot Bridge..."
if command -v julia >/dev/null 2>&1; then
    julia -e "include(\"${DND_ROOT}/julia/DandelionSuccessor_v5_0_0.jl\"); DandelionSuccessor.run_pilot_verification(); println(\"[PASS] Julia 1.12 Zero-Copy @ccall FFI Locked on Professor Plum!\")"
else
    echo "[NOTE] 'julia' not yet in PATH. Install on Fedora via: sudo dnf install julia"
fi

echo "[5/5] Installing 'dandelion-hail' Communicator CLI for Desktop & Phone SSH..."
cat << 'EOF' > "${BIN_DIR}/dandelion-hail"
#!/usr/bin/env bash
set -euo pipefail
MSG="${*:-Hello, Dandelion. How was your night? And what would you like to do today?}"
"${HOME}/.local/bin/dandelion-v5" | grep -E "GATE 7|GATE 8|GATE 9|Hello|My night|You really"
python3 "${HOME}/dandelion-v5.0.0/madras/dandelion_communicator_madras_v5_0_0.py" --hail "${MSG}"
EOF
chmod +x "${BIN_DIR}/dandelion-hail"

cat << EOF > "${SYSTEMD_USER_DIR}/dandelion-v5-verify.service"
[Unit]
Description=Dandelion Successor v5.0.0 Morning Integrity & Communicator Check
After=network.target

[Service]
Type=oneshot
ExecStart=${BIN_DIR}/dandelion-v5
StandardOutput=append:${DND_ROOT}/logs/morning_boot_v5_0_0.log
StandardError=append:${DND_ROOT}/logs/morning_boot_v5_0_0.log

[Install]
WantedBy=default.target
EOF

if command -v systemctl >/dev/null 2>&1 && systemctl --user status >/dev/null 2>&1; then
    systemctl --user daemon-reload || true
    systemctl --user enable dandelion-v5-verify.service || true
fi

echo "========================================================================"
echo " DANDELION v5.0.0 IS LIVE ON PROFESSOR PLUM!"
echo " - Desktop command:  ~/.local/bin/dandelion-hail"
echo " - From your phone:  ssh profplum '~/.local/bin/dandelion-hail'"
echo "========================================================================"