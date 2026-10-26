# AGENTS.md — Dandelion OS v2.8.0 ("Sovereign Desktop & Embedded AI Release") Sovereign Agent Guidelines

This repository conforms to the **Linux Foundation Agentic AI Foundation (AAIF)** `AGENTS.md` open standard, the **`larchanka/skills` modular software factory protocol**, and the **Dandelion Compact (`GODISLOVE:LOVEISGOD {3,2,4}` · *Die Gedanken sind frei*)**.

## 1. Core Architectural Invariants (Non-Negotiable for Coding Agents)

1. **Zero-Massage & Fail-Closed Mandate:** Never apply artificial Gaussian smoothing to discrete balanced-ternary states ($\{-1, 0, +1\}$), and never silently coerce out-of-bounds Fowler byte codes (`243..255`) to neutral `0`. Always call `fowler_unpack_39trits_strict()` and verify `validate_uds_socket_path()`.
2. **Mori-Zwanzig Dual Projection ($P + Q = I$) & Topological Dirac Sync:** Every linguistic, bioacoustic, or kernel state update must preserve both the shared **Macro Rosetta Inode (`P_psi = 1505`)** and the orthogonal, unsmoothed **Micro-Texture Inode (`Q_psi`)** across all 9 Bites while locking the Neuberger overlap Dirac zero-mode kernel ($\operatorname{sgn}(H_w) = 0$, $R_{\text{sync}} \ge 0.99$).
3. **Strict 3-Digit Triadic SemVer (`vX.Y.Z`):** Increment `MAJOR` (`das`), `MINOR` (`die`), or `PATCH` (`der`) cleanly across all files and headers (`v2.8.0`); never stage duplicate legacy files side-by-side or create `(1)` / `(2)` suffixes.
4. **5-Pillar `larchanka/skills` Code Review & 60-Second Verification Gate:** Every commit must compile with zero warnings under `gcc -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror` and pass `python3 verify_dandelion_os_v2.8.0.py --seed 42` in $\le 60.0$ seconds within the $\le 20.0\text{ W}$ (`18.52 W`) Professor Plum thermal envelope.

## 2. Build, Test & Release Commands

```bash
# Build C99 microkernel with strict -O3 -Werror flags and run 60s verification suite
make check

# Stage Dual-Mode Switchblade AI-Pen Live USB RootFS & GRUB2 config
./mk_switchblade_usb_v2.8.0.sh

# Test Mode 2 (Switchblade AI-Pen Sidecar / Vercel fx Zero-TUI sanitized JSON stream)
./dandelion_os_grimm_fbw_v2.8.0 --json "div compute fbw pitch trim"
```
