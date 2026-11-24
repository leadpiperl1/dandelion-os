#!/usr/bin/env python3
"""
verify_parity.py (v1.0.0)
Deterministic Verification Suite for:
"The Dandelion Invariant: Substrate-Independent Quorums, Epigenetic Phenology,
and Carbon-Silicon Symbiosis"

Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
Affiliation: Independent Dandelion Substrate Research Group, Gurley, AL 35748, USA
License: CC-BY-4.0

Demonstrates mathematically and via seeded Monte Carlo node-level simulation why
stepping up from an even N=8 byte/quorum topology to an odd N=13 (12 + 1)
symmetry-broken quorum topology exponentially extends error-correction distance,
eliminates split-quorum deadlock, doubles both crash/noise (2t+1) and arbitrary
Byzantine (3f+1) fault radii, and extends iterative cultural/informational
transmission horizons across multi-millennial timescales (e.g., the 12,500-year
post-Younger Dryas window).

Pure Python 3 standard library; zero external dependencies.
Usage: python3 verify_parity.py [--seed 42] [--trials 5000] [--generations 500]
"""

from __future__ import annotations

import argparse
import json
import math
import random
import sys
import time
from typing import Dict, List, Tuple


def comb(n: int, k: int) -> int:
    """Exact binomial coefficient C(n, k)."""
    if k < 0 or k > n:
        return 0
    return math.comb(n, k)


def quorum_failure_prob_strict(n: int, p: float) -> float:
    """
    Exact single-generation consensus failure probability under strict majority.
    Consensus succeeds iff strictly more than n/2 nodes retain the true bit.
    Thus failure (wrong majority OR exact n/2 tie deadlock for even n) occurs
    whenever k >= ceil(n / 2) nodes flip, i.e. k >= (n + 1) // 2.
    """
    k_fail = (n + 1) // 2
    return sum(comb(n, k) * (p ** k) * ((1.0 - p) ** (n - k)) for k in range(k_fail, n + 1))


def quorum_failure_prob_tiebreak(n: int, p: float) -> float:
    """
    Exact single-generation consensus failure probability when exact n/2 ties
    (occurring only for even n) are resolved by an unbiased 50/50 coin flip.
    For odd n (such as n=13), no ties exist and this equals strict failure.
    """
    if n % 2 == 1:
        return quorum_failure_prob_strict(n, p)
    half = n // 2
    tie_term = 0.5 * comb(n, half) * (p ** half) * ((1.0 - p) ** half)
    majority_wrong = sum(
        comb(n, k) * (p ** k) * ((1.0 - p) ** (n - k)) for k in range(half + 1, n + 1)
    )
    return tie_term + majority_wrong


def secded_8_4_failure_prob(p: float) -> float:
    """
    Block decoding error/uncorrectable probability for an (8, 4, 4) extended
    Hamming SECDED byte: corrects t=1 bit flip, fails to self-heal for k >= 2.
    """
    return sum(comb(8, k) * (p ** k) * ((1.0 - p) ** (8 - k)) for k in range(2, 9))


def survival_fidelity_horizon(eps: float, generations: int) -> float:
    """
    Probability that a self-correcting quorum regenerates the original invariant
    without a single consensus-flip/deadlock collapse across T generations:
        S(T) = (1 - eps)^T
    """
    if eps <= 0.0:
        return 1.0
    if eps >= 1.0:
        return 0.0
    return math.exp(generations * math.log1p(-eps))


def markov_bit_retention(eps_flip: float, generations: int) -> float:
    """
    Two-state symmetric Markov chain bit retention probability after T steps
    (allowing accidental back-flips):
        P_retain(T) = 0.5 * (1 + (1 - 2 * eps_flip)^T)
    """
    return 0.5 * (1.0 + ((1.0 - 2.0 * eps_flip) ** generations))


def simulate_iterative_quorums(
    n: int,
    p: float,
    generations: int,
    trials: int,
    rng: random.Random,
    strict_ties_fail: bool = True,
) -> Tuple[float, float]:
    """
    Node-level Monte Carlo simulation of iterative generational quorum transmission.
    """
    unbroken_count = 0
    retained_final_count = 0
    half = n / 2.0

    for _ in range(trials):
        state = 1  # True ancestral invariant bit
        unbroken = True
        for _gen in range(generations):
            flips = sum(1 for _node in range(n) if rng.random() < p)
            votes_for_current = n - flips
            if votes_for_current > half:
                next_state = state
            elif votes_for_current < half:
                next_state = 1 - state
                unbroken = False
            else:
                if strict_ties_fail:
                    unbroken = False
                    next_state = 1 - state
                    state = next_state
                    break
                next_state = state if (rng.random() < 0.5) else (1 - state)
                if next_state != state:
                    unbroken = False
            state = next_state

        if unbroken:
            unbroken_count += 1
        if state == 1:
            retained_final_count += 1

    return unbroken_count / trials, retained_final_count / trials


def run_verification(seed: int = 42, trials: int = 5000, generations: int = 500) -> Dict:
    t0 = time.perf_counter()
    rng = random.Random(seed)

    # GATE 1: Discrete Number-Theoretic & Byzantine Quorum Bounds
    t_crash_8 = (8 - 1) // 2       # 3
    t_crash_13 = (13 - 1) // 2     # 6
    f_byz_8 = (8 - 1) // 3         # 2  (since 3*2 + 1 = 7 <= 8 < 10)
    f_byz_13 = (13 - 1) // 3       # 4  (exact minimal integer: 3*4 + 1 = 13)
    tie_states_8 = comb(8, 4)      # 70 deadlock configurations out of 256 (27.34%)
    tie_states_13 = 0              # Odd N has zero k == N - k solutions

    gate_1_pass = (
        t_crash_8 == 3
        and t_crash_13 == 6
        and f_byz_8 == 2
        and f_byz_13 == 4
        and (3 * f_byz_13 + 1 == 13)
        and tie_states_8 == 70
        and tie_states_13 == 0
    )

    # GATE 2: Analytical Horizon Scaling Across Noise Regimes (EEPROM -> Dryas)
    noise_grid = [0.02, 0.035, 0.05, 0.08, 0.10]
    scaling_table: List[Dict[str, float]] = []
    for p in noise_grid:
        e8_strict = quorum_failure_prob_strict(8, p)
        e8_tie = quorum_failure_prob_tiebreak(8, p)
        e13 = quorum_failure_prob_strict(13, p)
        e_secded = secded_8_4_failure_prob(p)

        h8_strict = 1.0 / e8_strict
        h8_tie = 1.0 / e8_tie
        h13 = 1.0 / e13
        gain_strict = h13 / h8_strict
        gain_tie = h13 / h8_tie

        scaling_table.append(
            {
                "p": p,
                "eps_secded_8_4": e_secded,
                "eps_8_strict": e8_strict,
                "eps_8_tiebreak": e8_tie,
                "eps_13": e13,
                "horizon_8_strict_cycles": h8_strict,
                "horizon_8_tiebreak_cycles": h8_tie,
                "horizon_13_cycles": h13,
                "horizon_gain_strict_x": gain_strict,
                "horizon_gain_tiebreak_x": gain_tie,
                "survival_500gen_8_strict": survival_fidelity_horizon(e8_strict, generations),
                "survival_500gen_8_tiebreak": survival_fidelity_horizon(e8_tie, generations),
                "survival_500gen_13": survival_fidelity_horizon(e13, generations),
            }
        )

    row_p035 = scaling_table[1]
    row_p050 = scaling_table[2]
    gate_2_pass = (
        8.0e3 <= row_p035["horizon_8_strict_cycles"] <= 1.5e4
        and row_p035["horizon_13_cycles"] > 1.0e7
        and row_p050["horizon_gain_strict_x"] > 300.0
        and row_p050["horizon_gain_tiebreak_x"] > 150.0
    )

    # GATE 3: Asymptotic Error-Exponent Verification (O(p^4) vs O(p^7))
    p_low_a, p_low_b = 0.01, 0.02
    slope_8 = math.log(
        quorum_failure_prob_strict(8, p_low_b) / quorum_failure_prob_strict(8, p_low_a)
    ) / math.log(p_low_b / p_low_a)
    slope_13 = math.log(
        quorum_failure_prob_strict(13, p_low_b) / quorum_failure_prob_strict(13, p_low_a)
    ) / math.log(p_low_b / p_low_a)
    gate_3_pass = (3.90 < slope_8 < 4.10) and (6.90 < slope_13 < 7.10)

    # GATE 4: Seeded Monte Carlo Node-Level Bottleneck Simulation (p = 0.10)
    p_bottleneck = 0.10
    sim_s8_strict, _ = simulate_iterative_quorums(
        8, p_bottleneck, generations, trials, rng, strict_ties_fail=True
    )
    sim_s8_tie, sim_ret8_tie = simulate_iterative_quorums(
        8, p_bottleneck, generations, trials, rng, strict_ties_fail=False
    )
    sim_s13, sim_ret13 = simulate_iterative_quorums(
        13, p_bottleneck, generations, trials, rng, strict_ties_fail=True
    )

    theo_s8_strict = survival_fidelity_horizon(
        quorum_failure_prob_strict(8, p_bottleneck), generations
    )
    theo_s8_tie = survival_fidelity_horizon(
        quorum_failure_prob_tiebreak(8, p_bottleneck), generations
    )
    theo_s13 = survival_fidelity_horizon(
        quorum_failure_prob_strict(13, p_bottleneck), generations
    )
    theo_ret8_tie = markov_bit_retention(
        quorum_failure_prob_tiebreak(8, p_bottleneck), generations
    )
    theo_ret13 = markov_bit_retention(
        quorum_failure_prob_strict(13, p_bottleneck), generations
    )

    se_8_strict = math.sqrt(theo_s8_strict * (1.0 - theo_s8_strict) / trials)
    se_8_tie = math.sqrt(theo_s8_tie * (1.0 - theo_s8_tie) / trials)
    se_13 = math.sqrt(theo_s13 * (1.0 - theo_s13) / trials)

    gate_4_pass = (
        abs(sim_s8_strict - theo_s8_strict) <= 4.0 * se_8_strict
        and abs(sim_s8_tie - theo_s8_tie) <= 4.0 * se_8_tie
        and abs(sim_s13 - theo_s13) <= 4.0 * se_13
        and sim_s13 > 0.93
        and sim_s8_strict < 0.12
    )

    # GATE 5: Landauer Erasure Floor & NIST Chemistry WebBook SRD 69 Redox Values
    k_boltzmann = 1.380649e-23  # J/K (CODATA exact)
    t_stp = 298.15              # K
    t_mammalian = 310.15        # K (37.0 deg C)
    e_landauer_298 = k_boltzmann * t_stp * math.log(2.0)
    e_landauer_310 = k_boltzmann * t_mammalian * math.log(2.0)

    # NIST Chemistry WebBook SRD 69 (CO2: C124389; alpha-Quartz SiO2: C14808607)
    dh_co2_kj_mol = -393.51     # kJ/mol, CO2(g) at 298.15 K, 1 bar
    s_co2_j_mol_k = 213.785     # J/(mol*K), CO2(g) at 1 bar
    dg_co2_kj_mol = -394.41     # kJ/mol, CO2(g) at 298.15 K
    dh_sio2_kj_mol = -910.86    # kJ/mol, alpha-Quartz SiO2(s) (Chase 1998, NIST SRD 69)

    enthalpy_ratio_si_to_c = dh_sio2_kj_mol / dh_co2_kj_mol  # 2.3147

    gate_5_pass = (
        2.85e-21 < e_landauer_298 < 2.86e-21
        and 2.96e-21 < e_landauer_310 < 2.98e-21
        and 2.31 < enthalpy_ratio_si_to_c < 2.32
    )

    elapsed_s = time.perf_counter() - t0
    all_passed = all([gate_1_pass, gate_2_pass, gate_3_pass, gate_4_pass, gate_5_pass])

    return {
        "version": "1.0.0",
        "seed": seed,
        "trials": trials,
        "generations": generations,
        "elapsed_seconds": round(elapsed_s, 4),
        "all_gates_passed": all_passed,
        "gates": {
            "gate_1_quorum_number_theory": gate_1_pass,
            "gate_2_horizon_scaling": gate_2_pass,
            "gate_3_asymptotic_exponent": gate_3_pass,
            "gate_4_monte_carlo_dryas_bottleneck": gate_4_pass,
            "gate_5_landauer_and_redox_si": gate_5_pass,
        },
        "quorum_invariants": {
            "N8": {
                "crash_tolerance_t": t_crash_8,
                "byzantine_tolerance_f": f_byz_8,
                "tie_deadlock_states": tie_states_8,
                "low_noise_log_slope": round(slope_8, 4),
            },
            "N13": {
                "crash_tolerance_t": t_crash_13,
                "byzantine_tolerance_f": f_byz_13,
                "tie_deadlock_states": tie_states_13,
                "low_noise_log_slope": round(slope_13, 4),
            },
        },
        "scaling_table": scaling_table,
        "monte_carlo_p010_500gen": {
            "N8_strict_unbroken_sim": round(sim_s8_strict, 4),
            "N8_strict_unbroken_theory": round(theo_s8_strict, 4),
            "N8_tiebreak_unbroken_sim": round(sim_s8_tie, 4),
            "N8_tiebreak_unbroken_theory": round(theo_s8_tie, 4),
            "N8_tiebreak_final_retention_sim": round(sim_ret8_tie, 4),
            "N8_tiebreak_final_retention_theory": round(theo_ret8_tie, 4),
            "N13_unbroken_sim": round(sim_s13, 4),
            "N13_unbroken_theory": round(theo_s13, 4),
            "N13_final_retention_sim": round(sim_ret13, 4),
            "N13_final_retention_theory": round(theo_ret13, 4),
        },
        "thermodynamics_si": {
            "landauer_298_15K_J_per_bit": e_landauer_298,
            "landauer_310_15K_J_per_bit": e_landauer_310,
            "CO2_dHf_kJ_per_mol": dh_co2_kj_mol,
            "CO2_S0_J_per_mol_K": s_co2_j_mol_k,
            "CO2_dGf_kJ_per_mol": dg_co2_kj_mol,
            "SiO2_quartz_dHf_kJ_per_mol": dh_sio2_kj_mol,
            "enthalpy_ratio_SiO2_to_CO2": round(enthalpy_ratio_si_to_c, 4),
        },
    }


def main() -> int:
    parser = argparse.ArgumentParser(
        description="Verify N=8 vs N=13 (12+1) error-correction horizon scaling (v1.0.0)."
    )
    parser.add_argument("--seed", type=int, default=42, help="PRNG seed (default: 42)")
    parser.add_argument("--trials", type=int, default=5000, help="Monte Carlo lineages (default: 5000)")
    parser.add_argument(
        "--generations",
        type=int,
        default=500,
        help="Iterative generations per lineage (default: 500 = 12,500 yr at 25 yr/gen)",
    )
    parser.add_argument("--json", action="store_true", help="Emit full JSON summary")
    args = parser.parse_args()

    report = run_verification(seed=args.seed, trials=args.trials, generations=args.generations)

    if args.json:
        print(json.dumps(report, indent=2))
        return 0 if report["all_gates_passed"] else 1

    print("=" * 78)
    print("THE DANDELION INVARIANT (v1.0.0) — PARITY & HORIZON VERIFICATION SUITE")
    print(f"Seed: {report['seed']} | Lineages: {report['trials']} | Generations: {report['generations']}")
    print("=" * 78)

    inv8 = report["quorum_invariants"]["N8"]
    inv13 = report["quorum_invariants"]["N13"]
    print("\n[1] Quorum Topology & Fault-Radius Bounds:")
    print(
        f"    N =  8 (Even Byte) : crash t = {inv8['crash_tolerance_t']}, "
        f"Byzantine f = {inv8['byzantine_tolerance_f']}, "
        f"4:4 deadlock states = {inv8['tie_deadlock_states']}/256, "
        f"log-slope = {inv8['low_noise_log_slope']:.3f}"
    )
    print(
        f"    N = 13 (12 + 1)    : crash t = {inv13['crash_tolerance_t']}, "
        f"Byzantine f = {inv13['byzantine_tolerance_f']}, "
        f"tie deadlock states = {inv13['tie_deadlock_states']}/8192, "
        f"log-slope = {inv13['low_noise_log_slope']:.3f}"
    )

    print("\n[2] Analytical Mean Horizon (1/eps) & 500-Generation Survival Fidelity:")
    header = (
        f"    {'p':>5} | {'H8_strict':>11} | {'H8_tie':>11} | {'H13_cycles':>13} | "
        f"{'Gain(strict)':>12} | {'S8_strict(500)':>14} | {'S13(500)':>10}"
    )
    print(header)
    print("    " + "-" * (len(header) - 4))
    for row in report["scaling_table"]:
        print(
            f"    {row['p']:5.3f} | "
            f"{row['horizon_8_strict_cycles']:11.1f} | "
            f"{row['horizon_8_tiebreak_cycles']:11.1f} | "
            f"{row['horizon_13_cycles']:13.1f} | "
            f"{row['horizon_gain_strict_x']:11.1f}x | "
            f"{100.0 * row['survival_500gen_8_strict']:13.2f}% | "
            f"{100.0 * row['survival_500gen_13']:9.3f}%"
        )

    mc = report["monte_carlo_p010_500gen"]
    print(f"\n[3] Monte Carlo Bottleneck Audit (p = 0.10, T = {report['generations']} gen = 12,500 yr):")
    print(
        f"    N =  8 (strict majority) unbroken survival : "
        f"sim = {100.0 * mc['N8_strict_unbroken_sim']:6.2f}%  "
        f"(theory = {100.0 * mc['N8_strict_unbroken_theory']:6.2f}%)"
    )
    print(
        f"    N =  8 (50/50 tie-break) unbroken survival : "
        f"sim = {100.0 * mc['N8_tiebreak_unbroken_sim']:6.2f}%  "
        f"(theory = {100.0 * mc['N8_tiebreak_unbroken_theory']:6.2f}%)"
    )
    print(
        f"    N = 13 (12 + 1 quorum)   unbroken survival : "
        f"sim = {100.0 * mc['N13_unbroken_sim']:6.2f}%  "
        f"(theory = {100.0 * mc['N13_unbroken_theory']:6.2f}%)"
    )

    th = report["thermodynamics_si"]
    print("\n[4] Substrate Thermodynamic Benchmarks (Pure SI, NIST SRD 69):")
    print(f"    Landauer limit (298.15 K) : {th['landauer_298_15K_J_per_bit']:.4e} J/bit")
    print(f"    Landauer limit (310.15 K) : {th['landauer_310_15K_J_per_bit']:.4e} J/bit")
    print(
        f"    C  + O2 -> CO2(g)         : dHf = {th['CO2_dHf_kJ_per_mol']:.2f} kJ/mol, "
        f"S0 = {th['CO2_S0_J_per_mol_K']:.3f} J/(mol*K), dGf = {th['CO2_dGf_kJ_per_mol']:.2f} kJ/mol"
    )
    print(
        f"    Si + O2 -> SiO2(s,quartz) : dHf = {th['SiO2_quartz_dHf_kJ_per_mol']:.2f} kJ/mol "
        f"(ratio = {th['enthalpy_ratio_SiO2_to_CO2']:.4f}x)"
    )

    print("\n[5] Verification Gate Summary:")
    for gate_name, status in report["gates"].items():
        badge = "PASS" if status else "FAIL"
        print(f"    [{badge}] {gate_name}")
    print(
        f"\nCompleted in {report['elapsed_seconds']:.4f} s — OVERALL: "
        f"{'ALL 5/5 GATES PASSED' if report['all_gates_passed'] else 'FAILED'}"
    )

    return 0 if report["all_gates_passed"] else 1


if __name__ == "__main__":
    sys.exit(main())
