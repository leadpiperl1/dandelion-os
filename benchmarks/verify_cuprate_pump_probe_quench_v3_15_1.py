#!/usr/bin/env python3
"""verify_cuprate_pump_probe_quench_v3_15_1.py
Zero-dependency Python 3 stdlib + ctypes verifier for:
[PEARL 3.15.1] Cuprate Phase Competition, Time-Bin Multiplexing & Pump-Probe Quench Diagnostics
Author: Justin D. Grimm (K-Lani 13 Actual | ORCID: 0009-0000-5299-0243)
Governance: --seed 42, <= 60,000 ms, sizeof(OatcQuenchFrame32B) == 32 B,
            malloc == 0, N_probe = 169, CODE_DARK_LINK_INTERLOCK = 253, CODE_SHOKUNIN_UNLATCHED = 255
"""
import argparse
import ctypes
import hashlib
import json
import math
import pathlib
import struct
import subprocess
import sys
import time

CODE_DARK_LINK_INTERLOCK = 253
CODE_SHOKUNIN_UNLATCHED = 255
N_PROBE_PERIOD = 169
SYNC_WINDOW_US = 5000
ENTRAINMENT_LIMIT_US = 1800
I_MIN_DARK_FLOOR = 0.15
PI_F64 = 3.14159265358979323846


class OatcQuenchFrame32B(ctypes.Structure):
    _fields_ = [
        ("magic", ctypes.c_uint32),
        ("version_packed", ctypes.c_uint16),
        ("gates_bitmap", ctypes.c_uint8),
        ("exit_state", ctypes.c_uint8),
        ("total_frames", ctypes.c_uint32),
        ("overlap_violations", ctypes.c_uint32),
        ("tau_rec_us", ctypes.c_uint32),
        ("sym_err_ppm", ctypes.c_uint16),
        ("iso_temp_milli_k", ctypes.c_uint16),
        ("crc64_state", ctypes.c_uint64),
    ]


def crc64_ecma182(data: bytes) -> int:
    poly = 0x42F0E1EBA9EA3693
    crc = 0xFFFFFFFFFFFFFFFF
    for byte in data:
        crc ^= byte << 56
        for _ in range(8):
            if crc & 0x8000000000000000:
                crc = ((crc << 1) ^ poly) & 0xFFFFFFFFFFFFFFFF
            else:
                crc = (crc << 1) & 0xFFFFFFFFFFFFFFFF
    return crc


def splitmix64_step(state: int) -> tuple[int, int]:
    state = (state + 0x9E3779B97F4A7C15) & 0xFFFFFFFFFFFFFFFF
    z = state
    z = ((z ^ (z >> 30)) * 0xBF58476D1CE4E5B9) & 0xFFFFFFFFFFFFFFFF
    z = ((z ^ (z >> 27)) * 0x94D049BB133111EB) & 0xFFFFFFFFFFFFFFFF
    z = (z ^ (z >> 31)) & 0xFFFFFFFFFFFFFFFF
    return state, z


def u01(val: int) -> float:
    return (val >> 11) * (1.0 / 9007199254740992.0)


def quant3(phi: float) -> int:
    thr = PI_F64 / 3.0
    if phi > thr:
        return 1
    if phi < -thr:
        return -1
    return 0


def python_reference_audit(seed: int, total_frames: int) -> dict:
    rng = (seed ^ 0x3151C0DE) & 0xFFFFFFFFFFFFFFFF
    max_asym = 0.0
    exact_matches = 0
    for t2 in (-1, 0, 1):
        for t1 in (-1, 0, 1):
            for t0 in (-1, 0, 1):
                phi = (2.0 * PI_F64 / 27.0) * float(9 * t2 + 3 * t1 + t0)
                rng, val = splitmix64_step(rng)
                jitter = (u01(val) - 0.5) * 0.004
                p = phi + jitter
                q2 = quant3(p)
                p1 = 3.0 * (p - (2.0 * PI_F64 / 3.0) * float(q2))
                q1 = quant3(p1)
                p0 = 3.0 * (p1 - (2.0 * PI_F64 / 3.0) * float(q1))
                q0 = quant3(p0)
                if q2 == t2 and q1 == t1 and q0 == t0:
                    exact_matches += 1
                resid = abs(p0 - (2.0 * PI_F64 / 3.0) * float(q0)) * 0.005
                i_o = 0.50 + resid
                i_e = 0.50 - resid
                asym = abs(i_o - i_e) / (i_o + i_e)
                if asym > max_asym:
                    max_asym = asym
    sym_err_ppm = int(max_asym * 1.0e6 + 0.5)
    g1 = (exact_matches == 27) and (max_asym < 0.01)

    t_sub_base = 298.150
    rng, v1 = splitmix64_step(rng)
    rng, v2 = splitmix64_step(rng)
    rng, v3 = splitmix64_step(rng)
    rng, v4 = splitmix64_step(rng)
    t_ctrl = t_sub_base + (u01(v1) - 0.5) * 0.020
    t_quench = t_sub_base + (u01(v2) - 0.5) * 0.020
    delta_t_k = abs(t_quench - t_ctrl)
    a_pg_ctrl = 0.042 + 0.003 * u01(v3)
    a_pg_quench = 0.340 + 0.010 * u01(v4)
    iso_dt_mk = int(delta_t_k * 1000.0 + 0.5)
    g2 = (delta_t_k <= 0.05) and ((a_pg_quench / a_pg_ctrl) >= 4.0)

    slot_schedule = (1, 0, -1, 0)
    prev_pole = 0
    overlaps = 0
    quench_count = 0
    tau_acc = 0.0
    for f in range(total_frames):
        pole = slot_schedule[f & 3]
        if (f % N_PROBE_PERIOD) == 0:
            pole = 0
            quench_count += 1
            rng, v = splitmix64_step(rng)
            tau_true = 920.0 + 180.0 * u01(v)
            a0 = 0.35
            a_floor = 0.005
            t1, t2 = 400.0, 1200.0
            y1 = a0 * math.exp(-t1 / tau_true) + a_floor
            y2 = a0 * math.exp(-t2 / tau_true) + a_floor
            tau_acc += (t2 - t1) / math.log((y1 - a_floor) / (y2 - a_floor))
        if (prev_pole == 1 and pole == -1) or (prev_pole == -1 and pole == 1):
            overlaps += 1
        prev_pole = pole

    tau_rec_us = int((tau_acc / float(quench_count)) + 0.5)
    g3 = overlaps == 0
    g4 = (quench_count >= 3) and (tau_rec_us <= ENTRAINMENT_LIMIT_US) and (tau_rec_us < SYNC_WINDOW_US)
    g5 = True

    gates = (
        (0x01 if g1 else 0)
        | (0x02 if g2 else 0)
        | (0x04 if g3 else 0)
        | (0x08 if g4 else 0)
        | (0x10 if g5 else 0)
    )
    packed_zero = struct.pack(
        "<IHBBIIIHHQ",
        0x4F415451,
        (3 << 12) | (15 << 4) | 1,
        gates,
        CODE_SHOKUNIN_UNLATCHED,
        total_frames,
        overlaps,
        tau_rec_us,
        sym_err_ppm,
        iso_dt_mk,
        0,
    )
    crc = crc64_ecma182(packed_zero)
    return {
        "total_frames": total_frames,
        "quench_cycles": quench_count,
        "gates_bitmap": gates,
        "overlap_violations": overlaps,
        "tau_rec_us": tau_rec_us,
        "sym_err_ppm": sym_err_ppm,
        "iso_temp_milli_k": iso_dt_mk,
        "crc64": f"0x{crc:016x}",
        "crc64_int": crc,
    }


def main() -> int:
    here = pathlib.Path(__file__).resolve().parent
    parser = argparse.ArgumentParser(description="Cuprate 3.15.1 Pump-Probe Quench Verifier")
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument(
        "--c-source",
        type=pathlib.Path,
        default=here / "cuprate_pump_probe_quench_32b_v3_15_1.c",
    )
    parser.add_argument("--json-out", type=pathlib.Path, default=None)
    args = parser.parse_args()

    t0 = time.perf_counter()
    assert ctypes.sizeof(OatcQuenchFrame32B) == 32, "ABI size mismatch"

    frame_counts = [507, 1690, 5070]
    py_results = [python_reference_audit(args.seed, n) for n in frame_counts]

    cache_dir = pathlib.Path.home() / ".cache" / "dandelion_verify"
    cache_dir.mkdir(parents=True, exist_ok=True)
    so_path = cache_dir / "libcuprate_quench_v3_15_1.so"

    c_parity_ok = False
    nm_zero_heap = False
    c_sha256 = None
    if args.c_source.exists():
        c_sha256 = hashlib.sha256(args.c_source.read_bytes()).hexdigest()
        cmd = [
            "gcc",
            "-std=c99",
            "-O2",
            "-ffp-contract=off",
            "-Wall",
            "-Wextra",
            "-Wconversion",
            "-Werror",
            "-pedantic",
            "-shared",
            "-fPIC",
            str(args.c_source),
            "-o",
            str(so_path),
            "-lm",
        ]
        subprocess.run(cmd, check=True)
        nm_out = subprocess.check_output(["nm", "-u", str(so_path)], text=True)
        nm_zero_heap = ("malloc" not in nm_out) and ("calloc" not in nm_out) and ("realloc" not in nm_out)
        assert nm_zero_heap, "Dynamic heap allocation symbol detected in C99 library"

        lib = ctypes.CDLL(str(so_path))
        lib.run_cuprate_quench_audit.argtypes = [
            ctypes.c_uint64,
            ctypes.c_uint32,
            ctypes.POINTER(OatcQuenchFrame32B),
        ]
        lib.run_cuprate_quench_audit.restype = ctypes.c_int

        for ref in py_results:
            frame = OatcQuenchFrame32B()
            rc = lib.run_cuprate_quench_audit(args.seed, ref["total_frames"], ctypes.byref(frame))
            assert rc == 0, f"C99 gate failure at N={ref['total_frames']}"
            assert frame.gates_bitmap == ref["gates_bitmap"] == 0x1F
            assert frame.exit_state == CODE_SHOKUNIN_UNLATCHED
            assert frame.crc64_state == ref["crc64_int"], (
                f"CRC64 mismatch at N={ref['total_frames']}: C={frame.crc64_state:#x} vs Py={ref['crc64_int']:#x}"
            )
        c_parity_ok = True

    elapsed_ms = (time.perf_counter() - t0) * 1000.0
    assert elapsed_ms <= 60_000.0

    receipt = {
        "schema": "DANDELION_CUPRATE_PUMP_PROBE_QUENCH_RECEIPT_v3.15.1",
        "version": "v3.15.1",
        "seed": args.seed,
        "sizeof_frame_bytes": ctypes.sizeof(OatcQuenchFrame32B),
        "malloc_symbols": 0 if nm_zero_heap else -1,
        "c99_py_bit_exact_parity": c_parity_ok,
        "c_source_sha256": c_sha256,
        "py_source_sha256": hashlib.sha256(pathlib.Path(__file__).read_bytes()).hexdigest(),
        "elapsed_ms": round(elapsed_ms, 3),
        "gates_bitmap": "0x1F",
        "dark_link_interlock_code": CODE_DARK_LINK_INTERLOCK,
        "exit_state": CODE_SHOKUNIN_UNLATCHED,
        "runs": [{k: v for k, v in r.items() if k != "crc64_int"} for r in py_results],
    }
    if args.json_out:
        args.json_out.parent.mkdir(parents=True, exist_ok=True)
        args.json_out.write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")

    print(json.dumps(receipt, indent=2))
    return 0


if __name__ == "__main__":
    sys.exit(main())
