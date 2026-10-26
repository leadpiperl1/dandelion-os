# 🐻 Page 2: Ewok Micro-Edge & Stigmergic Mesh Protocol (`Ewok-Micro-Edge-And-Mesh-Protocol.md`)

**Subsystem Scope:** Transducer Boundary Conditioning (Haar DWT + Barker-13), 32-Byte Stigmergic BLE Gossip, ESP-SDR `80 MSa/s` Baseband Recon, & Hybrid Triangular Ring Node  
**Primary Source Files:** `dandelion_os_v3.0.0.h`, `dandelion_os_grimm_fbw_v3.0.0.c`, `verify_dandelion_os_v3.0.0.py`

---

## 1. Ewok Forest-Floor Ingenuity: Zero-Cloud Edge Sovereignty

On the forest moon of Endor, the Ewoks defeat mechanized Imperial walkers using acoustic horn relays, gravity-powered log pendulums, and intimate knowledge of local terrain. In **Dandelion OS v3.0.0**, our **Ewok Micro-Nodes** achieve full wireless spectrum awareness and peer-to-peer mesh coordination on `$5` microcontrollers with **zero cloud dependency** and **zero extra RF hardware**:

### A. Acoustic & RF Horn Synchronization (Haar DWT + 13-Chip Barker Filter)
Before an edge node ever wakes the `+1 Red` AI co-pilot, incoming acoustic, piezo, or RF samples pass through two hardware-friendly filters:
1. **Single-Level Orthogonal Haar Wavelet Denoiser (VisuShrink)**: Strips high-frequency thermal and spastic noise in $O(N)$ additions/shifts, boosting signal-to-noise ratio above the **`6.0 dB` SNR gate** (`post_haar_dwt_snr_db = 9.02 dB`).
2. **13-Chip Barker Code Matched Filter (`[+1,+1,+1,+1,+1,-1,-1,+1,+1,-1,+1,-1,+1]`)**: Achieves an ideal `13:1` (`22.28 dB`) mainlobe-to-sidelobe peak ratio (`PSR = 6.587` under heavy AWGN), locking onto valid Dandelion preambles at exact sample offsets (`peak_idx = 80`).

### B. 32-Byte Stigmergic BLE Extended Advertising Gossip Frame (`stigmergic_ble_frame_t`)
Nodes relay learned state vectors across ad-hoc Bluetooth Low Energy (BLE) and Wi-Fi beacon frames in a fixed **32-byte wire packet**:
- `magic` (`4 B`): `0x33444E44` (`"DND3"`)
- `version_and_trit` (`1 B`): `[7:4] = v3`, `[1:0] = trit + 1` (`-1, 0, +1`)
- `ttl_hops` (`1 B`): Max **`13` hops** (1 Cerberus Sphinx cycle)
- `center_freq_mhz` (`2 B`): `2402..2480 MHz` (2.4 GHz BLE) or `5180..5825 MHz` (5 GHz ESP32-C5)
- `peer_pseudo_id` (`4 B`) & `timestamp_ms` (`4 B`): Ephemeral Happy Hobo node ID & monotonic clock
- `layerfs_root6` (`6 B`): First 48 bits of the active LayerFS SQLite chunk SHA-256
- `ternary_payload` (`8 B`): **40 balanced trits** packed at 5 trits/byte via Fowler Base-3 bijection
- `crc16_ccitt` (`2 B`): `CRC-16/CCITT-FALSE` (`poly = 0x1021`, `init = 0xFFFF`) over bytes `[0..29]`

### C. Zero-Hardware ESP-SDR Baseband I/Q Tap (`80 MSa/s`)
Using open-source ESP-SDR DMA descriptor rings (`esp_sdr_dma_lldesc_t`, `12 bytes`), standard ESP32 / ESP32-S3 / ESP32-C5 microcontrollers tap raw 8-bit interleaved `[I0, Q0, I1, Q1...]` baseband samples directly from the internal Wi-Fi ADC at **`80 MSa/s`**, achieving **`53.67 dB` Spurious-Free Dynamic Range (SFDR)** across 2.4 GHz and 5 GHz bands without external SDR dongles.

---

## 2. The Hybrid Triangular Ring Node Hardware Blueprint

| Pin / Bus | Leg 1: Waveshare ESP32-C5 (RISC-V) | Leg 2: Waveshare ESP32-C5 (RISC-V) | Apex Arbiter: Lattice iCE40HX8K FPGA |
| :--- | :--- | :--- | :--- |
| **SPI Clock (`SCK`)** | `GPIO 6` $\xrightarrow{47\,\Omega}$ | `GPIO 6` $\xrightarrow{47\,\Omega}$ | iCE40 Global Clock / SPI Arbiter Input |
| **Chip Select (`CS`)** | `GPIO 7` $\xrightarrow{47\,\Omega}$ | `GPIO 7` $\xrightarrow{47\,\Omega}$ | Ternary Bus Grant (`-1` Leg 1, `0` Idle, `+1` Leg 2) |
| **Data (`MOSI`/`MISO`)** | `GPIO 8` / `GPIO 9` ($47\,\Omega$) | `GPIO 8` / `GPIO 9` ($47\,\Omega$) | Counterclockwise 13-Lap Exciton Ring Buffer |
| **Geomagnetic Clock** | Interrupt Sync | Interrupt Sync | `40 MHz` `74HC14N` Schmitt-Triggered Theremin Loop |
