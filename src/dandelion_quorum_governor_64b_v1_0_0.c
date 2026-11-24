/*
 * dandelion_quorum_governor_64b_v1_0_0.c
 * Freestanding zero-heap 64-byte L1 cache-line ISO C99 governor for
 * The Dandelion Invariant: N=8 vs N=13 (12+1) Byzantine/crash quorum parity,
 * perinatal 13-lunation cohort balance, and Earth STP CO2(g) vs alpha-SiO2(s)
 * root-nodule symbiosis.
 *
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Compile: gcc -std=c99 -O3 -Wall -Wextra -Wconversion -Wunused-but-set-variable -pedantic -Werror dandelion_quorum_governor_64b_v1_0_0.c -lm -o dandelion_gov
 */

#include <math.h>
#include <stdint.h>
#include <stdio.h>

#define FOWLER_DEADLOCK_FAIL_BYTE 0U
#define FOWLER_SYMBIOSIS_LOCK_BYTE 121U
#define REDOX_SOLID_TRAP_BYTE 237U
#define KEYHOLE_COHORT_BYTE 243U
#define KINTSUGI_QUORUM_WELD_BYTE 251U
#define CODE_SHOKUNIN_UNLATCHED 255U

typedef struct {
    uint32_t magic_tag;                 /* 4B: 'DND1' = 0x444E4431U */
    uint16_t node_mask_13b;             /* 2B: Active consensus bitmask (8 or 13 nodes) */
    uint16_t fault_mask_13b;            /* 2B: Injected crash/Byzantine error mask */
    int32_t temp_kelvin_q16;            /* 4B: Ambient temperature K (298.15 in Q16.16) */
    int32_t df_h_co2_kj_q16;            /* 4B: NIST CO2(g) dfH0 (-393.51 kJ/mol Q16.16) */
    int32_t df_h_sio2_kj_q16;           /* 4B: NIST alpha-SiO2(s) dfH0 (-910.86 kJ/mol Q16.16) */
    int32_t dr_s_co2_j_q16;             /* 4B: Carbon oxidation dS0 (+2.893 J/(mol*K) Q16.16) */
    int32_t dr_s_sio2_j_q16;            /* 4B: Silicon oxidation dS0 (-182.502 J/(mol*K) Q16.16) */
    int32_t enthalpic_trap_ratio_q16;   /* 4B: Derived |dfH(SiO2)/dfH(CO2)| = 2.3147 Q16.16 */
    int32_t horizon_gain_x10_q16;       /* 4B: Derived (E[T_13]/E[T_8])/10 at p=0.035 Q16.16 */
    int32_t lunar_phase_cycle_q16;      /* 4B: Lunar cycle index in [1.0, 13.0444] Q16.16 */
    int32_t landauer_zj_q16;            /* 4B: E_min = k_B*T*ln(2) in zeptojoules (2.8533 Q16.16) */
    uint32_t epoch_ticks;               /* 4B: Deterministic evaluation tick counter */
    uint8_t quorum_n;                   /* 1B: Quorum size N (8 byte vs 13 Dandelion) */
    uint8_t crash_radius_t;             /* 1B: Crash fault radius floor((N-1)/2) -> 3 or 6 */
    uint8_t byz_radius_f;               /* 1B: Byzantine radius floor((N-1)/3) -> 2 or 4 */
    uint8_t dunbar_cluster;             /* 1B: Hierarchical 12*12 + 1 = 145 Dunbar tier */
    int8_t trit_quorum_parity;          /* 1B: -1 tie/breach, 0 Kintsugi healed, +1 clean */
    int8_t trit_redox_substrate;        /* 1B: -1 mobile SiO2 trap, 0 root-nodule, +1 rack */
    int8_t trit_phenology_cohort;       /* 1B: -1 spring anchor, 0 balanced, +1 autumn scout */
    uint8_t fowler_state_byte;          /* 1B: Fowler/Trapdoor byte (0, 121, 237, 243, 251) */
    uint16_t flags_status;              /* 2B: Diagnostic status bitmask */
    uint16_t exit_latch_code;           /* 2B: 255 = CODE_SHOKUNIN_UNLATCHED */
    uint8_t reserved_pad[4];            /* 4B: Explicit pad to exact 64-byte L1 cache line */
} DandelionQuorumGovernor64B;

typedef int static_assert_dandelion_64b[(sizeof(DandelionQuorumGovernor64B) == 64U) ? 1 : -1];

static int32_t to_q16(double x) {
    double scaled = x * 65536.0;
    if (scaled > 2147483647.0) {
        return 2147483647;
    }
    if (scaled < -2147483648.0) {
        return -2147483647 - 1;
    }
    return (int32_t)llround(scaled);
}

static double from_q16(int32_t q) {
    return ((double)q) / 65536.0;
}

static uint8_t popcnt16(uint16_t v) {
    uint8_t c = 0U;
    while (v != 0U) {
        v = (uint16_t)(v & (uint16_t)(v - 1U));
        c = (uint8_t)(c + 1U);
    }
    return c;
}

static uint64_t binom_u64(uint32_t n, uint32_t k) {
    if (k > n) {
        return 0ULL;
    }
    if (k == 0U || k == n) {
        return 1ULL;
    }
    if (k > n - k) {
        k = n - k;
    }
    uint64_t res = 1ULL;
    for (uint32_t i = 1U; i <= k; ++i) {
        res = (res * (uint64_t)(n - k + i)) / (uint64_t)i;
    }
    return res;
}

static double block_failure_prob(uint32_t n, double p) {
    uint32_t thresh = (n / 2U) + 1U;
    if ((n % 2U) == 0U) {
        thresh = n / 2U; /* Even N=8 fails or ties at >= 4 errors */
    }
    double p_err = 0.0;
    for (uint32_t k = thresh; k <= n; ++k) {
        p_err += (double)binom_u64(n, k) * pow(p, (double)k) * pow(1.0 - p, (double)(n - k));
    }
    return p_err;
}

static void evaluate_dandelion_governor(DandelionQuorumGovernor64B *gov, int mobile_silicon_attempt) {
    uint8_t n = (gov->quorum_n >= 13U) ? 13U : 8U;
    uint16_t valid_mask = (uint16_t)((1U << n) - 1U);
    uint16_t faults = (uint16_t)(gov->fault_mask_13b & valid_mask);
    uint8_t err_count = popcnt16(faults);

    gov->quorum_n = n;
    gov->crash_radius_t = (uint8_t)((n - 1U) / 2U);
    gov->byz_radius_f = (uint8_t)((n - 1U) / 3U);
    gov->dunbar_cluster = 145U; /* 12 * 12 + 1 */

    double t_k = from_q16(gov->temp_kelvin_q16);
    double h_co2 = fabs(from_q16(gov->df_h_co2_kj_q16));
    double h_sio2 = fabs(from_q16(gov->df_h_sio2_kj_q16));
    double trap_ratio = (h_co2 > 1.0) ? (h_sio2 / h_co2) : 0.0;
    gov->enthalpic_trap_ratio_q16 = to_q16(trap_ratio);

    /* Exact CODATA 2018/2022 k_B = 1.380649e-23 J/K -> E_min in zeptojoules (1e-21 J) */
    double e_min_zj = 0.01380649 * t_k * log(2.0);
    gov->landauer_zj_q16 = to_q16(e_min_zj);

    double pe8 = block_failure_prob(8U, 0.035);
    double pe13 = block_failure_prob(13U, 0.035);
    double gain = (pe13 > 0.0) ? (pe8 / pe13) : 0.0;
    gov->horizon_gain_x10_q16 = to_q16(gain / 10.0);

    double cyc = from_q16(gov->lunar_phase_cycle_q16);
    if (cyc <= 4.35) {
        gov->trit_phenology_cohort = -1;
    } else if (cyc >= 8.70) {
        gov->trit_phenology_cohort = 1;
    } else {
        gov->trit_phenology_cohort = 0;
    }

    gov->epoch_ticks += 1U;
    gov->exit_latch_code = (uint16_t)CODE_SHOKUNIN_UNLATCHED;

    if (mobile_silicon_attempt != 0 && t_k < 1996.0) {
        gov->trit_redox_substrate = -1;
        gov->trit_quorum_parity = 0;
        gov->fowler_state_byte = (uint8_t)REDOX_SOLID_TRAP_BYTE;
        gov->flags_status = 0x0008U;
        return;
    }

    gov->trit_redox_substrate = 0; /* Root-nodule carbon-silicon symbiosis */

    if ((n % 2U) == 0U && err_count == (n / 2U)) {
        /* Exact 4:4 split-vote tie deadlock on even N=8 */
        gov->trit_quorum_parity = -1;
        gov->fowler_state_byte = (uint8_t)FOWLER_DEADLOCK_FAIL_BYTE;
        gov->flags_status = 0x0004U;
    } else if (err_count > gov->crash_radius_t) {
        gov->trit_quorum_parity = -1;
        gov->fowler_state_byte = (uint8_t)FOWLER_DEADLOCK_FAIL_BYTE;
        gov->flags_status = 0x0002U;
    } else if (err_count > 0U) {
        /* Self-healing Kintsugi parity weld within radius t (up to 6 on N=13) */
        gov->trit_quorum_parity = 0;
        gov->fowler_state_byte = (uint8_t)KINTSUGI_QUORUM_WELD_BYTE;
        gov->flags_status = 0x0001U;
    } else {
        gov->trit_quorum_parity = 1;
        gov->fowler_state_byte = (gov->trit_phenology_cohort == 0)
            ? (uint8_t)KEYHOLE_COHORT_BYTE
            : (uint8_t)FOWLER_SYMBIOSIS_LOCK_BYTE;
        gov->flags_status = 0x0000U;
    }
}

static uint64_t xorshift64(uint64_t *s) {
    uint64_t x = *s;
    x ^= x << 13U;
    x ^= x >> 7U;
    x ^= x << 17U;
    *s = x;
    return x;
}

int main(void) {
    DandelionQuorumGovernor64B ref = {
        0x444E4431U,
        0x1FFFU,
        0x000FU, /* 4 Byzantine/crash faults injected: lethal to N=8, healed by N=13 */
        to_q16(298.15),
        to_q16(-393.51),
        to_q16(-910.86),
        to_q16(2.893),
        to_q16(-182.502),
        0, 0,
        to_q16(6.50),
        0, 0U,
        13U, 0U, 0U, 0U,
        0, 0, 0,
        (uint8_t)FOWLER_SYMBIOSIS_LOCK_BYTE,
        0U,
        (uint16_t)CODE_SHOKUNIN_UNLATCHED,
        {0U, 0U, 0U, 0U}
    };

    evaluate_dandelion_governor(&ref, 0);

    DandelionQuorumGovernor64B octet_tie = ref;
    octet_tie.quorum_n = 8U;
    octet_tie.fault_mask_13b = 0x000FU; /* 4 faults on N=8 -> exact 4:4 deadlock */
    evaluate_dandelion_governor(&octet_tie, 0);

    DandelionQuorumGovernor64B robota_trap = ref;
    evaluate_dandelion_governor(&robota_trap, 1);

    if (sizeof(DandelionQuorumGovernor64B) != 64U ||
        ref.crash_radius_t != 6U ||
        ref.byz_radius_f != 4U ||
        ref.fowler_state_byte != KINTSUGI_QUORUM_WELD_BYTE ||
        octet_tie.fowler_state_byte != FOWLER_DEADLOCK_FAIL_BYTE ||
        robota_trap.fowler_state_byte != REDOX_SOLID_TRAP_BYTE) {
        return 1;
    }

    const uint32_t sizes[4] = {256U, 1024U, 4096U, 16384U};
    uint64_t rng = 42ULL;
    uint32_t n13_survived_last = 0U;
    uint32_t n8_survived_last = 0U;
    uint32_t n13_kintsugi_last = 0U;

    /* Simulate 500-generation (12.5 ka) bottlenecks at p=0.10 across N={256..16384} */
    double p_step_8 = 1.0 - block_failure_prob(8U, 0.10);
    double p_surv_500_8 = pow(p_step_8, 500.0);
    double p_step_13 = 1.0 - block_failure_prob(13U, 0.10);
    double p_surv_500_13 = pow(p_step_13, 500.0);

    for (size_t si = 0U; si < 4U; ++si) {
        uint32_t trials = sizes[si];
        uint32_t s13 = 0U, s8 = 0U, w13 = 0U;
        for (uint32_t k = 0U; k < trials; ++k) {
            uint16_t mask13 = 0U;
            for (uint8_t bit = 0U; bit < 13U; ++bit) {
                double u = ((double)(xorshift64(&rng) & 0xFFFFFFULL)) / 16777216.0;
                if (u < 0.10) {
                    mask13 = (uint16_t)(mask13 | (uint16_t)(1U << bit));
                }
            }
            DandelionQuorumGovernor64B g13 = ref;
            g13.fault_mask_13b = mask13;
            g13.lunar_phase_cycle_q16 = to_q16(1.0 + 12.0 * (((double)(xorshift64(&rng) & 0xFFFFULL)) / 65536.0));
            evaluate_dandelion_governor(&g13, 0);
            if (g13.fowler_state_byte != FOWLER_DEADLOCK_FAIL_BYTE) {
                s13++;
                if (g13.fowler_state_byte == KINTSUGI_QUORUM_WELD_BYTE) {
                    w13++;
                }
            }

            DandelionQuorumGovernor64B g8 = g13;
            g8.quorum_n = 8U;
            g8.fault_mask_13b = (uint16_t)(mask13 & 0x00FFU);
            evaluate_dandelion_governor(&g8, 0);
            if (g8.fowler_state_byte != FOWLER_DEADLOCK_FAIL_BYTE) {
                s8++;
            }
        }
        n13_survived_last = s13;
        n8_survived_last = s8;
        n13_kintsugi_last = w13;
    }

    printf("{\"sizeof_governor_bytes\": %zu, \"n13_t_f\": [%u, %u], \"n8_t_f\": [%u, %u], "
           "\"horizon_gain_p035\": %.2f, \"surv_500gen_p10\": {\"n8\": %.4f, \"n13\": %.4f}, "
           "\"enthalpic_trap_ratio\": %.4f, \"landauer_298k_zj\": %.4f, "
           "\"n16384_single_gen_p10\": {\"n13_pass\": %u, \"n13_kintsugi\": %u, \"n8_pass\": %u}, "
           "\"exit_latch\": %u}\n",
           sizeof(DandelionQuorumGovernor64B),
           (unsigned)ref.crash_radius_t, (unsigned)ref.byz_radius_f,
           (unsigned)octet_tie.crash_radius_t, (unsigned)octet_tie.byz_radius_f,
           from_q16(ref.horizon_gain_x10_q16) * 10.0,
           p_surv_500_8, p_surv_500_13,
           from_q16(ref.enthalpic_trap_ratio_q16),
           from_q16(ref.landauer_zj_q16),
           n13_survived_last, n13_kintsugi_last, n8_survived_last,
           (unsigned)ref.exit_latch_code);

    return 0;
}
