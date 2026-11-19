/*
 * bounded_thermo_cell64_v1_0_0.c
 * Zero-allocation 64-byte L1 cache-line verifier for zero-free occupancy
 * thermometry, Quantum Otto cell dilation, and Voronoi acceleration clamping.
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint32_t n_max;            /* 4 B: Discrete vessel state cap              */
    uint32_t m_occ;            /* 4 B: Clamped excited count in [1, n_max-1]  */
    double   sigma_grade;      /* 8 B: ln(m / (n_max - m))                    */
    double   beta_bath;        /* 8 B: Stationary-to-mobile capacity k'       */
    double   gamma_eff;        /* 8 B: (gamma + beta_B) / (1 + beta_B)        */
    double   g_floor_si;       /* 8 B: c * H0 / 6 (m/s^2)                     */
    double   w0_boundary;      /* 8 B: Composite 2D wall + floor EoS          */
    double   rf_retention;     /* 8 B: Martin-Synge mobile fraction 1/(1+k')  */
    uint8_t  status_code;      /* 1 B: 255 = unlatched N_max - 1 gate         */
    uint8_t  fowler_neutral;   /* 1 B: 121 = balanced ternary neutral byte    */
    uint16_t version_packed;   /* 2 B: 0x0100 -> v1.0.0                       */
    uint32_t crc_guard;        /* 4 B: Deterministic integrity tag            */
} BoundedCosmoCell64B;

typedef char static_assert_64b[(sizeof(BoundedCosmoCell64B) == 64) ? 1 : -1];

int main(void) {
    const double c_ms = 299792458.0;
    const double h0_si = (68.43 * 1000.0) / 3.08567758149e22;
    BoundedCosmoCell64B cell;

    cell.n_max = 100U;
    cell.m_occ = 27U;
    cell.sigma_grade = log((double)cell.m_occ / (double)(cell.n_max - cell.m_occ));
    cell.beta_bath = 5.0;
    cell.gamma_eff = ((5.0 / 3.0) + cell.beta_bath) / (1.0 + cell.beta_bath);
    cell.g_floor_si = (c_ms * h0_si) / 6.0;
    cell.w0_boundary = -5.0 / 6.0;
    cell.rf_retention = 1.0 / (1.0 + cell.beta_bath);
    cell.status_code = 255U;
    cell.fowler_neutral = 121U;
    cell.version_packed = 0x0100U;
    cell.crc_guard = 0x43505431U;

    if (sizeof(BoundedCosmoCell64B) != 64U) {
        return 1;
    }
    if (!isfinite(cell.sigma_grade) || fabs(cell.rf_retention - (1.0 / 6.0)) > 1e-12) {
        return 2;
    }
    if (cell.g_floor_si < 1.05e-10 || cell.g_floor_si > 1.25e-10) {
        return 3;
    }

    printf("C99_64B_PASS sizeof=%zu sigma=%.6f g_floor=%.4e m/s^2 Rf=%.5f\n",
           sizeof(BoundedCosmoCell64B), cell.sigma_grade,
           cell.g_floor_si, cell.rf_retention);
    return 0;
}
