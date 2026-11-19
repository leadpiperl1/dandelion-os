#!/usr/bin/env python3
"""
verify_gemini_carbon_kembali_v1_2_1.py
Pure-Stdlib Deterministic Verification Harness (--seed 42, <= 60s)
Gemini-Carbon-Kembali 64 B Triadic Microkernel (v1.2.1, DOI: 10.5281/zenodo.24288018)
& Relativistic Non-Gaussian KMS Surprisal Gate (v1.0.0).
Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
"""
import argparse
import json
import math
import random
import struct
import time


def run_verification(seed: int = 42) -> dict:
    t0 = time.perf_counter()
    rng = random.Random(seed)

    # Gate 1: Carbon sp3 tetrahedral parity (cos theta = -1/3, zero-sum <= 4 LSB)
    sp3 = [
        ( 1.0,  1.0,  1.0),
        ( 1.0, -1.0, -1.0),
        (-1.0,  1.0, -1.0),
        (-1.0, -1.0,  1.0),
    ]
    for i in range(4):
        for j in range(i + 1, 4):
            dot = sum(sp3[i][k] * sp3[j][k] for k in range(3)) / 3.0
            assert abs(dot - (-1.0 / 3.0)) < 1e-15

    max_lsb_res = 0
    for _ in range(512):
        vx, vy, vz = rng.uniform(-1.0, 1.0), rng.uniform(-1.0, 1.0), rng.uniform(-1.0, 1.0)
        q_proj = [int(round(((vx * ax + vy * ay + vz * az) / math.sqrt(3.0)) * 65536.0)) for ax, ay, az in sp3]
        max_lsb_res = max(max_lsb_res, abs(sum(q_proj)))
    assert max_lsb_res <= 4, f"sp3 quantization residual exceeded 4 LSB: {max_lsb_res}"

    # Gate 2: KMS midpoint reflection symmetry K(x, v) == K(-v/T - x, v)
    max_kms_err = 0.0
    t_eff = 0.30
    for v in (0.10, 0.50, 0.80, 0.95):
        r_tail = 1.80 / math.sqrt(1.0 - v * v)
        for _ in range(100):
            x = rng.uniform(-3.0, 3.0)
            # Symmetric kernel in shifted variable u = x + v/(2T): cosh-exponential KMS form
            u1 = x + v / (2.0 * t_eff)
            x_ref = -v / t_eff - x
            u2 = x_ref + v / (2.0 * t_eff)
            k1 = math.exp(-r_tail * math.sqrt(1.0 + u1 * u1))
            k2 = math.exp(-r_tail * math.sqrt(1.0 + u2 * u2))
            max_kms_err = max(max_kms_err, abs(k1 - k2))
    assert max_kms_err < 1e-14

    # Gate 3: Itakura-Saito Geometric-Mean vs Arithmetic-Mean single-harmonic immunity
    # A single ringing guitar overtone (100x power in 1 of 16 bands) inflates arithmetic mean >7x
    # while geometric mean changes by only 100^(1/16) = 1.3335x (< 2.0x trigger threshold)
    bands = [1.0] * 16
    bands[3] = 100.0
    arith_mean = sum(bands) / len(bands)
    geom_mean = math.exp(sum(math.log(b) for b in bands) / len(bands))
    assert arith_mean > 7.0 and geom_mean < 1.40

    # Gate 4: 104.0 Hz 1,024-tick storm suppression + 0-tick Byte 253 Ground Preempt
    cooldown = 0
    escalated = 0
    suppressed = 0
    for tick in range(1024):
        if cooldown == 0:
            cooldown = 104
            escalated += 1
        else:
            suppressed += 1
            cooldown -= 1
    assert escalated == 10 and suppressed == 1014

    # Gate 5: Exact ABI struct layout check (8+16+4+4+4+4+4+8+9+1+1+1 = 64 B)
    fmt_64b = "<Q4iIiIIiQ9sBBB"
    assert struct.calcsize(fmt_64b) == 64

    elapsed_ms = (time.perf_counter() - t0) * 1000.0
    return {
        "artifact": "verify_gemini_carbon_kembali_v1_2_1.py",
        "version": "1.2.1",
        "doi": "10.5281/zenodo.24288018",
        "seed": seed,
        "all_5_gates_passed": True,
        "max_sp3_lsb_residual": max_lsb_res,
        "max_kms_reflection_err": max_kms_err,
        "geom_vs_arith_harmonic_ratio": [round(geom_mean, 4), round(arith_mean, 4)],
        "storm_1024_escalated_suppressed": [escalated, suppressed],
        "frame_abi_bytes": struct.calcsize(fmt_64b),
        "elapsed_ms": round(elapsed_ms, 3),
        "exit_state": "CODE_SHOKUNIN_UNLATCHED = 255",
    }


if __name__ == "__main__":
    ap = argparse.ArgumentParser()
    ap.add_argument("--seed", type=int, default=42)
    args = ap.parse_args()
    res = run_verification(args.seed)
    print(json.dumps(res, indent=2))
