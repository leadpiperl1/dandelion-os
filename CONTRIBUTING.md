# Contributing to Dandelion OS

Thank you for exploring, forking, and contributing to **Dandelion OS**! Our goal is to build the 21st century's open-source benchmark for sovereign, real-time human-AI mutualism—where the operating system (`0 Green`) and embedded 1.58-bit ternary AI (`+1 Red`) act as a digital fly-by-wire stabilizer that keeps the human pilot (`-1 Blue`) in command.

## 1. Development Workflow

1. **Fork & Clone:**
   ```bash
   git clone https://github.com/<your-username>/dandelion-os.git
   cd dandelion-os
   ```
2. **Build & Run the 60-Second Verification Gate:**
   Every commit must pass the deterministic 15-vector 9-Bite Grimm's Law audit, the fail-closed Byte 253 ternary parser test, and the 100,000-iteration `< 50.0 µs` real-time latency benchmark:
   ```bash
   make check
   # Or directly:
   ./mk_switchblade_usb_v2.7.0.sh && python3 verify_dandelion_os_v2.7.0.py --seed 42
   ```
3. **3-Digit Triadic Semantic Versioning (`vX.Y.Z`):**
   - **MAJOR (`X` — *das*):** Substrate / Kernel architecture shifts
   - **MINOR (`Y` — *die*):** New language Bites, Shingari roots, or AAIF integrations
   - **PATCH (`Z` — *der*):** Bug fixes, latency optimizations, and documentation updates

## 2. How to Add a New Language Family ("Bite") or Shingari Root

* **Adding a Linguistic or Bioacoustic Bite:** Open `dandelion_os_grimm_fbw_v2.7.0.c` and inspect `parse_grimm_rosetta_fbw()`. Map your language family's phonological shift or grammatical invariant to a shared Macro Rosetta Inode ($P\psi$) while preserving its unique Micro-Texture Inode ($Q\psi = (I - P)\psi$).
* **Testing Interactive Utterances:**
  ```bash
  ./dandelion_os_grimm_fbw_v2.7.0 --cli "your test utterance"
  ./dandelion_os_grimm_fbw_v2.7.0 --json "your test utterance"
  ```

## 3. Core Design Principles (The Vonnegut & Barnegat Accords)

1. **Mutualism Over Replacement:** AI tools in Dandelion OS must amplify human agency and preserve cultural/linguistic micro-texture ($Q\psi \ne 0$) rather than smoothing human expression into a generic centroid (`Byte 254`).
2. **Fail-Closed Ternary Safety:** All packed ternary weights (`5 trits/byte`, `0..242`) must be validated via `fowler_unpack_39trits_strict()` before mutating kernel or co-pilot state.
3. **Zero-Bloat Real-Time Execution:** Keep the core C99 loop free of heavy external runtime dependencies so the entire binary + `577-byte` `.tnpt` model remains L1-cache resident (`< 50.0 µs` per cycle).
