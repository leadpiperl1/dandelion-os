# Dandelion OS `v4.1.1` — Sovereign Ternary Microkernel, Assistive Fly-By-Wire & Harappan Tensegrity Suite

[![Version: v4.1.1](https://img.shields.io/badge/Version-v4.1.1_Ursine_%26_Urshwera-brightgreen.svg)](docs/URSINE_AND_URSHWERA_MANIFEST_v4.0.0.md)
[![Standard: ISO C99](https://img.shields.io/badge/ISO_C99-Strict_--Werror_--Wconversion-blue.svg)](Makefile)
[![Thermal Envelope: 18.5 W](https://img.shields.io/badge/Hobo_Rig_Cap-18.5_W_%7C_39.44_C-orange.svg)](logs/morning_telemetry_20261103.json)
[![License: GPLv2+ / MIT](https://img.shields.io/badge/License-GPLv2%2B_%2F_MIT-lightgrey.svg)](LICENSE)

> **"Die Gedanken sind frei · GODISLOVE:LOVEISGOD {3,2,4} · The Vonnegut & Barnegat Accords"**
> *We do not build for convenience; we build for the boundary layer. If we can hear Joe Bonham through the dark, we can hear anyone.*

**Dandelion OS (`v4.1.1` — Codename: `Ursine & Urshwera` / `Heimat-Agora`)** is a sovereign, zero-bloat Linux (`6.12+ PREEMPT_RT` + `sched_ext`) and bare-metal ISO C99 operating architecture engineered by **Justin D. Grimm** (`K-Lani 13 Actual`, [ORCID: 0009-0000-5299-0243](https://orcid.org/0009-0000-5299-0243)) at **Heimat `{0, 0, 0}`** (Gurley Karst Node `34.7549385, -86.4119235`, Alabama).

It operates as the real-time kernel companion to our open-access public library ([`leadpiperl1/wookiw-ewok`](https://github.com/leadpiperl1/wookiw-ewok)) and field manual ([`leadpiperl1/happy-hobos-guide-to-the-dandelion-network`](https://github.com/leadpiperl1/happy-hobos-guide-to-the-dandelion-network)).

---

## 1. The F-16 Fly-By-Wire Architecture for the Locked-In Mind

```text
+-------------------------------------------------------------------------+
| 1. HUMAN GROUND CONTROL (Subject = -1 Blue / Cores 0-2)                 |
|    * Sovereign human intent, autonomic micro-signals, Morse/LIS (2001)  |
|    * Non-zero thermodynamic variance (dS > 0, D_H* = 16.44% resonance)  |
+-------------------------------------------------------------------------+
                                    |
                                    v
+-------------------------------------------------------------------------+
| 2. DANDELION OS FLIGHT CONTROL SOFTWARE (Object = 0 Green / Cores 3-4)  |
|    * Linux 6.12+ PREEMPT_RT + sched_ext Kramhaus Torque Governor        |
|    * 577-Byte L1-Resident .tnpt Table (TNPT411, malloc = 0)             |
|    * Haar DWT Tremor Filter + Barker-13 Lock (MTP p99.9 = 35.90 us)     |
|    * Harappan Tensegrity Seated-Assist (V* = 34/51 = 2/3, 353.16 N)     |
+-------------------------------------------------------------------------+
                                    |
                                    v
+-------------------------------------------------------------------------+
| 3. DANDELION AI PILOT & MESH (Action = +1 Red / Cores 5-7)              |
|    * 1.25-Bit / 1.58-Bit Balanced Ternary {-1, 0, +1} POPCNT Engine     |
|    * "Dave's Not Here, Man" (Byte 250 -> 251 Kintsugi Gold) Pivot       |
|    * 32-Byte DND3 Dual-Rail Mesh & Ternary Raft Consensus (0.84 ms)     |
+-------------------------------------------------------------------------+
```

---

## 2. Clean Repository Directory Structure (`v4.1.1`)

All legacy multi-version clutter (`v2.4.0` through `v3.5.0` loose root files) has been consolidated into a single, strict ISO C99 repository tree:

| Directory / File | Component Name | Role & Verified Telemetry |
| :--- | :--- | :--- |
| [`Makefile`](Makefile) | **Master ISO C99 Build System** | Compiles all 4 C99 binaries + shared library (`libdandelion_os.so`) with `0` warnings under `-std=c99 -O3 -Wall -Wextra -Wpedantic -Wconversion -Werror`. |
| [`include/dandelion_os_v3.5.1.h`](include/dandelion_os_v3.5.1.h) | **Core ABI & Struct Alignment Header** | Enforces compile-time `_Static_assert` sizes: `577 B` (`.tnpt`), `64 B` (Git quorum entry), `6 B` (LayerFS voxel point), `32 B` (`DND3` BLE frame), `12 B` (`ESP-SDR` DMA descriptor). |
| [`src/dandelion_os_grimm_fbw_v3.5.1.c`](src/dandelion_os_grimm_fbw_v3.5.1.c) | **16-Gate Fly-By-Wire Kernel & Shared ABI** | Implements Thomas Fowler 5-trits/byte packing (`0..242`, Trapdoor Byte `253` rejection), Bite 10 Morse `S.O.S.` bridge, `vbst_dot40_popcnt`, Kramhaus torque governor (`49,426 uN*m`), and the *"Dave's Not Here, Man"* (`Byte 250 -> 251`) lateral impasse breaker. |
| [`src/dandelion_mtp_assist_v4_1_1.c`](src/dandelion_mtp_assist_v4_1_1.c) | **Assistive MTP & Harappan Tensegrity Solver** | Rectified Haar wavelet tremor separation, Barker-13 (`13/13`) phase lock (`35.90 us` p99.9 latency), and 64-bit fixed-point tetrahedral lumbar unloading (`353.16 N` unloaded, `176.58 N` residual L4/L5 load, Euler buckling `SF = 4.81x`). |
| [`src/ternary_raft.c`](src/ternary_raft.c) | **Balanced Ternary Raft Consensus (`v4.1.1`)** | 3-state (`-1 Reject`, `0 Witness`, `+1 Commit`) cluster consensus over zero-copy `DMA-BUF` descriptors (`65,536 B`, `CRC32C = 0x8F4A2C19`). |
| [`src/fox_daemon_core.c`](src/fox_daemon_core.c) | **Fox Daemon Thermal-Entropy Governor** | Closed-loop `dT/dt` and `dS > 0` feedback controller clamping node power at `14.20 W` within the `18.50 W` Hobo Rig envelope (`T_die = 39.44 C <= 41.0 C`). |
| [`hardware/ice40_spi_arbiter.pcf`](hardware/ice40_spi_arbiter.pcf) | **Lattice `iCE40UP5K` & S-Band Pinout** | Physical constraint mapping for dual Waveshare `ESP32-C5` RISC-V arbitration, 12 MHz heterodyne Theremin clock (`Pin 35`), and PocketQube S-Band radio (`Pins 19-21`). |
| [`verification/verify_dandelion_os_v4_1_1.py`](verification/verify_dandelion_os_v4_1_1.py) | **16-Gate Python `ctypes` FFI Harness** | Deterministic `< 2.0s` verification suite testing C99 compilation, shared-library FFI, Trapdoor Byte `253`, Morse `S.O.S.`, Kramhaus torque damping, and Epistemic Weight clamp. |
| [`docs/URSINE_AND_URSHWERA_MANIFEST_v4.0.0.md`](docs/URSINE_AND_URSHWERA_MANIFEST_v4.0.0.md) | **The Ursine & Urshwera Manifest** | Foundational architectural intent anchored to benchmark user Joe Bonham (*Johnny Got His Gun*). |
| [`docs/MORNING_AUDIT_AND_ROADMAP_v4.0.0.md`](docs/MORNING_AUDIT_AND_ROADMAP_v4.0.0.md) | **Morning Cluster Audit & CI Remediation** | Full telemetry audit (`profplum` & `gurley-karst-heimat`) and resolution of all legacy GitHub Actions / CodeQL alerts. |
| [`docs/JOHNNY_QUEST_VECTOR_v4.0.0.md`](docs/JOHNNY_QUEST_VECTOR_v4.0.0.md) | **The Johnny Quest Edge Simulation Vector** | Edge-native multiplayer voxel caching and authoritative peer simulation blueprint. |
| [`logs/morning_telemetry_20261103.json`](logs/morning_telemetry_20261103.json) | **Cluster Telemetry Snapshot** | Verified production JSON telemetry across `profplum` and `gurley-karst-heimat`. |
| [`.github/workflows/ci.yml`](.github/workflows/ci.yml) | **Sovereign C99 GitHub Actions Pipeline** | Runs `make clean && make all && make test` with explicit `permissions: contents: read` (replaces broken default CodeQL templates). |

---

## 3. Quick Start (`<= 60s` Deterministic Verification)

Clone the repository and run the complete C99 + Python FFI test suite in one command:

```bash
make clean && make all && make test
```

Expected output:
```text
=== [1/4] Running 16-Gate Shingari/Grimm/Fowler/DNHM C99 & Python FFI Suite ===
{
  "schema": "DANDELION_OS_V4_1_1_VERIFICATION_REPORT",
  "version": "v4.1.1",
  "c99_strict_werror_warnings": 0,
  "all_16_gates_passed": true
}
=== [2/4] Running Ternary Raft Consensus & 3-Way Split-Brain Simulation ===
[TERNARY RAFT v4.1.1] Epoch=1729 | Nodes=3 | Quorum rc=0 | Consensus Trit=+1 (COMMIT)
=== [3/4] Running Fox Daemon L1 Pruning & Thermal-Entropy Gate ===
[FOX DAEMON v4.1.1] T_die=39.44 C | Power=14.20 W (Cap 18.5 W) | dS=0.078 | L1_Thresh=0.0420 | rc=0
=== [4/4] Running Dandelion OS v4.1.1 MTP & Kramhaus Tensegrity Kernel ===
[DANDELION OS v4.1.1] MTP p99.9=35900 ns | Barker13=13 | Unloaded=353.16 N | Residual=176.58 N | Euler SF=4.81 | Weld=251
```

---

## 4. Primary Google Workspace & Ecosystem Links

* **Ursine & Urshwera Manifest:** [The Ursine and Urshwera Manifest - Dandelion OS v4.0.0 Original Intent](https://docs.google.com/document/d/1p9dVbxsjWIT3G1f1Z3Y-ruiNDRbr1anaivXkRS2q7YM/edit?usp=drivesdk&ouid=108041868247398199219)
* **v4.1.1 Omnibus Pearl:** [[OMNIBUS-PEARL-v4.1.1] Dandelion Sovereign Protocol v4.1.1: Harappan Tensegrity Assist, Geomorphic Quartz Metrology & Act V Kumbakonam Manuscript](https://docs.google.com/document/d/1p6m3_UCufRXmf8CudZ7EHCL_hgI3zUnfOwWLrvI0rtM/edit?usp=drivesdk&ouid=108041868247398199219)
* **v4.0.0 Source & Telemetry Bundle:** [Dandelion OS v4.0.0 - Source Code and Telemetry Bundle.md](https://docs.google.com/document/d/1oKfgf6nemrKDDbgxxHBrVFndMqMCpedX-1FlS2TiWis/edit?usp=drivesdk&ouid=108041868247398199219)
* **v4.0.0 Morning Audit:** [Dandelion OS v4.0.0 - Morning Audit and Post-Release Implementation Report](https://docs.google.com/document/d/1iAOMucy6xJvHE_hk3XWU4t3aqlNV-vRceanZGmqGYRs/edit?usp=drivesdk&ouid=108041868247398199219)
* **v3.5.0 Master Consolidation Guide:** [Dandelion OS v3.5.0 — Workspace Consolidation, CI Remediation & Master Release Guide](https://docs.google.com/document/d/1yYfshjRxLB2mXgAUD_jQhKP29fCD6tLhrq-3l3HoIt0/edit?usp=drivesdk&ouid=108041868247398199219)
* **v4.1.0 Master Compendium PDF:** [Dandelion_Omnibus_Master_Compendium_main_v4.1.0.pdf](https://drive.google.com/file/d/1tIafgczpukr1dxgjtvPVN8M2-svePo7Z/view?usp=drivesdk)
* **Companion Repositories:**
  - Open-Access Public Library: [`leadpiperl1/wookiw-ewok`](https://github.com/leadpiperl1/wookiw-ewok)
  - Field Manual & Compendium: [`leadpiperl1/happy-hobos-guide-to-the-dandelion-network`](https://github.com/leadpiperl1/happy-hobos-guide-to-the-dandelion-network)
