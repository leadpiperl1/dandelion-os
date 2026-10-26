# Dandelion OS v2.8.0 ("Sovereign Desktop & Embedded AI Release")

[![Version: v2.8.0](https://img.shields.io/badge/Version-v2.8.0-15803D?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/releases/tag/v2.8.0)
[![Kernel: Linux 6.12+ PREEMPT_RT + sched_ext](https://img.shields.io/badge/Linux_6.12+-PREEMPT__RT_%2B_sched__ext-1E40AF?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_rt_v2.8.0.config)
[![AI Engine: 1.58-Bit Ternary VBST + Dirac Sync](https://img.shields.io/badge/1.58--Bit_Ternary-VBST_%2B_Dirac_Sync-B91C1C?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_os_grimm_fbw_v2.8.0.c)
[![ORCID: 0009-0000-5299-0243](https://img.shields.io/badge/ORCID-0009--0000--5299--0243-D97706?style=for-the-badge)](https://orcid.org/0009-0000-5299-0243)
[![DOI: 10.5281/zenodo.21255229](https://img.shields.io/badge/Zenodo_DOI-10.5281%2Fzenodo.21255229-0F172A?style=for-the-badge)](https://doi.org/10.5281/zenodo.21255229)

![Dandelion OS v2.8.0 Blueprint & Telemetry State Map](https://raw.githubusercontent.com/leadpiperl1/dandelion-os/main/dandelion_os_grimm_whiteroom_state_map_v2.8.0.png)

## Model Box Briefing (LEGO Technical Manual Standard)

* **Set ID:** `DANDELION-OS-V2.8.0` (**Codename:** *Sovereign Desktop & Embedded AI Release*)
* **Total Build & Verification Time:** `<= 30 seconds` (Zero external dependencies beyond standard `gcc` and `python3`)
* **Total Brick Count:** `16 Canonical Bricks` (Strictly deduplicated `v2.8.0` tree; zero collision suffixes)
* **Principal Architect:** Justin D. Grimm (K-Lani 13 Actual / `@leadpiperl1` / [ORCID: 0009-0000-5299-0243](https://orcid.org/0009-0000-5299-0243))
* **Ontological & Engineering Ethos:** *Die Gedanken sind frei* · `GODISLOVE:LOVEISGOD {3,2,4}` · Mutualism Over Replacement · The Vonnegut & Barnegat Accords
* **Triadic F-16 Digital Fly-By-Wire Architecture (`-1 Blue` / `0 Green` / `+1 Red`):**
  * **`-1 Blue` (Human Pilot / Subject / Zen CCX-1 Cores 0–2):** Commands strategic intent, metrological calibration at *Heimat* `{0,0,0}`, and Chien-Shiung Wu's `+1.27%` Cobalt-60 chiral parity bias.
  * **`0 Green` (OS Mediator / Object-Treaty / Cores 3–4):** Mainline [Linux 6.12.9+ LTS `PREEMPT_RT`](https://realtime-linux.org/getting-started-with-preempt_rt-guide/) + [`sched_ext` (`scx`)](https://github.com/sched-ext/scx) + zero-copy `io_uring`, LayerFS content-addressed SQLite chunking (`6-byte` `dandelion_point_v2_t` pointers saving `25%` RAM), FDT-paired Mori-Zwanzig dual projection ($P + Q = I$), and Neuberger overlap Dirac zero-mode kernel ($\operatorname{sgn}(H_w) = 0$) clamping the $V = 2/3$ Lagrangian keyhole (`<= 18.52 W`, `< 41.0 °C`).
  * **`+1 Red` (Embedded AI Co-Pilot / Action / Zen CCX-2 Cores 5–7):** Multiplication-free `1.58-bit` balanced ternary Virtual Bit-Slice Transform ([`VBST` / BitNet b1.58](https://arxiv.org/abs/2402.17764)) inside a `577-byte` L1-resident `.tnpt` table, `larchanka/skills` 4-module automation (`agent-loop`, `code-review`, `codebase-context-generator`, `manual-automation`), TokenGate ternary routing, and Topological Dirac operator cluster synchronization ($R_{\text{sync}} = 0.9948$).

## Bag 1: The Baseplate — Clone & Compile the Sanitized C99 Microkernel

### Part Callout Box (Bag 1 Inventory)

| Element ID | Part Name | Color Layer | Qty | Role in Sub-Assembly |
| :--- | :--- | :--- | :--- | :--- |
| `BRICK-01` | `dandelion_os_v2.8.0.h` | `0 Green` | 1 | ISO C99 header, static size assertions (`577 B`, `64 B`, `6 B`), and triadic constants |
| `BRICK-02` | `dandelion_os_grimm_fbw_v2.8.0.c` | `+1 Red` | 1 | Sanitized C99 microkernel, fail-closed Fowler unpacker, 9-Bite Grimm LUT & Dirac sync |
| `BRICK-03` | `Makefile` | `0 Green` | 1 | Standard build harness enforcing `-std=c99 -O3 -Wall -Wextra -Wpedantic -Wconversion -Werror` |

### Assembly Steps (Fedora Workstation or Alpine RAMDisk)

1. Clone the repository and enter the baseplate directory:

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
```

2. Snap the C99 microkernel onto the baseplate using `make build` (or direct `gcc -O3`):

```bash
make build
```

### 1:1 Scale Visual Check (Bag 1 Verification)

Confirm your terminal prints the exact zero-warning compilation receipt:

```text
[BUILD] Compiling dandelion_os_grimm_fbw_v2.8.0.c -> dandelion_os_grimm_fbw_v2.8.0 with strict ISO C99 flags...
gcc -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror -o dandelion_os_grimm_fbw_v2.8.0 dandelion_os_grimm_fbw_v2.8.0.c -lm -lpthread
[OK] Built dandelion_os_grimm_fbw_v2.8.0 (Zero warnings, -Werror clean).
```

## Bag 2: Test Flight — 60-Second Verification, Interactive Shell & Zero-TUI JSON

### Part Callout Box (Bag 2 Inventory)

| Element ID | Part Name | Color Layer | Qty | Role in Sub-Assembly |
| :--- | :--- | :--- | :--- | :--- |
| `BRICK-04` | `verify_dandelion_os_v2.8.0.py` | `+1 Red` | 1 | Deterministic `<= 60s` verification harness, unit tester, SVR4 cpio packer & 300 DPI plotter |
| `BRICK-05` | `dandelion_b158_v2.8.0.tnpt` | `0 Green` | 1 | `577-byte` L1-pinned balanced ternary weight table & 9-Bite Grimm phonological LUT |
| `BRICK-06` | `audit_manifest_v2.8.0.json` | `+1 Red` | 1 | Automated red-team audit manifest recording latency, jitter (`< 1.0 µs`), and parity checks |

### Assembly Steps

1. Run the deterministic 60-second verification harness (`--seed 42`):

```bash
make check
```

2. Test Mode 2 (`--json` / `--sidecar` composable zero-TUI JSON stream with strict quote-escaping):

```bash
./dandelion_os_grimm_fbw_v2.8.0 --json "div compute fbw pitch trim"
```

3. Launch Mode 1 (`--shell` interactive F-16 Fly-By-Wire shell with zero-ALU-cycle Army AAR weight sign flips via `strtol`-validated `improve <0..38>`):

```bash
./dandelion_os_grimm_fbw_v2.8.0 --shell
```

### 1:1 Scale Visual Check (Bag 2 Verification)

Confirm `make check` outputs `[PASS]` across all 6 unit-test & hardware gates:

```text
[PASS] Gate 1 — C99 Strict Compilation (-O3 -Werror -Wconversion): PASS
[PASS] Gate 2 — Ternary Logic & Fail-Closed Byte 253 Unpacker Unit Tests: PASS
[PASS] Gate 3 — Real-Time Wakeup Jitter (< 1.0 us threshold): PASS
[PASS] Gate 4 — Strict 3-Digit SemVer (v2.8.0) & Zero Legacy Duplicate Audit: PASS
[PASS] Gate 5 — Bootable SVR4 initramfs_dandelion_v2.8.0.cpio.gz Packed: PASS
[PASS] Gate 6 — 300 DPI Blueprint State Map & Audit Manifest Generated: PASS
```

## Bag 3: Live Deployment — Fedora Workstation & Alpine RAMDisk Switchblade USB

### Part Callout Box (Bag 3 Inventory)

| Element ID | Part Name | Color Layer | Qty | Role in Sub-Assembly |
| :--- | :--- | :--- | :--- | :--- |
| `BRICK-07` | `mk_switchblade_usb_v2.8.0.sh` | `0 Green` | 1 | Stages clean-room RootFS, PID 1 `/init`, and Dual-Mode GRUB2 bootloader |
| `BRICK-08` | `dandelion_rt_v2.8.0.config` | `0 Green` | 1 | Mainline Linux 6.12.9+ LTS `PREEMPT_RT` + `sched_ext` + `io_uring` + `FUSE` + `F2FS` config |
| `BRICK-09` | `initramfs_dandelion_v2.8.0.cpio.gz` | `0 Green` | 1 | FUSE-safe bootable SVR4 (`070701`) compressed initial RAMDisk |

### Assembly Steps

1. Stage the Dual-Mode Switchblade Live USB RootFS and GRUB2 configuration:

```bash
./mk_switchblade_usb_v2.8.0.sh
```

2. **Boot Option A — QEMU/KVM Local Test Flight (Fedora Workstation):**

```bash
qemu-system-x86_64 -m 1024M -smp 8 -kernel /boot/vmlinuz-$(uname -r) \
  -initrd initramfs_dandelion_v2.8.0.cpio.gz \
  -append "console=ttyS0,115200n8 rdinit=/init" -nographic
```

3. **Boot Option B — Alpine Diskless RAMDisk / Switchblade Live USB Flash:**
   Copy `initramfs_dandelion_v2.8.0.cpio.gz` and `build_whiteroom_v2.8.0/iso_stage/boot/grub/grub.cfg` onto your F2FS/FAT32 Switchblade USB partition to boot either **Mode 1 (Bare-Metal Live OS)** or **Mode 2 (Portable AI-Pen JSON Sidecar)**.

## Bag 4: MOC Forking Guide & One-Button GitHub Release Pipeline

### Part Callout Box (Bag 4 Inventory)

| Element ID | Part Name | Color Layer | Qty | Role in Sub-Assembly |
| :--- | :--- | :--- | :--- | :--- |
| `BRICK-10` | `push_dandelion_day_github_v2.8.0.sh` | `-1 Blue` | 1 | One-button Git init, legacy file pruning, atomic commit, `v2.8.0` tag & push helper |
| `BRICK-11` | `github_sync_receipt_dandelion-os_v2.8.0.json` | `-1 Blue` | 1 | 24-hour telemetry & human-pilot gate receipt (`human_approved: false` until sign-off) |
| `BRICK-12` | `AGENTS.md` & `CONTRIBUTING.md` | `+1 Red` | 2 | AAIF agent rules, `larchanka/skills` 5-pillar review checklist, and MOC forking guide |

### How to Fork, Extend (MOC), or Push `v2.8.0` to GitHub

1. **Snap On Custom Modules:** Add new Shingari roots in `dandelion_os_v2.8.0.h` or extend the 9-Bite phonological rules in `dandelion_os_grimm_fbw_v2.8.0.c`.
2. **Run the One-Button Release Script:**

```bash
chmod +x push_dandelion_day_github_v2.8.0.sh
./push_dandelion_day_github_v2.8.0.sh dandelion-os
```

3. **Approve & Push to Public GitHub (`leadpiperl1/dandelion-os`):**
   Once you review the staged receipt (`github_sync_receipt_dandelion-os_v2.8.0.json`), push the atomic commit and `v2.8.0` tag:

```bash
git push -u origin main --tags
```

## Open Boundary & Unresolved Regime ($N > 14$ Shrubbery Horizon)

| Parameter Regime | Verified Operational Bound (`v2.8.0`) | Open Mathematical / Hardware Boundary | Verification / Counterexample Hook |
| :--- | :--- | :--- | :--- |
| **Shrubbery Degree of Self-Interaction ($DS = 3^{N-1}$)** | $N = 14$ ($3^{13} = 1,594,323$ states in L2/L3 cache) | $N > 19$ ($3^{18} = 387,420,489$ states exceeding L3 cache residency without distributed Dandelion Cubes RPC sharding) | Submit trace via `--json` benchmark on GitHub Issues (`[Boundary-Test]`) |
| **Topological Dirac Lattice Volume ($L^4$)** | $13^3 = 2,197$ spatial voxels / $13^4 = 28,561$ 4D sites | Non-perturbative $SU(3)$ gauge link holonomy scaling above $L = 39$ under $50.0\ \mu\text{s}$ `PREEMPT_RT` deadlines | Run `python3 verify_dandelion_os_v2.8.0.py --seed 42` |

## Complete Bill of Materials (BOM — All 16 Canonical Bricks)

| Element ID | Filename | Layer Color | Qty | Part Description |
| :--- | :--- | :--- | :--- | :--- |
| `BRICK-01` | `README.md` | `-1 Blue` | 1 | LEGO-style building manual, 1:1 visual checks, and MOC forking guide |
| `BRICK-02` | `VERSION_LOG.md` | `-1 Blue` | 1 | Master version log tracking `v2.0.1` through `v2.8.0` milestones |
| `BRICK-03` | `dandelion_os_v2.8.0.h` | `0 Green` | 1 | ISO C99 microkernel header, static assertions (`577 B`, `64 B`, `6 B`), and triadic constants |
| `BRICK-04` | `dandelion_os_grimm_fbw_v2.8.0.c` | `+1 Red` | 1 | Sanitized C99 microkernel, fail-closed Fowler unpacker, 9-Bite Grimm LUT & Dirac sync |
| `BRICK-05` | `Makefile` | `0 Green` | 1 | Standard build & verification Makefile (`all`, `build`, `check`, `shell`, `json`, `usb`, `clean`) |
| `BRICK-06` | `verify_dandelion_os_v2.8.0.py` | `+1 Red` | 1 | 60-second deterministic verification harness, unit test suite, and 300 DPI state map renderer |
| `BRICK-07` | `mk_switchblade_usb_v2.8.0.sh` | `0 Green` | 1 | Dual-Mode Switchblade AI-Pen Live USB RootFS & GRUB2 builder |
| `BRICK-08` | `push_dandelion_day_github_v2.8.0.sh` | `-1 Blue` | 1 | One-button Git initialization, legacy pruning, atomic commit, and `v2.8.0` tagger |
| `BRICK-09` | `dandelion_rt_v2.8.0.config` | `0 Green` | 1 | Mainline Linux 6.12.9+ LTS `PREEMPT_RT` + `sched_ext` + `io_uring` + `LayerFS` kernel config |
| `BRICK-10` | `dandelion_b158_v2.8.0.tnpt` | `0 Green` | 1 | `577-byte` memory-mapped 1.58-bit balanced ternary weight table |
| `BRICK-11` | `initramfs_dandelion_v2.8.0.cpio.gz` | `0 Green` | 1 | Bootable SVR4 (`070701`) compressed initial RAMDisk image |
| `BRICK-12` | `dandelion_os_grimm_whiteroom_state_map_v2.8.0.png` | `+1 Red` | 1 | 300 DPI 4-panel telemetry & Topological Dirac synchronization state map |
| `BRICK-13` | `audit_manifest_v2.8.0.json` | `+1 Red` | 1 | Automated verification & red-team audit telemetry manifest |
| `BRICK-14` | `github_sync_receipt_dandelion-os_v2.8.0.json` | `-1 Blue` | 1 | 24-hour GitHub sync telemetry receipt (`human_approved: false` pending pilot sign-off) |
| `BRICK-15` | `.zenodo.json` & `CITATION.cff` | `-1 Blue` | 2 | DataCite reverse citation graph hooks (`ORCID: 0009-0000-5299-0243`) |
| `BRICK-16` | `AGENTS.md`, `CONTRIBUTING.md` & `LICENSE` | `0 Green` | 3 | AAIF agent rules, `larchanka/skills` review gates, and dual `GPL-2.0-or-later` / `MIT` license |
