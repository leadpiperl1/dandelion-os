#!/usr/bin/env bash
set -euo pipefail
VERSION="v3.2.0"
BUILD_DIR="${BUILD_DIR:-/tmp/build_whiteroom_${VERSION}}"
OUT_DIR="${OUT_DIR:-./build}"
ROOTFS="${BUILD_DIR}/rootfs"

mkdir -p "${OUT_DIR}"
mkdir -p "${ROOTFS}"/{bin,sbin,etc,proc,sys,dev/pts,dev/shm,var/run,var/log,mnt/models,mnt/switchblade,mnt/ramdisk/skills,opt/dandelion/bin,opt/dandelion/weights,kernel/skills}

gcc -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror \
    "dandelion_os_grimm_fbw_${VERSION}.c" -o "${OUT_DIR}/dandelion_os_grimm_fbw_${VERSION}" -lm

cp -f "${OUT_DIR}/dandelion_os_grimm_fbw_${VERSION}" "${ROOTFS}/opt/dandelion/bin/dandelion_os_grimm_fbw_${VERSION}"

cat << 'EOF' > "${ROOTFS}/init"
#!/bin/sh
mount -t proc proc /proc
mount -t sysfs sysfs /sys
exec /opt/dandelion/bin/dandelion_os_grimm_fbw_v3.2.0 --verify
EOF
chmod 0755 "${ROOTFS}/init"
echo "root:x:0:0:root:/root:/bin/sh" > "${ROOTFS}/etc/passwd"
echo "root:x:0:" > "${ROOTFS}/etc/group"
echo "dandelion-heimat-v3-2-0" > "${ROOTFS}/etc/hostname"
dd if=/dev/zero of="${ROOTFS}/mnt/models/dandelion_b158_${VERSION}.tnpt" bs=577 count=1 status=none
