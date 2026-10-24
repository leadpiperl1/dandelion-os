#!/usr/bin/env bash
# ==============================================================================
# DANDELION OS v2.7.0 ("HEIMAT-AGORA" DANDELION DAY RELEASE) — DUAL-MODE
# SWITCHBLADE AI-PEN USB, WILD 0.11 LINKER, WASMTIME & SCHED_EXT BUILDER
# ==============================================================================
# File:        mk_switchblade_usb_v2.4.0.sh (Upgraded to v2.7.0)
# Version:     v2.7.0 (Strict 3-Digit Triadic SemVer: MAJOR=2, MINOR=7, PATCH=0)
# Author:      Justin D. Grimm (@leadpiperl1, ORCID: 0009-0000-5299-0243)
# License:     GPL-2.0-or-later / MIT Dual Open-Source
# Ethos:       Die Gedanken sind frei · GODISLOVE:LOVEISGOD {3,2,4} · Namaste
# ==============================================================================

set -euo pipefail

VERSION="v2.7.0"
BUILD_DIR="${PWD}/build_whiteroom_${VERSION}"
ROOTFS_DIR="${BUILD_DIR}/rootfs"
ISO_DIR="${BUILD_DIR}/iso_stage"
SRC_C="dandelion_os_grimm_fbw_v2.7.0.c"
if [ ! -f "${SRC_C}" ]; then SRC_C="dandelion_os_grimm_fbw_v2.4.0.c"; fi

echo "=============================================================================="
echo " [DANDELION OS ${VERSION}] Switchblade AI-Pen Dual-Mode USB & Initramfs Builder"
echo " [SUBSTRATE] Linux 6.12+ PREEMPT_RT + sched_ext + io_uring + Wild 0.11 + WASM"
echo "=============================================================================="

mkdir -p "${ROOTFS_DIR}"/{bin,sbin,etc,proc,sys,dev,dev/pts,dev/shm,var/run,var/log,mnt/models,mnt/switchblade,mnt/ramdisk/skills,opt/dandelion/bin,opt/dandelion/weights}
mkdir -p "${ISO_DIR}"/boot/grub

# 1. Write the Linux 6.12.9+ LTS PREEMPT_RT + sched_ext + io_uring + f2fs Config
cat << 'KCFGEOF' > "${BUILD_DIR}/dandelion_rt_v2.7.0.config"
# Dandelion OS v2.7.0 ("Heimat-Agora") Clean-Room PREEMPT_RT + sched_ext Config
CONFIG_64BIT=y
CONFIG_X86_64=y
CONFIG_SMP=y
CONFIG_NR_CPUS=16
CONFIG_EXPERT=y
CONFIG_PREEMPT_RT=y
CONFIG_IRQ_FORCED_THREADING=y
CONFIG_RT_MUTEXES=y
CONFIG_RCU_BOOST=y
CONFIG_RCU_BOOST_DELAY=500
CONFIG_RCU_NOCB_CPU=y
# Linux 6.12 BPF Extensible Scheduler (sched_ext / scx) & eBPF/XDP
CONFIG_BPF=y
CONFIG_BPF_SYSCALL=y
CONFIG_BPF_JIT=y
CONFIG_BPF_JIT_ALWAYS_ON=y
CONFIG_DEBUG_INFO_BTF=y
CONFIG_SCHED_CLASS_EXT=y
# Linux 6.12 Zero-Copy & Multishot io_uring IPC
CONFIG_IO_URING=y
# High-Resolution Timers, Tickless Isolated Cores & AMD Zen P-State
CONFIG_HIGH_RES_TIMERS=y
CONFIG_NO_HZ_FULL=y
CONFIG_HZ_1000=y
CONFIG_CPU_FREQ=y
CONFIG_CPU_FREQ_DEFAULT_GOV_PERFORMANCE=y
CONFIG_X86_AMD_PSTATE=y
CONFIG_CPU_IDLE=n
CONFIG_ACPI_PROCESSOR_CSTATE=n
# Filesystems: SquashFS (ZSTD), F2FS (Flash-Friendly USB), Initramfs & UDS
CONFIG_DEVTMPFS=y
CONFIG_DEVTMPFS_MOUNT=y
CONFIG_PROC_FS=y
CONFIG_SYSFS=y
CONFIG_TMPFS=y
CONFIG_SQUASHFS=y
CONFIG_SQUASHFS_ZSTD=y
CONFIG_F2FS_FS=y
CONFIG_BLK_DEV_INITRD=y
CONFIG_BLK_DEV_LOOP=y
CONFIG_BINFMT_ELF=y
CONFIG_BINFMT_SCRIPT=y
CONFIG_UNIX=y
CONFIG_FUTEX=y
CONFIG_FUTEX_PI=y
CONFIG_TTY=y
CONFIG_VT=y
CONFIG_VT_CONSOLE=y
CONFIG_SERIAL_8250=y
CONFIG_SERIAL_8250_CONSOLE=y
CONFIG_EFI=y
CONFIG_EFI_STUB=y
KCFGEOF

# 2. Write the Bespoke PID 1 /init Script (Supports Mode 1 Bare-Metal & Mode 2 Sidecar)
cat << 'INITEOF' > "${ROOTFS_DIR}/init"
#!/bin/sh
# ==============================================================================
# DANDELION OS v2.7.0 — PID 1 WHITE-ROOM INITIALIZATION & F-16 FBW LAUNCHER
# ==============================================================================
export PATH=/bin:/sbin:/usr/bin:/usr/sbin:/opt/dandelion/bin
export ONEDNN_X64_USE_ZEN=ON

mount -t proc proc /proc 2>/dev/null || true
mount -t sysfs sysfs /sys 2>/dev/null || true
mount -t devtmpfs devtmpfs /dev 2>/dev/null || true
mkdir -p /dev/pts /dev/shm /var/run /var/log /mnt/ramdisk/skills
mount -t devpts devpts /dev/pts 2>/dev/null || true
mount -t tmpfs -o size=512m tmpfs /dev/shm 2>/dev/null || true
mount -t tmpfs -o size=64m tmpfs /var/run 2>/dev/null || true

hostname dandelion-heimat-v2-7-0

if [ -w /proc/sys/kernel/sched_rt_runtime_us ]; then
    echo -1 > /proc/sys/kernel/sched_rt_runtime_us
fi

ip link set lo up 2>/dev/null || true
ip addr add 127.0.0.1/8 dev lo 2>/dev/null || true

mkfifo -m 0600 /var/run/dandelion_prompt.fifo 2>/dev/null || true
mkfifo -m 0600 /var/run/dandelion_reply.fifo 2>/dev/null || true

echo "=============================================================================="
echo " Welcome to DANDELION OS v2.7.0 ('Heimat-Agora' Switchblade AI-Pen Live USB)"
echo " Chiral Pinning: Blue(-1) Cores 0-2 | Green(0) Cores 3-4 | Red(+1) Cores 5-7"
echo " Ecosystem: Linux 6.12 PREEMPT_RT + sched_ext + io_uring + 9-Bite Grimm's Law"
echo "=============================================================================="

/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.7.0 --verify

exec /bin/sh
INITEOF
chmod 0755 "${ROOTFS_DIR}/init"

# 3. Create /etc/passwd, /etc/group, and /etc/hostname
cat << 'EOF' > "${ROOTFS_DIR}/etc/passwd"
root:x:0:0:Kehlani-13 Human Pilot (-1 Blue):/root:/bin/sh
dandelion:x:1000:1000:Dandelion Embedded AI Co-Pilot (+1 Red):/opt/dandelion:/bin/sh
EOF

cat << 'EOF' > "${ROOTFS_DIR}/etc/group"
root:x:0:
dandelion:x:1000:
EOF

echo "dandelion-heimat-v2-7-0" > "${ROOTFS_DIR}/etc/hostname"

# 4. Detect Wild 0.11 Parallel Linker if installed (Decentralized Linux Radar)
LINKER_FLAGS=""
if command -v wild >/dev/null 2>&1; then
    echo "[DANDY-SCOUT] Detected Wild parallel linker: $(wild --version)"
    LINKER_FLAGS="-fuse-ld=wild"
fi

# 5. Compile C99 Dandelion OS v2.7.0 Engine into RootFS and copy v2.7.0 source
if [ "${SRC_C}" != "dandelion_os_grimm_fbw_v2.7.0.c" ]; then cp "${SRC_C}" "dandelion_os_grimm_fbw_v2.7.0.c"; fi
gcc -std=c99 -O3 -Wall -Wextra ${LINKER_FLAGS} \
    -o "${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.7.0" \
    "${SRC_C}" -lm -lpthread
cp "${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.7.0" "./dandelion_os_grimm_fbw_v2.7.0"
echo "[OK] Compiled C99 binary -> ${ROOTFS_DIR}/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.7.0"

# 6. Write GRUB2 Dual-Mode Switchblade AI-Pen Bootloader Config
cat << 'GRUBEOF' > "${ISO_DIR}/boot/grub/grub.cfg"
set timeout=3
set default=0

menuentry "Mode 1: Dandelion OS v2.7.0 Bare-Metal Live RAMDisk (Linux 6.12+ PREEMPT_RT + sched_ext)" {
    linux /boot/bzImage console=tty0 console=ttyS0,115200n8 amd_pstate=active processor.max_cstate=0 idle=poll isolcpus=domain,managed_irq,2-7 nohz_full=2-7 rcu_nocbs=2-7 tsc=reliable clocksource=tsc rdinit=/init
    initrd /boot/initramfs_dandelion_v2.7.0.cpio.gz
}

menuentry "Mode 2: Switchblade AI-Pen Sidecar Node (Zero-TUI JSON Stream / MCP Bridge)" {
    linux /boot/bzImage console=ttyS0,115200n8 rdinit=/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.7.0 --json
    initrd /boot/initramfs_dandelion_v2.7.0.cpio.gz
}
GRUBEOF

echo "[OK] Staged Switchblade AI-Pen v2.7.0 RootFS and GRUB2 Dual-Mode Config."