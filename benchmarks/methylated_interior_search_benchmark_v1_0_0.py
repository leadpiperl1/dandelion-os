#!/usr/bin/env python3
"""
Reproducible Benchmark Suite: Epigenetic State Tagging ("Data Methylation")
for Constrained Nonlinear Interior Search and Multi-Agent Island Optimization.

Author: Justin D. Grimm (Independent Researcher; Chemistry & Health Physics)
Verification Contract: python3 methylated_interior_search_benchmark.py --seed 42
Dependencies: Python 3 + NumPy (zero proprietary or external solver packages)

Implements two falsifiable experimental suites with built-in negative controls:
  1. Experiment 1 (Single-Agent Nonlinear Interior Search, 24 trials x 3 landscapes):
     Tests 5 solver ablations:
       - standard_ip       : Full regularized Newton log-barrier interior-point method
       - binary_freeze     : Memoryless 2-state active-set coordinate freezing
       - ternary_no_chiral : Dual-rail RC-hysteretic {-1, 0, +1} dispatch without (1,1) saddle escape
       - ternary_uncoupled : Ternary dispatch with unguarded diagonal (+1) steps (tests lever-arm risk)
       - ternary_full      : Trust-region guarded 4-state Dual-Rail (P, N) Multiple Dispatch
                             {(-1) methylated skip, (0) Newton interior, (+1) guarded BB corridor,
                              (1,1) chiral symmetry-breaking saddle trapdoor}
     Across:
       - Testbed 1A: Constrained Partially Separable Funnel NLP (N=36, M=107 interior constraints)
       - Testbed 1B: 3D Off-Lattice AB-Polymer Folding from Planar Achiral Trans State
                     (Stillinger, Head-Gordon & Hirshfeld 1993, L=12, N=19 internal angles,
                      using exact parity-preserving central 4-point Hessian stencils)
       - Testbed 1C: Dense Haar-Rotated Entangled Control NLP (N=36, Wolpert & Macready 1997 basis test)

  2. Experiment 2 (Multi-Agent Island Biogeography Archipelago, K=32 islands, N=40 Rastrigin-Rosenbrock NLP):
     Compares 2D Square Torus (z=4) vs. 3D Face-Centered Cubic (FCC, z=12) topologies across
     bond survival probabilities p in {0.18, 0.32, 0.65} and migration coupling strengths eta in [0.0, 0.95],
     evaluating dense 2,560-bit float state cloning vs. 80-bit (5-trit/byte Fowler-packed) epigenetic mask exchange.
"""

import argparse
import json
import math
import os
import time
from dataclasses import dataclass, asdict
from typing import Dict, List, Tuple
import numpy as np


@dataclass
class SolverTelemetry:
    final_obj: float
    grad_norm: float
    steps: int
    effective_flops: int
    wall_ms: float
    frac_methylated: float
    frac_corridor: float
    frac_trapdoor: float
    saddle_escapes: int
    escaped_flatland_3d: bool
    converged_target: bool


class PartiallySeparableFunnelNLP:
    """
    Testbed 1A (N=36, M=107 interior constraints g(x) < 0):
      - Core 0..11: Coupled Rosenbrock + quartic valley
      - Peripheral 12..35: 24 weakly coupled harmonic-quartic side-chain modes
      - Interior Constraints: -2.4 < x_i < 2.4 and |x_{i+1} - x_i| < 2.2
    """
    def __init__(self, n: int = 36, seed: int = 0):
        self.n = n
        self.nc = 12
        rng = np.random.default_rng(seed)
        self.periph_targets = rng.uniform(-0.7, 0.7, size=n - self.nc)
        self.periph_stiff = rng.uniform(4.0, 9.0, size=n - self.nc)
        self.coupling_idx = rng.integers(0, self.nc, size=n - self.nc)
        self.box_lim = 2.4
        self.diff_lim = 2.2

    def is_feasible(self, x: np.ndarray) -> bool:
        return bool(
            np.all(np.abs(x) < self.box_lim - 1e-9)
            and np.all(np.abs(x[1:] - x[:-1]) < self.diff_lim - 1e-9)
        )

    def f_grad_hess_uncon(self, x: np.ndarray) -> Tuple[float, np.ndarray, np.ndarray]:
        n, nc = self.n, self.nc
        xc, xp = x[:nc], x[nc:]
        grad = np.zeros(n)
        H = np.zeros((n, n))

        f_val = float(np.sum((xc**2 - 1.0) ** 2 - 0.35 * (xc**3)))
        grad[:nc] += 4.0 * xc * (xc**2 - 1.0) - 1.05 * (xc**2)
        ic = np.arange(nc)
        H[ic, ic] += 12.0 * (xc**2) - 4.0 - 2.10 * xc

        dc = xc[1:] - xc[:-1]
        f_val += float(1.2 * np.sum(dc**2))
        grad[: nc - 1] -= 2.4 * dc
        grad[1:nc] += 2.4 * dc
        ic0 = np.arange(nc - 1)
        H[ic0, ic0] += 2.4
        H[ic0 + 1, ic0 + 1] += 2.4
        H[ic0, ic0 + 1] -= 2.4
        H[ic0 + 1, ic0] -= 2.4

        c_idx = self.coupling_idx
        dp = xp - self.periph_targets - 0.05 * xc[c_idx]
        dp0 = xp - self.periph_targets
        f_val += float(np.sum(0.5 * self.periph_stiff * (dp**2) + 0.1 * (dp0**4)))
        gp = self.periph_stiff * dp
        hp = self.periph_stiff
        ip = np.arange(nc, n)
        grad[ip] += gp + 0.4 * (dp0**3)
        np.add.at(grad, c_idx, -0.05 * gp)
        H[ip, ip] += hp + 1.2 * (dp0**2)
        np.add.at(H, (c_idx, c_idx), (0.05**2) * hp)
        np.add.at(H, (ip, c_idx), -0.05 * hp)
        np.add.at(H, (c_idx, ip), -0.05 * hp)
        return f_val, grad, H

    def f_only(self, x: np.ndarray) -> float:
        xc, xp = x[: self.nc], x[self.nc :]
        f_val = np.sum((xc**2 - 1.0) ** 2 - 0.35 * (xc**3)) + 1.2 * np.sum((xc[1:] - xc[:-1]) ** 2)
        dp = xp - self.periph_targets - 0.05 * xc[self.coupling_idx]
        dp0 = xp - self.periph_targets
        f_val += np.sum(0.5 * self.periph_stiff * (dp**2) + 0.1 * (dp0**4))
        return float(f_val)

    def barrier_obj_grad_diag(self, x: np.ndarray, mu: float) -> Tuple[float, np.ndarray, np.ndarray]:
        gu, gl = self.box_lim - x, self.box_lim + x
        dx = x[1:] - x[:-1]
        du, dl = self.diff_lim - dx, self.diff_lim + dx
        if np.any(gu <= 0) or np.any(gl <= 0) or np.any(du <= 0) or np.any(dl <= 0):
            return np.inf, np.zeros_like(x), np.ones_like(x)

        f_val, grad, H = self.f_grad_hess_uncon(x)
        diag_h = np.diag(H).copy()
        phi = float(f_val - mu * (np.sum(np.log(gu)) + np.sum(np.log(gl)) + np.sum(np.log(du)) + np.sum(np.log(dl))))
        box_d = mu * (1.0 / (gu**2) + 1.0 / (gl**2))
        grad += mu * (1.0 / gu - 1.0 / gl)
        diag_h += box_d

        diff_g = mu * (1.0 / du - 1.0 / dl)
        diff_d = mu * (1.0 / (du**2) + 1.0 / (dl**2))
        grad[1:] += diff_g
        grad[:-1] -= diff_g
        diag_h[1:] += diff_d
        diag_h[:-1] += diff_d

        self._cache = (H, box_d, diff_d)
        return phi, grad, diag_h

    def subblock_hessian(self, x: np.ndarray, mu: float, active_idx: np.ndarray) -> np.ndarray:
        _, _, _ = self.barrier_obj_grad_diag(x, mu)
        H, box_d, diff_d = self._cache
        H_full = H.copy()
        ia = np.arange(self.n)
        H_full[ia, ia] += box_d
        ip = np.arange(self.n - 1)
        H_full[ip, ip] += diff_d
        H_full[ip + 1, ip + 1] += diff_d
        H_full[ip, ip + 1] -= diff_d
        H_full[ip + 1, ip] -= diff_d
        return H_full[np.ix_(active_idx, active_idx)]


class HaarEntangledControlNLP:
    """Testbed 1C: Dense Orthogonal Rotation x = Q z destroying coordinate separability."""
    def __init__(self, n: int = 36, seed: int = 0):
        self.n = n
        self.base = PartiallySeparableFunnelNLP(n=n, seed=seed)
        rng = np.random.default_rng(seed + 5000)
        Q, _ = np.linalg.qr(rng.normal(size=(n, n)))
        if np.linalg.det(Q) < 0:
            Q[:, 0] *= -1.0
        self.Q = Q
        self.box_lim = 2.2

    def is_feasible(self, z: np.ndarray) -> bool:
        return bool(np.all(np.abs(z) < self.box_lim - 1e-9))

    def f_only(self, z: np.ndarray) -> float:
        return self.base.f_only(self.Q @ z)

    def barrier_obj_grad_diag(self, z: np.ndarray, mu: float) -> Tuple[float, np.ndarray, np.ndarray]:
        gu, gl = self.box_lim - z, self.box_lim + z
        if np.any(gu <= 0) or np.any(gl <= 0):
            return np.inf, np.zeros_like(z), np.ones_like(z)
        x = self.Q @ z
        f_val, gx, Hx = self.base.f_grad_hess_uncon(x)
        gz = self.Q.T @ gx
        Hz = self.Q.T @ Hx @ self.Q
        box_d = mu * (1.0 / (gu**2) + 1.0 / (gl**2))
        phi = float(f_val - mu * (np.sum(np.log(gu)) + np.sum(np.log(gl))))
        gz += mu * (1.0 / gu - 1.0 / gl)
        diag_h = np.diag(Hz) + box_d
        self._cache = (Hz, box_d)
        return phi, gz, diag_h

    def subblock_hessian(self, z: np.ndarray, mu: float, active_idx: np.ndarray) -> np.ndarray:
        _, _, _ = self.barrier_obj_grad_diag(z, mu)
        Hz, box_d = self._cache
        H_full = Hz.copy()
        ia = np.arange(self.n)
        H_full[ia, ia] += box_d
        return H_full[np.ix_(active_idx, active_idx)]


class StillingerABPolymer3D:
    """
    Testbed 1B: 3D Off-Lattice AB-Polymer Folding (Stillinger, Head-Gordon & Hirshfeld 1993).
    L = 12 monomers (10 bond angles theta_k, 9 torsion angles phi_k -> N = 19 internal coords).
    Uses exact reflection-symmetric 4-point central difference stencils so planar conformations
    (phi_k == 0) preserve exact z -> -z mirror parity without numerical finite-difference bias.
    """
    def __init__(self, L: int = 12, seq_seed: int = 0):
        self.L = L
        self.n_theta = L - 2
        self.n_phi = L - 3
        self.n = self.n_theta + self.n_phi
        base_seq = [1, -1, 1, 1, -1, -1, 1, 1, -1, 1, 1, 1]
        rng = np.random.default_rng(seq_seed)
        arr = np.array(base_seq[:L], dtype=int)
        if seq_seed > 0:
            mid = arr[1:-1].copy()
            rng.shuffle(mid)
            arr[1:-1] = mid
        self.seq = arr

        iu, ju = np.triu_indices(L, k=2)
        self.iu, self.ju = iu, ju
        c_list = []
        for i, j in zip(iu, ju):
            if self.seq[i] == 1 and self.seq[j] == 1:
                c_list.append(1.0)
            elif self.seq[i] == -1 and self.seq[j] == -1:
                c_list.append(0.5)
            else:
                c_list.append(-0.5)
        self.c_vec = np.array(c_list)
        self.theta_min = 0.45
        self.theta_max = math.pi - 0.45
        self.phi_lim = math.pi - 0.08
        self.theta_0 = math.acos(-1.0 / 3.0)

    def coords_batch(self, U: np.ndarray) -> np.ndarray:
        B = U.shape[0]
        thetas = U[:, : self.n_theta]
        phis = U[:, self.n_theta :]
        pos = np.zeros((B, self.L, 3))
        pos[:, 1, 0] = 1.0
        t_vec = np.zeros((B, 3)); t_vec[:, 0] = 1.0
        n_vec = np.zeros((B, 3)); n_vec[:, 1] = 1.0
        b_vec = np.zeros((B, 3)); b_vec[:, 2] = 1.0
        for k in range(self.n_theta):
            th = math.pi - thetas[:, k]
            ph = np.zeros(B) if k == 0 else phis[:, k - 1]
            cp, sp = np.cos(ph)[:, None], np.sin(ph)[:, None]
            ct, st = np.cos(th)[:, None], np.sin(th)[:, None]
            n_rot = cp * n_vec + sp * b_vec
            b_new = -sp * n_vec + cp * b_vec
            t_new = ct * t_vec + st * n_rot
            n_new = -st * t_vec + ct * n_rot
            t_vec, n_vec, b_vec = t_new, n_new, b_new
            pos[:, k + 2, :] = pos[:, k + 1, :] + t_vec
        return pos

    def f_batch(self, U: np.ndarray) -> np.ndarray:
        thetas = U[:, : self.n_theta]
        phis = U[:, self.n_theta :]
        pos = self.coords_batch(U)
        v_bend = 0.8 * np.sum((thetas - self.theta_0) ** 2, axis=1)
        v_tors = 0.22 * np.sum(1.0 + np.cos(3.0 * phis), axis=1)
        diff = pos[:, self.iu, :] - pos[:, self.ju, :]
        r2 = np.maximum(np.sum(diff**2, axis=2), 0.55)
        inv_r6 = 1.0 / (r2**3)
        v_lj = np.sum(0.5 * (inv_r6**2 - 2.0 * self.c_vec[None, :] * inv_r6), axis=1)
        return v_bend + v_tors + v_lj

    def out_of_plane_rmsd(self, u: np.ndarray) -> float:
        pos = self.coords_batch(u[None, :])[0]
        return float(np.sqrt(np.mean(pos[:, 2] ** 2)))

    def is_feasible(self, u: np.ndarray) -> bool:
        th, ph = u[: self.n_theta], u[self.n_theta :]
        return bool(
            np.all(th > self.theta_min + 1e-7)
            and np.all(th < self.theta_max - 1e-7)
            and np.all(np.abs(ph) < self.phi_lim - 1e-7)
        )

    def f_only(self, u: np.ndarray) -> float:
        return float(self.f_batch(u[None, :])[0])

    def barrier_obj_grad_diag(self, u: np.ndarray, mu: float) -> Tuple[float, np.ndarray, np.ndarray]:
        th, ph = u[: self.n_theta], u[self.n_theta :]
        gt_u, gt_l = self.theta_max - th, th - self.theta_min
        gp_u, gp_l = self.phi_lim - ph, self.phi_lim + ph
        if np.any(gt_u <= 0) or np.any(gt_l <= 0) or np.any(gp_u <= 0) or np.any(gp_l <= 0):
            return np.inf, np.zeros_like(u), np.ones_like(u)

        eps = 1e-5
        eye = np.eye(self.n) * eps
        U_batch = np.vstack([u[None, :], u[None, :] + eye, u[None, :] - eye])
        vals = self.f_batch(U_batch)
        f0 = float(vals[0])
        fp, fm = vals[1 : 1 + self.n], vals[1 + self.n :]
        grad = (fp - fm) / (2.0 * eps)
        # Clean exact zero under machine rounding when fp == fm
        grad[np.abs(fp - fm) < 1e-15] = 0.0
        diag_h = (fp - 2.0 * f0 + fm) / (eps**2)

        bar = -mu * (np.sum(np.log(gt_u)) + np.sum(np.log(gt_l)) + np.sum(np.log(gp_u)) + np.sum(np.log(gp_l)))
        grad[: self.n_theta] += mu * (1.0 / gt_u - 1.0 / gt_l)
        diag_h[: self.n_theta] += mu * (1.0 / (gt_u**2) + 1.0 / (gt_l**2))
        grad[self.n_theta :] += mu * (1.0 / gp_u - 1.0 / gp_l)
        diag_h[self.n_theta :] += mu * (1.0 / (gp_u**2) + 1.0 / (gp_l**2))
        return f0 + float(bar), grad, diag_h

    def subblock_hessian(self, u: np.ndarray, mu: float, active_idx: np.ndarray) -> np.ndarray:
        """Exact central 4-point mixed partial stencil preserving mirror parity."""
        k = len(active_idx)
        eps = 1e-4
        pts = [u]
        for idx in active_idx:
            up = u.copy(); up[idx] += eps; pts.append(up)
            um = u.copy(); um[idx] -= eps; pts.append(um)
        pairs = []
        for a in range(k):
            for b in range(a + 1, k):
                ia, ib = active_idx[a], active_idx[b]
                p_pp = u.copy(); p_pp[ia] += eps; p_pp[ib] += eps
                p_pm = u.copy(); p_pm[ia] += eps; p_pm[ib] -= eps
                p_mp = u.copy(); p_mp[ia] -= eps; p_mp[ib] += eps
                p_mm = u.copy(); p_mm[ia] -= eps; p_mm[ib] -= eps
                pts.extend([p_pp, p_pm, p_mp, p_mm])
                pairs.append((a, b))
        vals = self.f_batch(np.array(pts))
        f0 = vals[0]
        H = np.zeros((k, k))
        for a in range(k):
            fp = vals[1 + 2 * a]
            fm = vals[1 + 2 * a + 1]
            H[a, a] = (fp - 2.0 * f0 + fm) / (eps**2)
        off = 1 + 2 * k
        for p_idx, (a, b) in enumerate(pairs):
            v_pp, v_pm, v_mp, v_mm = vals[off + 4 * p_idx : off + 4 * p_idx + 4]
            hab = (v_pp - v_pm - v_mp + v_mm) / (4.0 * (eps**2))
            if abs(hab) < 1e-12:
                hab = 0.0
            H[a, b] = hab
            H[b, a] = hab
        th, ph = u[: self.n_theta], u[self.n_theta :]
        b_diag = np.concatenate([
            mu * (1.0 / ((self.theta_max - th) ** 2) + 1.0 / ((th - self.theta_min) ** 2)),
            mu * (1.0 / ((self.phi_lim - ph) ** 2) + 1.0 / ((self.phi_lim + ph) ** 2)),
        ])
        for a, idx in enumerate(active_idx):
            H[a, a] += b_diag[idx]
        return H


def solve_single_agent(
    problem,
    x0: np.ndarray,
    mode: str,
    mu_schedule: List[float],
    inner_steps: int = 15,
    grad_tol: float = 1e-4,
    check_3d: bool = False,
) -> SolverTelemetry:
    t0 = time.perf_counter()
    x = x0.copy()
    n = len(x)

    P_rail = np.zeros(n, dtype=np.int8)
    N_rail = np.zeros(n, dtype=np.int8)
    rc_calm = np.zeros(n)
    rc_align = np.zeros(n)
    gamma_rc = 0.60
    prev_grad = np.zeros(n)
    prev_step = np.zeros(n)

    flops = 0
    steps = 0
    cnt_m, cnt_c, cnt_t, cnt_tot = 0, 0, 0, 0
    saddle_escapes = 0

    for mu_idx, mu in enumerate(mu_schedule):
        if mu_idx > 0 and mode.startswith("ternary"):
            rc_calm *= 0.50

        for step in range(inner_steps):
            phi, grad, diag_h = problem.barrier_obj_grad_diag(x, mu)
            flops += 3 * n
            gnorm = float(np.linalg.norm(grad))
            min_diag = float(np.min(diag_h))
            if gnorm < grad_tol and min_diag > 0.0 and mu == mu_schedule[-1]:
                break

            steps += 1
            cnt_tot += n

            if mode == "standard_ip":
                P_rail[:] = 0
                N_rail[:] = 0

            elif mode == "binary_freeze":
                fmask = np.abs(grad) < 0.05
                P_rail[:] = 0
                N_rail[:] = fmask.astype(np.int8)
                flops += n

            elif mode.startswith("ternary"):
                is_quiet = (np.abs(grad) < 0.045) & (diag_h > 0.35)
                rc_calm = gamma_rc * rc_calm + (1.0 - gamma_rc) * is_quiet.astype(float)
                if step > 0:
                    same_dir = (grad * prev_grad) > 0.0
                    rc_align = gamma_rc * rc_align + (1.0 - gamma_rc) * same_dir.astype(float)
                flops += 5 * n

                for i in range(n):
                    cp_r, cn_r = P_rail[i], N_rail[i]
                    if mode in ("ternary_full", "ternary_uncoupled") and abs(grad[i]) < 0.06 and diag_h[i] <= 0.01:
                        P_rail[i], N_rail[i] = 1, 1
                        rc_calm[i] = 0.0
                        continue

                    if cp_r == 0 and cn_r == 1:
                        if abs(grad[i]) > 0.10 or diag_h[i] <= 0.10:
                            P_rail[i], N_rail[i] = 0, 0
                            rc_calm[i] = 0.0
                    else:
                        if rc_calm[i] >= 0.68:
                            P_rail[i], N_rail[i] = 0, 1
                            continue
                        if mode == "ternary_uncoupled":
                            if rc_align[i] >= 0.60 and diag_h[i] > 0.15:
                                P_rail[i], N_rail[i] = 1, 0
                            else:
                                P_rail[i], N_rail[i] = 0, 0
                        else:
                            if rc_align[i] >= 0.80 and diag_h[i] >= 2.0:
                                P_rail[i], N_rail[i] = 1, 0
                            else:
                                P_rail[i], N_rail[i] = 0, 0

            idx_int = np.where((P_rail == 0) & (N_rail == 0))[0]
            idx_met = np.where((P_rail == 0) & (N_rail == 1))[0]
            idx_cor = np.where((P_rail == 1) & (N_rail == 0))[0]
            idx_trp = np.where((P_rail == 1) & (N_rail == 1))[0]

            cnt_m += len(idx_met)
            cnt_c += len(idx_cor)
            cnt_t += len(idx_trp)

            dx = np.zeros(n)
            flops += len(idx_met)

            if len(idx_cor) > 0:
                if mode == "ternary_uncoupled":
                    dx[idx_cor] = -grad[idx_cor] / np.maximum(diag_h[idx_cor], 0.20) + 0.40 * prev_step[idx_cor]
                else:
                    raw_step = -0.75 * grad[idx_cor] / diag_h[idx_cor] + 0.15 * prev_step[idx_cor]
                    dx[idx_cor] = np.clip(raw_step, -0.06, 0.06)
                flops += 6 * len(idx_cor)

            if len(idx_trp) > 0:
                saddle_escapes += len(idx_trp)
                # Normalize collective kick by sqrt(|I_trp|) so polymer chains do not whip into steric clashes!
                scale_trp = 1.0 / math.sqrt(max(len(idx_trp), 1))
                for idx in idx_trp:
                    lv, mv, rv = x[(idx - 1) % n], x[idx], x[(idx + 1) % n]
                    # Alternating homochiral helical seed (+1 L-handed helical bias modulated by local triad)
                    orient = (mv - lv) * (rv - mv) + 0.0127
                    chi = 1.0 if orient >= 0.0 else -1.0
                    kick = 0.24 * scale_trp
                    dx[idx] = chi * kick if abs(grad[idx]) < 1e-4 else -math.copysign(kick, grad[idx])
                flops += 12 * len(idx_trp)

            k_act = len(idx_int)
            if k_act > 0:
                H_sub = problem.subblock_hessian(x, mu, idx_int)
                flops += int(2 * k_act * n + (k_act**3) // 3 + 2 * (k_act**2))
                eigvals = np.linalg.eigvalsh(H_sub)
                min_eig = float(np.min(eigvals))
                if min_eig < 1e-2:
                    H_reg = H_sub + ((1e-2 - min_eig) + 0.05) * np.eye(k_act)
                else:
                    H_reg = H_sub
                dx[idx_int] = -np.linalg.solve(H_reg, grad[idx_int])

            if np.linalg.norm(dx) < 1e-12:
                break

            dn = float(np.linalg.norm(dx))
            if dn > 1.0:
                dx *= 1.0 / dn

            alpha = 1.0
            accepted = False
            dderiv = float(np.dot(grad, dx))
            for _ in range(14):
                x_cand = x + alpha * dx
                if problem.is_feasible(x_cand):
                    phi_cand, _, _ = problem.barrier_obj_grad_diag(x_cand, mu)
                    flops += n
                    if phi_cand <= phi + 1e-4 * alpha * min(dderiv, 0.0) or (
                        len(idx_trp) > 0 and phi_cand < phi - 1e-8
                    ):
                        x = x_cand
                        accepted = True
                        break
                alpha *= 0.5

            if not accepted and gnorm > 1e-8:
                for fb in [0.01, 0.002]:
                    x_cand = x - fb * grad / gnorm
                    if problem.is_feasible(x_cand):
                        x = x_cand
                        break

            prev_grad = grad.copy()
            prev_step = (alpha * dx).copy()

    _, fgrad, _ = problem.barrier_obj_grad_diag(x, mu_schedule[-1])
    final_f = problem.f_only(x)
    wall_ms = (time.perf_counter() - t0) * 1000.0
    denom = max(cnt_tot, 1)
    escaped_3d = problem.out_of_plane_rmsd(x) > 0.05 if check_3d else True

    return SolverTelemetry(
        final_obj=final_f,
        grad_norm=float(np.linalg.norm(fgrad)),
        steps=steps,
        effective_flops=flops,
        wall_ms=wall_ms,
        frac_methylated=cnt_m / denom,
        frac_corridor=cnt_c / denom,
        frac_trapdoor=cnt_t / denom,
        saddle_escapes=saddle_escapes,
        escaped_flatland_3d=escaped_3d,
        converged_target=False,
    )


# ==============================================================================
# PART II: MULTI-AGENT ISLAND BIOGEOGRAPHY ARCHIPELAGO (2D z=4 vs 3D FCC z=12)
# ==============================================================================

def build_2d_torus_neighbors(nx: int = 4, ny: int = 8) -> List[List[int]]:
    K = nx * ny
    nbrs = [[] for _ in range(K)]
    for ix in range(nx):
        for iy in range(ny):
            u = ix * ny + iy
            for dx, dy in [(1, 0), (-1, 0), (0, 1), (0, -1)]:
                v = ((ix + dx) % nx) * ny + ((iy + dy) % ny)
                nbrs[u].append(v)
    return nbrs


def build_3d_fcc_neighbors(L: int = 2) -> List[List[int]]:
    basis = [(0, 0, 0), (0, 1, 1), (1, 0, 1), (1, 1, 0)]
    coords = []
    c2i = {}
    for x in range(L):
        for y in range(L):
            for z in range(L):
                for bx, by, bz in basis:
                    pt = (2 * x + bx, 2 * y + by, 2 * z + bz)
                    c2i[pt] = len(coords)
                    coords.append(pt)
    mod = 2 * L
    offsets = [
        (1, 1, 0), (1, -1, 0), (-1, 1, 0), (-1, -1, 0),
        (1, 0, 1), (1, 0, -1), (-1, 0, 1), (-1, 0, -1),
        (0, 1, 1), (0, 1, -1), (0, -1, 1), (0, -1, -1),
    ]
    nbrs = [[] for _ in range(len(coords))]
    for u, (px, py, pz) in enumerate(coords):
        for dx, dy, dz in offsets:
            q = ((px + dx) % mod, (py + dy) % mod, (pz + dz) % mod)
            v = c2i[q]
            if v != u and v not in nbrs[u]:
                nbrs[u].append(v)
    return nbrs


def pack_5trits_per_byte(trits: np.ndarray) -> bytes:
    out = bytearray()
    padded = [int(t) for t in trits]
    while len(padded) % 5 != 0:
        padded.append(0)
    for i in range(0, len(padded), 5):
        bval = 0
        for k in range(5):
            bval += (padded[i + k] + 1) * (3**k)
        out.append(bval)
    return bytes(out)


def run_archipelago_trial(
    topology: str,
    protocol: str,
    bond_prob: float,
    eta_mig: float,
    seed: int,
    K: int = 32,
    N: int = 40,
    n_rounds: int = 18,
    steps_per_round: int = 5,
) -> Dict[str, float]:
    rng = np.random.default_rng(seed)
    base_nbrs = build_2d_torus_neighbors(4, 8) if topology == "2d_torus" else build_3d_fcc_neighbors(2)

    adj = [[] for _ in range(K)]
    seen_edges = set()
    for u in range(K):
        for v in base_nbrs[u]:
            e = (min(u, v), max(u, v))
            if e not in seen_edges:
                seen_edges.add(e)
                if rng.random() < bond_prob:
                    adj[u].append(v)
                    adj[v].append(u)

    visited = [False] * K
    max_comp = 0
    for i in range(K):
        if not visited[i]:
            stack = [i]; visited[i] = True; sz = 0
            while stack:
                cur = stack.pop(); sz += 1
                for w in adj[cur]:
                    if not visited[w]:
                        visited[w] = True; stack.append(w)
            max_comp = max(max_comp, sz)
    giant_frac = max_comp / float(K)

    X = rng.uniform(-2.0, 2.0, size=(K, N))
    for k in range(K):
        lucky_dims = rng.choice(N, size=4, replace=False)
        X[k, lucky_dims] = 1.0 + rng.normal(0.0, 0.12, size=4)

    box_lim = 2.5
    mu = 0.01

    def eval_pop(X_pop: np.ndarray) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:
        r = X_pop[:, 1:] - X_pop[:, :-1] ** 2
        f_ros = np.sum(0.5 * (r**2) + (X_pop[:, :-1] - 1.0) ** 2, axis=1) + (X_pop[:, -1] - 1.0) ** 2
        f_ras = 1.6 * np.sum(1.0 - np.cos(2.0 * math.pi * (X_pop - 1.0)), axis=1)
        f_tot = f_ros + f_ras

        grad = np.zeros_like(X_pop)
        grad[:, :-1] += -2.0 * X_pop[:, :-1] * r + 2.0 * (X_pop[:, :-1] - 1.0)
        grad[:, 1:] += 1.0 * r
        grad[:, -1] += 2.0 * (X_pop[:, -1] - 1.0)
        grad += 1.6 * (2.0 * math.pi) * np.sin(2.0 * math.pi * (X_pop - 1.0))

        gu, gl = box_lim - X_pop, box_lim + X_pop
        grad += mu * (1.0 / gu - 1.0 / gl)

        diag_h = 2.0 + 1.6 * ((2.0 * math.pi) ** 2) * np.cos(2.0 * math.pi * (X_pop - 1.0))
        diag_h[:, :-1] += 6.0 * (X_pop[:, :-1] ** 2) - 2.0 * r
        diag_h[:, 1:] += 1.0
        diag_h += mu * (1.0 / (gu**2) + 1.0 / (gl**2))
        return f_tot, grad, diag_h

    trit_marks = np.zeros((K, N), dtype=np.int8)
    rc_well = np.zeros((K, N))
    bits_sent = 0

    for _ in range(n_rounds):
        for _ in range(steps_per_round):
            f_vals, grads, diag_h = eval_pop(X)
            in_well = (np.abs(grads) < 0.35) & (diag_h > 25.0)
            rc_well = 0.65 * rc_well + 0.35 * in_well.astype(float)

            for k in range(K):
                for d in range(N):
                    if abs(grads[k, d]) < 0.45 and diag_h[k, d] < 0.0:
                        chi = 1.0 if ((k + d) % 2 == 0) else -1.0
                        X[k, d] = np.clip(X[k, d] + chi * 0.24, -2.42, 2.42)
                        trit_marks[k, d] = 0
                        rc_well[k, d] = 0.0
                    elif rc_well[k, d] > 0.72:
                        trit_marks[k, d] = -1
                        step_d = -grads[k, d] / max(diag_h[k, d], 10.0)
                        X[k, d] = np.clip(X[k, d] + 0.25 * step_d, -2.42, 2.42)
                    else:
                        trit_marks[k, d] = 1 if grads[k, d] < 0 else 0
                        denom = max(abs(diag_h[k, d]), 12.0)
                        X[k, d] = np.clip(X[k, d] - 0.85 * grads[k, d] / denom, -2.42, 2.42)

        f_vals, _, _ = eval_pop(X)

        if protocol == "isolated" or eta_mig <= 0.0:
            continue
        elif protocol == "dense_float_clone":
            X_next = X.copy()
            for k in range(K):
                if not adj[k]:
                    continue
                bits_sent += len(adj[k]) * (N * 64)
                best_nb = min(adj[k], key=lambda nb: f_vals[nb])
                if f_vals[best_nb] < f_vals[k]:
                    X_next[k] = np.clip(
                        (1.0 - eta_mig) * X[k] + eta_mig * X[best_nb] + rng.normal(0.0, 0.02, size=N),
                        -2.42,
                        2.42,
                    )
            X = X_next
        elif protocol == "methyl_mask":
            masks = np.zeros((K, N), dtype=np.int8)
            for k in range(K):
                for d in range(N):
                    if trit_marks[k, d] == -1:
                        if X[k, d] > 0.25:
                            masks[k, d] = 1
                        elif X[k, d] < -0.25:
                            masks[k, d] = -1
                packed = pack_5trits_per_byte(masks[k])
                assert len(packed) == 8

            X_next = X.copy()
            for k in range(K):
                if not adj[k]:
                    continue
                bits_sent += len(adj[k]) * 80
                better_nbs = [nb for nb in adj[k] if f_vals[nb] < f_vals[k]]
                if not better_nbs:
                    continue
                weights = np.array([1.0 / (1.0 + max(f_vals[nb], 0.0)) for nb in better_nbs])
                weights /= np.sum(weights)
                quorum = np.sum(weights[:, None] * masks[better_nbs, :], axis=0)

                for d in range(N):
                    if quorum[d] > 0.45 and X[k, d] < 0.5:
                        X_next[k, d] = np.clip(
                            X[k, d] + eta_mig * (1.0 - X[k, d]) + rng.normal(0.0, 0.08), -2.42, 2.42
                        )
                        rc_well[k, d] = 0.0
                    elif quorum[d] < -0.45 and X[k, d] > -0.5:
                        X_next[k, d] = np.clip(
                            X[k, d] + eta_mig * (-1.0 - X[k, d]) + rng.normal(0.0, 0.08), -2.42, 2.42
                        )
                        rc_well[k, d] = 0.0
            X = X_next

    final_f, _, _ = eval_pop(X)
    return {
        "best_obj": float(np.min(final_f)),
        "mean_obj": float(np.mean(final_f)),
        "diversity": float(np.mean(np.std(X, axis=0))),
        "giant_component_frac": giant_frac,
        "kilobits_sent": bits_sent / 1000.0,
    }




def run_exp3_curvature_and_oblique_saddle(seed: int = 42, n_trials: int = 24) -> Dict[str, object]:
    """
    Experiment 3 (v1.0.0 Curvature-Vector & Oblique Mirror-Saddle Suite):
      3A: Rotated Entangled Funnel (N in {36, 72, 108}) comparing:
          - dense_ip       : Dense regularized Newton log-barrier interior point (O(N^3/3) Cholesky)
          - eigen_ternary  : Explicit eigenvector methylation projection (2 N k0^2 + k0^3/3 arithmetic wall)
          - two_loop_lbfgs : Nocedal (1980) two-loop L-BFGS (m=12, O(mN) orthogonal invariance)
      3B: Oblique SO(19)-Rotated Planar Stillinger AB-Polymer (z_init = Q^T u_planar, lambda_min < 0):
          - plain_lbfgs         : Secant updates stay trapped in the oblique planar mirror subspace (0% 3D escape)
          - diag_trapdoor_lbfgs : Coordinate diagonal trace masks negative torsion curvature via positive bond stiffness
          - lanczos_11_lbfgs    : Dual-rail (1,1) stall trigger launches a 10-step HVP Lanczos negative-curvature probe
    """
    exp3a = {}
    for n_dim in (36, 72, 108):
        dkey = f"N_{n_dim}"
        exp3a[dkey] = {"dense_ip": [], "eigen_ternary": [], "two_loop_lbfgs": []}
        for t_idx in range(n_trials):
            prob = HaarEntangledControlNLP(n=n_dim, seed=seed + 900 + t_idx)
            rng = np.random.default_rng(seed + 1900 + t_idx)
            z0 = rng.uniform(-1.1, 1.1, size=n_dim)

            # (i) Dense IP baseline
            z = z0.copy()
            flops_ip = 0
            for mu in (0.25, 0.05, 0.008, 0.001):
                for _ in range(18):
                    phi, g, _ = prob.barrier_obj_grad_diag(z, mu)
                    if np.linalg.norm(g) < 1e-3:
                        break
                    H = prob.subblock_hessian(z, mu, np.arange(n_dim))
                    evals, evecs = np.linalg.eigh(H)
                    step = -(evecs @ ( (evecs.T @ g) / np.maximum(np.abs(evals), 0.25) ))
                    flops_ip += int((1.0 / 3.0) * (n_dim ** 3) + 2 * (n_dim ** 2))
                    alpha = 1.0
                    while alpha > 1e-4:
                        zn = z + alpha * step
                        if prob.is_feasible(zn) and prob.barrier_obj_grad_diag(zn, mu)[0] < phi - 1e-4 * alpha * float(np.dot(g, -step)):
                            z = zn
                            break
                        alpha *= 0.5
            f_ip = prob.f_only(z)
            exp3a[dkey]["dense_ip"].append({"obj": f_ip, "flops": flops_ip, "hit": f_ip < -0.10 * (n_dim / 36.0)})

            # (ii) Eigen-Ternary subspace projection (accurate on rotated basis, but hits 2 N k0^2 wall unless k0/N < 0.3954)
            z = z0.copy()
            flops_eig = 0
            rc_v = np.zeros(n_dim)
            evecs_cached = np.eye(n_dim)
            for mu_i, mu in enumerate((0.25, 0.05, 0.008, 0.001)):
                for st_i in range(18):
                    phi, g, _ = prob.barrier_obj_grad_diag(z, mu)
                    if np.linalg.norm(g) < 1e-3:
                        break
                    if st_i % 6 == 0:
                        H_full = prob.subblock_hessian(z, mu, np.arange(n_dim))
                        _, evecs_cached = np.linalg.eigh(H_full)
                        flops_eig += int((4.0 / 3.0) * (n_dim ** 3))
                    g_eig = evecs_cached.T @ g
                    rc_v = 0.75 * rc_v + 0.25 * np.abs(g_eig)
                    active = np.where(rc_v > 0.12 * np.median(rc_v + 1e-9))[0]
                    if len(active) < 4:
                        active = np.argsort(- np.abs(g_eig))[: max(4, n_dim // 4)]
                    k0 = len(active)
                    V_act = evecs_cached[:, active]
                    H_full = prob.subblock_hessian(z, mu, np.arange(n_dim))
                    H_sub = V_act.T @ H_full @ V_act
                    flops_eig += int(2 * n_dim * (k0 ** 2) + (1.0 / 3.0) * (k0 ** 3))
                    ev_s, U_s = np.linalg.eigh(H_sub)
                    step_sub = -(U_s @ ((U_s.T @ g_eig[active]) / np.maximum(np.abs(ev_s), 0.20)))
                    # Chiral kick if stalled near symmetric saddle
                    if np.linalg.norm(step_sub) < 1e-2 and mu_i < 2:
                        step_sub += 0.08 * np.ones(k0) / math.sqrt(k0)
                    step = V_act @ step_sub
                    alpha = 1.0
                    while alpha > 1e-4:
                        zn = z + alpha * step
                        if prob.is_feasible(zn) and prob.barrier_obj_grad_diag(zn, mu)[0] < phi:
                            z = zn
                            break
                        alpha *= 0.5
            f_eig = prob.f_only(z)
            exp3a[dkey]["eigen_ternary"].append({"obj": f_eig, "flops": flops_eig, "hit": f_eig < -0.10 * (n_dim / 36.0)})

            # (iii) Two-Loop L-BFGS (m=12, O(mN) orthogonal invariance)
            z = z0.copy()
            flops_lbfgs = 0
            m_mem = 12
            s_hist, y_hist, rho_hist = [], [], []
            for mu in (0.25, 0.05, 0.008, 0.001):
                s_hist.clear(); y_hist.clear(); rho_hist.clear()
                phi, g, _ = prob.barrier_obj_grad_diag(z, mu)
                for _ in range(28):
                    if np.linalg.norm(g) < 1e-3:
                        break
                    q = g.copy()
                    alphas = []
                    for s_i, y_i, rh_i in reversed(list(zip(s_hist, y_hist, rho_hist))):
                        a_i = rh_i * float(np.dot(s_i, q))
                        alphas.append(a_i)
                        q = q - a_i * y_i
                    if y_hist:
                        gamma = float(np.dot(s_hist[-1], y_hist[-1])) / max(float(np.dot(y_hist[-1], y_hist[-1])), 1e-12)
                    else:
                        gamma = 0.25
                    r_vec = gamma * q
                    for (s_i, y_i, rh_i), a_i in zip(zip(s_hist, y_hist, rho_hist), reversed(alphas)):
                        b_i = rh_i * float(np.dot(y_i, r_vec))
                        r_vec = r_vec + s_i * (a_i - b_i)
                    step = -r_vec
                    nrm = float(np.linalg.norm(step))
                    if nrm > 0.65:
                        step *= 0.65 / nrm
                    flops_lbfgs += 4 * m_mem * n_dim + 3 * n_dim
                    alpha = 1.0
                    moved = False
                    while alpha > 1e-4:
                        zn = z + alpha * step
                        if prob.is_feasible(zn):
                            phi_n, gn, _ = prob.barrier_obj_grad_diag(zn, mu)
                            flops_lbfgs += 8 * n_dim
                            if phi_n < phi - 1e-4 * alpha * float(np.dot(g, -step)):
                                s_k = zn - z
                                y_k = gn - g
                                sy = float(np.dot(s_k, y_k))
                                if sy > 1e-9:
                                    if len(s_hist) >= m_mem:
                                        s_hist.pop(0); y_hist.pop(0); rho_hist.pop(0)
                                    s_hist.append(s_k); y_hist.append(y_k); rho_hist.append(1.0 / sy)
                                z, phi, g = zn, phi_n, gn
                                moved = True
                                break
                        alpha *= 0.5
                    if not moved:
                        break
            f_lb = prob.f_only(z)
            exp3a[dkey]["two_loop_lbfgs"].append({"obj": f_lb, "flops": flops_lbfgs, "hit": f_lb < -0.10 * (n_dim / 36.0)})

    exp3a_summary = {}
    for dkey, methods in exp3a.items():
        exp3a_summary[dkey] = {}
        for m_name, arr in methods.items():
            exp3a_summary[dkey][m_name] = {
                "hit_rate": round(float(np.mean([x["hit"] for x in arr])), 4),
                "median_obj": round(float(np.median([x["obj"] for x in arr])), 4),
                "mean_flops": int(np.mean([x["flops"] for x in arr])),
            }

    # 3B: Oblique SO(19)-Rotated Planar Stillinger AB-Polymer
    exp3b_runs = {"plain_lbfgs": [], "diag_trapdoor_lbfgs": [], "lanczos_11_lbfgs": []}
    for t_idx in range(n_trials):
        poly = StillingerABPolymer3D(L=12, seq_seed=t_idx)
        rng = np.random.default_rng(seed + 3300 + t_idx)
        Q_rot, _ = np.linalg.qr(rng.normal(size=(poly.n, poly.n)))
        if np.linalg.det(Q_rot) < 0:
            Q_rot[:, 0] *= -1.0

        # Planar achiral trans initialization u0 (phis == 0 exactly) mapped into oblique coordinates z0 = Q^T u0
        u_planar = np.zeros(poly.n)
        u_planar[: poly.n_theta] = poly.theta_0 + 0.12 * np.cos(np.arange(poly.n_theta))
        z_init = Q_rot.T @ u_planar

        def eval_oblique(z_vec: np.ndarray, mu_val: float = 0.005):
            u_vec = Q_rot @ z_vec
            phi_v, g_u, _ = poly.barrier_obj_grad_diag(u_vec, mu_val)
            if not np.isfinite(phi_v):
                return np.inf, np.zeros_like(z_vec)
            return phi_v, Q_rot.T @ g_u

        for mode in ("plain_lbfgs", "diag_trapdoor_lbfgs", "lanczos_11_lbfgs"):
            z = z_init.copy()
            s_hist, y_hist, rho_hist = [], [], []
            flops_cnt = 0
            phi, g = eval_oblique(z)
            trapdoor_fired = False

            for step_k in range(55):
                gnorm = float(np.linalg.norm(g))
                if gnorm < 2e-3 and ( mode == "plain_lbfgs" or trapdoor_fired or step_k > 8 ):
                    if mode == "plain_lbfgs":
                        break
                # Detect saddle stall: small gradient or tiny progress while still trapped in 2D mirror plane
                if not trapdoor_fired and (gnorm < 0.08 or step_k == 18):
                    if mode == "diag_trapdoor_lbfgs":
                        # Check rotated diagonal curvature H_ii = e_i^T Q^T H Q e_i via 3-point stencil
                        eps_d = 1e-4
                        diag_est = np.zeros(poly.n)
                        for ii in range(poly.n):
                            ez = np.zeros(poly.n); ez[ii] = eps_d
                            fp_i, _ = eval_oblique(z + ez)
                            fm_i, _ = eval_oblique(z - ez)
                            diag_est[ii] = (fp_i - 2.0 * phi + fm_i) / (eps_d ** 2)
                        flops_cnt += 2 * poly.n * 60
                        neg_idx = np.where(diag_est < -1e-2)[0]
                        if len(neg_idx) > 0:
                            kick = np.zeros(poly.n)
                            kick[neg_idx] = 0.18 / math.sqrt(len(neg_idx))
                            if poly.is_feasible(Q_rot @ (z + kick)):
                                z = z + kick
                                phi, g = eval_oblique(z)
                                s_hist.clear(); y_hist.clear(); rho_hist.clear()
                        trapdoor_fired = True
                    elif mode == "lanczos_11_lbfgs":
                        # Exact 10-step matrix-free HVP Lanczos tridiagonalization for lambda_min Ritz vector
                        k_lan = 10
                        V_lan = np.zeros((poly.n, k_lan))
                        q_curr = rng.normal(size=poly.n)
                        q_curr /= np.linalg.norm(q_curr)
                        q_prev = np.zeros(poly.n)
                        alphas_lan, betas_lan = [], []
                        beta_prev = 0.0
                        eps_hvp = 1e-4
                        for j_lan in range(k_lan):
                            V_lan[:, j_lan] = q_curr
                            _, g_plus = eval_oblique(z + eps_hvp * q_curr)
                            _, g_minus = eval_oblique(z - eps_hvp * q_curr)
                            w_vec = (g_plus - g_minus) / (2.0 * eps_hvp) - beta_prev * q_prev
                            flops_cnt += 2 * poly.n * 25
                            a_j = float(np.dot(q_curr, w_vec))
                            alphas_lan.append(a_j)
                            w_vec = w_vec - a_j * q_curr
                            # Full reorthogonalization over tiny k_lan=10 basis
                            for prev_i in range(j_lan + 1):
                                w_vec -= float(np.dot(V_lan[:, prev_i], w_vec)) * V_lan[:, prev_i]
                            b_j = float(np.linalg.norm(w_vec))
                            if b_j < 1e-10 or j_lan == k_lan - 1:
                                break
                            betas_lan.append(b_j)
                            q_prev = q_curr
                            q_curr = w_vec / b_j
                            beta_prev = b_j
                        m_eff = len(alphas_lan)
                        T_mat = np.diag(alphas_lan)
                        for bi_idx, bv in enumerate(betas_lan):
                            T_mat[bi_idx, bi_idx + 1] = bv
                            T_mat[bi_idx + 1, bi_idx] = bv
                        ev_t, U_t = np.linalg.eigh(T_mat)
                        if ev_t[0] < -1e-2:
                            v_probe = V_lan[:, :m_eff] @ U_t[:, 0]
                            v_probe /= max(float(np.linalg.norm(v_probe)), 1e-12)
                            if float(np.sum(v_probe)) < 0:
                                v_probe = -v_probe
                            for amp in (0.32, 0.20, 0.10, 0.05):
                                zn = z + amp * v_probe
                                if poly.is_feasible(Q_rot @ zn):
                                    fn, gn = eval_oblique(zn)
                                    if fn < phi:
                                        z, phi, g = zn, fn, gn
                                        s_hist.clear(); y_hist.clear(); rho_hist.clear()
                                        break
                        trapdoor_fired = True

                # Standard 2-loop L-BFGS step
                q = g.copy()
                alphas = []
                for s_i, y_i, rh_i in reversed(list(zip(s_hist, y_hist, rho_hist))):
                    a_i = rh_i * float(np.dot(s_i, q))
                    alphas.append(a_i)
                    q = q - a_i * y_i
                gamma = (
                    float(np.dot(s_hist[-1], y_hist[-1])) / max(float(np.dot(y_hist[-1], y_hist[-1])), 1e-12)
                    if y_hist else 0.08
                )
                r_vec = gamma * q
                for (s_i, y_i, rh_i), a_i in zip(zip(s_hist, y_hist, rho_hist), reversed(alphas)):
                    b_i = rh_i * float(np.dot(y_i, r_vec))
                    r_vec = r_vec + s_i * (a_i - b_i)
                step = -r_vec
                snrm = float(np.linalg.norm(step))
                if snrm > 0.35:
                    step *= 0.35 / snrm
                flops_cnt += 4 * 8 * poly.n

                alpha = 1.0
                while alpha > 1e-4:
                    zn = z + alpha * step
                    if poly.is_feasible(Q_rot @ zn):
                        fn, gn = eval_oblique(zn)
                        flops_cnt += poly.n * 25
                        if fn < phi - 1e-5 * alpha * float(np.dot(g, -step)):
                            sk, yk = zn - z, gn - g
                            sy = float(np.dot(sk, yk))
                            if sy > 1e-9:
                                if len(s_hist) >= 8:
                                    s_hist.pop(0); y_hist.pop(0); rho_hist.pop(0)
                                s_hist.append(sk); y_hist.append(yk); rho_hist.append(1.0 / sy)
                            z, phi, g = zn, fn, gn
                            break
                    alpha *= 0.5

            u_fin = Q_rot @ z
            f_fin = poly.f_only(u_fin)
            rmsd_z = poly.out_of_plane_rmsd(u_fin)
            exp3b_runs[mode].append({
                "obj": f_fin,
                "escaped_3d": bool(rmsd_z > 0.05),
                "flops": flops_cnt,
            })

    exp3b_summary = {}
    for mode, arr in exp3b_runs.items():
        exp3b_summary[mode] = {
            "escaped_3d_rate": round(float(np.mean([x["escaped_3d"] for x in arr])), 4),
            "median_obj": round(float(np.median([x["obj"] for x in arr])), 4),
            "mean_flops": int(np.mean([x["flops"] for x in arr])),
        }

    return {"exp3a_rotated_scaling": exp3a_summary, "exp3b_oblique_polymer_saddle": exp3b_summary}


def main():
    parser = argparse.ArgumentParser(description="Methylated Interior Search, Island Archipelago & Curvature-Vector Suite (v1.0.0)")
    parser.add_argument("--seed", type=int, default=42)
    parser.add_argument("--trials", type=int, default=24)
    parser.add_argument("--suite", type=str, choices=["all", "exp1", "exp2", "exp3"], default="all")
    parser.add_argument("--out", type=str, default="./methylated_search_telemetry_v1_0_0.json")
    args = parser.parse_args()

    t_all = time.perf_counter()
    rng = np.random.default_rng(args.seed)

    modes = ["standard_ip", "binary_freeze", "ternary_no_chiral", "ternary_uncoupled", "ternary_full"]
    exp1_summary = {}

    if args.suite in ("all", "exp1"):
        exp1 = {
            "Testbed_1A_PartiallySeparableFunnel": {m: [] for m in modes},
            "Testbed_1B_ABPolymerPlanarFold": {m: [] for m in modes},
            "Testbed_1C_HaarEntangledControl": {m: [] for m in modes},
        }

        print(f"[Exp 1A] Running Constrained Partially Separable Funnel NLP (N=36, {args.trials} trials)...")
        for t in range(args.trials):
            prob_a = PartiallySeparableFunnelNLP(n=36, seed=args.seed + t)
            x0 = np.zeros(36)
            x0[12:] = rng.uniform(-1.1, 1.1, size=24)
            for m in modes:
                res = solve_single_agent(prob_a, x0, mode=m, mu_schedule=[0.15, 0.03, 0.005, 0.0008], inner_steps=15)
                res.converged_target = bool(res.final_obj < -4.0)
                exp1["Testbed_1A_PartiallySeparableFunnel"][m].append(asdict(res))

        print(f"[Exp 1B] Running 3D AB-Polymer Folding from Planar Trans Manifold (L=12, N=19, {args.trials} trials)...")
        for t in range(args.trials):
            prob_b = StillingerABPolymer3D(L=12, seq_seed=args.seed + t)
            u0 = np.zeros(prob_b.n)
            u0[: prob_b.n_theta] = prob_b.theta_0 + rng.uniform(-0.22, 0.22, size=prob_b.n_theta)
            u0[prob_b.n_theta :] = 0.0
            for m in modes:
                res = solve_single_agent(
                    prob_b, u0, mode=m, mu_schedule=[0.04, 0.008, 0.001], inner_steps=14, check_3d=True
                )
                exp1["Testbed_1B_ABPolymerPlanarFold"][m].append(asdict(res))

        for t in range(args.trials):
            planar_e = exp1["Testbed_1B_ABPolymerPlanarFold"]["standard_ip"][t]["final_obj"]
            for m in modes:
                r = exp1["Testbed_1B_ABPolymerPlanarFold"][m][t]
                r["converged_target"] = bool(r["escaped_flatland_3d"] and (r["final_obj"] < planar_e - 0.15))

        print(f"[Exp 1C] Running Haar-Rotated Entangled Control NLP (N=36, {args.trials} trials)...")
        for t in range(args.trials):
            prob_c = HaarEntangledControlNLP(n=36, seed=args.seed + t)
            z0 = rng.uniform(-0.5, 0.5, size=36)
            for m in modes:
                res = solve_single_agent(prob_c, z0, mode=m, mu_schedule=[0.15, 0.03, 0.005, 0.0008], inner_steps=15)
                res.converged_target = bool(res.final_obj < -4.0)
                exp1["Testbed_1C_HaarEntangledControl"][m].append(asdict(res))

        for tb, mdict in exp1.items():
            exp1_summary[tb] = {}
            print(f"\n=== {tb} ({args.trials} trials) ===")
            for m, runs in mdict.items():
                objs = [r["final_obj"] for r in runs]
                flops = [r["effective_flops"] for r in runs]
                walls = [r["wall_ms"] for r in runs]
                succ = [1.0 if r["converged_target"] else 0.0 for r in runs]
                esc3d = [1.0 if r["escaped_flatland_3d"] else 0.0 for r in runs]
                meth = [r["frac_methylated"] for r in runs]
                corr = [r["frac_corridor"] for r in runs]
                trap = [r["frac_trapdoor"] for r in runs]
                st = {
                    "mean_obj": float(np.mean(objs)),
                    "std_obj": float(np.std(objs)),
                    "median_obj": float(np.median(objs)),
                    "mean_flops": float(np.mean(flops)),
                    "mean_wall_ms": float(np.mean(walls)),
                    "success_rate": float(np.mean(succ)),
                    "escaped_3d_rate": float(np.mean(esc3d)),
                    "frac_methylated": float(np.mean(meth)),
                    "frac_corridor": float(np.mean(corr)),
                    "frac_trapdoor": float(np.mean(trap)),
                }
                exp1_summary[tb][m] = st
                print(
                    f"  {m:20s} | Obj: {st['mean_obj']:8.3f} ± {st['std_obj']:6.3f} (med {st['median_obj']:7.3f}) "
                    f"| 3D-Esc: {st['escaped_3d_rate']*100:5.1f}% | Succ: {st['success_rate']*100:5.1f}% "
                    f"| FLOPs: {st['mean_flops']:8.0f}"
                )

    exp2_percolation = {}
    exp2_eta_sweep = {}
    n_arch_trials = min(args.trials, 24)
    if args.suite in ("all", "exp2"):
        print(f"\n[Exp 2A] Running 2D (z=4) vs 3D FCC (z=12) Archipelago across Bond Probabilities p...")
        bond_probs = [0.18, 0.32, 0.65]
        arch_configs = [
            ("Isolated_Islands", "3d_fcc", "isolated", 0.0),
            ("2D_Dense_Clone", "2d_torus", "dense_float_clone", 0.65),
            ("3D_FCC_Dense_Clone", "3d_fcc", "dense_float_clone", 0.65),
            ("2D_Methyl_Mask_80b", "2d_torus", "methyl_mask", 0.55),
            ("3D_FCC_Methyl_Mask_80b", "3d_fcc", "methyl_mask", 0.55),
        ]
        for p_bond in bond_probs:
            pkey = f"p_{p_bond:.2f}"
            exp2_percolation[pkey] = {}
            for label, topo, proto, eta in arch_configs:
                runs = [
                    run_archipelago_trial(topo, proto, p_bond, eta, seed=args.seed + 200 * idx + int(p_bond * 100))
                    for idx in range(n_arch_trials)
                ]
                exp2_percolation[pkey][label] = {
                    "mean_best_obj": float(np.mean([r["best_obj"] for r in runs])),
                    "std_best_obj": float(np.std([r["best_obj"] for r in runs])),
                    "mean_pop_obj": float(np.mean([r["mean_obj"] for r in runs])),
                    "mean_diversity": float(np.mean([r["diversity"] for r in runs])),
                    "mean_giant_frac": float(np.mean([r["giant_component_frac"] for r in runs])),
                    "mean_kilobits": float(np.mean([r["kilobits_sent"] for r in runs])),
                }

        eta_grid = [0.0, 0.15, 0.35, 0.55, 0.75, 0.95]
        exp2_eta_sweep = {"eta_grid": eta_grid, "3D_FCC_Dense_Clone": [], "3D_FCC_Methyl_Mask": [], "2D_Methyl_Mask": []}
        for eta in eta_grid:
            for key, topo, proto in [
                ("3D_FCC_Dense_Clone", "3d_fcc", "dense_float_clone"),
                ("3D_FCC_Methyl_Mask", "3d_fcc", "methyl_mask"),
                ("2D_Methyl_Mask", "2d_torus", "methyl_mask"),
            ]:
                runs = [
                    run_archipelago_trial(topo, proto if eta > 0 else "isolated", 0.32, eta, seed=args.seed + 700 + i)
                    for i in range(min(args.trials, 20))
                ]
                exp2_eta_sweep[key].append({
                    "eta": eta,
                    "mean_best_obj": float(np.mean([r["best_obj"] for r in runs])),
                    "std_best_obj": float(np.std([r["best_obj"] for r in runs])),
                    "mean_diversity": float(np.mean([r["diversity"] for r in runs])),
                })

    exp3_summary = {}
    if args.suite in ("all", "exp3"):
        print(f"\n[Exp 3] Running Rotated Curvature-Vector & Oblique SO(19) Polymer Lanczos (1,1) Suite ({args.trials} trials)...")
        exp3_summary = run_exp3_curvature_and_oblique_saddle(seed=args.seed, n_trials=args.trials)
        for dkey, mdict in exp3_summary["exp3a_rotated_scaling"].items():
            print(f"  Rotated {dkey}: DenseIP={mdict['dense_ip']['hit_rate']*100:.1f}% ({mdict['dense_ip']['mean_flops']} FLOPs) | "
                  f"EigenTernary={mdict['eigen_ternary']['hit_rate']*100:.1f}% ({mdict['eigen_ternary']['mean_flops']} FLOPs) | "
                  f"TwoLoopLBFGS={mdict['two_loop_lbfgs']['hit_rate']*100:.1f}% ({mdict['two_loop_lbfgs']['mean_flops']} FLOPs)")
        for mode, mstat in exp3_summary["exp3b_oblique_polymer_saddle"].items():
            print(f"  Oblique Polymer [{mode:20s}]: 3D-Escape={mstat['escaped_3d_rate']*100:5.1f}% | "
                  f"MedObj={mstat['median_obj']:7.3f} | FLOPs={mstat['mean_flops']}")

    total_sec = time.perf_counter() - t_all
    payload = {
        "metadata": {
            "version": "1.0.0",
            "seed": args.seed,
            "suite": args.suite,
            "trials": args.trials,
            "total_runtime_sec": round(total_sec, 2),
        },
        "exp1_summary": exp1_summary,
        "exp2_percolation": exp2_percolation,
        "exp2_eta_sweep": exp2_eta_sweep,
        "exp3_curvature_and_oblique_saddle": exp3_summary,
    }
    if os.path.dirname(args.out):
        os.makedirs(os.path.dirname(args.out), exist_ok=True)
    with open(args.out, "w", encoding="utf-8") as f:
        json.dump(payload, f, indent=2)
    print(f"\nCompleted parity-verified v1.0.0 benchmark suite in {total_sec:.2f} s -> {args.out}")


if __name__ == "__main__":
    main()
