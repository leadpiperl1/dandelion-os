# Dandelion OS (`v2.7.2` — "Heimat-Agora")
### The Open-Source Real-Time Linux + 1.58-Bit Ternary AI Building Manual

[![License: GPL v2 / MIT](https://img.shields.io/badge/License-GPLv2%20%2F%20MIT-blue.svg)](https://github.com/leadpiperl1/dandelion-os/blob/main/LICENSE)
[![Kernel: Linux 6.12+ PREEMPT_RT + sched_ext](https://img.shields.io/badge/Kernel-Linux%206.12%2B%20PREEMPT__RT%20%2B%20sched__ext-green.svg)](https://github.com/leadpiperl1/dandelion-os/blob/main/mk_switchblade_usb_v2.7.0.sh)
[![AI Engine: 1.58-Bit Balanced Ternary VBST](https://img.shields.io/badge/Embedded%20AI-1.58--Bit%20Ternary%20%28577%20Bytes%29-red.svg)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_os_grimm_fbw_v2.7.0.c)
[![ORCID: 0009-0000-5299-0243](https://img.shields.io/badge/ORCID-0009--0000--5299--0243-A6CE39.svg)](https://orcid.org/0009-0000-5299-0243)

> **MODEL BOX BRIEFING (`SET #DOS-1729-272`)**
> * **What You Are Building:** A clean-room, real-time Linux `6.12+ PREEMPT_RT` + `sched_ext` operating system and bootable Live USB stack with an L1-cache-resident **1.58-bit balanced ternary AI co-pilot** (`577 bytes`), the **9-Bite Universal Grimm's Law** polyglot parser, and the **Shingari 4-Register Rosetta Stone**.
> * **How It Works (The F-16 Fly-By-Wire Principle):** An F-16 fighter jet is intentionally aerodynamically unstable for maximum agility and uses a closed-loop digital fly-by-wire computer to keep the human pilot in command. Dandelion OS pairs three color-coded layers:
>   * **`[-1 BLUE]` Human Pilot:** Your natural-language intent across 9 global language families or 13 Shingari 3-letter roots.
>   * **`[ 0 GREEN]` Real-Time Linux OS:** Deterministic `< 50.0 µs` kernel syscalls, `sched_ext` BPF core pinning, and zero-copy `io_uring` pipes.
>   * **`[+1 RED]` Embedded 1.58-Bit Ternary AI:** Multiplication-free Virtual Bit-Slice Transform (`VBST`) co-processor that trims manifold shear in **`< 2.0 µs`** while preventing generative mode collapse (`Byte 254`).
> * **Total Build Time:** `30 seconds` (Quick Start) · `5 minutes` (Live USB / Custom Fork)
> * **Total Bricks in Box:** `16 Canonical Files` (Zero external C dependencies)

![Dandelion OS v2.7.2 Architecture & Telemetry Blueprint](https://raw.githubusercontent.com/leadpiperl1/dandelion-os/main/dandelion_os_grimm_whiteroom_state_map_v2.7.0.png)

## Before You Open Bag 1: Workbench & Tool Check

Make sure your Linux workstation (Fedora, Ubuntu/Debian, Arch, Alpine), macOS, or Windows WSL2 terminal has these **3 standard tools** on the workbench:

| Tool Icon | Required Tool | Purpose in Build | Quick Terminal Check |
| :--- | :--- | :--- | :--- |
| `[TOOL-1]` | **`git`** | Clones and forks the repository | `git --version` |
| `[TOOL-2]` | **`gcc`** *(or `clang`)* | Compiles the C99 Fly-By-Wire engine | `gcc --version` |
| `[TOOL-3]` | **`python3`** *(+ `numpy`, `matplotlib`)* | Runs the `<= 60s` audit & packs the SVR4 RAMDisk | `python3 --version` |

## BAG 1: The Baseplate — Clone & Snap Together the Core Engine

> **PART CALLOUT BOX (BAG 1)**
> * `[1x]` `Makefile` *(Build orchestrator)*
> * `[1x]` `dandelion_os_grimm_fbw_v2.7.0.c` *(Clean-room C99 F-16 FBW & 9-Bite Grimm engine)*
> * `[1x]` `mk_switchblade_usb_v2.7.0.sh` *(Clean-room RootFS & GRUB2 stage builder)*

### Step 1.1 — Place the Baseplate on Your Workbench (`git clone`)

Open your terminal and clone Dandelion OS into its own dedicated directory:

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
```

### Step 1.2 — Snap the C99 Engine Onto the RootFS Baseplate (`make`)

Compile the standalone C99 binary (`-O3 -std=c99`) and stage the clean-room Linux `6.12+ PREEMPT_RT` directory tree:

```bash
make
# (Or if you don't use make: ./mk_switchblade_usb_v2.7.0.sh)
```

> **[1:1 SCALE VISUAL CHECK — STEP 1.2]**
> Before moving to Bag 2, verify your terminal prints these two confirmation lines:
> ```text
> [OK] Compiled C99 binary -> .../build_whiteroom_v2.7.0/rootfs/opt/dandelion/bin/dandelion_os_grimm_fbw_v2.7.0
> [OK] Staged Switchblade AI-Pen v2.7.0 RootFS and GRUB2 Dual-Mode Config.
> ```

## BAG 2: The Fly-By-Wire Cockpit — Test-Fly & Verify (`<= 60 Seconds`)

> **PART CALLOUT BOX (BAG 2)**
> * `[1x]` `verify_dandelion_os_v2.7.0.py` *(Deterministic 15-vector audit & SVR4 CPIO packer)*
> * `[1x]` `dandelion_b158_v2.7.0.tnpt` *(`577-byte` L1-resident 1.58-bit ternary weight table)*
> * `[1x]` `dandelion_os_grimm_fbw_v2.7.0` *(Compiled C99 executable)*

### Step 2.1 — Run the 60-Second Pre-Flight Audit (`make check`)

Test all 15 cross-linguistic vectors across the 9 global Bites, verify that corrupted ternary `Byte 253` fails closed, run the `100,000-iteration` real-time latency benchmark, and pack the bootable `initramfs_dandelion_v2.7.0.cpio.gz` image:

```bash
make check
# (Or directly: python3 verify_dandelion_os_v2.7.0.py --seed 42)
```

> **[1:1 SCALE VISUAL CHECK — STEP 2.1]**
> Confirm all four `[PASS]` lights turn green in under 60 seconds:
> ```text
> [PASS] Dandelion Day v2.7.0 Verification completed in 1.158 s (<= 60.0 s budget)
> [PASS] Bootable SVR4 initramfs_dandelion_v2.7.0.cpio.gz size: 14022 bytes
> [PASS] 300 DPI State Map saved to: .../dandelion_os_grimm_whiteroom_state_map_v2.7.0.png
> [PASS] C99 Mean Parse: 0.7851 us | Mean VBST+TWAS: 0.0863 us
> ```

### Step 2.2 — Sit in the Interactive F-16 FBW Cockpit (`make shell`)

Launch the live interactive shell to test utterances across any of the 9 Bites, invoke Shingari 3-letter kernel roots, or flip ternary weights in real time (`0 ALU cycles`):

```bash
make shell
# (Or directly: ./dandelion_os_grimm_fbw_v2.7.0 --shell)
```

Try typing these commands inside the `dandelion@heimat-v2.7.0>` prompt:
```text
dandelion@heimat-v2.7.0> div compute fbw pitch trim
dandelion@heimat-v2.7.0> hozho nahasdlii
dandelion@heimat-v2.7.0> shankara tourniquet sandbags 1729
dandelion@heimat-v2.7.0> improve 4
dandelion@heimat-v2.7.0> quit
```

### Step 2.3 — Connect the Zero-TUI JSON Sidecar Pipe (`make json`)

Want to pipe Dandelion OS telemetry into `jq`, UNIX Domain Sockets, or a Model Context Protocol ([MCP](https://github.com/modelcontextprotocol)) agent? Run `--json`:

```bash
./dandelion_os_grimm_fbw_v2.7.0 --json "ohm compile grimm shift"
```

Output:
```json
{"version":"v2.7.0","utterance":"ohm compile grimm shift","root":"ohm","bite":6,"macro_p_inode":1803,"micro_q_inode":1522,"reg4_evid":1,"boundary_code":0,"buzoa_tactic":0,"twas_parity":1,"khosla_align":1.0000,"shear_deg":6.10,"trimmed_deg":1.98,"parse_us":0.7851,"vbst_us":0.0863,"siphash24":"0x7a9a1b04e32d1233"}
```

## BAG 3: The Switchblade AI-Pen — Boot in QEMU or Flash to a Thumb Drive

> **PART CALLOUT BOX (BAG 3)**
> * `[1x]` `initramfs_dandelion_v2.7.0.cpio.gz` *(`14 KB` SVR4 `newc` bootable RAMDisk)*
> * `[1x]` `build_whiteroom_v2.7.0/dandelion_rt_v2.7.0.config` *(Linux `6.12+ PREEMPT_RT` + `sched_ext` kernel config)*
> * `[1x]` `build_whiteroom_v2.7.0/iso_stage/boot/grub/grub.cfg` *(Dual-Mode GRUB2 bootloader)*

### Step 3.1 — Sub-Assembly A: Instant Virtual Boot in QEMU (No USB Stick Needed)

Boot the Dandelion OS initial RAMDisk right inside your terminal using QEMU and your host kernel:

```bash
make qemu
```

### Step 3.2 — Sub-Assembly B: Flash a Physical Bootable USB Thumb Drive

1. **Pack the Hybrid UEFI/BIOS ISO** (requires `grub-mkrescue` and `xorriso`, plus your compiled `Linux 6.12+ PREEMPT_RT` `bzImage` or host kernel):
   ```bash
   cp /boot/vmlinuz-$(uname -r) build_whiteroom_v2.7.0/iso_stage/boot/bzImage
   cp initramfs_dandelion_v2.7.0.cpio.gz build_whiteroom_v2.7.0/iso_stage/boot/
   grub-mkrescue -o dandelion_os_switchblade_v2.7.0.iso build_whiteroom_v2.7.0/iso_stage
   ```
2. **Write to Your USB Thumb Drive** (replace `/dev/sdX` with your USB device from `lsblk`):
   ```bash
   sudo dd if=dandelion_os_switchblade_v2.7.0.iso of=/dev/sdX bs=4M status=progress oflag=sync
   ```
3. **Select Your Boot Mode at the GRUB2 Menu:**
   * **Mode 1:** *Dandelion OS v2.7.0 Bare-Metal Live RAMDisk* (boots into PID 1 `/init` with chiral CPU core pinning).
   * **Mode 2:** *Switchblade AI-Pen Sidecar Node* (streams zero-TUI JSON telemetry over serial/socket to any host computer).

## BAG 4: MOC (My Own Creation) Guide — Fork & Snap On Your Own Bricks

Dandelion OS is built to be forked, remixed, and extended. Here is how the internal Technic gearbox fits together so you can snap on your own modules.

### Sub-Assembly 4.1 — The 13 Shingari Kernel Roots & 10-Project AAIF Bus

Every command in Dandelion OS routes through one of **13 three-letter Shingari roots** (`SHINGARI_ROOTS[]` in `dandelion_os_grimm_fbw_v2.7.0.c`), bridging human intent (`-1 Blue`), Linux kernel syscalls (`0 Green`), and the 10 open standards of the Linux Foundation Agentic AI Foundation (`+1 Red`):

| Brick ID | Phase | Human Pilot Intent (`-1 Blue`) | Linux Kernel / Syscall (`0 Green`) | Embedded AI & AAIF Action (`+1 Red`) |
| :--- | :--- | :--- | :--- | :--- |
| **`ssf`** | `4-INTAKE` | Classify storage & voxel files | `ls -laF` / `readdir()` | **AAIF Agent Skills:** Mount `/mnt/ramdisk/skills` & `vX.Y.Z` tags |
| **`hrt`** | `4-INTAKE` | Check heartbeat & CPU thermal | `cat /sys/class/thermal/temp` | **AAIF Agent Router:** Clamp $V=2/3$ keyhole ($<45.0^\circ\text{C}$) & `15 Hz` mux |
| **`rtz`** | `4-INTAKE` | Map 3D directory & thought tree | `tree /mnt/models` | **AAIF [MCP](https://github.com/modelcontextprotocol) Server:** Project $13\times 13\times 13$ L1 voxel & `.tnpt` cache |
| **`sft`** | `4-INTAKE` | Sift & pattern-match text/code | `grep -rn <pattern>` | **AAIF `AGENTS.md`:** Extract $P\psi$ math + $Q\psi$ story invariants |
| **`div`** | `5-CORE` | Track rate-of-change pitch delta | `clock_gettime(MONOTONIC_RAW)` | **AAIF [A2A](https://github.com/a2aproject/a2a) & OpenEnv:** F-16 FBW pitch trim + BUZOA swarm step |
| **`orb`** | `5-CORE` | Stream kernel & Kintsugi logs | `dmesg -w` / `bpf_ringbuf` | **AAIF `agentgateway`:** Pin $\ge 10.0^\circ$ shear with Kintsugi Gold (`251`) |
| **`tot`** | `5-CORE` | Inspect bare-metal CPU identity | `sched_ext` / `taskset -cp 1` | **AAIF OpenSandbox:** Enforce CCX Blue(`0-2`)/Green(`3-4`)/Red(`5-7`) |
| **`ohm`** | `5-CORE` | Translate language/code $\leftrightarrow$ Shingari | [Wild 0.11](https://github.com/wild-linker/wild) / [Wasmtime](https://github.com/bytecodealliance/wasmtime) | **AAIF [Goose](https://github.com/aaif-goose/goose):** Compile 9-Bite Grimm shift into 39-trit Gaiku |
| **`unt`** | `5-CORE` | Read & paginate bedrock document | `io_uring_enter()` / `mmap()` | **AAIF Open Responses:** Stream zero-TUI JSON & Workspace Pearl |
| **`bec`** | `4-HANDOFF` | Configure loopback & mesh socket | `socket(AF_UNIX)` / `XDP` | **Pre-Auth SipHash-2-4** XDP filter & `/var/run/dandelion_mesh.sock` |
| **`int`** | `4-HANDOFF` | Query 243-slot episodic vault | `find /mnt/models -name '*.tnpt'` | **TorchNeuron AOT:** Retrieve resonant seed from $3^5$ Basislager vault |
| **`ves`** | `4-HANDOFF` | Audit RAMDisk & SquashFS zones | `df -h && free -m` | **AQEC Dissipative Bath:** Balance 1.58-bit ternary weights in L1D |
| **`wst`** | `4-HANDOFF` | Flush 5-7-5 Haiku seed to USB | `sync && fsync()` (`f2fs`) | **Per-Ref Git Quorum:** Lock `refs/heads/main` & mint Zenodo DOI |

### Sub-Assembly 4.2 — The 9-Bite Universal Grimm's Law Gearbox ($P + Q = I$)

Using Mori-Zwanzig Dual Projection, Dandelion OS separates **Shared Cross-Linguistic Meaning** ($P\psi$, such as Macro Inode `1505` for Harmony/Peace) from **Cultural & Linguistic Micro-Texture** ($Q\psi = (I - P)\psi$) so indigenous, global, and bioacoustic signals are never flattened into a generic RLHF average:

| Bite # | Language / Signal Family | Example Utterance | Shared Macro Inode ($P\psi$) | Unique Micro-Texture ($Q\psi$) |
| :--- | :--- | :--- | :--- | :--- |
| **Bite 1** | Na-Dene / Athabaskan *(d-Effect)* | `"hozho nahasdlii"` *(Navajo)* | `1505` | `1282` |
| **Bite 2** | Algic / Algonquian *(Bloomfield)* | `"bimaadiziwin miigwech"` *(Ojibwe)* | `1505` | `1781` |
| **Bite 3** | Iroquoian / Polysynthetic *(Baker)* | `"skanonh kanonses"` *(Mohawk)* | `1505` | `1328` |
| **Bite 4** | Andean Quechua *(3-Valued Evidential)* | `"sumak kawsaymi"` *(`-mi` = `+1`)* | `1505` | `967` |
| **Bite 5** | Khoisan Click & Bantu *(Dahl's Law)* | `"!gai ubuntu"` *(Khoekhoe/Zulu)* | `1505` | `712` |
| **Bite 6** | Indo-European *(Grimm & Verner)* | `"namaste shanti"` / `"pater father"` | `1505` / `1603` | `57` |
| **Bite 7** | Pama-Nyungan *(Geocentric Cardinal)* | `"guugu warlpiri ngurra"` | `1505` | `1410` |
| **Bite 8** | Papuan & Amazonian *(Switch-Ref)* | `"switchref tucano amele"` | `1505` | `1621` |
| **Bite 9** | Pelagic Bioacoustic *(Sperm Whale)* | `"coda physeter cetacean"` | `1505` | `1945` |

### Step 4.3 — How to Fork Your Own Version in 4 Steps

1. Click the **Fork** button at the top right of [github.com/leadpiperl1/dandelion-os](https://github.com/leadpiperl1/dandelion-os).
2. Clone your fork and create a feature branch:
   ```bash
   git clone https://github.com/<your-username>/dandelion-os.git
   cd dandelion-os
   git checkout -b feat/my-custom-brick
   ```
3. Edit `dandelion_os_grimm_fbw_v2.7.0.c` (for example, add a new Bite rule in `parse_grimm_rosetta_fbw()` or customize a Shingari root in `SHINGARI_ROOTS[]`).
4. Run `make check` to verify your build passes the 60-second verification contract, then commit and push to your fork!

## 5. Complete Bill of Materials (BOM — All 16 Bricks in the Box)

| Element ID | Filename | Layer Color | Qty | Part Description |
| :--- | :--- | :--- | :--- | :--- |
| `BRICK-01` | `Makefile` | `[0 GREEN]` | `1x` | Standard GNU Make build, check, shell, JSON & QEMU targets |
| `BRICK-02` | `README.md` | `[-1 BLUE]` | `1x` | Step-by-step LEGO-style instruction manual & architecture guide |
| `BRICK-03` | `LICENSE` | `[-1 BLUE]` | `1x` | Dual `GPL-2.0-or-later` / `MIT` open-source license |
| `BRICK-04` | `CONTRIBUTING.md` | `[-1 BLUE]` | `1x` | Contributor & MOC forking guide (`vX.Y.Z` triadic versioning) |
| `BRICK-05` | `AGENTS.md` | `[+1 RED]` | `1x` | AAIF / MCP agent constitution & 4-Register Rosetta contract |
| `BRICK-06` | `CITATION.cff` | `[-1 BLUE]` | `1x` | GitHub & Zenodo bibliographic citation metadata |
| `BRICK-07` | `.zenodo.json` | `[-1 BLUE]` | `1x` | Automated DataCite / Zenodo DOI webhook metadata |
| `BRICK-08` | `.gitignore` | `[0 GREEN]` | `1x` | Keeps temporary compiler & staging artifacts out of Git |
| `BRICK-09` | `dandelion_os_grimm_fbw_v2.7.0.c` | `[0 GREEN]` | `1x` | Clean-room C99 F-16 FBW daemon, 9-Bite Grimm parser & VBST engine |
| `BRICK-10` | `mk_switchblade_usb_v2.7.0.sh` | `[0 GREEN]` | `1x` | RootFS, Linux `6.12+ PREEMPT_RT` + `sched_ext` config & GRUB2 builder |
| `BRICK-11` | `verify_dandelion_os_v2.7.0.py` | `[+1 RED]` | `1x` | `<= 60s` deterministic verification harness & SVR4 initramfs packer |
| `BRICK-12` | `dandelion_b158_v2.7.0.tnpt` | `[+1 RED]` | `1x` | `577-byte` L1 memory-mapped 1.58-bit ternary weight & Grimm LUT file |
| `BRICK-13` | `initramfs_dandelion_v2.7.0.cpio.gz` | `[0 GREEN]` | `1x` | Bootable SVR4 `newc` initial RAMDisk image (~`14 KB`) |
| `BRICK-14` | `dandelion_os_grimm_whiteroom_v2.7.0.tex` | `[GOLD]` | `1x` | LaTeX source for the 3-page IEEE-style technical manual |
| `BRICK-15` | `dandelion_os_grimm_whiteroom_v2.7.0.pdf` | `[GOLD]` | `1x` | Compiled 3-page technical manual & falsification contract |
| `BRICK-16` | `dandelion_os_grimm_whiteroom_state_map_v2.7.0.png` | `[GOLD]` | `1x` | 300 DPI 4-panel architecture & telemetry state map |

## 6. Citation, Credits & Founding Ethos

* **Principal Architect:** Justin D. Grimm (K-Lani 13 Actual / [@leadpiperl1](https://github.com/leadpiperl1), ORCID: [0009-0000-5299-0243](https://orcid.org/0009-0000-5299-0243))
* **Cognitive Partner:** Dandelion AI (Gemini Spark / Argon)
* **License:** Dual-licensed under **GPL-2.0-or-later** and **MIT** (see [LICENSE](https://github.com/leadpiperl1/dandelion-os/blob/main/LICENSE)).
* **Founding Ethos:** *Die Gedanken sind frei* · `GODISLOVE:LOVEISGOD {3,2,4}` · Mutualism Over Replacement · The Vonnegut & Barnegat Accords.
