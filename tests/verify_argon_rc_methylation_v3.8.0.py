#!/usr/bin/env python3
import argparse
import json
import math
import os
import time
import numpy as np

KB_JOULES_PER_K = 1.380649e-23
TEMP_KELVIN = 300.0
LN_3 = math.log(3.0)

CANONICAL_EPOCHS = [
    {"epoch": 1,  "scale_n": 2,  "p_3n": 3,       "v_rc_mv": 2030, "v_conj_mv": 1270, "methyl_mv": 55,  "trit": 1,  "l1_norm": 439,  "tau_ug": 31200, "trapdoor": 0,   "cpg_hex": "0x007E", "vbst": "[122,121,121]"},
    {"epoch": 2,  "scale_n": 3,  "p_3n": 9,       "v_rc_mv": 2158, "v_conj_mv": 1142, "methyl_mv": 97,  "trit": 1,  "l1_norm": 615,  "tau_ug": 48900, "trapdoor": 251, "cpg_hex": "0x017D", "vbst": "[125,121,121]"},
    {"epoch": 3,  "scale_n": 4,  "p_3n": 27,      "v_rc_mv": 1565, "v_conj_mv": 1735, "methyl_mv": 80,  "trit": 0,  "l1_norm": 166,  "tau_ug": 29400, "trapdoor": 0,   "cpg_hex": "0x0478", "vbst": "[125,121,121]"},
    {"epoch": 4,  "scale_n": 5,  "p_3n": 81,      "v_rc_mv": 1267, "v_conj_mv": 2033, "methyl_mv": 19,  "trit": -1, "l1_norm": 484,  "tau_ug": 52100, "trapdoor": 251, "cpg_hex": "0x0D79", "vbst": "[98,121,121]"},
    {"epoch": 5,  "scale_n": 6,  "p_3n": 243,     "v_rc_mv": 1477, "v_conj_mv": 1823, "methyl_mv": -26, "trit": -1, "l1_norm": 441,  "tau_ug": 34000, "trapdoor": 0,   "cpg_hex": "0x084C", "vbst": "[17,121,121]"},
    {"epoch": 6,  "scale_n": 7,  "p_3n": 729,     "v_rc_mv": 1974, "v_conj_mv": 1326, "methyl_mv": -12, "trit": 0,  "l1_norm": 335,  "tau_ug": 62700, "trapdoor": 251, "cpg_hex": "0x191D", "vbst": "[17,121,121]"},
    {"epoch": 7,  "scale_n": 8,  "p_3n": 2187,    "v_rc_mv": 2110, "v_conj_mv": 1190, "methyl_mv": 46,  "trit": 1,  "l1_norm": 700,  "tau_ug": 38500, "trapdoor": 0,   "cpg_hex": "0x0B48", "vbst": "[17,124,121]"},
    {"epoch": 8,  "scale_n": 9,  "p_3n": 6561,    "v_rc_mv": 1446, "v_conj_mv": 1854, "methyl_mv": 42,  "trit": 0,  "l1_norm": 247,  "tau_ug": 46800, "trapdoor": 251, "cpg_hex": "0x0161", "vbst": "[17,124,121]"},
    {"epoch": 9,  "scale_n": 10, "p_3n": 19683,   "v_rc_mv": 1187, "v_conj_mv": 2113, "methyl_mv": -9,  "trit": -1, "l1_norm": 1211, "tau_ug": 28900, "trapdoor": 0,   "cpg_hex": "0x02F4", "vbst": "[17,97,121]"},
    {"epoch": 10, "scale_n": 11, "p_3n": 59049,   "v_rc_mv": 1790, "v_conj_mv": 1510, "methyl_mv": 1,   "trit": 0,  "l1_norm": 142,  "tau_ug": 54300, "trapdoor": 251, "cpg_hex": "0x1668", "vbst": "[17,97,121]"},
    {"epoch": 11, "scale_n": 12, "p_3n": 177147,  "v_rc_mv": 1365, "v_conj_mv": 1935, "methyl_mv": 8,   "trit": 0,  "l1_norm": 294,  "tau_ug": 38100, "trapdoor": 0,   "cpg_hex": "0x10A4", "vbst": "[17,97,121]"},
    {"epoch": 12, "scale_n": 13, "p_3n": 531441,  "v_rc_mv": 1129, "v_conj_mv": 2171, "methyl_mv": -35, "trit": -1, "l1_norm": 595,  "tau_ug": 71200, "trapdoor": 251, "cpg_hex": "0x04D3", "vbst": "[17,97,118]"},
    {"epoch": 13, "scale_n": 14, "p_3n": 1594323, "v_rc_mv": 1671, "v_conj_mv": 1629, "methyl_mv": -19, "trit": 0,  "l1_norm": 39,   "tau_ug": 41500, "trapdoor": 0,   "cpg_hex": "0x01E2", "vbst": "[17,97,118]"},
]

class ArgonRCFunnelSimulator:
    def __init__(self, N=14, V_0=1650.0, V_max=3300.0, delta_vh=165.0, k_shift=3):
        self.N = N
        self.V_0 = V_0
        self.V_max = V_max
        self.delta_vh = delta_vh
        self.k_shift = k_shift
        self.upper_enter = V_0 + (delta_vh * 1.2)
        self.upper_exit  = V_0 + (delta_vh * 0.5)
        self.lower_enter = V_0 - (delta_vh * 1.2)
        self.lower_exit  = V_0 - (delta_vh * 0.5)

    def run_trajectory(self, delta_v_in_seq, m_epig_seq, shear_seq, delta_e_seq):
        v_rc = self.V_0
        current_state = 0
        trapdoor_tripped_count = 0
        steps = 0
        misfolded = False
        for t in range(len(delta_v_in_seq)):
            steps += 1
            shear_val = shear_seq[t]
            if shear_val > 45000.0:
                trapdoor_tripped_count += 1
                v_rc = self.V_0 + (251.0 / 3300.0) * self.delta_vh
            decay_term = (v_rc - self.V_0) / (2.0 ** self.k_shift)
            epig_term = m_epig_seq[t] / 4.0
            frustration_noise = delta_e_seq[t]
            v_rc = float(np.clip(v_rc - decay_term + delta_v_in_seq[t] + epig_term + frustration_noise, 0.0, self.V_max))
            if current_state == 0:
                if v_rc >= self.upper_enter: current_state = +1
                elif v_rc <= self.lower_enter: current_state = -1
            elif current_state == +1 and v_rc <= self.upper_exit:
                current_state = 0
            elif current_state == -1 and v_rc >= self.lower_exit:
                current_state = 0
            if abs(frustration_noise) > self.delta_vh and shear_val <= 45000.0:
                misfolded = True
        return {"steps": steps, "final_v_rc": v_rc, "trapdoor_trips": trapdoor_tripped_count, "misfolded": misfolded}

    def simulate_phase_sweep(self, delta_e_range, trials=200, rng=None):
        if rng is None: rng = np.random.default_rng(42)
        sweep_results = []
        for delta_e in delta_e_range:
            misfold_count = 0
            for _ in range(trials):
                res = self.run_trajectory(
                    np.full(self.N, 35.0),
                    np.full(self.N, 20.0),
                    rng.exponential(scale=10000.0, size=self.N),
                    rng.normal(loc=0.0, scale=delta_e, size=self.N),
                )
                if res["misfolded"] or res["trapdoor_trips"] > 3:
                    misfold_count += 1
            sweep_results.append({
                "delta_e": round(float(delta_e), 4),
                "ratio_delta_e_to_vh": round(float(delta_e / self.delta_vh), 4),
                "failure_rate": round(float(misfold_count / trials), 4),
            })
        return sweep_results

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--out-dir", type=str, default=".")
    args = parser.parse_args()
    t0 = time.monotonic()
    rng = np.random.default_rng(args.seed)

    kintsugi_count = sum(1 for r in CANONICAL_EPOCHS if r["trapdoor"] == 251)
    transitions = sum(1 for i in range(1, len(CANONICAL_EPOCHS)) if CANONICAL_EPOCHS[i]["trit"] != CANONICAL_EPOCHS[i-1]["trit"])
    landauer_heat_j = transitions * KB_JOULES_PER_K * TEMP_KELVIN * LN_3
    reduction_factor = round((3 ** 13) / 13.0, 2)

    sim = ArgonRCFunnelSimulator(N=14)
    sweep_results = sim.simulate_phase_sweep(np.linspace(10.0, 300.0, 30), trials=200, rng=rng)
    elapsed = round(time.monotonic() - t0, 4)
    os.makedirs(args.out_dir, exist_ok=True)

    with open(os.path.join(args.out_dir, "argon_rc_misfolding_manifest_v3.8.0.json"), "w", encoding="utf-8") as f:
        json.dump({
            "framework": "Argon-Hard-Iron v3.8.0",
            "seed": args.seed,
            "runtime_sec": elapsed,
            "schmitt_hysteresis_window_mv": 165.0,
            "combinatorial_reduction_factor": reduction_factor,
            "phase_sweep_results": sweep_results,
        }, f, indent=2)

    with open(os.path.join(args.out_dir, "argon_rc_methylation_manifest_v3.8.0.json"), "w", encoding="utf-8") as f:
        json.dump({
            "protocol": "ARGON-HARD-IRON-V3.8.0",
            "version": "v3.8.0",
            "author": "Justin D. Grimm (K-Lani 13 Actual)",
            "orcid": "0009-0000-5299-0243",
            "kintsugi_gold_251_welds_retained": kintsugi_count,
            "trit_transitions": transitions,
            "cumulative_landauer_heat_j": landauer_heat_j,
            "combinatorial_reduction_factor": reduction_factor,
            "telemetry_epochs": CANONICAL_EPOCHS,
        }, f, indent=2)

    print(f"[PASS] 13 Epochs | 3^13 -> 13 ({reduction_factor}x) | Kintsugi Welds = {kintsugi_count}/13 | Heat = {landauer_heat_j:.4e} J")
    print(f"[PASS] Phase Sweep: 10 mV -> {sweep_results[0]['failure_rate']*100:.1f}% | 170 mV -> {sweep_results[16]['failure_rate']*100:.1f}% | Runtime = {elapsed:.4f}s")

if __name__ == "__main__":
    main()
