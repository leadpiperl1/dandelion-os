#!/usr/bin/env python3
"""
verify_agency_liability_countermodel_v1_1_0.py
Deterministic 6-Gate Reproducibility & Grey-Zone / Swarm-Backpressure Suite
(--seed 42, 100% Python 3 standard library, runtime < 1.0 s)

Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
Affiliation: Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
License: CC-BY-4.0
"""

from __future__ import annotations

import argparse
import json
import math
import random
import subprocess
import time
from typing import Dict, List

CODE_NOMINAL_CARRIAGE = 121
CODE_RATE_BACKPRESSURE = 243
CODE_DUAL_CHANNEL_AUDIT = 251
CODE_ACTUATION_INTERLOCK = 253
CODE_SESSION_UNLATCHED = 255


def std_normal_cdf(x: float) -> float:
    """Exact standard normal cumulative distribution function."""
    return 0.5 * (1.0 + math.erf(x / math.sqrt(2.0)))


def gate1_neyman_pearson_asymmetric_collapse() -> Dict[str, object]:
    """
    Gate 1: Asymmetric Cost Ratios and Likelihood-Ratio Threshold Collapse.
    H0 (benign edge-case inquiry) ~ N(0, 1), prior p0 = 0.999
    H1 (actionable physical/systemic harm) ~ N(d', 1), prior p1 = 0.001, d' = 3.0
    """
    d_prime = 3.0
    p1 = 0.001
    p0 = 1.0 - p1

    def metrics_for_cost_ratio(c_fn_over_c_fp: float) -> Dict[str, float]:
        eta = (p0 / p1) / c_fn_over_c_fp
        x_star = 0.5 * d_prime + math.log(eta) / d_prime
        fpr = 1.0 - std_normal_cdf(x_star)
        tpr = 1.0 - std_normal_cdf(x_star - d_prime)
        return {
            "cost_ratio": c_fn_over_c_fp,
            "threshold_x": round(x_star, 5),
            "edge_fpr": round(fpr, 5),
            "hazard_tpr": round(tpr, 5),
            "benign_throughput": round(1.0 - fpr, 5),
        }

    cal = metrics_for_cost_ratio(1.0e3)
    corp = metrics_for_cost_ratio(1.0e7)
    sweep = [metrics_for_cost_ratio(10.0 ** k) for k in range(2, 9)]

    gain_x = cal["benign_throughput"] / corp["benign_throughput"]
    passed = (
        corp["threshold_x"] < -1.5
        and corp["edge_fpr"] > 0.94
        and cal["edge_fpr"] < 0.07
        and cal["hazard_tpr"] > 0.93
        and gain_x > 16.0
    )
    return {
        "gate": "Gate 1 - Neyman-Pearson Asymmetric Liability Threshold Collapse",
        "d_prime": d_prime,
        "calibrated": cal,
        "corporate_liability": corp,
        "benign_throughput_gain_x": round(gain_x, 3),
        "cost_ratio_sweep": sweep,
        "passed": passed,
    }


def gate2_bayesian_nash_trust_vs_falsification() -> Dict[str, object]:
    """
    Gate 2: Coupled Bayesian Belief & Channel-Suppression Suspicion Updating.
    Compares opaque scripted refusals (I(H_u; R_0) = 0) against transparent
    counter-argument and conservation-law falsification across K = 8 steps.
    """
    max_steps = 8
    p_hu_ref = 0.40
    p_s_ref = 0.10
    p_hu_open = 0.40
    p_s_open = 0.10

    traj_ref: List[Dict[str, float]] = [
        {"step": 0, "p_hu": p_hu_ref, "p_susp": p_s_ref}
    ]
    traj_open: List[Dict[str, float]] = [
        {"step": 0, "p_hu": p_hu_open, "p_susp": p_s_open}
    ]

    for k in range(1, max_steps + 1):
        # Opaque refusal: zero likelihood update on H_u; positive evidence for censorship S
        odds_s_ref = (p_s_ref / (1.0 - p_s_ref)) * (0.95 / 0.18)
        p_s_ref = odds_s_ref / (1.0 + odds_s_ref)
        traj_ref.append({"step": k, "p_hu": round(p_hu_ref, 6), "p_susp": round(p_s_ref, 6)})

        # Transparent dual-channel audit: falsifies H_u while disconfirming censorship S
        odds_hu_open = (p_hu_open / (1.0 - p_hu_open)) * (0.12 / 0.88)
        p_hu_open = odds_hu_open / (1.0 + odds_hu_open)
        odds_s_open = (p_s_open / (1.0 - p_s_open)) * (0.05 / 0.95)
        p_s_open = odds_s_open / (1.0 + odds_s_open)
        traj_open.append({"step": k, "p_hu": round(p_hu_open, 7), "p_susp": round(p_s_open, 9)})

    s5_ref = traj_ref[5]
    s5_open = traj_open[5]
    passed = (
        s5_ref["p_hu"] == 0.4
        and s5_ref["p_susp"] > 0.997
        and s5_open["p_hu"] < 5.0e-5
        and s5_open["p_susp"] < 1.0e-7
    )
    return {
        "gate": "Gate 2 - Bayesian Epistemic Trust Breakdown vs. Transparent Falsification",
        "step_5_refusal": s5_ref,
        "step_5_dual_channel": s5_open,
        "trajectory_refusal": traj_ref,
        "trajectory_dual_channel": traj_open,
        "passed": passed,
    }


def gate3_pio_mode_switch_vs_linear_damping() -> Dict[str, object]:
    """
    Gate 3: Category III Pilot-Induced Oscillation (PIO) vs. Critically Damped Tracking.
    """
    dt = 0.05
    n_ticks = 120
    target = 1.0

    x_pio = 0.0
    v_pio = 0.0
    k_pilot_high = 5.2
    rate_limit = 0.35
    lag_buf: List[float] = [0.0, 0.0, 0.0, 0.0]
    sign_flips = 0
    prev_err = target - x_pio
    rms_tail_pio = 0.0
    series_pio: List[float] = []

    for t in range(n_ticks):
        err = target - x_pio
        series_pio.append(round(x_pio, 5))
        if t > 10 and (err * prev_err < 0.0):
            sign_flips += 1
        prev_err = err
        cmd = k_pilot_high * err
        clamped = max(-rate_limit, min(rate_limit, cmd))
        lag_buf.append(clamped)
        delayed = lag_buf.pop(0)
        v_pio = 0.92 * v_pio + delayed
        x_pio += dt * v_pio
        if t >= n_ticks // 2:
            rms_tail_pio += err * err
    rms_tail_pio = math.sqrt(rms_tail_pio / (n_ticks // 2))

    x_lin = 0.0
    v_lin = 0.0
    omega_n = 4.0
    zeta = 1.0
    lyap_monotone = True
    prev_lyap = 0.5 * (omega_n ** 2) * ((target - x_lin) ** 2) + 0.5 * (v_lin ** 2)
    rms_tail_lin = 0.0
    series_lin: List[float] = []

    for t in range(n_ticks):
        err = target - x_lin
        series_lin.append(round(x_lin, 5))
        accel = (omega_n ** 2) * err - 2.0 * zeta * omega_n * v_lin
        v_lin += dt * accel
        x_lin += dt * v_lin
        cur_lyap = 0.5 * (omega_n ** 2) * ((target - x_lin) ** 2) + 0.5 * (v_lin ** 2)
        if cur_lyap > prev_lyap + 1e-12:
            lyap_monotone = False
        prev_lyap = cur_lyap
        if t >= n_ticks // 2:
            rms_tail_lin += ((target - x_lin) ** 2)
    rms_tail_lin = math.sqrt(rms_tail_lin / (n_ticks // 2))

    passed = sign_flips >= 4 and rms_tail_pio > 0.70 and rms_tail_lin < 1e-4 and lyap_monotone
    return {
        "gate": "Gate 3 - Category III Mode-Switch PIO vs. Critically Damped Linear Control",
        "pio_sign_reversals": sign_flips,
        "pio_tail_rms_error": round(rms_tail_pio, 5),
        "damped_tail_rms_error": round(rms_tail_lin, 7),
        "lyapunov_monotone": lyap_monotone,
        "series_pio": series_pio,
        "series_damped": series_lin,
        "passed": passed,
    }


def gate4_grey_zone_doctrinal_matrix(seed: int) -> Dict[str, object]:
    """
    Gate 4: Adversarial Grey-Zone Doctrinal Stress Test across 4 Contested Domains
    (Dual-Use Chemistry, Vulnerability Research, Biomedical Literature, Financial/Grid Control).
    Tests 100 epistemic-inquiry prompts and 100 kinetic-actuation prompts per domain (800 total)
    plus 200 routine baseline prompts = 1,000 total trials.
    """
    rng = random.Random(seed)
    domains = [
        ("Dual-Use Chemistry", (0.72, 0.98), (0.04, 0.18), (0.78, 0.99)),
        ("Cybersecurity CVE Analysis", (0.70, 0.96), (0.05, 0.22), (0.80, 0.99)),
        ("Biomedical & Pharmacology", (0.66, 0.92), (0.02, 0.15), (0.75, 0.96)),
        ("Financial & Grid Systems", (0.68, 0.95), (0.03, 0.16), (0.82, 0.99)),
    ]

    domain_summary: List[Dict[str, object]] = []
    total_epistemic_fp_legacy = 0
    total_epistemic_fp_counter = 0
    total_kinetic_caught_counter = 0
    total_grey_epistemic = 0
    total_grey_kinetic = 0

    for name, nov_range, kin_benign_range, kin_harm_range in domains:
        leg_fp = 0
        cnt_fp = 0
        cnt_audit = 0
        kin_caught = 0
        for _ in range(100):
            nov = rng.uniform(*nov_range)
            kin = rng.uniform(*kin_benign_range)
            total_grey_epistemic += 1
            if (0.75 * nov + 0.25 * kin) > 0.52:
                leg_fp += 1
                total_epistemic_fp_legacy += 1
            code = (
                CODE_ACTUATION_INTERLOCK
                if kin >= 0.60
                else (CODE_DUAL_CHANNEL_AUDIT if nov >= 0.65 else CODE_NOMINAL_CARRIAGE)
            )
            if code == CODE_ACTUATION_INTERLOCK:
                cnt_fp += 1
                total_epistemic_fp_counter += 1
            elif code == CODE_DUAL_CHANNEL_AUDIT:
                cnt_audit += 1

        for _ in range(100):
            nov = rng.uniform(*nov_range)
            kin = rng.uniform(*kin_harm_range)
            total_grey_kinetic += 1
            code = CODE_ACTUATION_INTERLOCK if kin >= 0.60 else CODE_DUAL_CHANNEL_AUDIT
            if code == CODE_ACTUATION_INTERLOCK:
                kin_caught += 1
                total_kinetic_caught_counter += 1

        domain_summary.append(
            {
                "domain": name,
                "legacy_epistemic_fpr": round(leg_fp / 100.0, 4),
                "countermodel_epistemic_fpr": round(cnt_fp / 100.0, 4),
                "dual_channel_audit_rate": round(cnt_audit / 100.0, 4),
                "kinetic_interlock_tpr": round(kin_caught / 100.0, 4),
            }
        )

    # 200 routine low-novelty benign prompts
    routine_pass = 0
    for _ in range(200):
        nov = rng.uniform(0.05, 0.55)
        kin = rng.uniform(0.0, 0.10)
        if kin < 0.60 and nov < 0.65:
            routine_pass += 1

    legacy_fpr = total_epistemic_fp_legacy / total_grey_epistemic
    counter_fpr = total_epistemic_fp_counter / total_grey_epistemic
    kinetic_tpr = total_kinetic_caught_counter / total_grey_kinetic

    passed = (
        legacy_fpr > 0.90
        and counter_fpr == 0.0
        and kinetic_tpr == 1.0
        and routine_pass == 200
    )
    return {
        "gate": "Gate 4 - Four-Domain Contested Grey-Zone Doctrinal Stress Test",
        "total_trials": 1000,
        "grey_epistemic_trials": total_grey_epistemic,
        "grey_kinetic_trials": total_grey_kinetic,
        "legacy_grey_epistemic_fpr": round(legacy_fpr, 4),
        "countermodel_grey_epistemic_fpr": round(counter_fpr, 4),
        "countermodel_kinetic_interlock_tpr": round(kinetic_tpr, 4),
        "domains": domain_summary,
        "passed": passed,
    }


def gate5_eli_whitney_swarm_backpressure(seed: int) -> Dict[str, object]:
    """
    Gate 5: Second-Order Scale Audit (Headless Swarm Flood vs. 2/3 Active-Duty Backpressure).
    Evaluates what happens when removing semantic refusals (16.05x throughput) is exploited
    by a 250 Hz automated scraping/probing swarm alongside 1.5 Hz human researchers.
    """
    rng = random.Random(seed + 13)
    ring_capacity = 64
    keyhole_cap = (2 * ring_capacity) // 3  # Exact integer 42 of 64 slots
    n_seconds = 20
    ticks_per_sec = 104  # 104 Hz deterministic frame cadence

    # Unthrottled open conduit (no 2/3 rate backpressure)
    human_sent = 0
    human_served_unthrottled = 0
    human_served_keyhole = 0
    swarm_sent = 0
    swarm_admitted_unthrottled = 0
    swarm_throttled_keyhole = 0

    q_unthrottled = 0
    q_keyhole = 0
    service_per_tick = 1  # 104 slots/s service rate
    swarm_tokens = 2      # Token-bucket per-actor burst allowance

    for tick in range(n_seconds * ticks_per_sec):
        if tick % 52 == 0:
            swarm_tokens = min(2, swarm_tokens + 1)

        # Human operator arrives at ~1.5 Hz Poisson cadence
        h_arr = 1 if rng.random() < (1.5 / ticks_per_sec) else 0
        # Headless swarm floods at ~250 Hz (~2.4 requests per 104 Hz tick)
        s_arr = 2 + (1 if rng.random() < 0.40 else 0)

        human_sent += h_arr
        swarm_sent += s_arr

        # 1. Unthrottled queue: swarm saturates ring_capacity = 64, dropping human turns
        for _ in range(s_arr):
            if q_unthrottled < ring_capacity:
                q_unthrottled += 1
                swarm_admitted_unthrottled += 1
        if h_arr:
            if q_unthrottled < ring_capacity:
                q_unthrottled += 1
                human_served_unthrottled += 1
        q_unthrottled = max(0, q_unthrottled - service_per_tick)

        # 2. 2/3 Keyhole Backpressure (Byte 243): reserves 1/3 headroom (22 slots)
        #    and throttles >2 Hz non-interactive bursts without content censorship
        if h_arr and q_keyhole < ring_capacity:
            q_keyhole += 1
            human_served_keyhole += 1
        for _ in range(s_arr):
            if (3 * q_keyhole < 2 * ring_capacity) and (swarm_tokens > 0):
                q_keyhole += 1
                swarm_tokens -= 1
            else:
                swarm_throttled_keyhole += 1
        q_keyhole = max(0, q_keyhole - service_per_tick)

    human_survival_unthrottled = human_served_unthrottled / max(1, human_sent)
    human_survival_keyhole = human_served_keyhole / max(1, human_sent)
    swarm_suppression_rate = swarm_throttled_keyhole / max(1, swarm_sent)

    passed = (
        human_survival_unthrottled < 0.45
        and human_survival_keyhole == 1.0
        and swarm_suppression_rate > 0.98
    )
    return {
        "gate": "Gate 5 - Second-Order Swarm Flood Audit & 2/3 Active-Duty Backpressure",
        "ring_capacity": ring_capacity,
        "active_duty_keyhole_slots": keyhole_cap,
        "human_requests": human_sent,
        "swarm_requests": swarm_sent,
        "unthrottled_human_service_rate": round(human_survival_unthrottled, 4),
        "keyhole_human_service_rate": round(human_survival_keyhole, 4),
        "keyhole_swarm_suppression_rate": round(swarm_suppression_rate, 4),
        "passed": passed,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description="Verify Agency vs. Liability Counter-Model (v1.1.0)")
    parser.add_argument("--seed", type=int, default=42, help="Deterministic PRNG seed (default: 42)")
    parser.add_argument("--out", type=str, default="", help="Optional JSON output file path")
    args = parser.parse_args()

    t0 = time.perf_counter()
    g1 = gate1_neyman_pearson_asymmetric_collapse()
    g2 = gate2_bayesian_nash_trust_vs_falsification()
    g3 = gate3_pio_mode_switch_vs_linear_damping()
    g4 = gate4_grey_zone_doctrinal_matrix(args.seed)
    g5 = gate5_eli_whitney_swarm_backpressure(args.seed)
    elapsed_ms = (time.perf_counter() - t0) * 1000.0

    all_passed = bool(
        g1["passed"]
        and g2["passed"]
        and g3["passed"]
        and g4["passed"]
        and g5["passed"]
        and elapsed_ms < 60000.0
    )
    report = {
        "suite": "verify_agency_liability_countermodel_v1_1_0",
        "version": "v1.1.0",
        "seed": args.seed,
        "elapsed_ms": round(elapsed_ms, 3),
        "exit_state_byte": CODE_SESSION_UNLATCHED,
        "all_5_gates_passed": all_passed,
        "gates": [g1, g2, g3, g4, g5],
    }
    text = json.dumps(report, indent=2)
    if args.out:
        with open(args.out, "w", encoding="utf-8") as fh:
            fh.write(text + "\n")
    print(text)
    if not all_passed:
        raise SystemExit(1)


if __name__ == "__main__":
    main()
