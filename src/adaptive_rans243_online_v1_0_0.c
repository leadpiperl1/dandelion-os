#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* =========================================================================
 * Dandelion Online Adaptive 243-State (5-Trit Byte-Packed) rANS Codec
 * ISO C99 | Zero External Dependencies | PRNG-Seeded Triadic Predictor
 * Artifact: adaptive_rans243_online_v1_0_0.c | CODE_SHOKUNIN_UNLATCHED = 255
 * ========================================================================= */

#define TRITS_PER_BYTE   5U
#define RADIX_BYTE       243U        /* 3^5 = 243 valid balanced-ternary states */
#define PROB_SCALE_EXP   6U          /* M = 3^6 = 729 probability denominator   */
#define PROB_SCALE       729U        /* 3^6                                     */
#define RANS_L           14348907U   /* 243^3 = 14,348,907                      */
#define MAX_ALPHABET     32U
#define NUM_CONTEXTS     (MAX_ALPHABET * MAX_ALPHABET)

/* Kintsugi out-of-band control bytes (243..255) */
#define KINTSUGI_SYNC    0xF3U       /* 243: Stream header marker               */
#define KINTSUGI_EOF     0xF4U       /* 244: End-of-frame marker                */

typedef struct {
    uint16_t freq;
    uint16_t cum_freq;
} SymbolInterval;

#define RANS_MAX_MSG     60000U
#define RANS_MAX_CAP     (RANS_MAX_MSG * 2U)

static SymbolInterval g_adapt_history[RANS_MAX_MSG];
static uint8_t g_adapt_scratch[RANS_MAX_CAP];
static uint8_t g_adapt_msg[RANS_MAX_MSG];
static uint8_t g_adapt_comp[RANS_MAX_CAP];
static uint8_t g_adapt_dec[RANS_MAX_MSG];

typedef struct {
    uint32_t n_syms;
    uint32_t scale;        /* M = 729 = 3^6 */
    uint32_t lr_shift;     /* Learning rate step size (in 1/729 quanta) */
    uint32_t step_count;
    uint8_t  prev1;
    uint8_t  prev2;
    uint16_t ctx_freq[NUM_CONTEXTS][MAX_ALPHABET];
} AdaptiveTriadicPredictor;

static uint32_t splitmix32(uint32_t *state) {
    uint32_t z = (*state += 0x9E3779B9U);
    z = (z ^ (z >> 16)) * 0x85EBCA6BU;
    z = (z ^ (z >> 13)) * 0xC2B2AE35U;
    return z ^ (z >> 16);
}

static void predictor_init(AdaptiveTriadicPredictor *p, uint32_t seed,
                           uint32_t n_syms, uint32_t lr_quanta) {
    memset(p, 0, sizeof(*p));
    p->n_syms = n_syms;
    p->scale = PROB_SCALE;
    p->lr_shift = lr_quanta;
    p->step_count = 0U;
    p->prev1 = 0U;
    p->prev2 = 0U;

    uint32_t rng = seed;
    const uint32_t n_ctx = n_syms * n_syms;

    for (uint32_t c = 0; c < n_ctx; ++c) {
        uint32_t sum = 0;
        uint32_t raw[MAX_ALPHABET];
        for (uint32_t s = 0; s < n_syms; ++s) {
            int32_t trit = (int32_t)(splitmix32(&rng) % 3U) - 1;
            raw[s] = (trit < 0) ? 1U : ((trit == 0) ? 3U : 9U);
            sum += raw[s];
        }

        uint32_t assigned = 0;
        for (uint32_t s = 0; s < n_syms; ++s) {
            uint32_t f = (raw[s] * PROB_SCALE) / sum;
            if (f == 0U) f = 1U;
            p->ctx_freq[c][s] = (uint16_t)f;
            assigned += f;
        }
        while (assigned < PROB_SCALE) {
            uint32_t best = 0;
            for (uint32_t s = 1; s < n_syms; ++s) {
                if (raw[s] > raw[best]) best = s;
            }
            p->ctx_freq[c][best]++;
            assigned++;
        }
        while (assigned > PROB_SCALE) {
            uint32_t best = 0;
            for (uint32_t s = 1; s < n_syms; ++s) {
                if (p->ctx_freq[c][s] > p->ctx_freq[c][best]) best = s;
            }
            if (p->ctx_freq[c][best] > 1U) {
                p->ctx_freq[c][best]--;
                assigned--;
            }
        }
    }
}

static inline void predictor_get_tables(const AdaptiveTriadicPredictor *p,
                                        uint16_t cum_freq[MAX_ALPHABET],
                                        uint8_t slot_to_sym[PROB_SCALE]) {
    uint32_t ctx = (uint32_t)p->prev2 * p->n_syms + (uint32_t)p->prev1;
    const uint16_t *f = p->ctx_freq[ctx];
    uint16_t acc = 0;
    for (uint32_t s = 0; s < p->n_syms; ++s) {
        cum_freq[s] = acc;
        if (slot_to_sym) {
            for (uint16_t k = 0; k < f[s]; ++k) {
                slot_to_sym[acc + k] = (uint8_t)s;
            }
        }
        acc = (uint16_t)(acc + f[s]);
    }
}

static inline void predictor_update(AdaptiveTriadicPredictor *p, uint8_t observed_sym) {
    uint32_t ctx = (uint32_t)p->prev2 * p->n_syms + (uint32_t)p->prev1;
    uint16_t *f = p->ctx_freq[ctx];

    uint32_t boost = p->lr_shift;
    uint32_t donor_cursor = (observed_sym + 1U) % p->n_syms;
    for (uint32_t step = 0; step < boost; ++step) {
        bool donated = false;
        for (uint32_t tries = 0; tries < p->n_syms; ++tries) {
            uint32_t d = (donor_cursor + tries) % p->n_syms;
            if (d != observed_sym && f[d] > 1U) {
                f[d]--;
                f[observed_sym]++;
                donor_cursor = (d + 1U) % p->n_syms;
                donated = true;
                break;
            }
        }
        if (!donated) break;
    }

    p->prev2 = p->prev1;
    p->prev1 = observed_sym;
    p->step_count++;
}

static inline bool unpack_byte_to_5trits(uint8_t b, int8_t t[5]) {
    if (b >= RADIX_BYTE) return false;
    uint32_t v = b;
    for (int i = 0; i < 5; ++i) {
        t[i] = (int8_t)((v % 3U) - 1);
        v /= 3U;
    }
    return true;
}

static inline uint8_t pack_5trits_to_byte(const int8_t t[5]) {
    uint32_t acc = 0;
    for (int i = 4; i >= 0; --i) {
        acc = acc * 3U + (uint32_t)(t[i] + 1);
    }
    return (uint8_t)acc;
}

static size_t adaptive_rans243_encode(uint32_t prng_seed, uint32_t n_syms, uint32_t lr_quanta,
                                      const uint8_t *src, size_t len,
                                      uint8_t *dst, size_t dst_cap) {
    if (!src || !dst || n_syms == 0 || n_syms > MAX_ALPHABET || len > RANS_MAX_MSG || dst_cap < 16 || dst_cap > RANS_MAX_CAP) return 0;

    SymbolInterval *history = g_adapt_history;
    uint8_t *scratch = g_adapt_scratch;

    AdaptiveTriadicPredictor pred;
    predictor_init(&pred, prng_seed, n_syms, lr_quanta);

    uint16_t cum_freq[MAX_ALPHABET];
    for (size_t t = 0; t < len; ++t) {
        uint8_t s = src[t];
        if (s >= n_syms) {
            return 0;
        }
        uint32_t ctx = (uint32_t)pred.prev2 * n_syms + (uint32_t)pred.prev1;
        predictor_get_tables(&pred, cum_freq, NULL);

        history[t].freq = pred.ctx_freq[ctx][s];
        history[t].cum_freq = cum_freq[s];

        predictor_update(&pred, s);
    }

    size_t sp = 0;
    uint32_t x = RANS_L;
    const uint32_t M = PROB_SCALE;

    for (size_t idx = len; idx > 0; --idx) {
        uint32_t f_s = history[idx - 1].freq;
        uint32_t c_s = history[idx - 1].cum_freq;

        uint32_t x_max = (RANS_L / M) * RADIX_BYTE * f_s;
        while (x >= x_max) {
            if (sp >= dst_cap) {
                return 0;
            }
            scratch[sp++] = (uint8_t)(x % RADIX_BYTE);
            x /= RADIX_BYTE;
        }
        x = (x / f_s) * M + c_s + (x % f_s);
    }

    for (int i = 0; i < 4; ++i) {
        if (sp >= dst_cap) {
            return 0;
        }
        scratch[sp++] = (uint8_t)(x % RADIX_BYTE);
        x /= RADIX_BYTE;
    }

    size_t out_len = sp + 2;
    if (out_len > dst_cap) {
        return 0;
    }

    dst[0] = KINTSUGI_SYNC;
    for (size_t i = 0; i < sp; ++i) {
        dst[1 + i] = scratch[sp - 1 - i];
    }
    dst[1 + sp] = KINTSUGI_EOF;

    return out_len;
}

static bool adaptive_rans243_decode(uint32_t prng_seed, uint32_t n_syms, uint32_t lr_quanta,
                                    const uint8_t *src, size_t src_len,
                                    uint8_t *dst, size_t expected_len) {
    if (!src || !dst || n_syms == 0 || n_syms > MAX_ALPHABET || src_len < 6) return false;
    if (src[0] != KINTSUGI_SYNC || src[src_len - 1] != KINTSUGI_EOF) return false;

    const uint8_t *ptr = src + 1;
    const uint8_t *end = src + src_len - 1;
    const uint32_t M = PROB_SCALE;

    uint32_t x = 0;
    for (int i = 0; i < 4; ++i) {
        if (ptr >= end || *ptr >= RADIX_BYTE) return false;
        x = x * RADIX_BYTE + (*ptr++);
    }

    AdaptiveTriadicPredictor pred;
    predictor_init(&pred, prng_seed, n_syms, lr_quanta);

    uint16_t cum_freq[MAX_ALPHABET];
    uint8_t slot_to_sym[PROB_SCALE];

    for (size_t t = 0; t < expected_len; ++t) {
        uint32_t ctx = (uint32_t)pred.prev2 * n_syms + (uint32_t)pred.prev1;
        predictor_get_tables(&pred, cum_freq, slot_to_sym);

        uint32_t slot = x % M;
        uint8_t s = slot_to_sym[slot];
        dst[t] = s;

        uint32_t f_s = pred.ctx_freq[ctx][s];
        uint32_t c_s = cum_freq[s];

        x = f_s * (x / M) + slot - c_s;
        while (x < RANS_L) {
            if (ptr >= end || *ptr >= RADIX_BYTE) return false;
            x = x * RADIX_BYTE + (*ptr++);
        }

        predictor_update(&pred, s);
    }

    return (x == RANS_L && ptr == end);
}

int main(void) {
    const uint32_t SHARED_SEED = 0x20261122U;
    const uint32_t N_SYMS = 16U;
    const size_t N_MSG = RANS_MAX_MSG;

    uint8_t *msg = g_adapt_msg;
    uint8_t *comp = g_adapt_comp;
    uint8_t *dec = g_adapt_dec;

    for (size_t i = 0; i < 20000; ++i) {
        msg[i] = (uint8_t)(i % 4U);
    }
    const uint8_t reg2[4] = {7, 11, 15, 2};
    for (size_t i = 20000; i < 40000; ++i) {
        msg[i] = reg2[i % 4U];
    }
    msg[40000] = 1;
    msg[40001] = 2;
    for (size_t i = 40002; i < N_MSG; ++i) {
        msg[i] = (uint8_t)((msg[i - 1] + msg[i - 2]) % N_SYMS);
    }

    const uint32_t lrs[] = {0U, 9U, 27U, 81U, 162U};
    bool all_ok = true;

    for (size_t k = 0; k < sizeof(lrs) / sizeof(lrs[0]); ++k) {
        uint32_t lr = lrs[k];
        size_t c_len = adaptive_rans243_encode(SHARED_SEED, N_SYMS, lr, msg, N_MSG, comp, N_MSG * 2);
        bool ok = (c_len > 0) &&
                  adaptive_rans243_decode(SHARED_SEED, N_SYMS, lr, comp, c_len, dec, N_MSG) &&
                  (memcmp(msg, dec, N_MSG) == 0);

        size_t payload_bytes = c_len - 2;
        int8_t trits[5];
        for (size_t b = 1; b <= payload_bytes; ++b) {
            if (!unpack_byte_to_5trits(comp[b], trits) || pack_5trits_to_byte(trits) != comp[b]) {
                ok = false;
            }
        }
        all_ok = all_ok && ok;
        printf("lr=%3u/729 -> payload=%zu B (%.5f trits/sym) [%s]\n",
               lr, payload_bytes,
               (double)(payload_bytes * TRITS_PER_BYTE) / (double)N_MSG,
               ok ? "PASS" : "FAIL");
    }

    return all_ok ? 0 : 1;
}
