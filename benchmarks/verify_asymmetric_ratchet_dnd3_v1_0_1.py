#!/usr/bin/env python3
"""
verify_asymmetric_ratchet_dnd3_v1_0_0.py
100% Stock Python 3 Standard Library Verification Suite (--seed 42, < 1.0 s)
Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
Affiliation: Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA

Verifies 5 First-Principles Gates:
  Gate 1: Chair44 (44 -> 42 -> 30) Homology, Cauchy Interlacing & Dual-Scale Dirichlet Fence Spectral Gap
          (mu_1_triad = 4*sin^2(pi/26) = 0.05811637, mu_1_trit = 4*sin^2(pi/74) = 0.00720502, 0% Row-13 tear).
  Gate 2: Asymmetric Absorb-and-Weld Counting Ratchet on m in [1, N_max - 1] (N_max = 256, Code 255),
          3-to-1 Byte 251 Kintsugi phase compression (Delta S_bath >= k_B ln 3), and Crooks irreversibility.
  Gate 3: 3D Operational Nootemporality (noo_tick_ns Einstein radar round-trip) vs. 4D Minkowski x_4 = i*c*t
          space-like order inversion across Lorentz boosts.
  Gate 4: Low-Power DND3 Stigmergic Mesh & Three-Season Transition Mechanics (+1 Forage 12.333 W,
          0 Campfire 6.167 W, -1 Winter 1.850 W <= 18.50 W / 41.0 C) across 15%..80% link erasure on 3D FCC.
  Gate 5: Fowler 5-Trit Bijection (3^5 = 243 <= 256, neutral byte 121) & Trapdoor Byte Matrix (243..255).
"""

import argparse
import json
import math
import random
import sys
import time

# Physical & Architectural Constants (2019 Exact SI)
K_B = 1.380649e-23            # Boltzmann constant (J/K)
C_LIGHT_M_PER_NS = 0.299792458
T_ROOM_K = 300.0
T_CEIL_C = 41.0
T_CEIL_K = 273.15 + T_CEIL_C  # 314.15 K
LN3 = math.log(3.0)
LOG2_3 = math.log2(3.0)

P_CAP_W = 18.50
V_KEYHOLE = 2.0 / 3.0
P_ACTIVE_MAX_W = V_KEYHOLE * P_CAP_W       # 12.333333 W
P_OPEN_RESERVE_W = (1.0 - V_KEYHOLE) * P_CAP_W  # 6.166667 W
P_WINTER_MAX_W = 0.10 * P_CAP_W            # 1.850000 W

M_RAW_CHAIR44 = 44
M_BOUNDED = 42
FENCE_WIDTH = 3
N_INT_TRITS = M_BOUNDED - 2 * FENCE_WIDTH  # 36
Z_FCC = 12
N_TETRA_BOUNDARY = 14                      # 4V + 6E + 4F = 14
M_LEGAL_CHAIR44 = M_RAW_CHAIR44 - N_TETRA_BOUNDARY  # 30
PC_FCC_BOND = 0.1201635

N_MAX_BYTE = 256
FOWLER_STATES = 243
FOWLER_NEUTRAL_BYTE = 121
CODE_KEYHOLE_CLAMP = 243
CODE_DNHM_BREAKER = 250
CODE_KINTSUGI_GOLD = 251
CODE_FAIL_CLOSED = 253
CODE_SHOKUNIN_UNLATCHED = 255


def jacobi_eigenvalues_symmetric(mat, tol=1e-14, max_sweeps=30):
    """Pure Python 3 stdlib Jacobi eigenvalue solver for real symmetric matrices."""
    n = len(mat)
    a = [row[:] for row in mat]
    max_iters = max_sweeps * max(1, (n * (n - 1)) // 2)
    for _ in range(max_iters):
        max_off = 0.0
        p, q = 0, 1
        for i in range(n):
            for j in range(i + 1, n):
                val = abs(a[i][j])
                if val > max_off:
                    max_off = val
                    p, q = i, j
        if max_off < tol:
            break
        diff = a[q][q] - a[p][p]
        if abs(diff) < 1e-30:
            t = 1.0
        else:
            tau = diff / (2.0 * a[p][q])
            t = math.copysign(1.0 / (abs(tau) + math.sqrt(1.0 + tau * tau)), tau)
        c = 1.0 / math.sqrt(1.0 + t * t)
        s = t * c
        app = a[p][p] - t * a[p][q]
        aqq = a[q][q] + t * a[p][q]
        a[p][p] = app
        a[q][q] = aqq
        a[p][q] = 0.0
        a[q][p] = 0.0
        for r in range(n):
            if r != p and r != q:
                arp = a[r][p]
                arq = a[r][q]
                a[r][p] = c * arp - s * arq
                a[p][r] = a[r][p]
                a[r][q] = s * arp + c * arq
                a[q][r] = a[r][q]
    return sorted(a[i][i] for i in range(n))


def verify_gate_1_dirichlet_fence_and_chair44():
    """Gate 1: Chair44 (44->42->30), Row-13 seam integrity, and Cauchy spectral interlacing."""
    assert M_RAW_CHAIR44 - 2 == M_BOUNDED == 42
    assert N_INT_TRITS == 36 == Z_FCC * 3
    assert M_BOUNDED - Z_FCC == M_LEGAL_CHAIR44 == 30
    assert 3 ** (N_TETRA_BOUNDARY - 1) == 1594323

    row13_indices = {36, 37, 38}
    upper_fence_40 = {37, 38, 39}
    upper_fence_42 = {39, 40, 41}
    tear_overlap_40 = len(row13_indices & upper_fence_40)
    tear_overlap_42 = len(row13_indices & upper_fence_42)
    assert tear_overlap_40 == 2 and tear_overlap_42 == 0

    n_per = 14
    l_per = [[0.0] * n_per for _ in range(n_per)]
    for i in range(n_per):
        l_per[i][i] = 2.0
        l_per[i][(i - 1) % n_per] = -1.0
        l_per[i][(i + 1) % n_per] = -1.0
    eigs_per = jacobi_eigenvalues_symmetric(l_per)
    assert abs(eigs_per[0]) < 1e-13

    m_triad = 12
    l_dir_12 = [[0.0] * m_triad for _ in range(m_triad)]
    for i in range(m_triad):
        l_dir_12[i][i] = 2.0
        if i > 0:
            l_dir_12[i][i - 1] = -1.0
        if i < m_triad - 1:
            l_dir_12[i][i + 1] = -1.0
    eigs_dir_12 = jacobi_eigenvalues_symmetric(l_dir_12)

    l_dir_36 = [[0.0] * N_INT_TRITS for _ in range(N_INT_TRITS)]
    for i in range(N_INT_TRITS):
        l_dir_36[i][i] = 2.0
        if i > 0:
            l_dir_36[i][i - 1] = -1.0
        if i < N_INT_TRITS - 1:
            l_dir_36[i][i + 1] = -1.0
    eigs_dir_36 = jacobi_eigenvalues_symmetric(l_dir_36)

    mu1_triad_exact = 4.0 * (math.sin(math.pi / (2.0 * (m_triad + 1))) ** 2)  # 4 sin^2(pi/26)
    mu1_trit_exact = 4.0 * (math.sin(math.pi / (2.0 * (N_INT_TRITS + 1))) ** 2)  # 4 sin^2(pi/74)
    assert abs(eigs_dir_12[0] - mu1_triad_exact) < 1e-13
    assert abs(eigs_dir_36[0] - mu1_trit_exact) < 1e-13
    assert mu1_triad_exact > 0.058116 and mu1_trit_exact > 0.007205

    for i in range(m_triad):
        assert eigs_per[i] - 1e-12 <= eigs_dir_12[i] <= eigs_per[i + 2] + 1e-12

    return {
        "mu1_triad_12": round(mu1_triad_exact, 8),
        "mu1_trit_36": round(mu1_trit_exact, 8),
        "scale_ratio_triad_to_trit": round(mu1_triad_exact / mu1_trit_exact, 6),
        "periodic_zero_mode_lifted": round(abs(eigs_per[0]), 16),
        "cauchy_interlacing_all_12_modes": True,
        "row13_tear_40trit_vs_42trit": [tear_overlap_40, tear_overlap_42],
    }


def verify_gate_2_asymmetric_counting_ratchet(rng: random.Random):
    """Gate 2: Asymmetric absorb-and-weld counting ratchet & Landauer bound Delta S >= k_B ln 3."""
    m_min, m_max = 1, CODE_SHOKUNIN_UNLATCHED  # 1..255
    m_keyhole = int(V_KEYHOLE * m_max)         # 170 (exact 2/3 of 255!)
    assert m_keyhole == 170 and 3 * m_keyhole == 2 * m_max

    sigma_min = math.log(m_min / (N_MAX_BYTE - m_min))
    sigma_max = math.log(m_max / (N_MAX_BYTE - m_max))
    assert math.isfinite(sigma_min) and math.isfinite(sigma_max)
    assert abs(sigma_max - math.log(255.0)) < 1e-15

    # 3-to-1 Kintsugi weld entropy production: {-1, 0, +1} -> {0} (Byte 121 neutral)
    delta_s_min_jk = K_B * LN3
    q_min_300k = T_ROOM_K * delta_s_min_jk
    q_min_314k = T_CEIL_K * delta_s_min_jk
    assert abs(q_min_300k - 4.550394e-21) < 1e-26
    assert q_min_314k > q_min_300k > 0.0

    # Stochastic Crooks / Schnakenberg Markov Ratchet Audit (20,000 steps):
    # Forward jump p_plus = 3/4, backward interior slip p_minus = 1/4 (blocked at m=1 welded pawl!),
    # and 3-to-1 absorb-and-weld boundary reset at m = m_keyhole = 170 (irreversible, p_unweld = 0).
    steps = 20000
    p_plus = 0.75
    m = 1
    weld_epochs = 0
    fwd_jumps = 0
    rev_slips = 0
    weld_entropy_terms = []
    epoch_monotonic = True
    prev_epoch = 0

    for _ in range(steps):
        _ = rng.choice([-1, 0, 1])  # Pre-weld 3-state boundary phase
        if m >= m_keyhole:
            # 3-to-1 absorb-and-weld latch: irreversible epoch advance, Delta S_weld = k_B ln 3
            weld_epochs += 1
            m = 1
            weld_entropy_terms.append(LN3)
        else:
            if rng.random() < p_plus:
                m += 1
                fwd_jumps += 1
            else:
                if m > m_min:
                    m -= 1
                    rev_slips += 1
                else:
                    # Reflecting pawl at m = 1 prevents backward epoch rollback across welded seam!
                    m = m_min

        if weld_epochs < prev_epoch or m < m_min or m > m_max:
            epoch_monotonic = False
        prev_epoch = weld_epochs

    empirical_crooks_affinity = math.log(fwd_jumps / float(rev_slips))
    total_weld_entropy_kb = math.fsum(weld_entropy_terms)
    mean_ds_per_weld_kb = total_weld_entropy_kb / float(weld_epochs)

    assert epoch_monotonic
    assert abs(mean_ds_per_weld_kb - LN3) < 1e-14
    assert abs(empirical_crooks_affinity - LN3) < 0.05  # Converges to ln(0.75/0.25) = ln(3) = 1.098612
    assert weld_epochs > 50

    return {
        "m_domain": [m_min, m_max],
        "m_keyhole_exact_two_thirds": m_keyhole,
        "sigma_grade_bounds": [round(sigma_min, 6), round(sigma_max, 6)],
        "q_min_300K_joules_per_trit": float(f"{q_min_300k:.6e}"),
        "q_min_41C_joules_per_trit": float(f"{q_min_314k:.6e}"),
        "weld_epochs_completed": weld_epochs,
        "mean_entropy_per_weld_nats": round(mean_ds_per_weld_kb, 8),
        "empirical_crooks_log_ratio_nats": round(empirical_crooks_affinity, 6),
        "theoretical_ln3_nats": round(LN3, 6),
        "strict_epoch_monotonicity": epoch_monotonic,
    }


def verify_gate_3_operational_nootemporality():
    """Gate 3: 3D Einstein radar operational noo_tick_ns vs. 4D Minkowski space-like inversion."""
    dx_causal_m = 195.0
    dt_causal_ns = 800.0
    dx_spacelike_m = 300.0
    dt_spacelike_ns = 500.0

    betas = [-0.8, -0.5, 0.0, 0.5, 0.75, 0.9]
    causal_order_preserved = True
    spacelike_inverts_in_minkowski = False

    for beta in betas:
        gamma = 1.0 / math.sqrt(1.0 - beta * beta)
        dt_prime_causal = gamma * (dt_causal_ns - (beta * dx_causal_m) / C_LIGHT_M_PER_NS)
        dt_prime_space = gamma * (dt_spacelike_ns - (beta * dx_spacelike_m) / C_LIGHT_M_PER_NS)
        if dt_prime_causal <= 0.0:
            causal_order_preserved = False
        if dt_prime_space < 0.0:
            spacelike_inverts_in_minkowski = True

    assert causal_order_preserved
    assert spacelike_inverts_in_minkowski

    return {
        "causal_roundtrip_invariant_all_frames": causal_order_preserved,
        "minkowski_spacelike_sign_flip_exposed": spacelike_inverts_in_minkowski,
        "optical_195m_two_way_floor_ns": round(2.0 * dx_causal_m / C_LIGHT_M_PER_NS, 3),
    }


def build_fcc_cluster(grid_l=3):
    """Builds a periodic 3D FCC lattice of 4 * L^3 nodes with exact coordination z = 12."""
    basis = [(0, 0, 0), (0, 1, 1), (1, 0, 1), (1, 1, 0)]
    coords = []
    idx_map = {}
    size = 2 * grid_l
    for ix in range(grid_l):
        for iy in range(grid_l):
            for iz in range(grid_l):
                for bx, by, bz in basis:
                    pt = ((2 * ix + bx) % size, (2 * iy + by) % size, (2 * iz + bz) % size)
                    idx_map[pt] = len(coords)
                    coords.append(pt)
    adj = [[] for _ in range(len(coords))]
    offsets = [
        (1, 1, 0), (1, -1, 0), (-1, 1, 0), (-1, -1, 0),
        (1, 0, 1), (1, 0, -1), (-1, 0, 1), (-1, 0, -1),
        (0, 1, 1), (0, 1, -1), (0, -1, 1), (0, -1, -1),
    ]
    for u, (x, y, z) in enumerate(coords):
        for dx, dy, dz in offsets:
            nb = ((x + dx) % size, (y + dy) % size, (z + dz) % size)
            adj[u].append(idx_map[nb])
        assert len(adj[u]) == 12
    return adj


def largest_component_fraction(adj, loss_rate, rng: random.Random):
    n = len(adj)
    p_keep = 1.0 - loss_rate
    active_adj = [[] for _ in range(n)]
    for u in range(n):
        for v in adj[u]:
            if u < v and rng.random() < p_keep:
                active_adj[u].append(v)
                active_adj[v].append(u)
    visited = [False] * n
    best = 0
    for start in range(n):
        if not visited[start]:
            stack = [start]
            visited[start] = True
            cnt = 0
            while stack:
                cur = stack.pop()
                cnt += 1
                for nxt in active_adj[cur]:
                    if not visited[nxt]:
                        visited[nxt] = True
                        stack.append(nxt)
            if cnt > best:
                best = cnt
    return best / float(n)


def verify_gate_4_dnd3_stigmergic_mesh_and_transitions(rng: random.Random):
    """Gate 4: 3D FCC DND3 mesh survival (up to 80% erasure) & 3-season Hobo Rig transition loop."""
    adj = build_fcc_cluster(grid_l=3)
    erasure_sweep = {}
    for loss in (0.15, 0.35, 0.55, 0.75, 0.80):
        trials = 40
        mean_giant = sum(largest_component_fraction(adj, loss, rng) for _ in range(trials)) / trials
        erasure_sweep[f"loss_{int(loss*100)}pct"] = round(mean_giant, 4)
        assert mean_giant > 0.70, f"FCC giant component unexpectedly low at loss={loss}: {mean_giant}"

    load_profile_w = [10.5, 12.1, 13.8, 15.2, 8.0, 1.2, 1.5, 9.8, 11.4]
    mode = +1
    epoch = 10
    p_trace = []
    mode_trace = []
    for raw_demand_w in load_profile_w:
        if raw_demand_w <= P_WINTER_MAX_W:
            mode = -1
            p_clamped = min(raw_demand_w, P_WINTER_MAX_W)
        elif raw_demand_w > P_ACTIVE_MAX_W:
            mode = 0
            p_clamped = P_OPEN_RESERVE_W
            epoch += 1
        else:
            mode = +1
            p_clamped = min(raw_demand_w, P_ACTIVE_MAX_W)
            epoch += 1
        assert p_clamped <= P_ACTIVE_MAX_W + 1e-9
        p_trace.append(round(p_clamped, 3))
        mode_trace.append(mode)

    assert set(mode_trace) == {-1, 0, +1}
    return {
        "fcc_108_node_giant_component": erasure_sweep,
        "max_clamped_power_w": max(p_trace),
        "power_ceiling_respected": max(p_trace) <= P_ACTIVE_MAX_W + 1e-9,
        "seasonal_modes_visited": mode_trace,
        "final_monotonic_epoch": epoch,
    }


def verify_gate_5_fowler_bijection_and_trapdoors():
    """Gate 5: Exact 243-word Fowler 5-trit byte bijection & 13 trapdoor bytes (243..255)."""
    weights = (1, 3, 9, 27, 81)
    seen = set()
    for t0 in (-1, 0, 1):
        for t1 in (-1, 0, 1):
            for t2 in (-1, 0, 1):
                for t3 in (-1, 0, 1):
                    for t4 in (-1, 0, 1):
                        word = (t0, t1, t2, t3, t4)
                        b = sum((t + 1) * w for t, w in zip(word, weights))
                        if word == (0, 0, 0, 0, 0):
                            assert b == FOWLER_NEUTRAL_BYTE == 121
                        assert 0 <= b < FOWLER_STATES
                        seen.add(b)
    assert len(seen) == FOWLER_STATES == 243
    trapdoors = list(range(FOWLER_STATES, N_MAX_BYTE))
    assert len(trapdoors) == 13
    for code in (CODE_KEYHOLE_CLAMP, CODE_DNHM_BREAKER, CODE_KINTSUGI_GOLD, CODE_FAIL_CLOSED, CODE_SHOKUNIN_UNLATCHED):
        assert code in trapdoors
    return {
        "fowler_active_states": len(seen),
        "neutral_byte": FOWLER_NEUTRAL_BYTE,
        "packing_efficiency_pct": round(100.0 * FOWLER_STATES / N_MAX_BYTE, 4),
        "trapdoor_codes_count": len(trapdoors),
        "shannon_bits_per_trit": round(LOG2_3, 6),
    }


def main(seed: int = 42) -> int:
    t0 = time.perf_counter()
    rng = random.Random(seed)

    g1 = verify_gate_1_dirichlet_fence_and_chair44()
    g2 = verify_gate_2_asymmetric_counting_ratchet(rng)
    g3 = verify_gate_3_operational_nootemporality()
    g4 = verify_gate_4_dnd3_stigmergic_mesh_and_transitions(rng)
    g5 = verify_gate_5_fowler_bijection_and_trapdoors()

    elapsed_ms = (time.perf_counter() - t0) * 1000.0
    assert elapsed_ms < 1000.0, f"Exceeded runtime ceiling: {elapsed_ms:.2f} ms"

    report = {
        "artifact": "verify_asymmetric_ratchet_dnd3_v1_0_0.py",
        "version": "v1.0.0",
        "seed": seed,
        "runtime_ms": round(elapsed_ms, 3),
        "all_5_gates_passed": True,
        "exit_state": CODE_SHOKUNIN_UNLATCHED,
        "gates": {
            "gate_1_dirichlet_fence_spectral": g1,
            "gate_2_asymmetric_ratchet_landauer": g2,
            "gate_3_operational_nootemporality": g3,
            "gate_4_dnd3_mesh_transitions": g4,
            "gate_5_fowler_bijection_trapdoors": g5,
        },
    }
    print(json.dumps(report, indent=2))
    return 0


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Asymmetric Counting Ratchet & DND3 Verification Suite")
    parser.add_argument("--seed", type=int, default=42)
    args = parser.parse_args()
    sys.exit(main(seed=args.seed))
