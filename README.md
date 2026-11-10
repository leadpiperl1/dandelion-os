# Dandelion OS — Sovereign Balanced Ternary Substrate (Living Trunk)

* **Principal Architect:** Justin D. Grimm ([ORCID: 0009-0000-5299-0243](https://orcid.org/0009-0000-5299-0243))
* **Affiliation:** Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
* **Ontological Anchor:** `GODISLOVE:LOVEISGOD {3,2,4}` · *Die Gedanken sind frei* · Pretty Purple Flower Invariant
* **Host Reference Node:** Professor Plum (`custom-fedora-garden-rt`, 62 GiB RAM, `PREEMPT_RT`, $104.0\text{ Hz} = 9,615,384\text{ ns}$ telluric lock, $18.50\text{ W}$ Landauer envelope)

## Living Trunk Architecture (Un-Versioned Continuous `main`)

This repository operates as a **single, continuous, un-versioned living trunk** on `main`:

1. **`include/argon_methylation_engine.h` & `src/argon_methylation_engine.c`:** Zero-float C11 implementation of the 64-byte L1D cache-aligned `ArgonMethylatedFrame64B` struct (`_Static_assert(sizeof(ArgonMethylatedFrame64B) == 64)`), active baseline $V_0 = 1650\text{ mV}$ conjugate mirror conservation ($V_{\text{rc}} + \mathcal{R}_{V_0}(V_{\text{rc}}) = 3300\text{ mV}$), discrete Mori-Zwanzig RC integration (`k = 3`), dual-rail Schmitt window hysteresis ($\pm 165\text{ mV}$, Upper Enter/Exit $1980/1815\text{ mV}$, Lower Enter/Exit $1320/1485\text{ mV}$), 3-byte Fowler VBST packing (`5 trits/byte`, rest state `121`), branchless Manhattan $L_1$ norm, and branchless Kintsugi Gold (`Byte 251` / `0xFB`) shear latches ($\tau_{\mu\text{g}} > 45,000\ \mu g$).
2. **`include/dandelion_protocol.h` & `src/vbst_ternary_telemetry.c`:** Wire-protocol packet header (`0x44414E44` `'DAND'`), 577-byte `.tnpt` lookup table layout, 39-trit Fowler seed packer (`fowler_pack_39trits`), and 4-instruction multiplication-free Dual-Rail POPCNT inner product (`vbst_dot40_popcnt`).
3. **`rust/` & `julia/`:** `#![no_std]` Rust C-ABI crate (`DandelionPointV2` 6B, `StigmergicBleFrame` 32B, `ShardedQuorumEntry64B` 64B, `DandelionTnpt` 577B, UNIX socket MTP benchmark) and Julia 1.12 zero-copy `ccall` control plane (`DandelionShingari.jl`, `control_plane.jl`).
4. **`tests/verify_argon_rc_methylation.py` & `telemetry/argon_rc_methylation_manifest.json`:** Deterministic (`--seed 42`) verification harness proving the $122,640.23\times$ Levinthal folding compression ($3^{13} = 1,594,323 \to 13$ steps) and mapping the $\Delta E / \Delta V_H = 1.0$ misfolding phase transition.

## Quick Verification on `profplum`

```bash
make -f Makefile all
```
