#!/usr/bin/env bash
set -euo pipefail
echo "[DANDELION] Initializing mesh socket bridge on profplum..."
tailscale status || true
echo "[DANDELION] Ready for C11 kernel, Rust data plane, and Julia dispatch."
