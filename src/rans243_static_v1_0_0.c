#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* =========================================================================
 * Dandelion 5-Trit Byte-Packed rANS (M = 3^5 = 243, Kintsugi Bytes 243..255)
 * ISO C99 | Zero External Dependencies | Exact KL-Optimal Simplex Quantizer
 * Artifact: rans243_static_v1_0_0.c | CODE_SHOKUNIN_UNLATCHED = 255
 * ========================================================================= */

#define TRIT_BASE        3U
#define TRITS_PER_BYTE   5U
#define RADIX_BYTE       243U   /* 3^5 = 243 valid balanced-ternary states */
#define PROB_SCALE_EXP   6U     /* M = 3^6 = 729 probability denominator   */
#define PROB_SCALE       729U   /* 3^6                                     */
#define RANS_L           (RADIX_BYTE * RADIX_BYTE * RADIX_BYTE) /* 243^3 = 14,348,907 */
#define MAX_ALPHABET     256U
#define RANS_MAX_MSG     100000U
#define RANS_MAX_CAP     (RANS_MAX_MSG * 2U)

static uint8_t g_rans_scratch[RANS_MAX_CAP];
static uint8_t g_rans_msg[RANS_MAX_MSG];
static uint8_t g_rans_compressed[RANS_MAX_CAP];
static uint8_t g_rans_decoded[RANS_MAX_MSG];

/* Kintsugi out-of-band control bytes (243..255) */
#define KINTSUGI_SYNC    0xF3U  /* 243: Stream header / synchronization marker */
#define KINTSUGI_EOF     0xF4U  /* 244: End-of-frame marker                    */
#define KINTSUGI_TRAP    0xFFU  /* 255: Hardware / parity trapdoor             */

typedef struct {
    uint16_t freq[MAX_ALPHABET];      /* Quantized integer frequencies summing to M */
    uint16_t cum_freq[MAX_ALPHABET];  /* Cumulative frequency table                 */
    uint8_t  slot_to_sym[PROB_SCALE]; /* O(1) reverse lookup table                  */
    uint32_t alphabet_size;
    uint32_t scale;
} TernaryModel;

/* -------------------------------------------------------------------------
 * 1. Exact KL-Optimal Simplex Quantization onto M = 3^q (Greedily Optimal)
 *    Minimizes D_KL(P || c/M) = sum_i p_i * ln(p_i * M / c_i) subject to
 *    c_i >= 1 (for p_i > 0) and sum_i c_i = M.
 * ------------------------------------------------------------------------- */
static bool build_kl_optimal_model(TernaryModel *m, const double *probs,
                                   uint32_t n_syms, uint32_t scale) {
    if (!m || !probs || n_syms == 0 || n_syms > MAX_ALPHABET || scale < n_syms || scale > PROB_SCALE) {
        return false;
    }

    memset(m, 0, sizeof(*m));
    m->alphabet_size = n_syms;
    m->scale = scale;

    uint32_t active = 0;
    double sum_p = 0.0;
    for (uint32_t i = 0; i < n_syms; ++i) {
        if (probs[i] > 0.0) {
            m->freq[i] = 1;
            active++;
            sum_p += probs[i];
        } else {
            m->freq[i] = 0;
        }
    }
    if (active == 0 || active > scale || sum_p <= 0.0) {
        return false;
    }

    /* Greedy concave marginal log-likelihood maximization */
    uint32_t remaining = scale - active;
    for (uint32_t step = 0; step < remaining; ++step) {
        uint32_t best_idx = 0;
        double best_gain = -1.0;
        for (uint32_t i = 0; i < n_syms; ++i) {
            if (m->freq[i] > 0) {
                double c = (double)m->freq[i];
                double gain = (probs[i] / sum_p) * log((c + 1.0) / c);
                if (gain > best_gain) {
                    best_gain = gain;
                    best_idx = i;
                }
            }
        }
        m->freq[best_idx]++;
    }

    uint16_t acc = 0;
    for (uint32_t i = 0; i < n_syms; ++i) {
        m->cum_freq[i] = acc;
        for (uint16_t k = 0; k < m->freq[i]; ++k) {
            m->slot_to_sym[acc + k] = (uint8_t)i;
        }
        acc = (uint16_t)(acc + m->freq[i]);
    }
    return (acc == scale);
}

/* -------------------------------------------------------------------------
 * 2. Balanced-Trit <-> 243-State Byte Transcoders (Zero Branch, Pure Base-3)
 *    Maps 5 balanced trits t[0..4] in {-1, 0, +1} <-> byte b in [0..242].
 * ------------------------------------------------------------------------- */
static inline uint8_t pack_5trits_to_byte(const int8_t t[5]) {
    uint32_t acc = 0;
    for (int i = 4; i >= 0; --i) {
        uint32_t digit = (uint32_t)(t[i] + 1); /* {-1, 0, +1} -> {0, 1, 2} */
        acc = acc * 3U + digit;
    }
    return (uint8_t)acc; /* Strictly in [0, 242] */
}

static inline bool unpack_byte_to_5trits(uint8_t b, int8_t t[5]) {
    if (b >= RADIX_BYTE) {
        return false; /* Kintsugi control/trapdoor byte 243..255 */
    }
    uint32_t v = b;
    for (int i = 0; i < 5; ++i) {
        t[i] = (int8_t)((v % 3U) - 1);
        v /= 3U;
    }
    return true;
}

/* -------------------------------------------------------------------------
 * 3. 243-Radix rANS Encoder (Backwards Pass -> Forward Byte Stream)
 *    State invariant: X in [RANS_L, RANS_L * 243) = [243^3, 243^4)
 *    243^4 = 3,486,784,401 < 2^32 (4,294,967,295) -> zero 32-bit overflow.
 * ------------------------------------------------------------------------- */
static size_t rans243_encode(const TernaryModel *m, const uint8_t *src, size_t len,
                             uint8_t *dst, size_t dst_cap) {
    if (!m || !src || !dst || dst_cap < 16 || dst_cap > RANS_MAX_CAP) return 0;

    uint8_t *scratch = g_rans_scratch;

    size_t sp = 0;
    uint32_t x = RANS_L;
    const uint32_t M = m->scale;

    /* Encode symbols in reverse order so decoder reads forward */
    for (size_t idx = len; idx > 0; --idx) {
        uint8_t s = src[idx - 1];
        uint32_t f_s = m->freq[s];
        uint32_t c_s = m->cum_freq[s];
        if (f_s == 0) {
            return 0;
        }

        /* Renormalize: flush base-243 digits (5 trits each) while X >= x_max */
        uint32_t x_max = (RANS_L / M) * RADIX_BYTE * f_s;
        while (x >= x_max) {
            if (sp >= dst_cap) {
                return 0;
            }
            scratch[sp++] = (uint8_t)(x % RADIX_BYTE); /* Strictly in [0..242] */
            x /= RADIX_BYTE;
        }

        /* Core rANS transition: C(s, x) = floor(x / f_s) * M + c_s + (x mod f_s) */
        x = (x / f_s) * M + c_s + (x % f_s);
    }

    /* Flush final 4 base-243 digits of state X (243^4 > 2^32 > X) */
    for (int i = 0; i < 4; ++i) {
        if (sp >= dst_cap) {
            return 0;
        }
        scratch[sp++] = (uint8_t)(x % RADIX_BYTE);
        x /= RADIX_BYTE;
    }

    /* Frame output with Kintsugi SYNC (0xF3) and EOF (0xF4) boundary markers */
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

/* -------------------------------------------------------------------------
 * 4. 243-Radix rANS Decoder (Forward Pass)
 * ------------------------------------------------------------------------- */
static bool rans243_decode(const TernaryModel *m, const uint8_t *src, size_t src_len,
                           uint8_t *dst, size_t expected_len) {
    if (!m || !src || !dst || src_len < 6) return false;
    if (src[0] != KINTSUGI_SYNC || src[src_len - 1] != KINTSUGI_EOF) return false;

    const uint8_t *ptr = src + 1;
    const uint8_t *end = src + src_len - 1;
    const uint32_t M = m->scale;

    uint32_t x = 0;
    for (int i = 0; i < 4; ++i) {
        if (ptr >= end || *ptr >= RADIX_BYTE) return false;
        x = x * RADIX_BYTE + (*ptr++);
    }

    for (size_t i = 0; i < expected_len; ++i) {
        uint32_t slot = x % M;
        uint8_t s = m->slot_to_sym[slot];
        dst[i] = s;

        uint32_t f_s = m->freq[s];
        uint32_t c_s = m->cum_freq[s];

        x = f_s * (x / M) + slot - c_s;

        while (x < RANS_L) {
            if (ptr >= end || *ptr >= RADIX_BYTE) return false;
            x = x * RADIX_BYTE + (*ptr++);
        }
    }

    return (x == RANS_L && ptr == end);
}

static uint32_t xorshift32(uint32_t *state) {
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

int main(void) {
    const uint32_t N_SYMS = 9;
    const double weights[9] = {
        1.0 / 9.0, 1.0 / 3.0, 1.0 / 3.0,
        1.0,       1.0,       1.0,
        3.0,       3.0,       9.0
    };
    double probs[9];
    double Z = 0.0;
    for (uint32_t i = 0; i < N_SYMS; ++i) Z += weights[i];
    for (uint32_t i = 0; i < N_SYMS; ++i) probs[i] = weights[i] / Z;

    double H2 = 0.0;
    for (uint32_t i = 0; i < N_SYMS; ++i) {
        H2 -= probs[i] * log2(probs[i]);
    }
    double H3 = H2 / log2(3.0);

    TernaryModel model;
    if (!build_kl_optimal_model(&model, probs, N_SYMS, PROB_SCALE)) {
        fprintf(stderr, "Failed to build KL-optimal ternary model.\n");
        return 1;
    }

    double d_kl_bits = 0.0;
    for (uint32_t i = 0; i < N_SYMS; ++i) {
        double q_i = (double)model.freq[i] / (double)model.scale;
        d_kl_bits += probs[i] * log2(probs[i] / q_i);
    }

    const size_t N_MSG = RANS_MAX_MSG;
    uint8_t *msg = g_rans_msg;
    uint8_t *compressed = g_rans_compressed;
    uint8_t *decoded = g_rans_decoded;

    double cdf[9];
    double acc = 0.0;
    for (uint32_t i = 0; i < N_SYMS; ++i) {
        acc += probs[i];
        cdf[i] = acc;
    }
    cdf[N_SYMS - 1] = 1.0;

    uint32_t rng = 0x1337BEEFU;
    for (size_t i = 0; i < N_MSG; ++i) {
        double u = (double)xorshift32(&rng) / (double)UINT32_MAX;
        uint8_t sym = 0;
        while (sym < N_SYMS - 1 && u > cdf[sym]) sym++;
        msg[i] = sym;
    }

    size_t comp_bytes = rans243_encode(&model, msg, N_MSG, compressed, N_MSG * 2);
    if (comp_bytes == 0) return 1;

    size_t payload_bytes = comp_bytes - 2;
    int8_t trits[5];
    for (size_t i = 1; i <= payload_bytes; ++i) {
        if (!unpack_byte_to_5trits(compressed[i], trits) ||
            pack_5trits_to_byte(trits) != compressed[i]) {
            return 1;
        }
    }

    bool ok = rans243_decode(&model, compressed, comp_bytes, decoded, N_MSG);
    bool exact_match = ok && (memcmp(msg, decoded, N_MSG) == 0);

    double achieved_trits_per_sym = (double)(payload_bytes * TRITS_PER_BYTE) / (double)N_MSG;
    double achieved_bits_equiv  = achieved_trits_per_sym * log2(3.0);

    printf("=== Dandelion 243-State (5-Trit Byte-Packed) rANS Benchmark ===\n");
    printf("Quantization D_KL     : %.10f bits/sym\n", d_kl_bits);
    printf("Shannon Limit H_3(P)  : %.6f trits/sym (%.6f bits/sym)\n", H3, H2);
    printf("Achieved Rate         : %.6f trits/sym (%.6f info-bits/sym)\n",
           achieved_trits_per_sym, achieved_bits_equiv);
    printf("Round-Trip Integrity  : %s\n", exact_match ? "PASS" : "FAIL");

    return exact_match ? 0 : 1;
}
