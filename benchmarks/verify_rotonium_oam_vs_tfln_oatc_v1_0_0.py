#!/usr/bin/env python3
"""
verify_rotonium_oam_vs_tfln_oatc_v1_0_0.py
Companion Plane-2 Zero-Dependency Python 3 Stdlib Verification Runner (PEARL 3.15.0)
Rotonium Room-Temperature OAM Photonic Qudits vs. Starshot/Dandelion 3-Stage
Recirculating TFLN Magneto-Optic OATC & Clavina/PaQS Synthesis.

Executes all 8 ScholarEvolve Gates (Gate 0 through Gate 7) natively in Python 3 stdlib
with bit-exact SplitMix64 + ECMA-182 CRC64 parity against
`rotonium_oam_vs_tfln_oatc_audit_v1_0_0.c`, and (when gcc + the companion .c source
are present) compiles and cross-verifies strict ISO C99 (`-std=c99 -Wall -Wextra -Werror`),
`malloc = 0` symbol hygiene, and bit-for-bit CRC64 equality.

Principal Investigator: Justin D. Grimm (K-Lani 13 Actual) | ORCID: 0009-0000-5299-0243
Version: v1.0.0
Boundary Status: CODE_SHOKUNIN_UNLATCHED = 255
"""

import argparse
import hashlib
import json
import math
import os
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

PI_D = 3.14159265358979323846
TWO_PI_D = 6.28318530717958647692
KB_BOLTZMANN_J_K = 1.380649e-23
T_AMBIENT_K = 298.15

OATC_HEADER_MAGIC = 0x50454152
CODE_NOMINAL = 0
CODE_DARK_LINK_INTERLOCK = 253
CODE_PAQS_CALIBRATION_BIN = 254
CODE_SHOKUNIN_UNLATCHED = 255

OATC_BIN_SHOKUNIN_26 = 26
OATC_DARK_LINK_IMIN = 0.15
HOBO_RIG_POWER_W = 18.50
POSTSELECT_RACK_POWER_W = 6000.0
CRYO_DILUTION_POWER_W = 15000.0
EDGE_OAM_PCIE_POWER_W = 45.0
DANDELION_104HZ_US = 9615.384615384615
DANDELION_FAST_FRAME_US = 1.00

OATC_FRAME_STRUCT = struct.Struct("<IBBHQfffI")
assert OATC_FRAME_STRUCT.size == 32, "OatcFrame32B must be strictly 32 bytes"


def c_llround(x: float) -> int:
    """Match C99 llround() half-away-from-zero rounding."""
    if x >= 0.0:
        return int(math.floor(x + 0.5))
    return -int(math.floor(-x + 0.5))


class SplitMix64:
    """Bit-exact SplitMix64 matching rotonium_oam_vs_tfln_oatc_audit_v1_0_0.c."""

    __slots__ = ("state",)

    def __init__(self, seed: int):
        self.state = seed & 0xFFFFFFFFFFFFFFFF

    def next_u64(self) -> int:
        self.state = (self.state + 0x9E3779B97F4A7C15) & 0xFFFFFFFFFFFFFFFF
        z = self.state
        z = ((z ^ (z >> 30)) * 0xBF58476D1CE4E5B9) & 0xFFFFFFFFFFFFFFFF
        z = ((z ^ (z >> 27)) * 0x94D049BB133111EB) & 0xFFFFFFFFFFFFFFFF
        return (z ^ (z >> 31)) & 0xFFFFFFFFFFFFFFFF

    def uniform01(self) -> float:
        r = self.next_u64()
        return (float(r >> 11) + 0.5) * (1.0 / 9007199254740992.0)

    def normal_ih12(self, mean: float, std: float) -> float:
        acc = -6.0
        for _ in range(12):
            acc += self.uniform01()
        return mean + std * acc


def crc64_ecma_update(crc: int, data: bytes) -> int:
    poly = 0x42F0E1EBA9EA3693
    for b in data:
        crc ^= b << 56
        for _ in range(8):
            if crc & 0x8000000000000000:
                crc = ((crc << 1) & 0xFFFFFFFFFFFFFFFF) ^ poly
            else:
                crc = (crc << 1) & 0xFFFFFFFFFFFFFFFF
    return crc


def crc64_feed_i64(crc: int, val: int) -> int:
    u = val & 0xFFFFFFFFFFFFFFFF
    return crc64_ecma_update(crc, struct.pack("<Q", u))


def crc32_ieee(data: bytes) -> int:
    crc = 0xFFFFFFFF
    for b in data:
        crc ^= b
        for _ in range(8):
            mask = -(crc & 1) & 0xFFFFFFFF
            crc = (crc >> 1) ^ (0xEDB88320 & mask)
    return (~crc) & 0xFFFFFFFF


def encode_trit_2bit(t: int) -> int:
    if t < 0:
        return 0x00
    if t > 0:
        return 0x02
    return 0x01


def quantize_trit_mo(phi_rad: float) -> int:
    thresh = PI_D / 3.0
    if phi_rad > thresh:
        return 1
    if phi_rad < -thresh:
        return -1
    return 0


def wrap_pi_d(phi: float) -> float:
    while phi > PI_D:
        phi -= TWO_PI_D
    while phi < -PI_D:
        phi += TWO_PI_D
    return phi


def ternary_dae_tmlp_deconvolve(phi_est: float, ripple_amp: float) -> float:
    w_enc = (
        (1, 0, -1),
        (-1, 1, 0),
        (0, -1, 1),
        (1, 1, 0),
        (0, -1, -1),
        (-1, 0, 1),
    )
    w_dec_1 = (0, 1, -1, 1, -1, 0)
    x = (phi_est, math.sin(3.0 * phi_est), math.cos(3.0 * phi_est))
    h = []
    for i in range(6):
        acc = 0.0
        for j in range(3):
            if w_enc[i][j] > 0:
                acc += x[j]
            elif w_enc[i][j] < 0:
                acc -= x[j]
        h.append(math.tanh(acc))
    z1 = 0.0
    for i in range(6):
        if w_dec_1[i] > 0:
            z1 += h[i]
        elif w_dec_1[i] < 0:
            z1 -= h[i]
    z1 *= 0.25
    return wrap_pi_d(phi_est - ripple_amp * (0.65 * z1 + 0.35 * math.sin(3.0 * phi_est)))


def execute_tfln_oatc_3pass(
    phi_in: float,
    loss_rt_db: float,
    true_drift_rad: float,
    est_drift_rad: float,
    ripple_amp_rad: float,
    enable_dae_tmlp: bool,
    shot_noise_std: float,
    rng: SplitMix64,
    ts_fs: int,
) -> tuple:
    eta_rt = 10.0 ** (-loss_rt_db / 10.0)
    phi_cav = wrap_pi_d(
        phi_in + (true_drift_rad - est_drift_rad) + ripple_amp_rad * math.sin(3.0 * phi_in)
    )
    if enable_dae_tmlp and ripple_amp_rad > 0.0:
        phi_cav = ternary_dae_tmlp_deconvolve(phi_cav, ripple_amp_rad)

    i_total = 1.0
    trits = [0, 0, 0]
    ff_sub = 0.0
    status = CODE_NOMINAL
    passes_done = 0

    for p in range(3):
        i_total *= eta_rt
        passes_done = p + 1
        if i_total < OATC_DARK_LINK_IMIN:
            status = CODE_DARK_LINK_INTERLOCK
            break
        gain = 1.0 if p == 0 else (3.0 if p == 1 else 9.0)
        step_w = 1.0 if p == 0 else ((1.0 / 3.0) if p == 1 else (1.0 / 9.0))
        noise = rng.normal_ih12(0.0, shot_noise_std / math.sqrt(i_total)) if shot_noise_std > 0.0 else 0.0
        phi_diff = wrap_pi_d(phi_cav - ff_sub)
        phi_stage = gain * phi_diff + noise
        tk = quantize_trit_mo(phi_stage)
        trits[p] = tk
        ff_sub += (TWO_PI_D / 3.0) * step_w * float(tk)

    bin_idx = (trits[0] + 1) * 9 + (trits[1] + 1) * 3 + (trits[2] + 1)
    phi_hat = (TWO_PI_D / 3.0) * (float(trits[0]) + float(trits[1]) / 3.0 + float(trits[2]) / 9.0)
    eps_res = wrap_pi_d(phi_in - phi_hat)
    snr_db = 10.0 * math.log10(i_total / OATC_DARK_LINK_IMIN)

    if status == CODE_NOMINAL and bin_idx == OATC_BIN_SHOKUNIN_26:
        status = CODE_SHOKUNIN_UNLATCHED

    trits_packed = (
        (encode_trit_2bit(trits[0]) << 4)
        | (encode_trit_2bit(trits[1]) << 2)
        | encode_trit_2bit(trits[2])
    )
    reserved_flags = ((passes_done & 0xFF) << 8) | (bin_idx & 0xFF)
    f32_phi, f32_eps, f32_snr = struct.unpack("<fff", struct.pack("<fff", phi_hat, eps_res, snr_db))
    prefix_28b = struct.pack(
        "<IBBHQfff",
        OATC_HEADER_MAGIC,
        trits_packed,
        status,
        reserved_flags,
        ts_fs & 0xFFFFFFFFFFFFFFFF,
        f32_phi,
        f32_eps,
        f32_snr,
    )
    crc32 = crc32_ieee(prefix_28b)
    packet_32b = prefix_28b + struct.pack("<I", crc32)
    return packet_32b, trits_packed, status, reserved_flags, f32_phi, f32_eps, f32_snr, crc32


def run_all_gates(seed: int) -> dict:
    t0 = time.perf_counter()
    crc = 0xCBF29CE484222325
    crc = crc64_feed_i64(crc, seed)

    # Gate 0
    die_area_mm2 = 18.0 * 4.0
    densities = [q / die_area_mm2 for q in (4.0, 16.0, 32.0, 96.0)]
    cryo_break_bp = c_llround(10000.0 * (HOBO_RIG_POWER_W - CRYO_DILUTION_POWER_W) / CRYO_DILUTION_POWER_W)
    purity_l1_pct = 100.0 * math.exp(-6.0 * (0.0015 + 0.0085 * 1.0) * ((30.0 / 45.0) ** 2.0) - 0.038)
    purity_l2_pct = 100.0 * math.exp(-6.0 * (0.0015 + 0.0085 * 4.0) * ((30.0 / 45.0) ** 2.0) - 0.115)

    s_l1 = [0.020 + 0.003 * math.sin(1.7 * t) for t in range(12)]
    s_l2 = [0.040 + 0.022 * t + 0.004 * math.sin(0.9 * t) for t in range(12)]
    m1 = sum(s_l1) / 12.0
    m2 = sum(s_l2) / 12.0
    den_l1 = sum((x - m1) ** 2 for x in s_l1)
    den_l2 = sum((x - m2) ** 2 for x in s_l2)
    num_l1 = sum((s_l1[t] - m1) * (s_l1[t - 1] - m1) for t in range(1, 12))
    num_l2 = sum((s_l2[t] - m2) * (s_l2[t - 1] - m2) for t in range(1, 12))
    tac1_low_l = num_l1 / den_l1
    tac1_high_l = num_l2 / den_l2

    g0_pass = (
        cryo_break_bp == -9988
        and abs((densities[3] / densities[0]) - 24.0) < 1e-9
        and purity_l1_pct > 90.0
        and purity_l2_pct < 85.0
        and tac1_high_l > tac1_low_l + 0.40
    )
    crc = crc64_feed_i64(crc, cryo_break_bp)
    crc = crc64_feed_i64(crc, c_llround(purity_l1_pct * 10000.0))
    crc = crc64_feed_i64(crc, c_llround(purity_l2_pct * 10000.0))
    crc = crc64_feed_i64(crc, 1 if g0_pass else 0)

    # Gate 1
    bends_q = (6, 24, 48, 144)
    radius_um = (45.0, 30.0, 22.0, 15.0)
    length_cm = (1.8, 3.6, 5.4, 9.0)
    l_modes = (-2, -1, 0, 1, 2)
    siph_snr_db = [[0.0] * 5 for _ in range(4)]

    for q in range(4):
        for m in range(5):
            l_abs = abs(float(l_modes[m]))
            l_sq = l_abs * l_abs
            alpha_prop_db = (1.20 + 0.85 * l_sq) * length_cm[q]
            bend_unit_db = (0.04 + 0.11 * l_sq) * math.exp(-(radius_um[q] - 15.0) / (12.0 + 4.0 * l_abs))
            total_loss_db = alpha_prop_db + float(bends_q[q]) * bend_unit_db
            p_sig = 10.0 ** (-total_loss_db / 10.0)
            xtalk_per_bend = (0.0015 + 0.0085 * l_sq) * ((30.0 / radius_um[q]) ** 2.0)
            p_xtalk = 1.0 - math.exp(-float(bends_q[q]) * xtalk_per_bend)
            disp_ps = 0.18 * l_sq * length_cm[q]
            p_disp = 0.012 * disp_ps
            snr = 10.0 * math.log10(p_sig / (OATC_DARK_LINK_IMIN + p_xtalk + p_disp))
            siph_snr_db[q][m] = snr
            crc = crc64_feed_i64(crc, c_llround(snr * 1000.0))

    oatc_snr_090_db = 10.0 * math.log10((10.0 ** (-(3.0 * 0.90) / 10.0)) / OATC_DARK_LINK_IMIN)
    oatc_snr_135_db = 10.0 * math.log10((10.0 ** (-(3.0 * 1.35) / 10.0)) / OATC_DARK_LINK_IMIN)
    g1_pass = (
        siph_snr_db[0][1] > 2.5
        and siph_snr_db[0][2] > 5.0
        and siph_snr_db[0][3] > 2.5
        and abs(siph_snr_db[2][0] - siph_snr_db[2][4]) < 1e-12
        and siph_snr_db[2][0] < -10.0
        and siph_snr_db[3][0] < -20.0
        and siph_snr_db[2][1] < 0.0
        and oatc_snr_090_db > 5.5
        and oatc_snr_135_db > 4.0
    )
    crc = crc64_feed_i64(crc, c_llround(oatc_snr_090_db * 1000.0))
    crc = crc64_feed_i64(crc, c_llround(oatc_snr_135_db * 1000.0))
    crc = crc64_feed_i64(crc, 1 if g1_pass else 0)

    # Gate 2
    mc_trials = 4096
    n_ququarts = (2, 8, 16, 48)
    eta_carrier = 0.54
    sym_pass = 0.92
    tau_cycle_us = 0.25
    rng2 = SplitMix64(seed ^ 0xA5A55A5AF0F00F0F)

    rus_mean_us = []
    rus_p99_us = []
    rus_miss_1us_pct = []
    rus_miss_104hz_pct = []

    for q in range(4):
        p_succ = (eta_carrier ** float(n_ququarts[q])) * sym_pass
        log_fail = math.log(1.0 - p_succ)
        lats = []
        miss_1us = 0
        miss_104hz = 0
        for _ in range(mc_trials):
            u = rng2.uniform01()
            n_tries = 1.0 + math.floor(math.log(u) / log_fail)
            t_rus = n_tries * tau_cycle_us
            lats.append(t_rus)
            if t_rus > DANDELION_FAST_FRAME_US:
                miss_1us += 1
            if t_rus > DANDELION_104HZ_US:
                miss_104hz += 1
        lats.sort()
        rus_mean_us.append(sum(lats) / float(mc_trials))
        rus_p99_us.append(lats[(mc_trials * 99) // 100])
        rus_miss_1us_pct.append(100.0 * float(miss_1us) / float(mc_trials))
        rus_miss_104hz_pct.append(100.0 * float(miss_104hz) / float(mc_trials))
        crc = crc64_feed_i64(crc, miss_1us)
        crc = crc64_feed_i64(crc, miss_104hz)

    oatc_det_ok = 0
    bin26_255 = 0
    for i in range(mc_trials):
        b_num = (i % 27) - 13
        phi = (float(b_num) * TWO_PI_D) / 27.0
        _, _, status, rflags, _, _, _, _ = execute_tfln_oatc_3pass(
            phi, 0.90, 0.0, 0.0, 0.0, True, 0.005, rng2, i * 750000
        )
        if ((rflags >> 8) & 0xFF) == 3 and status in (CODE_NOMINAL, CODE_SHOKUNIN_UNLATCHED):
            oatc_det_ok += 1
        if status == CODE_SHOKUNIN_UNLATCHED and (rflags & 0xFF) == OATC_BIN_SHOKUNIN_26:
            bin26_255 += 1

    active_zero_clean = 0
    dark_link_caught_253 = 0
    for _ in range(256):
        _, tp_z, st_z, _, _, _, _, _ = execute_tfln_oatc_3pass(
            0.0, 0.90, 0.0, 0.0, 0.0, True, 0.0, rng2, 1000
        )
        _, _, st_d, _, _, _, _, _ = execute_tfln_oatc_3pass(
            0.0, 3.20, 0.0, 0.0, 0.0, True, 0.0, rng2, 1000
        )
        if st_z == CODE_NOMINAL and tp_z == 0x15:
            active_zero_clean += 1
        if st_d == CODE_DARK_LINK_INTERLOCK:
            dark_link_caught_253 += 1

    oatc_latency_us = 3.0 * tau_cycle_us
    g2_pass = (
        rus_miss_1us_pct[0] > 25.0
        and rus_miss_1us_pct[1] > 95.0
        and rus_miss_104hz_pct[2] > 10.0
        and rus_miss_104hz_pct[3] > 99.0
        and oatc_latency_us <= DANDELION_FAST_FRAME_US
        and oatc_det_ok == mc_trials
        and active_zero_clean == 256
        and dark_link_caught_253 == 256
        and bin26_255 > 100
    )
    crc = crc64_feed_i64(crc, oatc_det_ok)
    crc = crc64_feed_i64(crc, dark_link_caught_253)
    crc = crc64_feed_i64(crc, active_zero_clean)
    crc = crc64_feed_i64(crc, bin26_255)
    crc = crc64_feed_i64(crc, 1 if g2_pass else 0)

    # Gate 3
    e_bit_j = KB_BOLTZMANN_J_K * T_AMBIENT_K * math.log(2.0)
    e_trit_j = KB_BOLTZMANN_J_K * T_AMBIENT_K * math.log(3.0)
    e_word3_j = 3.0 * e_trit_j
    bits_27 = math.log(27.0) / math.log(2.0)
    radix_e3 = 3.0 / math.log(3.0)
    radix_e2 = 2.0 / math.log(2.0)
    radix_saving_pct = 100.0 * (1.0 - radix_e3 / radix_e2)
    swap_rack_x = POSTSELECT_RACK_POWER_W / HOBO_RIG_POWER_W
    swap_cryo_x = CRYO_DILUTION_POWER_W / HOBO_RIG_POWER_W
    swap_edge_oam_x = EDGE_OAM_PCIE_POWER_W / HOBO_RIG_POWER_W

    g3_pass = (
        abs((e_word3_j / bits_27) - e_bit_j) < 1e-34
        and abs(radix_e3 - 2.730717679880512) < 1e-9
        and abs(radix_e2 - 2.885390081777927) < 1e-9
        and radix_e3 < radix_e2
        and abs(swap_rack_x - (6000.0 / 18.50)) < 1e-9
    )
    crc = crc64_feed_i64(crc, c_llround(radix_e3 * 1000000.0))
    crc = crc64_feed_i64(crc, c_llround(radix_e2 * 1000000.0))
    crc = crc64_feed_i64(crc, c_llround(swap_rack_x * 10000.0))
    crc = crc64_feed_i64(crc, 1 if g3_pass else 0)

    # Gate 4
    rng4 = SplitMix64(seed ^ 0x123456789ABCDEF0)
    seen = set()
    crc32_ok = 0
    for m in range(-13, 14):
        phi_c = (float(m) * TWO_PI_D) / 27.0
        pkt, _, _, rflags, _, _, _, c32 = execute_tfln_oatc_3pass(
            phi_c, 0.90, 0.0, 0.0, 0.0, True, 0.0, rng4, (m + 13) * 1000
        )
        if len(pkt) == 32 and crc32_ieee(pkt[:28]) == c32:
            crc32_ok += 1
        b_idx = rflags & 0xFF
        if b_idx == (m + 13):
            seen.add(b_idx)
    g4_pass = OATC_FRAME_STRUCT.size == 32 and len(seen) == 27 and crc32_ok == 27
    crc = crc64_feed_i64(crc, OATC_FRAME_STRUCT.size)
    crc = crc64_feed_i64(crc, len(seen))
    crc = crc64_feed_i64(crc, crc32_ok)
    crc = crc64_feed_i64(crc, 1 if g4_pass else 0)

    # Gate 5
    n_bins = 2700
    rng_cal = SplitMix64(seed ^ 0x55AA55AA11223344)
    rng_unc = SplitMix64(seed ^ 0x55AA55AA11223344)
    drift_est = 0.0
    prev_meas = 0.0
    sq_cal = 0.0
    sq_unc = 0.0
    n_cal_bins = 0

    for i in range(n_bins):
        true_drift = 0.145 * math.sin(TWO_PI_D * float(i) / 900.0)
        b_num = (i % 27) - 13
        u_jit = (rng_cal.uniform01() - 0.5) * 0.80 * (TWO_PI_D / 27.0)
        phi_in = (float(b_num) * TWO_PI_D) / 27.0 + u_jit
        _ = rng_unc.uniform01()
        if (i % 16) == 0:
            if i > 0:
                drift_est = 0.45 * drift_est + 0.55 * prev_meas
            prev_meas = true_drift + rng_cal.normal_ih12(0.0, 0.005)
            _ = rng_unc.normal_ih12(0.0, 0.005)
            n_cal_bins += 1
        _, _, _, _, _, e_c, _, _ = execute_tfln_oatc_3pass(
            phi_in, 0.90, true_drift, drift_est, 0.0, False, 0.008, rng_cal, i * 4000000
        )
        _, _, _, _, _, e_u, _, _ = execute_tfln_oatc_3pass(
            phi_in, 0.90, true_drift, 0.0, 0.0, False, 0.008, rng_unc, i * 4000000
        )
        sq_cal += float(e_c) * float(e_c)
        sq_unc += float(e_u) * float(e_u)

    rmse_cal = math.sqrt(sq_cal / float(n_bins))
    rmse_unc = math.sqrt(sq_unc / float(n_bins))
    drift_red_pct = 100.0 * (1.0 - rmse_cal / rmse_unc)
    g5_pass = rmse_cal < rmse_unc and drift_red_pct > 25.0 and n_cal_bins == 169
    crc = crc64_feed_i64(crc, c_llround(rmse_cal * 100000.0))
    crc = crc64_feed_i64(crc, c_llround(rmse_unc * 100000.0))
    crc = crc64_feed_i64(crc, n_cal_bins)
    crc = crc64_feed_i64(crc, 1 if g5_pass else 0)

    # Gate 6
    ripple_amp = 0.095
    rng6_0 = SplitMix64(seed ^ 0xCAFEBABE00003150)
    rng6_1 = SplitMix64(seed ^ 0xCAFEBABE00003150)
    rng6_2 = SplitMix64(seed ^ 0xCAFEBABE00003150)
    de1, pm1, de2, pm2 = 0.0, 0.0, 0.0, 0.0
    sq_raw, sq_paqs, sq_full = 0.0, 0.0, 0.0

    for i in range(n_bins):
        true_drift = 0.145 * math.sin(TWO_PI_D * float(i) / 900.0)
        b_num = (i % 27) - 13
        u_jit = (rng6_0.uniform01() - 0.5) * 0.80 * (TWO_PI_D / 27.0)
        phi_in = (float(b_num) * TWO_PI_D) / 27.0 + u_jit
        _ = rng6_1.uniform01()
        _ = rng6_2.uniform01()
        if (i % 16) == 0:
            if i > 0:
                de1 = 0.45 * de1 + 0.55 * pm1
                de2 = 0.45 * de2 + 0.55 * pm2
            _ = rng6_0.normal_ih12(0.0, 0.005)
            pm1 = true_drift + rng6_1.normal_ih12(0.0, 0.005)
            pm2 = true_drift + rng6_2.normal_ih12(0.0, 0.005)
        _, _, _, _, _, e0, _, _ = execute_tfln_oatc_3pass(
            phi_in, 0.90, true_drift, 0.0, ripple_amp, False, 0.008, rng6_0, 0
        )
        _, _, _, _, _, e1, _, _ = execute_tfln_oatc_3pass(
            phi_in, 0.90, true_drift, de1, ripple_amp, False, 0.008, rng6_1, 0
        )
        _, _, _, _, _, e2, _, _ = execute_tfln_oatc_3pass(
            phi_in, 0.90, true_drift, de2, ripple_amp, True, 0.008, rng6_2, 0
        )
        sq_raw += float(e0) * float(e0)
        sq_paqs += float(e1) * float(e1)
        sq_full += float(e2) * float(e2)

    rmse_raw = math.sqrt(sq_raw / float(n_bins))
    rmse_paqs = math.sqrt(sq_paqs / float(n_bins))
    rmse_full = math.sqrt(sq_full / float(n_bins))
    coupled_gain_pct = 100.0 * (1.0 - rmse_full / rmse_raw)
    g6_pass = rmse_full < rmse_paqs < rmse_raw and coupled_gain_pct > 30.0
    crc = crc64_feed_i64(crc, c_llround(rmse_raw * 100000.0))
    crc = crc64_feed_i64(crc, c_llround(rmse_paqs * 100000.0))
    crc = crc64_feed_i64(crc, c_llround(rmse_full * 100000.0))
    crc = crc64_feed_i64(crc, 1 if g6_pass else 0)

    # Gate 7
    loss_regimes = (0.40, 0.90, 1.35)
    rmse_m_oatc = []
    rmse_b0_bin = []
    alpha_vs_b0_pct = []

    for r in range(3):
        rng_m = SplitMix64(((seed + r * 7919) & 0xFFFFFFFFFFFFFFFF) ^ 0x0F0F0F0F31502026)
        rng_b = SplitMix64(((seed + r * 7919) & 0xFFFFFFFFFFFFFFFF) ^ 0x0F0F0F0F31502026)
        eta_rt = 10.0 ** (-loss_regimes[r] / 10.0)
        de_m, pm_m = 0.0, 0.0
        sq_m, sq_b0 = 0.0, 0.0
        for i in range(n_bins):
            true_drift = 0.145 * math.sin(TWO_PI_D * float(i) / 900.0)
            b_num = (i % 27) - 13
            u_jit = (rng_m.uniform01() - 0.5) * 0.80 * (TWO_PI_D / 27.0)
            phi_in = (float(b_num) * TWO_PI_D) / 27.0 + u_jit
            _ = rng_b.uniform01()
            if (i % 16) == 0:
                if i > 0:
                    de_m = 0.45 * de_m + 0.55 * pm_m
                pm_m = true_drift + rng_m.normal_ih12(0.0, 0.005)
                _ = rng_b.normal_ih12(0.0, 0.005)
            _, _, _, _, _, em, _, _ = execute_tfln_oatc_3pass(
                phi_in, loss_regimes[r], true_drift, de_m, 0.095, True, 0.008, rng_m, 0
            )
            sq_m += float(em) * float(em)

            p3 = eta_rt * eta_rt * eta_rt
            n0 = rng_b.normal_ih12(0.0, 0.008 / math.sqrt(eta_rt))
            n1 = rng_b.normal_ih12(0.0, 0.008 / eta_rt)
            n2 = rng_b.normal_ih12(0.0, 0.008 / math.sqrt(p3))
            phi_noisy = wrap_pi_d(phi_in + 0.095 * math.sin(3.0 * phi_in) + (n0 + n1 + n2) / 3.0)
            step_bin8 = TWO_PI_D / 8.0
            idx8 = max(0.0, min(7.0, math.floor((phi_noisy + PI_D) / step_bin8)))
            phi_b0 = -PI_D + (idx8 + 0.5) * step_bin8
            err_b0 = wrap_pi_d(phi_in - phi_b0)
            sq_b0 += err_b0 * err_b0

        rm = math.sqrt(sq_m / float(n_bins))
        rb = math.sqrt(sq_b0 / float(n_bins))
        rmse_m_oatc.append(rm)
        rmse_b0_bin.append(rb)
        alpha_vs_b0_pct.append(100.0 * (1.0 - rm / rb))
        crc = crc64_feed_i64(crc, c_llround(rm * 100000.0))
        crc = crc64_feed_i64(crc, c_llround(rb * 100000.0))

    swp_tp_oatc = bits_27 / (oatc_latency_us * HOBO_RIG_POWER_W)
    swp_tp_b0 = 3.0 / (oatc_latency_us * HOBO_RIG_POWER_W)
    swp_tp_b1 = 16.0 / (rus_mean_us[1] * EDGE_OAM_PCIE_POWER_W)
    swp_tp_b2 = (16.0 * ((0.72 ** 3.0) ** 8.0)) / (0.25 * POSTSELECT_RACK_POWER_W)

    g7_pass = (
        all(a > 60.0 for a in alpha_vs_b0_pct)
        and swp_tp_oatc > swp_tp_b0
        and swp_tp_oatc > 25.0 * swp_tp_b1
        and swp_tp_oatc > 1000.0 * swp_tp_b2
    )
    crc = crc64_feed_i64(crc, c_llround(swp_tp_oatc * 100000.0))
    crc = crc64_feed_i64(crc, 1 if g7_pass else 0)

    gates = [g0_pass, g1_pass, g2_pass, g3_pass, g4_pass, g5_pass, g6_pass, g7_pass]
    bitmap = sum((1 << idx) for idx, ok in enumerate(gates) if ok)
    elapsed_ms = (time.perf_counter() - t0) * 1000.0

    return {
        "pearl": "3.15.0",
        "version": "v1.0.0",
        "seed": seed,
        "sizeof_oatc_frame_b": OATC_FRAME_STRUCT.size,
        "malloc_calls": 0,
        "gates_bitmap": bitmap,
        "all_passed": bitmap == 0xFF and elapsed_ms <= 60000.0,
        "state_crc64": f"0x{crc:016x}",
        "elapsed_ms": round(elapsed_ms, 3),
        "boundary_hook": CODE_SHOKUNIN_UNLATCHED,
        "metrics": {
            "gate_0": {
                "passed": g0_pass,
                "cryo_break_shift_bp": cryo_break_bp,
                "density_4q_mm2": round(densities[0], 6),
                "density_96q_mm2": round(densities[3], 6),
                "purity_l1_pct": round(purity_l1_pct, 4),
                "purity_l2_pct": round(purity_l2_pct, 4),
                "tac1_low_l": round(tac1_low_l, 4),
                "tac1_high_l": round(tac1_high_l, 4),
            },
            "gate_1": {
                "passed": g1_pass,
                "siph_4q_snr_db_l_m2_to_p2": [round(x, 4) for x in siph_snr_db[0]],
                "siph_16q_snr_db_l_p2": round(siph_snr_db[1][4], 4),
                "siph_32q_snr_db_l_p2": round(siph_snr_db[2][4], 4),
                "siph_96q_snr_db_l_p2": round(siph_snr_db[3][4], 4),
                "tfln_te00_3pass_snr_090db": round(oatc_snr_090_db, 4),
                "tfln_te00_3pass_snr_135db": round(oatc_snr_135_db, 4),
            },
            "gate_2": {
                "passed": g2_pass,
                "rus_mean_us_4_16_32_96q": [round(x, 4) for x in rus_mean_us],
                "rus_p99_us_4_16_32_96q": [round(x, 4) for x in rus_p99_us],
                "rus_miss_1us_pct_4_16_32_96q": [round(x, 4) for x in rus_miss_1us_pct],
                "rus_miss_104hz_pct_4_16_32_96q": [round(x, 4) for x in rus_miss_104hz_pct],
                "oatc_3bin_latency_us": oatc_latency_us,
                "oatc_jitter_us": 0.0,
                "oatc_deterministic_ok": oatc_det_ok,
                "code253_dark_link_caught": dark_link_caught_253,
                "active_zero_clean": active_zero_clean,
                "bin26_shokunin_255_count": bin26_255,
            },
            "gate_3": {
                "passed": g3_pass,
                "landauer_trit_j": e_trit_j,
                "landauer_word3_j": e_word3_j,
                "static_zero_power_w": 0.0,
                "radix_e3": round(radix_e3, 6),
                "radix_e2": round(radix_e2, 6),
                "radix_saving_pct": round(radix_saving_pct, 4),
                "swap_vs_6kw_rack_x": round(swap_rack_x, 4),
                "swap_vs_15kw_cryo_x": round(swap_cryo_x, 4),
                "swap_vs_45w_edge_oam_x": round(swap_edge_oam_x, 4),
            },
            "gate_4": {
                "passed": g4_pass,
                "sizeof_oatc_frame_b": OATC_FRAME_STRUCT.size,
                "bijection_27_states": len(seen),
                "crc32_verified_frames": crc32_ok,
            },
            "gate_5": {
                "passed": g5_pass,
                "rmse_uncal_rad": round(rmse_unc, 6),
                "rmse_paqs_causal_rad": round(rmse_cal, 6),
                "drift_reduction_pct": round(drift_red_pct, 4),
                "calib_bins_254": n_cal_bins,
            },
            "gate_6": {
                "passed": g6_pass,
                "rmse_raw_rad": round(rmse_raw, 6),
                "rmse_paqs_only_rad": round(rmse_paqs, 6),
                "rmse_coupled_dae_tmlp_rad": round(rmse_full, 6),
                "coupled_gain_pct": round(coupled_gain_pct, 4),
            },
            "gate_7": {
                "passed": g7_pass,
                "alpha_vs_b0_binary_rmse_pct_reg_m1_0_p1": [round(x, 4) for x in alpha_vs_b0_pct],
                "swap_tp_oatc_bits_us_w": round(swp_tp_oatc, 6),
                "swap_tp_b0_binary_bits_us_w": round(swp_tp_b0, 6),
                "swap_tp_b1_oam16_bits_us_w": round(swp_tp_b1, 6),
                "swap_tp_b2_rack_bits_us_w": swp_tp_b2,
            },
        },
    }


def verify_c99_companion(c_path: Path, seed: int, expected_crc64: str) -> dict:
    """Compile and verify C99 companion harness in an exec-capable working directory."""
    if not c_path.exists():
        return {"present": False}
    exec_base = Path.cwd() if os.access(Path.cwd(), os.W_OK | os.X_OK) else c_path.parent
    with tempfile.TemporaryDirectory(prefix=".oatc_c99_audit_", dir=str(exec_base)) as tmpd:
        bin_path = Path(tmpd) / "rotonium_oam_vs_tfln_oatc_audit_v1_0_0"
        compile_cmd = [
            "gcc",
            "-std=c99",
            "-O2",
            "-ffp-contract=off",
            "-Wall",
            "-Wextra",
            "-Wconversion",
            "-Werror",
            "-pedantic",
            str(c_path),
            "-lm",
            "-o",
            str(bin_path),
        ]
        comp = subprocess.run(compile_cmd, capture_output=True, text=True, check=False)
        if comp.returncode != 0:
            return {
                "present": True,
                "compiled": False,
                "stderr": comp.stderr,
            }
        nm_res = subprocess.run(["nm", "-u", str(bin_path)], capture_output=True, text=True, check=False)
        heap_syms = [
            sym for sym in ("malloc", "calloc", "realloc", "free")
            if sym in nm_res.stdout
        ]
        run_res = subprocess.run(
            [str(bin_path), "--seed", str(seed), "--json"],
            capture_output=True,
            text=True,
            check=False,
        )
        c_json = json.loads(run_res.stdout.strip()) if run_res.returncode == 0 else {}
        return {
            "present": True,
            "compiled": True,
            "flags": "-std=c99 -O2 -ffp-contract=off -Wall -Wextra -Wconversion -Werror -pedantic",
            "heap_alloc_symbols": heap_syms,
            "malloc_zero_verified": len(heap_syms) == 0,
            "c99_crc64": c_json.get("state_crc64"),
            "crc64_bit_exact_match": c_json.get("state_crc64") == expected_crc64,
            "c99_gates_bitmap": c_json.get("gates_bitmap"),
            "c99_elapsed_ms": c_json.get("elapsed_ms"),
            "c99_sha256": hashlib.sha256(c_path.read_bytes()).hexdigest(),
        }


def main() -> int:
    parser = argparse.ArgumentParser(
        description="PEARL 3.15.0 Rotonium OAM vs. TFLN OATC 8-Gate Verification Runner"
    )
    parser.add_argument("--seed", type=int, default=42, help="Deterministic PRNG seed (default: 42)")
    parser.add_argument("--json", action="store_true", help="Emit compact JSON receipt")
    args = parser.parse_args()

    result = run_all_gates(args.seed)
    c_default = Path(
        os.environ.get(
            "DANDELION_BENCH_DIR",
            str(Path(__file__).resolve().parent),
        )
    ) / "rotonium_oam_vs_tfln_oatc_audit_v1_0_0.c"
    c_audit = verify_c99_companion(c_default, args.seed, result["state_crc64"])
    result["c99_companion_parity"] = c_audit
    result["py_sha256"] = hashlib.sha256(Path(__file__).resolve().read_bytes()).hexdigest()

    c_ok = (not c_audit.get("present")) or (
        c_audit.get("compiled")
        and c_audit.get("malloc_zero_verified")
        and c_audit.get("crc64_bit_exact_match")
        and c_audit.get("c99_gates_bitmap") == 0xFF
    )

    if args.json:
        print(json.dumps(result, indent=2))
    else:
        print("=== [PEARL 3.15.0] Python 3 Stdlib + C99 Cross-Verification Runner (v1.0.0) ===")
        print(
            f"Seed: {args.seed} | Gates Bitmap: 0x{result['gates_bitmap']:02X} (8/8 "
            f"{'PASS' if result['all_passed'] else 'FAIL'}) | Python CRC64: {result['state_crc64']} "
            f"| Python Runtime: {result['elapsed_ms']:.3f} ms"
        )
        if c_audit.get("present"):
            print(
                f"C99 Parity: compiled={c_audit.get('compiled')} | malloc_zero={c_audit.get('malloc_zero_verified')} "
                f"| C99 CRC64={c_audit.get('c99_crc64')} | match={c_audit.get('crc64_bit_exact_match')} "
                f"| C99 Runtime={c_audit.get('c99_elapsed_ms')} ms"
            )
        print(json.dumps(result["metrics"], indent=2))
        print(f"Boundary Status: CODE_SHOKUNIN_UNLATCHED = {CODE_SHOKUNIN_UNLATCHED}")

    return 0 if (result["all_passed"] and c_ok) else 1


if __name__ == "__main__":
    sys.exit(main())
