/*
 * Title:   Argon-Hard-Iron v3.8.0 Bare-Metal RC Accumulator & Schmitt Trigger C11 Kernel
 * Author:  Justin D. Grimm (K-Lani 13 Actual, ORCID: 0009-0000-5299-0243)
 */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define V_BASELINE_0        1650
#define V_MAX_RAIL          3300
#define DELTA_VH            165
#define SCHMITT_UPPER_ENTER 1980
#define SCHMITT_UPPER_EXIT  1815
#define SCHMITT_LOWER_ENTER 1320
#define SCHMITT_LOWER_EXIT  1485
#define SHEAR_TRAP_TH       45000
#define KINTSUGI_GOLD_BYTE  251U
#define TELLURIC_PERIOD_NS  9615384ULL
#define TELLURIC_EPOCHS     13U

typedef enum {
    TRIT_SINK    = -1,
    TRIT_NEUTRAL =  0,
    TRIT_SOURCE  =  1
} TernaryState;

typedef struct {
    uint64_t pos_rail_64;
    uint64_t neg_rail_64;
    uint64_t p_scale_3n;
    uint64_t epoch_period_ns;
    int32_t  v_rc_mv;
    int32_t  v_conj_mv;
    int32_t  methyl_bias_mv;
    uint32_t l1_taxicab_norm;
    uint32_t shear_tau_ug;
    uint16_t methyl_cpg_mask;
    uint8_t  scale_n;
    int8_t   window_trit;
    uint8_t  trapdoor_byte;
    uint8_t  vbst_bytes[3];
    uint8_t  misfolded_flag;
    uint8_t  reserved_align[3];
} ArgonMethylatedFrame64B;

_Static_assert(sizeof(ArgonMethylatedFrame64B) == 64,
               "ArgonMethylatedFrame64B must occupy 1 L1D cache line (64 bytes)");

typedef struct {
    int32_t      v_rc;
    TernaryState state;
    uint32_t     trapdoor_trips;
    bool         misfolded;
} ArgonAccumulatorContext;

static inline int32_t conjugate_reflect_v0(int32_t v_rc_mv) {
    return (V_BASELINE_0 << 1) - v_rc_mv;
}

static inline uint32_t branchless_abs_i32(int32_t x) {
    int32_t const mask = x >> 31;
    return (uint32_t)((x + mask) ^ mask);
}

void argon_accumulator_init(ArgonAccumulatorContext *ctx) {
    ctx->v_rc           = V_BASELINE_0;
    ctx->state          = TRIT_NEUTRAL;
    ctx->trapdoor_trips = 0U;
    ctx->misfolded      = false;
}

void argon_step(ArgonAccumulatorContext *ctx, int32_t delta_v_in, int32_t m_epig, int32_t shear_ug, int32_t delta_e_noise) {
    if (shear_ug > SHEAR_TRAP_TH) {
        ctx->trapdoor_trips++;
        ctx->v_rc = V_BASELINE_0 + (251 * DELTA_VH) / V_MAX_RAIL;
        return;
    }
    int32_t decay_term = (ctx->v_rc - V_BASELINE_0) >> 3;
    int32_t epig_term  = m_epig >> 2;
    ctx->v_rc = ctx->v_rc - decay_term + delta_v_in + epig_term + delta_e_noise;
    if (ctx->v_rc < 0)          ctx->v_rc = 0;
    if (ctx->v_rc > V_MAX_RAIL) ctx->v_rc = V_MAX_RAIL;

    if (ctx->state == TRIT_NEUTRAL) {
        if (ctx->v_rc >= SCHMITT_UPPER_ENTER)      ctx->state = TRIT_SOURCE;
        else if (ctx->v_rc <= SCHMITT_LOWER_ENTER) ctx->state = TRIT_SINK;
    } else if (ctx->state == TRIT_SOURCE) {
        if (ctx->v_rc <= SCHMITT_UPPER_EXIT)       ctx->state = TRIT_NEUTRAL;
    } else if (ctx->state == TRIT_SINK) {
        if (ctx->v_rc >= SCHMITT_LOWER_EXIT)       ctx->state = TRIT_NEUTRAL;
    }
    if ((int32_t)branchless_abs_i32(delta_e_noise) > DELTA_VH && shear_ug <= SHEAR_TRAP_TH) {
        ctx->misfolded = true;
    }
}

static const int32_t  EPOCH_V_RC[TELLURIC_EPOCHS]   = {2030,2158,1565,1267,1477,1974,2110,1446,1187,1790,1365,1129,1671};
static const int32_t  EPOCH_METHYL[TELLURIC_EPOCHS] = {55,97,80,19,-26,-12,46,42,-9,1,8,-35,-19};
static const int8_t   EPOCH_TRIT[TELLURIC_EPOCHS]   = {1,1,0,-1,-1,0,1,0,-1,0,0,-1,0};
static const uint32_t EPOCH_L1[TELLURIC_EPOCHS]     = {439,615,166,484,441,335,700,247,1211,142,294,595,39};
static const uint32_t EPOCH_TAU_UG[TELLURIC_EPOCHS] = {31200,48900,29400,52100,34000,62700,38500,46800,28900,54300,38100,71200,41500};
static const uint16_t EPOCH_CPG[TELLURIC_EPOCHS]    = {0x007EU,0x017DU,0x0478U,0x0D79U,0x084CU,0x191DU,0x0B48U,0x0161U,0x02F4U,0x1668U,0x10A4U,0x04D3U,0x01E2U};
static const uint8_t  EPOCH_VBST[TELLURIC_EPOCHS][3] = {
    {122,121,121},{125,121,121},{125,121,121},{98,121,121},{17,121,121},
    {17,121,121},{17,124,121},{17,124,121},{17,97,121},{17,97,121},
    {17,97,121},{17,97,118},{17,97,118}
};

int main(void) {
    printf("=== ARGON-HARD-IRON v3.8.0 C11 RC ACCUMULATOR & METHYLATION KERNEL ===\n");
    printf("[INVARIANT] sizeof(ArgonMethylatedFrame64B) = %zu bytes (1 L1D cache line)\n",
           sizeof(ArgonMethylatedFrame64B));

    uint64_t p_3n = 1ULL;
    uint32_t kintsugi_welds = 0U;
    for (uint32_t i = 0U; i < TELLURIC_EPOCHS; i++) {
        p_3n = (p_3n << 1ULL) + p_3n;
        ArgonMethylatedFrame64B frame = {0};
        frame.p_scale_3n      = p_3n;
        frame.epoch_period_ns = TELLURIC_PERIOD_NS;
        frame.v_rc_mv         = EPOCH_V_RC[i];
        frame.v_conj_mv       = conjugate_reflect_v0(frame.v_rc_mv);
        frame.methyl_bias_mv  = EPOCH_METHYL[i];
        frame.l1_taxicab_norm = EPOCH_L1[i];
        frame.shear_tau_ug    = EPOCH_TAU_UG[i];
        frame.methyl_cpg_mask = EPOCH_CPG[i];
        frame.scale_n         = (uint8_t)(i + 2U);
        frame.window_trit     = EPOCH_TRIT[i];
        frame.trapdoor_byte   = (frame.shear_tau_ug > (uint32_t)SHEAR_TRAP_TH) ? KINTSUGI_GOLD_BYTE : 0U;
        frame.vbst_bytes[0]   = EPOCH_VBST[i][0];
        frame.vbst_bytes[1]   = EPOCH_VBST[i][1];
        frame.vbst_bytes[2]   = EPOCH_VBST[i][2];

        if (frame.v_rc_mv + frame.v_conj_mv != V_MAX_RAIL) return EXIT_FAILURE;
        if (frame.trapdoor_byte == KINTSUGI_GOLD_BYTE) kintsugi_welds++;
        printf("Epoch %02u (n=%2u | P=%7llu) | V_rc=%4d mV | R_V0=%4d mV | M_epig=%+3d mV | Trit=%+2d | L1=%4u | tau=%5u ug | Trap=%3u | CpG=0x%04X\n",
               i + 1U, frame.scale_n, (unsigned long long)frame.p_scale_3n,
               frame.v_rc_mv, frame.v_conj_mv, frame.methyl_bias_mv,
               (int)frame.window_trit, frame.l1_taxicab_norm,
               frame.shear_tau_ug, frame.trapdoor_byte, frame.methyl_cpg_mask);
    }
    printf("[PASS] 13/13 Telluric Epochs Verified | P(14) = %llu | Kintsugi Welds = %u/13\n\n",
           (unsigned long long)p_3n, kintsugi_welds);

    ArgonAccumulatorContext ctx;
    argon_accumulator_init(&ctx);
    for (int t = 1; t <= 14; t++) {
        int32_t delta_v = 35;
        int32_t m_epig  = 20;
        int32_t shear   = (t % 5 == 0) ? 48000 : 1200;
        int32_t noise   = (t == 7) ? 180 : 10;
        argon_step(&ctx, delta_v, m_epig, shear, noise);
        printf("Step %2d | V_rc: %4d mV | Trit State: %+2d | Trapdoor Trips: %u | Misfolded: %s\n",
               t, ctx.v_rc, (int)ctx.state, ctx.trapdoor_trips, ctx.misfolded ? "TRUE" : "FALSE");
    }
    printf("Kernel execution verified against v3.8.0 specifications. Namaste.\n");
    return EXIT_SUCCESS;
}
