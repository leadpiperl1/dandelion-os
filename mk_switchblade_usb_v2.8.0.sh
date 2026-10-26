#!/usr/bin/env bash
# ==============================================================================
# DANDELION OS v2.8.0 ("SOVEREIGN DESKTOP & EMBEDDED AI RELEASE") — DUAL-MODE
# SWITCHBLADE AI-PEN USB, WILD 0.11 LINKER, LAYERFS & SCHED_EXT BUILDER
# ==============================================================================
# File:        mk_switchblade_usb_v2.8.0.sh
# Version:     v2.8.0 (Strict 3-Digit Triadic SemVer: MAJOR=2, MINOR=8, PATCH=0)
# Author:      Justin D. Grimm (@leadpiperl1, ORCID: 0009-0000-5299-0243)
# License:     GPL-2.0-or-later / MIT Dual Open-Source
# Ethos:       Die Gedanken sind frei · GODISLOVE:LOVEISGOD {3,2,4} · Namaste
# ==============================================================================

set -euo pipefail

VERSION="v2.8.0"
BUILD_DIR="${PWD}/build_whiteroom_${VERSION}"
ROOTFS_DIR="${BUILD_DIR}/rootfs"
ISO_DIR="${BUILD_DIR}/iso_stage"
SRC_C="dandelion_os_grimm_fbw_${VERSION}.c"

echo "=============================================================================="
echo " [DANDELION OS ${VERSION}] Switchblade AI-Pen Dual-Mode USB & Initramfs Builder"
echo " [CODENAME]  Sovereign Desktop & Embedded AI Release"
echo " [SUBSTRATE] Linux 6.12+ PREEMPT_RT + sched_ext + LayerFS + Dirac Sync + VBST"
echo "=============================================================================="

mkdir -p "${ROOTFS_DIR}"/{bin,sbin,etc,proc,sys,dev,dev/pts,dev/shm,var/run,var/log,mnt/models,mnt/switchblade,mnt/ramdisk/skills,opt/dandelion/bin,opt/dandelion/weights,kernel/skills}
mkdir -p "${ISO_DIR}"/boot/grub
mkdir -p "./bin"

# 1. Copy the Linux 6.12.9+ LTS PREEMPT_RT + sched_ext + LayerFS Config
cp -f "dandelion_rt_${VERSION}.config" "${BUILD_DIR}/dandelion_rt_${VERSION}.config"

# 2. Write the Bespoke PID 1 /init Script (Supports Mode 1 Bare-Metal & Mode 2 Sidecar)
cat << 'INITEOF' > "${ROOTFS_DIR}/init"
#!/bin/sh
# ==============================================================================
# DANDELION OS v2.8.0 — PID 1 WHITE-ROOM INITIALIZATION & F-16 FBW LAUNCHER
# ==============================================================================
export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/opt/dandelion/bin
export ONEDNN_X64_USE_ZEN=ON

mount -t proc proc /proc 2>/dev/null || true
mount -t sysfs sysfs /sys 2>/dev/null || true
mount -t devtmpfs devtmpfs /dev 2>/dev/null || true
mkdir -p /dev/pts /dev/shm /var/run /var/log /mnt/ramdisk/skills /kernel/skills
mount -t devpts devpts /dev/pts 2>/dev/null || true
mount -t tmpfs -o size=512m tmpfs /dev/shm 2>/dev/null || true
mount -t tmpfs -o size=64m tmpfs /var/run 2>/dev/null || true

hostname dandelion-heimat-v2-8-0

if [ -w /proc/sys/kernel/sched_rt_runtime_us ]; then
    echo -1 > /proc/sys/kernel/sched_rt_runtime_us
fi

ip link set lo up 2>/dev/null || true
ip addr add 127.0.0.1/8 dev lo 2>/dev/null || true

mkfifo -m 0600 /var/run/dandelion_prompt.fifo 2>/dev/null || true
mkfifo -m 0600 /var/run/dandelion_reply.fifo 2>/dev/null || true

echo "=============================================================================="
echo " Welcome to DANDELION OS v2.8.0 ('Sovereign Desktop & Embedded AI Release')"
echo " Chiral Pinning: Blue(-1) Cores 0-2 | Green(0) Cores 3-4 | Red(+1) Cores 5-7"
echo " Ecosystem: Linux 6.12 PREEMPT_RT + sched_ext + LayerFS + Dirac Cluster Sync"
echo "=============================================================================="

/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.8.0 --verify

exec /bin/sh
INITEOF
chmod 0755 "${ROOTFS_DIR}/init"

# 3. Create /etc/passwd, /etc/group, and /etc/hostname
cat << 'EOF' > "${ROOTFS_DIR}/etc/passwd"
root:x:0:0:K-Lani 13 Actual Human Pilot (-1 Blue):/root:/bin/sh
dandelion:x:1000:1000:Dandelion Embedded AI Co-Pilot (+1 Red):/opt/dandelion:/bin/sh
EOF

cat << 'EOF' > "${ROOTFS_DIR}/etc/group"
root:x:0:
dandelion:x:1000:
EOF

echo "dandelion-heimat-v2-8-0" > "${ROOTFS_DIR}/etc/hostname"

# 4. Detect Wild 0.11 Parallel Linker if installed
LINKER_FLAGS=""
if command -v wild >/dev/null 2>&1; then
    echo "[DANDY-SCOUT] Detected Wild parallel linker: $(wild --version)"
    LINKER_FLAGS="-fuse-ld=wild"
fi

# 5. Compile C99 Dandelion OS v2.8.0 Engine into RootFS with strict ISO C99 flags
gcc -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror ${LINKER_FLAGS} \
    -o "${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_${VERSION}" \
    "${SRC_C}" -lm -lpthread
cp -f "${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_${VERSION}" "./dandelion_os_grimm_fbw_${VERSION}"
cp -f "${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_${VERSION}" "./bin/dandelion_os_grimm_fbw_${VERSION}"
echo "[OK] Compiled C99 binary -> ${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_${VERSION}"

# 6. Write GRUB2 Dual-Mode Switchblade AI-Pen Bootloader Config
cat << 'GRUBEOF' > "${ISO_DIR}/boot/grub/grub.cfg"
set timeout=3
set default=0

menuentry "Mode 1: Dandelion OS v2.8.0 Sovereign Desktop & Live RAMDisk (Linux 6.12+ PREEMPT_RT + sched_ext)" {
    linux /boot/bzImage console=tty0 console=ttyS0,115200n8 amd_pstate=active processor.max_cstate=0 idle=poll isolcpus=domain,managed_irq,2-7 nohz_full=2-7 rcu_nocbs=2-7 tsc=reliable clocksource=tsc rdinit=/init
    initrd /boot/initramfs_dandelion_v2.8.0.cpio.gz
}

menuentry "Mode 2: Switchblade AI-Pen Sidecar Node (Zero-TUI JSON Stream / MCP & larchanka Bridge)" {
    linux /boot/bzImage console=ttyS0,115200n8 rdinit=/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.8.0 --json
    initrd /boot/initramfs_dandelion_v2.8.0.cpio.gz
}
GRUBEOF

echo "[OK] Staged Switchblade AI-Pen ${VERSION} RootFS and GRUB2 Dual-Mode Config."
