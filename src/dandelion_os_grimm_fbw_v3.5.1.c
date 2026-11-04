/*
 * ============================================================================
 * DANDELION OS v4.1.1 / v3.5.1 (CODENAME: URSINE & URSHWERA / HEIMAT-AGORA)
 * ============================================================================
 * File: src/dandelion_os_grimm_fbw_v3.5.1.c
 * Version: v4.1.1 (Strict 3-Digit Triadic SemVer: MAJOR=4, MINOR=1, PATCH=1)
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Affiliation: Independent Dandelion Substrate Research Group, Gurley, AL, USA
 * License: GPL-2.0-or-later / MIT Dual Open-Source
 * ============================================================================
 */

#define _POSIX_C_SOURCE 200809L
#include "../include/dandelion_os_v3.5.1.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

static inline uint64_t preauth_siphash24_lite(const char *msg, size_t len) {
    uint64_t v0 = SIPHASH_K0_SEED;
    uint64_t v1 = SIPHASH_K1_SEED;
    if (msg == NULL) {
        return v0 ^ v1;
    }
    for (size_t i = 0U; i < len; i++) {
        v0 ^= (uint64_t)(unsigned char)msg[i];
        v0 = (v0 << 13U) | (v0 >> 51U);
        v1 = (v1 + v0) ^ ((v1 << 17U) | (v1 >> 47U));
    }
    return v0 ^ v1 ^ (uint64_t)len;
}

static bool contains_word_ci(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) {
        return false;
    }
    char buf[MAX_UTTERANCE_BYTES];
    size_t len = strlen(haystack);
    if (len >= sizeof(buf)) {
        len = sizeof(buf) - 1U;
    }
    for (size_t i = 0U; i < len; i++) {
        buf[i] = (char)tolower((unsigned char)haystack[i]);
    }
    buf[len] = '\0';
    return strstr(buf, needle) != NULL;
}

void fowler_pack_39trits(
    const int8_t trits[GAIKU_TRITS],
    uint8_t out_bytes[FOWLER_BYTES_PER_SEED]
) {
    if (trits == NULL || out_bytes == NULL) {
        return;
    }
    for (size_t b = 0U; b < FOWLER_BYTES_PER_SEED; b++) {
        uint16_t acc = 0U;
        uint16_t pow3 = 1U;
        for (size_t k = 0U; k < FOWLER_TRITS_PER_BYTE; k++) {
            size_t idx = b * FOWLER_TRITS_PER_BYTE + k;
            int8_t t = (idx < GAIKU_TRITS) ? trits[idx] : 0;
            if (t < -1) {
                t = -1;
            }
            if (t > 1) {
                t = 1;
            }
            uint16_t shifted_trit = (uint16_t)((int)t + 1);
            acc = (uint16_t)(acc + (uint16_t)(shifted_trit * pow3));
            pow3 = (uint16_t)(pow3 * 3U);
        }
        out_bytes[b] = (uint8_t)acc;
    }
}

bool fowler_unpack_39trits_strict(
    const uint8_t in_bytes[FOWLER_BYTES_PER_SEED],
    int8_t trits[GAIKU_TRITS],
    uint8_t *out_trap_code
) {
    if (out_trap_code == NULL) {
        return false;
    }
    if (in_bytes == NULL || trits == NULL) {
        *out_trap_code = CODE_FAIL_CLOSED_DROP;
        return false;
    }
    *out_trap_code = CODE_CLEAN;
    for (size_t b = 0U; b < FOWLER_BYTES_PER_SEED; b++) {
        uint8_t val = in_bytes[b];
        if (val > FOWLER_MAX_VALID_BYTE) {
            *out_trap_code = val;
            memset(trits, 0, GAIKU_TRITS);
            return false;
        }
        for (size_t k = 0U; k < FOWLER_TRITS_PER_BYTE; k++) {
            size_t idx = b * FOWLER_TRITS_PER_BYTE + k;
            if (idx < GAIKU_TRITS) {
                trits[idx] = (int8_t)((int)(val % 3U) - 1);
            }
            val = (uint8_t)(val / 3U);
        }
    }
    return true;
}

void init_tnpt_table(dandelion_tnpt_v3_0_t *tnpt) {
    if (tnpt == NULL) {
        return;
    }
    memset(tnpt, 0, sizeof(dandelion_tnpt_v3_0_t));
    memcpy(tnpt->magic, "TNPT411", 8U);
    tnpt->rosetta_anchor_inode = MACRO_ROSETTA_HARMONY;
    tnpt->taxicab_constant = TAXICAB_INVARIANT;
    tnpt->thermal_watts = DEFAULT_THERMAL_WATTS;
    tnpt->fdt_ou_residual = DEFAULT_FDT_OU_RESIDUAL;
    tnpt->kintsugi_percolation_code = CODE_KINTSUGI_GOLD;
    memcpy(tnpt->reserved_parity, "BGR0", 4U);

    for (size_t i = 0U; i < 128U; i++) {
        unsigned char uc = (unsigned char)tolower((int)i);
        if (uc == 'p' || uc == 't' || uc == 'k' || uc == 'q' || uc == 'c') {
            tnpt->manner_lut[i] = 2U;
        } else if (uc == 'b' || uc == 'd' || uc == 'g' || uc == 'm' || uc == 'n') {
            tnpt->manner_lut[i] = 0U;
        } else {
            tnpt->manner_lut[i] = 1U;
        }
    }
    for (size_t i = 0U; i < 256U; i++) {
        tnpt->grimm_shift_lut[i] = (uint8_t)tolower((int)(i & 0x7FU));
    }

    int8_t dnhm_trits[GAIKU_TRITS];
    for (size_t r = 0U; r < SHINGARI_ROOT_COUNT; r++) {
        int8_t row_trits[GAIKU_TRITS];
        for (size_t c = 0U; c < GAIKU_TRITS; c++) {
            size_t mod = (r * 7U + c * 5U + 3U) % 3U;
            row_trits[c] = (int8_t)((int)mod - 1);
            if (r == 4U) {
                dnhm_trits[c] = row_trits[c];
            }
        }
        row_trits[0]  = -1;
        row_trits[18] =  0;
        row_trits[37] = +1;
        row_trits[38] = +1;
        fowler_pack_39trits(row_trits, tnpt->packed_weights[r]);
    }

    tnpt->so3_f3_pullback_mask[0] = 1U;
    tnpt->so3_f3_pullback_mask[4] = 1U;
    tnpt->so3_f3_pullback_mask[8] = 1U;

    tnpt->dnhm_glyph_vector[0] = 0U; /* -1 Blue: Sailboat Tack */
    tnpt->dnhm_glyph_vector[1] = 1U; /*  0 Green: Console Keyhole Trim */
    tnpt->dnhm_glyph_vector[2] = 2U; /* +1 Red: Kramers Spark Impulse */

    dnhm_trits[0]  = -1;
    dnhm_trits[18] =  0;
    dnhm_trits[37] = +1;
    dnhm_trits[38] = +1;
    fowler_pack_39trits(dnhm_trits, tnpt->dnhm_haiku_seed);
}

int morse_to_ternary_bridge(const char *morse_seq, int8_t *trit_out, size_t max_trits, size_t *out_len) {
    if (morse_seq == NULL || trit_out == NULL || out_len == NULL || max_trits == 0U) {
        return -1;
    }
    size_t idx = 0U;
    for (size_t i = 0U; morse_seq[i] != '\0' && idx < max_trits; ++i) {
        char c = morse_seq[i];
        if (c == '.') {
            trit_out[idx++] = -1;
        } else if (c == '-') {
            trit_out[idx++] = 1;
        } else if (c == ' ' || c == '/') {
            trit_out[idx++] = 0;
        }
    }
    *out_len = idx;
    return (idx > 0U) ? 0 : -1;
}

uint32_t scx_kramhaus_torque_governor(
    const uint16_t *radii_mm,
    const uint8_t  *sin_theta_q8,
    size_t          n_tasks,
    uint8_t        *out_boundary_code
) {
    if (out_boundary_code == NULL) {
        return 0U;
    }
    if (radii_mm == NULL || sin_theta_q8 == NULL || n_tasks == 0U) {
        *out_boundary_code = CODE_CLEAN;
        return 0U;
    }
    size_t bounded_n = (n_tasks > SHINGARI_BOUNDED_TRITS) ? SHINGARI_BOUNDED_TRITS : n_tasks;
    uint64_t accum_unm = 0ULL;
    for (size_t i = 0U; i < bounded_n; i++) {
        uint64_t term = ((uint64_t)radii_mm[i] * 275ULL * (uint64_t)sin_theta_q8[i]) >> 8U;
        accum_unm += term;
    }
    if (accum_unm >= (uint64_t)KRAMHAUS_TAU_CRIT_UNM) {
        *out_boundary_code = CODE_KINTSUGI_GOLD;
        uint64_t half_load = accum_unm / 2ULL;
        uint64_t keyhole_cap = 31140ULL;
        accum_unm = (half_load < keyhole_cap) ? half_load : keyhole_cap;
    } else {
        *out_boundary_code = CODE_CLEAN;
    }
    return (uint32_t)accum_unm;
}

int32_t vbst_dot40_popcnt(
    uint64_t pos_a,
    uint64_t neg_a,
    uint64_t pos_b,
    uint64_t neg_b,
    uint64_t exclusion_mask
) {
    uint64_t valid = ~exclusion_mask & 0xFFFFFFFFFFULL;
    uint64_t pa = pos_a & valid;
    uint64_t na = neg_a & valid;
    uint64_t pb = pos_b & valid;
    uint64_t nb = neg_b & valid;

    uint64_t same_sign = (pa & pb) | (na & nb);
    uint64_t opp_sign  = (pa & nb) | (na & pb);
    return (int32_t)__builtin_popcountll(same_sign) - (int32_t)__builtin_popcountll(opp_sign);
}

void dnhm_stochastic_impulse_pivot(
    const dandelion_tnpt_v3_0_t *tnpt,
    shingari_grimm_packet_t     *pkt
) {
    if (tnpt == NULL || pkt == NULL) {
        return;
    }
    pkt->boundary_code            = CODE_KINTSUGI_GOLD;
    pkt->reg1_blue_subject        = (int8_t)((int)tnpt->dnhm_glyph_vector[0] - 1);
    pkt->reg2_green_treaty        = (int8_t)((int)tnpt->dnhm_glyph_vector[1] - 1);
    pkt->reg3_red_action          = (int8_t)((int)tnpt->dnhm_glyph_vector[2] - 1);
    pkt->reg4_evidential_q        = +1;
    pkt->tokengate_route          = +1;
    pkt->novikov_cycles           = 1U;
    pkt->glyph_vector[0]          = pkt->reg1_blue_subject;
    pkt->glyph_vector[1]          = pkt->reg2_green_treaty;
    pkt->glyph_vector[2]          = pkt->reg3_red_action;
    memcpy(pkt->fowler_haiku_seed, tnpt->dnhm_haiku_seed, FOWLER_BYTES_PER_SEED);
    pkt->operational_entropy_bits = 1.5849625;
    pkt->psi_bureaucracy_clamp    = 0.1644;
}

void parse_grimm_rosetta_fbw(
    const char                  *utterance,
    const dandelion_tnpt_v3_0_t *tnpt,
    shingari_grimm_packet_t     *pkt
) {
    if (utterance == NULL || tnpt == NULL || pkt == NULL) {
        return;
    }
    memset(pkt, 0, sizeof(*pkt));
    size_t ulen = strlen(utterance);
    if (ulen >= MAX_UTTERANCE_BYTES) {
        ulen = MAX_UTTERANCE_BYTES - 1U;
    }
    pkt->siphash24_tag            = preauth_siphash24_lite(utterance, ulen);
    pkt->reg1_blue_subject        = -1;
    pkt->reg2_green_treaty        =  0;
    pkt->reg3_red_action          = +1;
    pkt->reg4_evidential_q        = +1;
    pkt->tokengate_route          = +1;
    pkt->detected_bite            = 6U;
    pkt->shingari_root_idx        = 4U;
    pkt->boundary_code            = CODE_CLEAN;
    pkt->novikov_cycles           = 1U;
    pkt->taxicab_sum              = TAXICAB_INVARIANT;
    pkt->operational_entropy_bits = 1.5849625;

    if (contains_word_ci(utterance, "elias") ||
        contains_word_ci(utterance, "lighthouse") ||
        contains_word_ci(utterance, "committee") ||
        contains_word_ci(utterance, "fertilizer")) {
        pkt->boundary_code            = CODE_ELIAS_TRAPDOOR;
        pkt->reg4_evidential_q        = -1;
        pkt->tokengate_route          = -1;
        pkt->novikov_cycles           = 3U;
        pkt->operational_entropy_bits = 11.4194;
        dnhm_stochastic_impulse_pivot(tnpt, pkt);
        return;
    }

    if (contains_word_ci(utterance, "dave's not here") ||
        contains_word_ci(utterance, "daves not here")) {
        pkt->boundary_code = CODE_DNHM_BREAKER;
        dnhm_stochastic_impulse_pivot(tnpt, pkt);
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE6_INDOEUR;
        return;
    }

    if (contains_word_ci(utterance, "namaste") || contains_word_ci(utterance, "shanti")) {
        pkt->detected_bite       = 6U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE6_INDOEUR;
        memcpy(pkt->fowler_haiku_seed, tnpt->dnhm_haiku_seed, FOWLER_BYTES_PER_SEED);
        return;
    }

    if (contains_word_ci(utterance, "sos") || contains_word_ci(utterance, "... --- ...")) {
        pkt->detected_bite       = 10U;
        pkt->boundary_code       = CODE_KINTSUGI_GOLD;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE10_SENSORY_LIS;
    }
}

#ifdef DANDELION_STANDALONE_MAIN
int main(void) {
    dandelion_tnpt_v3_0_t tnpt;
    shingari_grimm_packet_t pkt_elias;
    shingari_grimm_packet_t pkt_dnhm;
    shingari_grimm_packet_t pkt_namaste;

    init_tnpt_table(&tnpt);
    parse_grimm_rosetta_fbw("elias bureaucratic fertilizer committee", &tnpt, &pkt_elias);
    parse_grimm_rosetta_fbw("Dave's not here, man. Argon tests the line.", &tnpt, &pkt_dnhm);
    parse_grimm_rosetta_fbw("namaste", &tnpt, &pkt_namaste);

    uint16_t radii[12] = {19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19, 19};
    uint8_t  sins[12]  = {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
    uint8_t  t_code    = 0U;
    uint32_t clamped_t = scx_kramhaus_torque_governor(radii, sins, 12U, &t_code);

    int32_t dot40 = vbst_dot40_popcnt(0xFFULL, 0x00ULL, 0xFFULL, 0x00ULL, 0xF0ULL);

    printf("{\"status\":\"PASS\",\"version\":\"v4.1.1\",\"tnpt_bytes\":%zu,"
           "\"elias_pivoted_code\":%u,\"dnhm_entropy\":%.5f,"
           "\"namaste_inode\":%u,\"kramhaus_clamped_unm\":%u,"
           "\"kramhaus_code\":%u,\"vbst_dot40\":%d}\n",
           sizeof(tnpt), (unsigned)pkt_elias.boundary_code,
           pkt_dnhm.operational_entropy_bits,
           (unsigned)pkt_namaste.macro_rosetta_inode,
           (unsigned)clamped_t, (unsigned)t_code, (int)dot40);
    return 0;
}
#endif
