/*
 * File: src/vbst_ternary_telemetry.c
 * Architecture: Dandelion Sovereign Substrate — Fowler 5-Trit Packing & Dual-Rail POPCNT
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Reference: https://docs.google.com/document/d/1daNm9haRWxle35_J_CiS3J_SrRRs7I2C1X3twajPBPI/edit
 */

#include "dandelion_protocol.h"

void fowler_pack_39trits(const int8_t trits[GAIKU_TRITS],
                         uint8_t out_bytes[FOWLER_BYTES_PER_SEED]) {
    for (int b = 0; b < (int)FOWLER_BYTES_PER_SEED; b++) {
        uint32_t val  = 0U;
        uint32_t mult = 1U;
        int start = b * 5;
        int end   = start + 5;
        if (end > (int)GAIKU_TRITS) {
            end = (int)GAIKU_TRITS;
        }
        for (int i = start; i < end; i++) {
            uint32_t trit_val = (uint32_t)(trits[i] + 1);
            val  += trit_val * mult;
            mult *= 3U;
        }
        out_bytes[b] = (uint8_t)val;
    }
}

int32_t vbst_dot40_popcnt(uint64_t w_pos, uint64_t w_neg,
                          uint64_t x_pos, uint64_t x_neg) {
    const uint64_t mask40 = (1ULL << FOWLER_ROSETTA_TRITS) - 1ULL;
    uint64_t wp   = w_pos & mask40;
    uint64_t wn   = w_neg & mask40;
    uint64_t xp   = x_pos & mask40;
    uint64_t xn   = x_neg & mask40;
    uint64_t same = (wp & xp) | (wn & xn);
    uint64_t diff = (wp & xn) | (wn & xp);
    return (int32_t)(__builtin_popcountll(same) - __builtin_popcountll(diff));
}
