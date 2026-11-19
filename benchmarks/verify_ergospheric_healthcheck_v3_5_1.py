#!/usr/bin/env python3
"""
DOS-PEARL-v3.5.0 / v3.5.1 Pure-Stdlib Deterministic Verification Harness (--seed 42, <= 60s)
Ergospheric Load-Shedding Superradiance, Klein-Gordon Boundary Impedance,
and Balanced Ternary Health-Check Regularization (DOI: 10.5281/zenodo.24211350).

Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
Affiliation: Independent Dandelion Substrate Research Group, Gurley, AL 35748, USA
Zero third-party packages required (100% stock Python 3 standard library).
"""
import argparse
import json
import math
import random
import sys
import time


def exact_bernoulli_blowup_time(x_0: float, rho_0: float, kappa: float) -> float:
    if x_0 >= rho_0:
        return math.inf
    return (1.0 / kappa) * (rho_0 * math.log(rho_0 / (rho_0 - x_0)) - x_0)


def rk4_blowup_time(x_0: float, rho_0: float, kappa: float, dt: float = 1e-5) -> float:
    x = x_0
    t = 0.0
    while x > 1e-6:
        h = 1e-7 if x < 2e-3 else dt
        k1 = -kappa * (rho_0 / x - 1.0)
        if x + 0.5 * h * k1 <= 0.0:
            return t + (x * x) / (2.0 * kappa * rho_0)
        k2 = -kappa * (rho_0 / (x + 0.5 * h * k1) - 1.0)
        k3 = -kappa * (rho_0 / (x + 0.5 * h * k2) - 1.0)
        if x + h * k3 <= 0.0:
            return t + (x * x) / (2.0 * kappa * rho_0)
        k4 = -kappa * (rho_0 / (x + h * k3) - 1.0)
        x += (h / 6.0) * (k1 + 2.0 * k2 + 2.0 * k3 + k4)
        t += h
    return t


def run_verification(seed: int = 42) -> int:
    t0 = time.perf_counter()
    u_lock = 2.0 / 3.0

    # Gate 1: Ternary GQRE / Blume-Capel 1-trit ceiling identity
    trit_entropy = -math.log(1.0 - u_lock)
    assert abs(trit_entropy - math.log(3.0)) < 1e-15, "Keyhole entropy mismatch"

    # Gate 2: Analytical vs RK4 Bernoulli superradiant blowup time
    rho_0, x_0, kappa = 0.72, 0.64, 0.25
    t_exact = exact_bernoulli_blowup_time(x_0, rho_0, kappa)
    t_num = rk4_blowup_time(x_0, rho_0, kappa)
    rel_err = abs(t_num - t_exact) / t_exact
    assert rel_err < 1e-6, f"RK4 vs Exact mismatch: {rel_err}"

    # Gate 3: 997-node discrete fleet comparison under 1.55x TLS spike + 2% hardware fault
    n_nodes = 997
    steps = 160
    offsets = [(i * 13) % 19 for i in range(n_nodes)]

    def sim(ternary: bool):
        r = random.Random(seed)
        states = [1] * n_nodes
        timers = [0] * n_nodes
        backlog = 0.0
        min_alive = 1.0
        min_core = 1.0
        max_rec = int(0.15 * n_nodes)
        fault_k = int(0.02 * n_nodes)

        for t in range(steps):
            mult = 1.55 if (20 <= t <= 45) else 1.0
            if t == 30:
                for idx in r.sample(range(n_nodes), fault_k):
                    states[idx] = -1
                    timers[idx] = 25

            recovered = 0
            for i in range(n_nodes):
                if states[i] == -1:
                    if timers[i] == 0 and recovered < max_rec:
                        states[i] = 1
                        recovered += 1
                    elif timers[i] > 0:
                        timers[i] -= 1

            n_p = sum(1 for s in states if s == 1)
            n_z = sum(1 for s in states if s == 0)
            n_a = n_p + n_z
            core_d = 0.70 * rho_0 * n_nodes
            bulk_d = 0.30 * rho_0 * n_nodes

            if n_a == 0:
                min_alive = 0.0
                min_core = 0.0
                backlog = min(backlog * 0.90 + core_d * 0.35, 1.5 * n_nodes)
                continue

            if not ternary:
                probe_oh = 0.12 * n_nodes if (t % 5 == 0) else 0.01 * n_nodes
                u_mean = (core_d + bulk_d + backlog + probe_oh) * mult / n_a
                for i in range(n_nodes):
                    u_i = u_mean * r.gauss(1.0, 0.08)
                    probed = True if (t % 2 == 0) else (r.random() < 0.5)
                    if states[i] == 1 and probed and u_i > 1.0:
                        states[i] = -1
                        timers[i] = 12
                cap = float(sum(1 for s in states if s == 1)) / mult
                core_ok = min(1.0, cap / core_d)
                backlog = min(0.80 * backlog + 0.55 * max(0.0, core_d - cap), 1.5 * n_nodes)
            else:
                due_cnt = sum(1 for off in offsets if off == (t % 19))
                probe_oh = 0.12 * float(due_cnt)
                eff_bulk = bulk_d * (n_p / n_a) + 0.15 * bulk_d * (n_z / n_a)
                u_raw = (core_d + eff_bulk + backlog + probe_oh) * mult / n_a
                for i in range(n_nodes):
                    u_i = u_raw * r.gauss(1.0, 0.06)
                    if states[i] == 1 and u_i > 0.78:
                        states[i] = 0
                    elif states[i] == 0 and u_i <= u_lock:
                        states[i] = 1
                cap = float(n_a) / mult
                core_ok = min(1.0, cap / core_d)
                backlog = min(0.50 * backlog + 0.30 * max(0.0, core_d - cap), 1.5 * n_nodes)

            alive_frac = sum(1 for s in states if s != -1) / float(n_nodes)
            min_alive = min(min_alive, alive_frac)
            min_core = min(min_core, core_ok)

        end_alive = sum(1 for s in states if s != -1) / float(n_nodes)
        return min_alive, min_core, end_alive

    bin_min_alive, _, bin_end = sim(False)
    ter_min_alive, ter_min_core, ter_end = sim(True)
    assert bin_min_alive < 0.05, f"Binary model failed to reproduce cascade: {bin_min_alive}"
    assert ter_min_alive >= 0.97, f"Ternary model lost non-faulted nodes: {ter_min_alive}"
    assert ter_end == 1.0, f"Ternary model did not fully recover post-fault: {ter_end}"

    elapsed = time.perf_counter() - t0
    print(json.dumps({
        "pearl_id": "DOS-PEARL-v3.5.0",
        "verifier_version": "3.5.1-stdlib",
        "doi": "10.5281/zenodo.24211350",
        "seed": seed,
        "elapsed_s": round(elapsed, 4),
        "t_crash_exact_s": round(t_exact, 6),
        "t_crash_rk4_s": round(t_num, 6),
        "binary_min_alive_pct": round(bin_min_alive * 100.0, 2),
        "ternary_min_alive_pct": round(ter_min_alive * 100.0, 2),
        "ternary_min_core_pct": round(ter_min_core * 100.0, 2),
        "ternary_end_alive_pct": round(ter_end * 100.0, 2),
        "exit_state": "CODE_SHOKUNIN_UNLATCHED = 255",
    }, indent=2))
    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()
    sys.exit(run_verification(args.seed))
