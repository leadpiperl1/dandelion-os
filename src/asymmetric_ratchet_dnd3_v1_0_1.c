/*
 * asymmetric_ratchet_dnd3_v1_0_0.c
 * Strict ISO C99 (-std=c99 -O3 -Wall -Wextra -Wpedantic -Wconversion -Werror)
 * Zero-Heap (malloc = 0) 64-Byte Asymmetric Counting Ratchet Governor &
 * 32-Byte DND3 Low-Power Stigmergic Transition Kernel (v1.0.0)
 *
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Affiliation: Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
 */

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define FOWLER_NEUTRAL_BYTE     121U
#define FOWLER_ACTIVE_CEIL      242U
#define CODE_CLEAN              0U
#define CODE_KEYHOLE_CLAMP      243U
#define CODE_DNHM_BREAKER       250U
#define CODE_KINTSUGI_GOLD      251U
#define CODE_FAIL_CLOSED        253U
#define CODE_SHOKUNIN_UNLATCHED 255U

#define M_BOUNDED_TRITS         42U
#define FENCE_LOWER_MASK        0x00000000007ULL  /* Trits [0..2] */
#define FENCE_UPPER_MASK        0x38000000000ULL  /* Trits [39..41] */
#define INTERIOR_36_MASK        0x07FFFFFFFF8ULL  /* Protected Interior [3..38] (36 = 12 x 3) */

#define COUNT_M_MIN             1U
#define COUNT_M_KEYHOLE         170U              /* Exact 2/3 of 255 */
#define COUNT_M_MAX             255U              /* N_max - 1 */

#define P_ACTIVE_MAX_MW         12333U            /* 2/3 of 18,500 mW */
#define P_CAMPFIRE_MAX_MW       6167U             /* 1/3 Open-Circle Reserve */
#define P_WINTER_MAX_MW         1850U             /* <= 1.85 W Winter / Dream-Time */
#define T_CEIL_CENTI_C          4100U             /* <= 41.00 C */
#define MAX_HOP_TTL             258U              /* Finite Rubik orbital horizon */

/* Exact 64-Byte L1D Cache-Line Asymmetric Counting Ratchet & Fence Governor */
typedef struct {
    uint64_t noo_tick_ns;       /* [0..7]   Monotonic 3D operational round-trip clock */
    uint64_t trits_pos;         /* [8..15]  +1 Red dual-rail bit-plane (42 trits active) */
    uint64_t trits_neg;         /* [16..23] -1 Blue dual-rail bit-plane (42 trits active) */
    uint64_t weld_epoch;        /* [24..31] Monotonic Kintsugi absorb-and-weld counter */
    uint32_t kintsugi_welds;    /* [32..35] Cumulative 3-to-1 boundary seam welds */
    uint32_t rejected_frames;   /* [36..39] Cumulative 0-tick Byte 253 drops */
    uint16_t power_mw;          /* [40..41] Clamped package power (<= 12,333 mW active) */
    uint16_t temp_centi_c;      /* [42..43] Die temperature in 0.01 C (<= 4100) */
    uint8_t  fowler_seed[8];    /* [44..51] 40-trit Pocket Rosetta seed (neutral = 121) */
    uint8_t  count_m;           /* [52]     Clamped interior occupancy m in [1..255] */
    int8_t   season_mode;       /* [53]     +1 Forage, 0 Campfire, -1 Winter */
    uint8_t  last_trapdoor;     /* [54]     0 or trapdoor byte 243..255 */
    uint8_t  exit_gate;         /* [55]     CODE_SHOKUNIN_UNLATCHED = 255 */
    uint16_t crc16_ccitt;       /* [56..57] State integrity check */
    uint8_t  reserved_pad[6];   /* [58..63] Explicit cache-line alignment (zero implicit pad) */
} AsymmetricRatchetState64B;

/* Exact 32-Byte DND3 Low-Power Stigmergic Mesh & Proximity Frame */
typedef struct {
    uint8_t  magic[4];          /* [0..3]   "DND3" */
    uint8_t  fowler_seed[8];    /* [4..11]  8-byte Fowler seed (bytes 0..242) */
    uint32_t round_trip_ns;     /* [12..15] Measured two-way causal light-cone RTT */
    uint32_t weld_epoch_lo;     /* [16..19] Sender monotonic ratchet epoch */
    uint16_t hop_ttl;           /* [20..21] Remaining FCC mesh hops (<= 258) */
    uint16_t crc16_ccitt;       /* [22..23] Frame CRC-16/CCITT-FALSE */
    int8_t   season_mode;       /* [24]     Sender mode {-1, 0, +1} */
    uint8_t  trapdoor_code;     /* [25]     250 DNHM, 251 Kintsugi, 253 Drop */
    uint8_t  payload_fowler[6]; /* [26..31] 30 legal Chair44 orientation trits (6 x 5) */
} Dnd3TransitionFrame32B;

typedef int assert_ratchet_64b[(sizeof(AsymmetricRatchetState64B) == 64U) ? 1 : -1];
typedef int assert_dnd3_32b[(sizeof(Dnd3TransitionFrame32B) == 32U) ? 1 : -1];

static uint16_t crc16_ccitt_false(const uint8_t *buf, size_t len) {
    uint16_t crc = 0xFFFFU;
    for (size_t i = 0U; i < len; ++i) {
        crc ^= (uint16_t)((uint16_t)buf[i] << 8U);
        for (uint8_t b = 0U; b < 8U; ++b) {
            if ((crc & 0x8000U) != 0U) {
                crc = (uint16_t)((uint16_t)(crc << 1U) ^ 0x1021U);
            } else {
                crc = (uint16_t)(crc << 1U);
            }
        }
    }
    return crc;
}

/* Multiplication-free 36-trit protected interior POPCNT inner product */
static int32_t popcnt_dot36_interior(
    uint64_t a_pos, uint64_t a_neg,
    uint64_t b_pos, uint64_t b_neg
) {
    const uint64_t ap = a_pos & INTERIOR_36_MASK;
    const uint64_t an = a_neg & INTERIOR_36_MASK;
    const uint64_t bp = b_pos & INTERIOR_36_MASK;
    const uint64_t bn = b_neg & INTERIOR_36_MASK;

    const uint64_t same = (ap & bp) | (an & bn);
    const uint64_t diff = (ap & bn) | (an & bp);
    return (int32_t)(__builtin_popcountll(same) - __builtin_popcountll(diff));
}

static uint8_t step_ratchet_and_dnd3(
    AsymmetricRatchetState64B *st,
    const Dnd3TransitionFrame32B *rx,
    uint32_t max_rtt_ns,
    uint16_t demand_power_mw,
    uint16_t measured_temp_centi_c,
    uint64_t candidate_pos,
    uint64_t candidate_neg
) {
    if (st == NULL || rx == NULL) {
        return (uint8_t)CODE_FAIL_CLOSED;
    }
    if (rx->round_trip_ns > max_rtt_ns ||
        rx->hop_ttl > MAX_HOP_TTL ||
        (candidate_pos & candidate_neg) != 0ULL) {
        st->rejected_frames += 1U;
        st->last_trapdoor = (uint8_t)CODE_FAIL_CLOSED;
        return (uint8_t)CODE_FAIL_CLOSED;
    }
    for (size_t i = 0U; i < 8U; ++i) {
        if (rx->fowler_seed[i] > FOWLER_ACTIVE_CEIL) {
            st->rejected_frames += 1U;
            st->last_trapdoor = (uint8_t)CODE_FAIL_CLOSED;
            return (uint8_t)CODE_FAIL_CLOSED;
        }
    }

    st->noo_tick_ns += (uint64_t)rx->round_trip_ns;
    st->temp_centi_c = (measured_temp_centi_c > T_CEIL_CENTI_C) ? T_CEIL_CENTI_C : measured_temp_centi_c;

    uint8_t code = (uint8_t)CODE_CLEAN;
    if (demand_power_mw <= P_WINTER_MAX_MW) {
        st->season_mode = (int8_t)-1;
        st->power_mw = demand_power_mw;
    } else if (demand_power_mw > P_ACTIVE_MAX_MW || measured_temp_centi_c > T_CEIL_CENTI_C) {
        st->season_mode = (int8_t)0;
        st->power_mw = (uint16_t)P_CAMPFIRE_MAX_MW;
        code = (uint8_t)CODE_KEYHOLE_CLAMP;
    } else {
        st->season_mode = (int8_t)+1;
        st->power_mw = demand_power_mw;
    }

    const uint64_t fence_mask = FENCE_LOWER_MASK | FENCE_UPPER_MASK;
    const uint64_t fence_hit = (candidate_pos | candidate_neg) & fence_mask;
    st->trits_pos = candidate_pos & INTERIOR_36_MASK;
    st->trits_neg = candidate_neg & INTERIOR_36_MASK;

    if (fence_hit != 0ULL || st->count_m >= COUNT_M_KEYHOLE) {
        st->weld_epoch += 1ULL;
        st->kintsugi_welds += 1U;
        st->count_m = (uint8_t)COUNT_M_MIN;
        code = (uint8_t)CODE_KINTSUGI_GOLD;
    } else {
        st->count_m = (uint8_t)(st->count_m + 1U);
    }

    st->last_trapdoor = code;
    st->crc16_ccitt = crc16_ccitt_false((const uint8_t *)st, offsetof(AsymmetricRatchetState64B, crc16_ccitt));
    return code;
}

int main(void) {
    AsymmetricRatchetState64B st = {
        .noo_tick_ns = 1000ULL,
        .trits_pos = 0ULL,
        .trits_neg = 0ULL,
        .weld_epoch = 0ULL,
        .kintsugi_welds = 0U,
        .rejected_frames = 0U,
        .power_mw = 9500U,
        .temp_centi_c = 3650U,
        .fowler_seed = {121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U},
        .count_m = 1U,
        .season_mode = (int8_t)+1,
        .last_trapdoor = 0U,
        .exit_gate = (uint8_t)CODE_SHOKUNIN_UNLATCHED,
        .crc16_ccitt = 0U,
        .reserved_pad = {0U, 0U, 0U, 0U, 0U, 0U}
    };

    Dnd3TransitionFrame32B rx = {
        .magic = {'D', 'N', 'D', '3'},
        .fowler_seed = {121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U},
        .round_trip_ns = 1300U,
        .weld_epoch_lo = 0U,
        .hop_ttl = 13U,
        .crc16_ccitt = 0U,
        .season_mode = (int8_t)+1,
        .trapdoor_code = (uint8_t)CODE_DNHM_BREAKER,
        .payload_fowler = {121U, 121U, 121U, 121U, 121U, 121U}
    };
    rx.crc16_ccitt = crc16_ccitt_false((const uint8_t *)&rx, offsetof(Dnd3TransitionFrame32B, crc16_ccitt));

    /* Drive 250 valid operational ticks across fence hit (step 50) + keyhole roll (step 219) */
    for (uint32_t step = 0U; step < 250U; ++step) {
        const uint16_t p_mw = (step == 180U) ? 14200U : ((step == 190U) ? 1500U : 11200U);
        const uint64_t c_pos = (step == 50U) ? (1ULL << 40U) : (0x00000FFFULL << 3U);
        const uint64_t c_neg = (0x00000FFFULL << 15U);
        const uint8_t rc = step_ratchet_and_dnd3(&st, &rx, 1500U, p_mw, 3720U, c_pos, c_neg);
        if (rc == CODE_FAIL_CLOSED) {
            fprintf(stderr, "[FAIL] Unexpected fail-closed drop at step %u\n", step);
            return 1;
        }
    }

    Dnd3TransitionFrame32B spoof = rx;
    spoof.round_trip_ns = 2200U;
    const uint8_t drop_rc = step_ratchet_and_dnd3(&st, &spoof, 1500U, 10000U, 3700U, 0ULL, 0ULL);

    const int32_t dot36 = popcnt_dot36_interior(
        st.trits_pos, st.trits_neg,
        INTERIOR_36_MASK, 0ULL
    );

    if (drop_rc != CODE_FAIL_CLOSED || st.rejected_frames != 1U || st.weld_epoch != 2ULL || dot36 != 0) {
        fprintf(stderr, "[FAIL] Invariant mismatch: drop=%u rej=%u epoch=%llu dot36=%d\n",
                (unsigned)drop_rc, st.rejected_frames,
                (unsigned long long)st.weld_epoch, dot36);
        return 1;
    }

    printf("[C99_ABI_PASS] sizeof(State64)=%zu B | sizeof(DND3)=%zu B | noo_tick_ns=%llu | epochs=%llu | welds=%u | rej=%u | dot36=%d | exit=%u\n",
           sizeof(AsymmetricRatchetState64B),
           sizeof(Dnd3TransitionFrame32B),
           (unsigned long long)st.noo_tick_ns,
           (unsigned long long)st.weld_epoch,
           st.kintsugi_welds,
           st.rejected_frames,
           dot36,
           (unsigned)st.exit_gate);
    return 0;
}
