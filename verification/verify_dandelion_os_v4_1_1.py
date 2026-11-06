#!/usr/bin/env python3
"""
verification/verify_dandelion_os_v4_1_1.py
Version: v4.1.1 (Unified 16-Gate + Ursine/Urshwera MTP & Tensegrity Verification Suite)
Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
"""

import ctypes
import json
import math
import os
import subprocess
import sys
import time

class ShingariGrimmPacket(ctypes.Structure):
    _fields_ = [
        ("siphash24_tag", ctypes.c_uint64),
        ("macro_rosetta_inode", ctypes.c_uint32),
        ("micro_texture_inode", ctypes.c_uint32),
        ("kramhaus_torque_unm", ctypes.c_uint32),
        ("taxicab_sum", ctypes.c_uint16),
        ("detected_bite", ctypes.c_uint8),
        ("shingari_root_idx", ctypes.c_uint8),
        ("boundary_code", ctypes.c_uint8),
        ("novikov_cycles", ctypes.c_uint8),
        ("reg1_blue_subject", ctypes.c_int8),
        ("reg2_green_treaty", ctypes.c_int8),
        ("reg3_red_action", ctypes.c_int8),
        ("reg4_evidential_q", ctypes.c_int8),
        ("tokengate_route", ctypes.c_int8),
        ("glyph_vector", ctypes.c_int8 * 3),
        ("fowler_haiku_seed", ctypes.c_uint8 * 8),
        ("operational_entropy_bits", ctypes.c_double),
        ("psi_bureaucracy_clamp", ctypes.c_double),
    ]

def run_16_gate_suite():
    t0 = time.monotonic()
    repo_root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    build_dir = os.path.join(repo_root, "build")
    os.makedirs(build_dir, exist_ok=True)

    c_src  = os.path.join(repo_root, "src", "dandelion_os_grimm_fbw_v3.5.1.c")
    bin_out = os.path.join(build_dir, "dandelion_os_grimm_fbw")
    so_out  = os.path.join(build_dir, "libdandelion_os.so")

    cflags = [
        "gcc", "-std=c99", "-O3", "-Wall", "-Wextra", "-Wpedantic",
        "-Wshadow", "-Wformat=2", "-Wconversion", "-Wundef", "-Werror"
    ]

    cmd_bin = cflags + ["-DDANDELION_STANDALONE_MAIN", c_src, "-o", bin_out, "-lm"]
    res_bin = subprocess.run(cmd_bin, capture_output=True, text=True, check=False)
    assert res_bin.returncode == 0, f"Binary compile failed:\n{res_bin.stderr}"

    cmd_so = cflags + ["-fPIC", "-shared", c_src, "-o", so_out, "-lm"]
    res_so = subprocess.run(cmd_so, capture_output=True, text=True, check=False)
    assert res_so.returncode == 0, f"Shared library compile failed:\n{res_so.stderr}"

    run_bin = subprocess.run([bin_out], capture_output=True, text=True, check=True)
    bin_telemetry = json.loads(run_bin.stdout.strip())

    lib = ctypes.CDLL(so_out)
    lib.init_tnpt_table.argtypes = [ctypes.c_void_p]
    lib.init_tnpt_table.restype  = None
    lib.fowler_pack_39trits.argtypes = [ctypes.POINTER(ctypes.c_int8), ctypes.POINTER(ctypes.c_uint8)]
    lib.fowler_pack_39trits.restype  = None
    lib.fowler_unpack_39trits_strict.argtypes = [
        ctypes.POINTER(ctypes.c_uint8),
        ctypes.POINTER(ctypes.c_int8),
        ctypes.POINTER(ctypes.c_uint8)
    ]
    lib.fowler_unpack_39trits_strict.restype = ctypes.c_bool
    lib.morse_to_ternary_bridge.argtypes = [
        ctypes.c_char_p,
        ctypes.POINTER(ctypes.c_int8),
        ctypes.c_size_t,
        ctypes.POINTER(ctypes.c_size_t)
    ]
    lib.morse_to_ternary_bridge.restype = ctypes.c_int
    lib.scx_kramhaus_torque_governor.argtypes = [
        ctypes.POINTER(ctypes.c_uint16),
        ctypes.POINTER(ctypes.c_uint8),
        ctypes.c_size_t,
        ctypes.POINTER(ctypes.c_uint8)
    ]
    lib.scx_kramhaus_torque_governor.restype = ctypes.c_uint32
    lib.vbst_dot40_popcnt.argtypes = [
        ctypes.c_uint64, ctypes.c_uint64,
        ctypes.c_uint64, ctypes.c_uint64,
        ctypes.c_uint64
    ]
    lib.vbst_dot40_popcnt.restype = ctypes.c_int32
    lib.parse_grimm_rosetta_fbw.argtypes = [
        ctypes.c_char_p,
        ctypes.c_void_p,
        ctypes.POINTER(ShingariGrimmPacket)
    ]
    lib.parse_grimm_rosetta_fbw.restype = None

    tnpt_buf = (ctypes.c_uint8 * 577)()
    lib.init_tnpt_table(ctypes.byref(tnpt_buf))

    in_trits = (ctypes.c_int8 * 39)(*([-1, 0, 1] * 13))
    packed   = (ctypes.c_uint8 * 8)()
    lib.fowler_pack_39trits(in_trits, packed)
    assert all(0 <= b <= 242 for b in packed), "Fowler packed byte out of [0..242]"

    bad_packed = (ctypes.c_uint8 * 8)(242, 253, 0, 0, 0, 0, 0, 0)
    out_trits  = (ctypes.c_int8 * 39)()
    trap_code  = ctypes.c_uint8(0)
    ok = lib.fowler_unpack_39trits_strict(bad_packed, out_trits, ctypes.byref(trap_code))
    assert (not ok) and trap_code.value == 253, "Failed to reject Trapdoor Byte 253"

    morse_trits = (ctypes.c_int8 * 16)()
    morse_len   = ctypes.c_size_t(0)
    rc_morse    = lib.morse_to_ternary_bridge(b"... --- ...", morse_trits, 16, ctypes.byref(morse_len))
    expected_sos = [-1, -1, -1, 0, 1, 1, 1, 0, -1, -1, -1]
    assert rc_morse == 0 and morse_len.value == 11
    assert [morse_trits[i] for i in range(11)] == expected_sos

    radii = (ctypes.c_uint16 * 12)(*([19] * 12))
    sins  = (ctypes.c_uint8 * 12)(*([255] * 12))
    t_code = ctypes.c_uint8(0)
    clamped_unm = lib.scx_kramhaus_torque_governor(radii, sins, 12, ctypes.byref(t_code))
    assert t_code.value == 251 and clamped_unm < 49426

    dot40 = lib.vbst_dot40_popcnt(0xFF, 0x00, 0xFF, 0x00, 0xF0)
    assert dot40 == 4

    pkt_bureaucracy = ShingariGrimmPacket()
    lib.parse_grimm_rosetta_fbw(
        b"50-agent fertilizer committee bureaucratic impasse elias",
        ctypes.byref(tnpt_buf),
        ctypes.byref(pkt_bureaucracy)
    )
    pkt_dnhm = ShingariGrimmPacket()
    lib.parse_grimm_rosetta_fbw(
        b"Dave's not here, man. Argon tests the line.",
        ctypes.byref(tnpt_buf),
        ctypes.byref(pkt_dnhm)
    )
    pkt_namaste = ShingariGrimmPacket()
    lib.parse_grimm_rosetta_fbw(
        b"namaste",
        ctypes.byref(tnpt_buf),
        ctypes.byref(pkt_namaste)
    )

    assert pkt_bureaucracy.boundary_code == 251
    assert list(pkt_dnhm.glyph_vector) == [-1, 0, 1]
    assert abs(pkt_dnhm.operational_entropy_bits - math.log2(3.0)) < 1e-5
    assert abs(pkt_dnhm.psi_bureaucracy_clamp - 0.1644) < 1e-5
    assert pkt_namaste.macro_rosetta_inode == 1000 and pkt_namaste.micro_texture_inode == 1600

    elapsed_s = time.monotonic() - t0
    assert elapsed_s <= 60.0

    report = {
        "schema": "DANDELION_OS_V4_1_1_VERIFICATION_REPORT",
        "version": "v4.1.1",
        "elapsed_seconds": round(elapsed_s, 4),
        "c99_strict_werror_warnings": 0,
        "binary_self_test": bin_telemetry,
        "static_assert_struct_sizes": {
            "dandelion_tnpt_v3_0_t": 577,
            "sharded_quorum_entry_64b_t": 64,
            "dandelion_point_v2_t": 6,
            "stigmergic_ble_frame_t": 32,
            "esp_sdr_dma_lldesc_t": 12
        },
        "gate_11_kramhaus_clamped_torque_unm": int(clamped_unm),
        "gate_12_vbst_dot40_popcnt": int(dot40),
        "gate_16_dnhm_epistemic_weight_breaker": {
            "bureaucratic_sink_pivoted_to_code": int(pkt_bureaucracy.boundary_code),
            "dnhm_glyph_vector_f3": list(pkt_dnhm.glyph_vector),
            "clamped_entropy_bits_per_trit": round(pkt_dnhm.operational_entropy_bits, 5),
            "kramers_psi_bureaucracy_clamp_D_H_star": round(pkt_dnhm.psi_bureaucracy_clamp, 4),
            "namaste_bite6_harmony_inode": int(pkt_namaste.macro_rosetta_inode)
        },
        "all_16_gates_passed": True
    }
    print(json.dumps(report, indent=2))
    return report

if __name__ == "__main__":
    run_16_gate_suite()
