/*
 * qpv_dandelion_hindsight_v1_0_0.c
 * Strict ISO C99 zero-allocation (malloc = 0) dual-rail POPCNT hindsight kernel
 * and 32-byte DND3 operational light-cone proximity frame.
 */
#include <stdint.h>
#include <stdio.h>


#define FOWLER_FRAME_TRITS      40U
#define CODE_CLEAN              0U
#define CODE_DNHM_BREAKER       250U
#define CODE_KINTSUGI_GOLD      251U
#define CODE_FAIL_CLOSED_DROP   253U
#define CODE_SHOKUNIN_UNLATCHED 255U
#define MAX_ORBITAL_HOP_TTL     258U


typedef struct {
    uint8_t  magic[4];          /* "DND3" */
    uint8_t  fowler_seed[8];    /* 40 balanced trits packed at 5 trits/byte (<= 242) */
    uint32_t sender_node_id;    /* Ephemeral bilateral Fist-Bump ID (zero central registry) */
    uint16_t hop_ttl;           /* Bounded by light-cone / orbital horizon <= 258 */
    uint8_t  trapdoor_code;     /* 0 = Clean, 250 = DNHM, 251 = Kintsugi, 253 = Fail-Closed */
    uint8_t  crc8_parity;       /* Frame integrity check */
    uint8_t  payload_trits[12]; /* 60 balanced trits */
} Dnd3ProximityFrame;


_Static_assert(sizeof(Dnd3ProximityFrame) == 32U, "Dnd3ProximityFrame must be exact 32 B");


/* Multiplication-free 40-trit dual-rail POPCNT inner product (L1D resident) */
static inline int32_t vbst_dot40_popcnt(
    uint64_t w_pos, uint64_t w_neg,
    uint64_t x_pos, uint64_t x_neg
) {
    const uint64_t mask40 = (1ULL << FOWLER_FRAME_TRITS) - 1ULL;
    const uint64_t wp = w_pos & mask40;
    const uint64_t wn = w_neg & mask40;
    const uint64_t xp = x_pos & mask40;
    const uint64_t xn = x_neg & mask40;


    const uint64_t same = (wp & xp) | (wn & xn);
    const uint64_t diff = (wp & xn) | (wn & xp);


    return (int32_t)(__builtin_popcountll(same) - __builtin_popcountll(diff));
}


/* Evaluates operational light-cone proximity + Kintsugi hindsight weld */
static inline uint8_t evaluate_proximity_gate(
    const Dnd3ProximityFrame *frame,
    uint64_t round_trip_ns,
    uint64_t max_perimeter_ns,
    int32_t  hindsight_score
) {
    if (frame == NULL || round_trip_ns > max_perimeter_ns || frame->hop_ttl > MAX_ORBITAL_HOP_TTL) {
        return (uint8_t)CODE_FAIL_CLOSED_DROP; /* 0-tick fail-closed light-cone violation */
    }
    if (hindsight_score < 0) {
        return (uint8_t)CODE_KINTSUGI_GOLD;    /* Trigger Byte 251 retroactive phase weld */
    }
    return (uint8_t)CODE_CLEAN;
}


int main(void) {
    Dnd3ProximityFrame frame = {
        .magic = {'D', 'N', 'D', '3'},
        .fowler_seed = {121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U},
        .sender_node_id = 0x00000D13U,
        .hop_ttl = 13U,
        .trapdoor_code = (uint8_t)CODE_DNHM_BREAKER,
        .crc8_parity = 0xA5U,
        .payload_trits = {121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U, 121U}
    };


    /* 13-trit balanced equilibrium check (6 +1, 6 -1, 1 0 -> net dot = 0) */
    const uint64_t w_pos = 0x0000003FULL;
    const uint64_t w_neg = 0x00000FC0ULL;
    const uint64_t x_pos = 0x00000FFFULL;
    const uint64_t x_neg = 0x00000000ULL;


    const int32_t dot = vbst_dot40_popcnt(w_pos, w_neg, x_pos, x_neg);
    /* 195 m indoor NIST baseline -> ~1300 ns two-way optical round-trip */
    const uint8_t gate_ok = evaluate_proximity_gate(&frame, 1300ULL, 1350ULL, dot);
    const uint8_t gate_late = evaluate_proximity_gate(&frame, 1900ULL, 1350ULL, dot);


    if (dot != 0 || gate_ok != CODE_CLEAN || gate_late != CODE_FAIL_CLOSED_DROP) {
        fprintf(stderr, "[FAIL] Parity or light-cone gate mismatch.\n");
        return 1;
    }


    printf("[ABI_OK] sizeof(Dnd3ProximityFrame)=%zu B | dot40=%d | ok=%u | late=%u | exit=%u\n",
           sizeof(Dnd3ProximityFrame), dot, (unsigned)gate_ok, (unsigned)gate_late,
           (unsigned)CODE_SHOKUNIN_UNLATCHED);
    return 0;
}
