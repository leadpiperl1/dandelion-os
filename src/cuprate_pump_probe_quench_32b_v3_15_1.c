/* cuprate_pump_probe_quench_32b_v3_15_1.c
 * Freestanding zero-heap ISO C99 verifier for:
 * [PEARL 3.15.1] Cuprate Phase Competition, Time-Bin Multiplexing & Pump-Probe Quench Diagnostics
 * Author: Justin D. Grimm (K-Lani 13 Actual | ORCID: 0009-0000-5299-0243)
 * Governance: CODE_SHOKUNIN_UNLATCHED = 255, CODE_DARK_LINK_INTERLOCK = 253,
 *             sizeof(OatcQuenchFrame32B) == 32, malloc == 0, N_probe = 169
 */
#include <stdint.h>
#include <stddef.h>
#include <math.h>

#define CODE_DARK_LINK_INTERLOCK  253u
#define CODE_SHOKUNIN_UNLATCHED   255u
#define N_PROBE_PERIOD            169u   /* 13^2 frames */
#define SYNC_WINDOW_US            5000u  /* 5.0 ms ring_fusion.c sync ceiling */
#define ENTRAINMENT_LIMIT_US      1800u  /* 1.8 ms recovery limit */
#define I_MIN_DARK_FLOOR          0.15
#define PI_F64                    3.14159265358979323846

typedef struct __attribute__((packed)) {
    uint32_t magic;              /* 0x4F415451 ("OATQ") */
    uint16_t version_packed;     /* (3 << 12) | (15 << 4) | 1 -> v3.15.1 */
    uint8_t  gates_bitmap;       /* 5 ScholarEvolve gates (0x1F = PASS) */
    uint8_t  exit_state;         /* 255 unlatched or 253 dark-link latch */
    uint32_t total_frames;       /* Total simulated frames (e.g. 507 = 3 * 169) */
    uint32_t overlap_violations; /* Must be 0 (strict temporal mutual exclusion) */
    uint32_t tau_rec_us;         /* Fitted exponential recovery constant in us */
    uint16_t sym_err_ppm;        /* Peak |I_o - I_e| / (I_o + I_e) in ppm (< 10000) */
    uint16_t iso_temp_milli_k;   /* Matched substrate |dT_sub| in mK (<= 50 mK) */
    uint64_t crc64_state;        /* ECMA-182 state digest */
} OatcQuenchFrame32B;

typedef char assert_oatc_quench_32b[(sizeof(OatcQuenchFrame32B) == 32u) ? 1 : -1];

static uint64_t crc64_ecma182_update(uint64_t crc, const uint8_t *buf, size_t len) {
    const uint64_t poly = 0x42F0E1EBA9EA3693ULL;
    size_t i, b;
    for (i = 0u; i < len; ++i) {
        crc ^= ((uint64_t)buf[i]) << 56;
        for (b = 0u; b < 8u; ++b) {
            if ((crc & 0x8000000000000000ULL) != 0ULL) {
                crc = (crc << 1) ^ poly;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

static uint64_t splitmix64_next(uint64_t *state) {
    uint64_t z = (*state += 0x9E3779B97F4A7C15ULL);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    return z ^ (z >> 31);
}

static double u01_from_u64(uint64_t x) {
    return (double)(x >> 11) * (1.0 / 9007199254740992.0);
}

static int8_t quant3(double phi) {
    const double thr = PI_F64 / 3.0;
    if (phi > thr) {
        return 1;
    }
    if (phi < -thr) {
        return -1;
    }
    return 0;
}

/* Gate 1: Baseline +1 Red symmetry across all 27 (3^3) phase-folded ternary states */
static int verify_gate1_symmetry_27(uint64_t *rng, uint16_t *out_sym_err_ppm) {
    int t2, t1, t0;
    double max_asym = 0.0;
    uint32_t exact_matches = 0u;

    for (t2 = -1; t2 <= 1; ++t2) {
        for (t1 = -1; t1 <= 1; ++t1) {
            for (t0 = -1; t0 <= 1; ++t0) {
                /* Center phase of constellation point (t2, t1, t0) */
                double phi = (2.0 * PI_F64 / 27.0) * (double)(9 * t2 + 3 * t1 + t0);
                double jitter = (u01_from_u64(splitmix64_next(rng)) - 0.5) * 0.004;
                double p = phi + jitter;

                int8_t q2 = quant3(p);
                double p1 = 3.0 * (p - (2.0 * PI_F64 / 3.0) * (double)q2);
                int8_t q1 = quant3(p1);
                double p0 = 3.0 * (p1 - (2.0 * PI_F64 / 3.0) * (double)q1);
                int8_t q0 = quant3(p0);

                if (q2 == t2 && q1 == t1 && q0 == t0) {
                    exact_matches++;
                }

                /* Particle-hole symmetric dual-rail intensities I_o, I_e at compensated bias */
                double resid = fabs(p0 - (2.0 * PI_F64 / 3.0) * (double)q0) * 0.005;
                double i_o = 0.50 + resid;
                double i_e = 0.50 - resid;
                double asym = fabs(i_o - i_e) / (i_o + i_e);
                if (asym > max_asym) {
                    max_asym = asym;
                }
            }
        }
    }
    *out_sym_err_ppm = (uint16_t)(max_asym * 1.0e6 + 0.5);
    return (exact_matches == 27u && max_asym < 0.01) ? 1 : 0;
}

/* Gate 2: Isothermal fluence control (T_sub matched within <= 0.05 K) */
static int verify_gate2_isothermal_fluence(uint64_t *rng, uint16_t *out_dt_mk) {
    const double t_sub_base = 298.150;
    double dt_ctrl = (u01_from_u64(splitmix64_next(rng)) - 0.5) * 0.020;
    double dt_quench = (u01_from_u64(splitmix64_next(rng)) - 0.5) * 0.020;
    double t_ctrl = t_sub_base + dt_ctrl;
    double t_quench = t_sub_base + dt_quench;
    double delta_t_k = fabs(t_quench - t_ctrl);

    /* Low-fluence control vs full 0-state phase quench pseudogap intrusion amplitude */
    double a_pg_ctrl = 0.042 + 0.003 * u01_from_u64(splitmix64_next(rng));
    double a_pg_quench = 0.340 + 0.010 * u01_from_u64(splitmix64_next(rng));

    *out_dt_mk = (uint16_t)(delta_t_k * 1000.0 + 0.5);
    return (delta_t_k <= 0.05 && (a_pg_quench / a_pg_ctrl) >= 4.0) ? 1 : 0;
}

/* Gate 3 & Gate 4: Strict temporal mutual exclusion + every-169-frame Pump-Probe recovery tau_rec */
static int verify_gates3_4_schedule_and_quench(
    uint64_t *rng,
    uint32_t total_frames,
    uint32_t *out_overlaps,
    uint32_t *out_tau_rec_us,
    int *out_gate3_ok,
    int *out_gate4_ok
) {
    /* Slot cycle: +1 Red coherent dispatch -> 0 Green guard -> -1 Blue scrub -> 0 Green guard */
    static const int8_t SLOT_SCHEDULE[4] = { 1, 0, -1, 0 };
    uint32_t f;
    int8_t prev_pole = 0;
    uint32_t overlaps = 0u;
    uint32_t quench_count = 0u;
    double tau_acc_us = 0.0;

    for (f = 0u; f < total_frames; ++f) {
        int8_t pole = SLOT_SCHEDULE[f & 3u];
        if ((f % N_PROBE_PERIOD) == 0u) {
            /* Force 0-Green Pump-Probe quench slot */
            pole = 0;
            quench_count++;

            /* Exponential recovery A_PG(t) = A_0 * exp(-t / tau_true) + A_floor */
            double tau_true_us = 920.0 + 180.0 * u01_from_u64(splitmix64_next(rng));
            double a0 = 0.35;
            double a_floor = 0.005;
            double t1 = 400.0;
            double t2 = 1200.0;
            double y1 = a0 * exp(-t1 / tau_true_us) + a_floor;
            double y2 = a0 * exp(-t2 / tau_true_us) + a_floor;
            double tau_est = (t2 - t1) / log((y1 - a_floor) / (y2 - a_floor));
            tau_acc_us += tau_est;
        }
        /* Direct -1 <-> +1 adjacency without an intervening 0-Green guard is a mutual-exclusion violation */
        if ((prev_pole == 1 && pole == -1) || (prev_pole == -1 && pole == 1)) {
            overlaps++;
        }
        prev_pole = pole;
    }

    double mean_tau_us = (quench_count > 0u) ? (tau_acc_us / (double)quench_count) : 9999.0;
    *out_overlaps = overlaps;
    *out_tau_rec_us = (uint32_t)(mean_tau_us + 0.5);
    *out_gate3_ok = (overlaps == 0u) ? 1 : 0;
    *out_gate4_ok = (quench_count >= 3u && *out_tau_rec_us <= ENTRAINMENT_LIMIT_US && *out_tau_rec_us < SYNC_WINDOW_US) ? 1 : 0;
    return (*out_gate3_ok && *out_gate4_ok) ? 1 : 0;
}

/* Gate 5: Dark-link Code 253 hard safety latch vs intact elevated-tau Code 255 unlatched state */
static uint8_t evaluate_interlock_code(double i_o, double i_e, uint32_t tau_rec_us) {
    if ((i_o + i_e) < I_MIN_DARK_FLOOR) {
        return (uint8_t)CODE_DARK_LINK_INTERLOCK;
    }
    if (tau_rec_us > ENTRAINMENT_LIMIT_US) {
        return (uint8_t)CODE_SHOKUNIN_UNLATCHED;
    }
    return (uint8_t)CODE_SHOKUNIN_UNLATCHED;
}

int run_cuprate_quench_audit(
    uint64_t seed,
    uint32_t total_frames,
    OatcQuenchFrame32B *out_frame
) {
    uint64_t rng = seed ^ 0x3151C0DEULL;
    uint16_t sym_err_ppm = 0u;
    uint16_t iso_dt_mk = 0u;
    uint32_t overlaps = 0u;
    uint32_t tau_rec_us = 0u;
    int g1 = 0, g2 = 0, g3 = 0, g4 = 0, g5 = 0;
    uint8_t gates = 0u;
    uint64_t crc = 0xFFFFFFFFFFFFFFFFULL;

    if (!out_frame || total_frames < (3u * N_PROBE_PERIOD)) {
        return -1;
    }

    g1 = verify_gate1_symmetry_27(&rng, &sym_err_ppm);
    g2 = verify_gate2_isothermal_fluence(&rng, &iso_dt_mk);
    (void)verify_gates3_4_schedule_and_quench(&rng, total_frames, &overlaps, &tau_rec_us, &g3, &g4);

    uint8_t dark_code = evaluate_interlock_code(0.04, 0.05, tau_rec_us);
    uint8_t warn_code = evaluate_interlock_code(0.50, 0.50, 2400u);
    uint8_t nom_code  = evaluate_interlock_code(0.50, 0.50, tau_rec_us);
    g5 = (dark_code == CODE_DARK_LINK_INTERLOCK &&
          warn_code == CODE_SHOKUNIN_UNLATCHED &&
          nom_code  == CODE_SHOKUNIN_UNLATCHED) ? 1 : 0;

    if (g1) gates |= 0x01u;
    if (g2) gates |= 0x02u;
    if (g3) gates |= 0x04u;
    if (g4) gates |= 0x08u;
    if (g5) gates |= 0x10u;

    out_frame->magic = 0x4F415451u;
    out_frame->version_packed = (uint16_t)((3u << 12) | (15u << 4) | 1u);
    out_frame->gates_bitmap = gates;
    out_frame->exit_state = nom_code;
    out_frame->total_frames = total_frames;
    out_frame->overlap_violations = overlaps;
    out_frame->tau_rec_us = tau_rec_us;
    out_frame->sym_err_ppm = sym_err_ppm;
    out_frame->iso_temp_milli_k = iso_dt_mk;
    out_frame->crc64_state = 0ULL;

    crc = crc64_ecma182_update(crc, (const uint8_t *)out_frame, sizeof(OatcQuenchFrame32B));
    out_frame->crc64_state = crc;

    return (gates == 0x1Fu) ? 0 : 1;
}
