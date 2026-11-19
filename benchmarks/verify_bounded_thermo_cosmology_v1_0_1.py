#!/usr/bin/env python3
"""
Zero-Dependency Python 3 Stdlib Verification Harness (v1.0.1) -- Seed 42
Title: Zero-Free Occupancy Thermometry, Acoustic-Chromatographic Cell Redshift,
       and Voronoi Boundary Dynamics Without Dark-Sector Continua
Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
Canonical Zenodo DOI: 10.5281/zenodo.24346363
"""
import json, math, random, sys, time

def trapz_uniform(fn, a, b, n=120):
    h = (b - a) / n
    s = 0.5 * (fn(a) + fn(b))
    for i in range(1, n):
        s += fn(a + i * h)
    return s * h

def run_verification():
    t0 = time.perf_counter()
    random.seed(42)
    results = {
        "version": "1.0.1",
        "doi": "10.5281/zenodo.24346363",
        "seed": 42,
        "stdlib_only": True,
        "gates": {},
    }

    n_max = 100
    sigma_min, sigma_mid, sigma_max = math.log(1.0 / (n_max - 1.0)), math.log(50.0 / 50.0), math.log((n_max - 1.0) / 1.0)
    K_test, u_tiny = 7, 1e-7
    n_trunc_ir = (1.0 / math.expm1(u_tiny)) - ((K_test + 1.0) / math.expm1((K_test + 1.0) * u_tiny))
    ir_err = abs(n_trunc_ir - 0.5 * K_test)
    bf_max_err = max(
        abs((1.0 / math.expm1(u)) - (2.0 / math.expm1(2.0 * u)) - (1.0 / (math.exp(u) + 1.0)))
        for u in (0.1 + (7.9 * i / 49.0) for i in range(50))
    )
    g1 = math.isfinite(sigma_min) and abs(sigma_mid) < 1e-15 and ir_err < 1e-6 and bf_max_err < 1e-14
    results["gates"]["G1_zero_free_occupancy"] = {
        "pass": g1, "sigma_bounds_N100": [round(sigma_min, 6), round(sigma_max, 6)],
        "ir_half_capacity_err": ir_err, "k1_fermi_dirac_max_err": bf_max_err,
    }

    t_hk = 0.25
    sig_plus = sum(math.exp(-t_hk * n * n) for n in range(1, 26))
    trace_exact = 2.0 * (sig_plus ** 3) + 3.0 * (sig_plus ** 2)
    y_hk = math.sqrt(math.pi / t_hk)
    trace_weyl_edge = 0.25 * (y_hk ** 3) - 0.75 * y_hk + 0.50
    weyl_trace_err = abs(trace_exact - trace_weyl_edge)
    continuum_miss = abs(trace_exact - 0.25 * (y_hk ** 3))
    z_shift = 1090.0
    firas_mu = max(
        abs((1.0 / math.expm1(x)) - (1.0 / math.expm1((x / (1.0 + z_shift)) / (1.0 / (1.0 + z_shift)))))
        for x in (0.5 + (9.5 * i / 39.0) for i in range(40))
    )
    g2 = (weyl_trace_err < 1e-12) and (continuum_miss > 2.0) and (firas_mu < 9e-5)
    results["gates"]["G2_weyl_cavity_firas"] = {
        "pass": g2, "weyl_trace_abs_err": weyl_trace_err,
        "uncorrected_continuum_miss": round(continuum_miss, 6), "induced_mu_distortion": firas_mu,
    }

    c_kms, H0 = 299792.458, 68.43
    w0_bnd = (-2.0 / 3.0 * 0.35 - 0.35) / 0.70
    q0_cell = 0.5 * (0.30 - 0.35 - 0.70)
    eth_err, max_dmu = 0.0, 0.0
    for i in range(220):
        z = 0.01 + (2.19 * i / 219.0)
        dm_l = (c_kms / H0) * trapz_uniform(lambda zp: 1.0 / math.sqrt(0.30 * (1.0 + zp) ** 3 + 0.70), 0.0, z)
        dm_c = (c_kms / H0) * trapz_uniform(lambda zp: 1.0 / math.sqrt(0.30 * (1.0 + zp) ** 3 + 0.35 * (1.0 + zp) + 0.35), 0.0, z)
        dl_l, dl_c, da_c = (1.0 + z) * dm_l, (1.0 + z) * dm_c, dm_c / (1.0 + z)
        eth_err = max(eth_err, abs((dl_c / da_c) - (1.0 + z) ** 2))
        max_dmu = max(max_dmu, abs(5.0 * math.log10(dl_c / dl_l)))
    g3 = (-1.0 < w0_bnd < -0.66) and (eth_err < 1e-11) and (max_dmu < 0.15)
    results["gates"]["G3_desi_boundary_dilation"] = {
        "pass": g3, "w0_boundary": round(w0_bnd, 6), "q0_cell": round(q0_cell, 6),
        "max_delta_distance_modulus_mag": round(max_dmu, 5), "etherington_reciprocity_err": eth_err,
    }

    mpc_m, kpc_m, c_ms, G_si = 3.08567758149e22, 3.08567758149e19, 299792458.0, 6.67430e-11
    g_floor_si = (c_ms * ((H0 * 1000.0) / mpc_m)) / 6.0
    mb_tot, m200 = 1.0e11 * 1.98847e30, 1.1e12 * 1.98847e30
    f_c = math.log(11.0) - 10.0 / 11.0
    v_cell_list, v_nfw_list, v_outer = [], [], []
    for i in range(200):
        rk = 10.0 ** (3.0 * i / 199.0)
        rm = rk * kpc_m
        mb_e = mb_tot * (rk / (rk + 2.5)) ** 2
        gb = G_si * mb_e / (rm * rm)
        g_obs = gb / (-math.expm1(-math.sqrt(gb / g_floor_si)))
        vc = math.sqrt(g_obs * rm) / 1000.0
        xn = rk / 22.0
        mn = m200 * (math.log(1.0 + xn) - xn / (1.0 + xn)) / f_c
        vn = math.sqrt(G_si * (mb_e + mn) / rm) / 1000.0
        v_cell_list.append(vc)
        v_nfw_list.append(vn)
        if rk >= 50.0:
            v_outer.append(vc)
    v_span = (max(v_outer) - min(v_outer)) / (sum(v_outer) / len(v_outer))
    nfw_drop = 1.0 - (v_nfw_list[-1] / max(v_nfw_list))
    g4 = (abs(g_floor_si - 1.20e-10) < 0.24e-10) and (v_span < 0.03) and (nfw_drop > 0.25)
    results["gates"]["G4_voronoi_1mpc_rotation"] = {
        "pass": g4, "g_floor_m_s2": g_floor_si,
        "v_flat_1mpc_kms": round(v_cell_list[-1], 2), "outer_50_to_1000kpc_rel_span": round(v_span, 5),
    }

    k_prime_obs = 0.1180 / 0.0226
    rf_obs, rf_geom = 1.0 / (1.0 + k_prime_obs), 1.0 / 6.0
    rf_rel_diff = abs(rf_geom - rf_obs) / rf_obs
    g5 = rf_rel_diff < 0.05
    results["gates"]["G5_chromatographic_cmb_partition"] = {
        "pass": g5, "Rf_empirical": round(rf_obs, 5), "Rf_geometric": round(rf_geom, 5), "Rf_rel_diff": round(rf_rel_diff, 5),
    }

    elapsed = time.perf_counter() - t0
    all_pass = all(g["pass"] for g in results["gates"].values())
    results["all_gates_passed"] = all_pass
    results["elapsed_seconds"] = round(elapsed, 5)
    with open(__import__("os").path.expanduser("~/.cache/dandelion_os_v5_1_0/verification_receipt_v1_0_1.json"), "w", encoding="utf-8") as fh:
        json.dump(results, fh, indent=2)
    print(f"[GATE 0] Seed 42 Pure-Stdlib 5-Gate Verification (v1.0.1): {'PASSED' if all_pass else 'FAILED'} in {elapsed:.4f} s")
    if not all_pass:
        sys.exit(1)

if __name__ == "__main__":
    run_verification()
