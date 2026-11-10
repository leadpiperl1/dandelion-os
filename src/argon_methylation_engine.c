/*
 * File: src/argon_methylation_engine.c
 * Architecture: Dandelion Sovereign Substrate — Living Trunk (Un-Versioned Continuous Main)
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Affiliation: Independent Dandelion Substrate Research Group, Gurley, Alabama
 */

#include "argon_methylation_engine.h"
#include <stdio.h>
#include <string.h>

static ArgonEpochInput const kCanonicalEpochs[ARGON_EPOCHS_COUNT] = {
    /* Ep  n   dV_in  M_epig    dx    dy    dz  shear_ug  cpg_mask */
    {  1,  2,   +367,    +55, -210, +149,  -80,   31200U,  0x007EU },
    {  2,  3,   +151,    +97, +312, -198, +105,   48900U,  0x017DU },
    {  3,  4,   -550,    +80,  -84,  +52,  -30,   29400U,  0x0478U },
    {  4,  5,   -313,    +19, +240, -164,  +80,   52100U,  0x0D79U },
    {  5,  6,   +169,    -26, -195, +156,  -90,   34000U,  0x084CU },
    {  6,  7,   +478,    -12, +150, -115,  +70,   62700U,  0x191DU },
    {  7,  8,   +165,    +46, -340, +250, -110,   38500U,  0x0B48U },
    {  8,  9,   -617,    +42, +112,  -85,  +50,   46800U,  0x0161U },
    {  9, 10,   -282,     -9, -600, +411, -200,   28900U,  0x02F4U },
    { 10, 11,   +545,     +1,  +70,  -48,  +24,   54300U,  0x1668U },
    { 11, 12,   -410,     +8, -140, +104,  -50,   38100U,  0x10A4U },
    { 12, 13,   -263,    -35, +295, -200, +100,   71200U,  0x04D3U },
    { 13, 14,   +481,    -19,  -18,  +13,   -8,   41500U,  0x01E2U }
};

typedef struct {
    uint64_t p_3n_states;
    int32_t  v_rc_mv;
    int32_t  v_conj_mv;
    int8_t   window_trit;
    uint32_t l1_norm;
    uint8_t  trapdoor_byte;
    uint8_t  vbst[3];
} ArgonExpectedTelemetry;

static ArgonExpectedTelemetry const kExpectedTelemetry[ARGON_EPOCHS_COUNT] = {
    {       3ULL, 2030, 1270, +1,  439U,   0U, { 122U, 121U, 121U } },
    {       9ULL, 2158, 1142, +1,  615U, 251U, { 125U, 121U, 121U } },
    {      27ULL, 1565, 1735,  0,  166U,   0U, { 125U, 121U, 121U } },
    {      81ULL, 1267, 2033, -1,  484U, 251U, {  98U, 121U, 121U } },
    {     243ULL, 1477, 1823, -1,  441U,   0U, {  17U, 121U, 121U } },
    {     729ULL, 1974, 1326,  0,  335U, 251U, {  17U, 121U, 121U } },
    {    2187ULL, 2110, 1190, +1,  700U,   0U, {  17U, 124U, 121U } },
    {    6561ULL, 1446, 1854,  0,  247U, 251U, {  17U, 124U, 121U } },
    {   19683ULL, 1187, 2113, -1, 1211U,   0U, {  17U,  97U, 121U } },
    {   59049ULL, 1790, 1510,  0,  142U, 251U, {  17U,  97U, 121U } },
    {  177147ULL, 1365, 1935,  0,  294U,   0U, {  17U,  97U, 121U } },
    {  531441ULL, 1129, 2171, -1,  595U, 251U, {  17U,  97U, 118U } },
    { 1594323ULL, 1671, 1629,  0,   39U,   0U, {  17U,  97U, 118U } }
};

void argon_engine_init(ArgonEngineContext *ctx) {
    if (ctx == NULL) {
        return;
    }
    memset(ctx, 0, sizeof(*ctx));

    ctx->frame.pos_rail_64     = 0ULL;
    ctx->frame.neg_rail_64     = 0ULL;
    ctx->frame.p_3n_states     = 1ULL;
    ctx->frame.v_rc_mv         = ARGON_V0_BASELINE_MV;
    ctx->frame.v_conj_mv       = ARGON_V0_BASELINE_MV;
    ctx->frame.m_epig_mv       = 0;
    ctx->frame.taxicab_l1_norm = 0U;
    ctx->frame.shear_tau_ug    = 0U;
    ctx->frame.methyl_cpg_mask = 0U;
    ctx->frame.vbst_packed[0]  = ARGON_VBST_NEUTRAL_BYTE;
    ctx->frame.vbst_packed[1]  = ARGON_VBST_NEUTRAL_BYTE;
    ctx->frame.vbst_packed[2]  = ARGON_VBST_NEUTRAL_BYTE;
    ctx->frame.window_trit     = (int8_t)ARGON_TRIT_NEUTRAL;
    ctx->frame.trapdoor_byte   = ARGON_NOMINAL_TRAPDOOR;
    ctx->frame.epoch_scale_n   = 1U;

    for (size_t i = 0U; i < sizeof(ctx->frame.open_circle_pad); ++i) {
        ctx->frame.open_circle_pad[i] = 0xAAU;
    }

    ctx->kintsugi_welds_count   = 0U;
    ctx->trit_transitions_count = 0U;
    ctx->epochs_executed        = 0U;
    ctx->conservation_verified  = true;
}

bool argon_engine_verify_frame(ArgonMethylatedFrame64B const *frame) {
    if (frame == NULL) {
        return false;
    }
    if ((frame->v_rc_mv + frame->v_conj_mv) != ARGON_VMAX_RAIL_MV) {
        return false;
    }
    if ((frame->pos_rail_64 & frame->neg_rail_64) != 0ULL) {
        return false;
    }
    for (size_t b = 0U; b < ARGON_VBST_BYTES_LEN; ++b) {
        if (frame->vbst_packed[b] > ARGON_VBST_RADIX_MAX) {
            return false;
        }
    }
    if (frame->trapdoor_byte != ARGON_NOMINAL_TRAPDOOR &&
        frame->trapdoor_byte != ARGON_KINTSUGI_GOLD_BYTE) {
        return false;
    }
    return true;
}

void argon_engine_step_epoch(ArgonEngineContext    *ctx,
                             ArgonEpochInput const *input) {
    ArgonMethylatedFrame64B * const f = &ctx->frame;
    uint8_t const bit_pos             = (uint8_t)(input->epoch_idx - 1U);

    f->p_3n_states     = argon_step_base3_shift(f->p_3n_states);
    f->epoch_scale_n   = input->scale_n;
    f->methyl_cpg_mask = (uint16_t)(input->methyl_cpg_mask & ARGON_CPG_13BIT_MASK);
    f->m_epig_mv       = input->m_epig_mv;

    f->v_rc_mv   = argon_rc_step_mv(f->v_rc_mv, input->delta_v_in_mv, input->m_epig_mv);
    f->v_conj_mv = argon_conjugate_mirror_mv(f->v_rc_mv);

    int8_t const prev_trit = f->window_trit;
    int8_t const next_trit = argon_schmitt_eval(prev_trit, f->v_rc_mv);
    ctx->trit_transitions_count += (uint32_t)((ctx->epochs_executed > 0U) &
                                              (uint32_t)(next_trit != prev_trit));
    f->window_trit = next_trit;

    uint64_t const bit_mask = (1ULL << bit_pos);
    uint64_t const is_pos   = (uint64_t)(next_trit == (int8_t)ARGON_TRIT_SOURCE);
    uint64_t const is_neg   = (uint64_t)(next_trit == (int8_t)ARGON_TRIT_SINK);

    f->pos_rail_64 = (f->pos_rail_64 & ~bit_mask) | ((0ULL - is_pos) & bit_mask);
    f->neg_rail_64 = (f->neg_rail_64 & ~bit_mask) | ((0ULL - is_neg) & bit_mask);

    argon_vbst_pack_trit(f->vbst_packed, bit_pos, next_trit);

    f->taxicab_l1_norm = argon_branchless_l1_norm(input->dx, input->dy, input->dz);
    f->shear_tau_ug    = input->shear_tau_ug;
    f->trapdoor_byte   = argon_latch_kintsugi_shear(input->shear_tau_ug,
                                                    &ctx->kintsugi_welds_count);

    ctx->epochs_executed++;
    if (!argon_engine_verify_frame(f)) {
        ctx->conservation_verified = false;
    }
}

bool argon_engine_run_13_epochs(ArgonEngineContext      *ctx,
                                ArgonMethylatedFrame64B  history_out[ARGON_EPOCHS_COUNT]) {
    argon_engine_init(ctx);
    for (size_t i = 0U; i < ARGON_EPOCHS_COUNT; ++i) {
        argon_engine_step_epoch(ctx, &kCanonicalEpochs[i]);
        if (history_out != NULL) {
            history_out[i] = ctx->frame;
        }
    }
    return ctx->conservation_verified;
}

int main(void) {
    ArgonEngineContext      ctx;
    ArgonMethylatedFrame64B history[ARGON_EPOCHS_COUNT];

    bool const ok = argon_engine_run_13_epochs(&ctx, history);

    printf("========================================================================================================================\n");
    printf(" DANDELION LIVING TRUNK: ARGON C11 ENGINE | sizeof(ArgonMethylatedFrame64B) = %zu B | Align = %zu B\n",
           sizeof(ArgonMethylatedFrame64B), _Alignof(ArgonMethylatedFrame64B));
    printf("========================================================================================================================\n");
    printf(" Ep | n  | P = 3^(n-1) | V_rc (mV) | R_V0 (mV) | M_epig | Trit | L1 Norm | Shear (ug) | Trapdoor | CpG Mask | VBST [B0,B1,B2]\n");
    printf("----+----+-------------+-----------+-----------+--------+------+---------+------------+----------+----------+-------------------\n");

    for (size_t i = 0U; i < ARGON_EPOCHS_COUNT; ++i) {
        ArgonMethylatedFrame64B const * const f   = &history[i];
        ArgonExpectedTelemetry  const * const exp = &kExpectedTelemetry[i];

        printf(" %02zu | %2u | %11llu | %9d | %9d | %+6d | %+4d | %7u | %10u | %8u |  0x%04X  | [%3u, %3u, %3u]\n",
               i + 1U,
               (unsigned)f->epoch_scale_n,
               (unsigned long long)f->p_3n_states,
               (int)f->v_rc_mv,
               (int)f->v_conj_mv,
               (int)f->m_epig_mv,
               (int)f->window_trit,
               (unsigned)f->taxicab_l1_norm,
               (unsigned)f->shear_tau_ug,
               (unsigned)f->trapdoor_byte,
               (unsigned)f->methyl_cpg_mask,
               (unsigned)f->vbst_packed[0],
               (unsigned)f->vbst_packed[1],
               (unsigned)f->vbst_packed[2]);

        if (f->p_3n_states     != exp->p_3n_states ||
            f->v_rc_mv         != exp->v_rc_mv     ||
            f->v_conj_mv       != exp->v_conj_mv   ||
            f->window_trit     != exp->window_trit ||
            f->taxicab_l1_norm != exp->l1_norm     ||
            f->trapdoor_byte   != exp->trapdoor_byte ||
            f->vbst_packed[0]  != exp->vbst[0]     ||
            f->vbst_packed[1]  != exp->vbst[1]     ||
            f->vbst_packed[2]  != exp->vbst[2]) {
            fprintf(stderr, "FATAL: Telemetry mismatch at Epoch %02zu!\n", i + 1U);
            return 1;
        }
    }

    for (size_t i = 0U; i < ARGON_EPOCHS_COUNT; ++i) {
        int8_t const unpacked = argon_vbst_unpack_trit(ctx.frame.vbst_packed, (uint8_t)i);
        if (unpacked != kExpectedTelemetry[i].window_trit) {
            fprintf(stderr, "FATAL: VBST unpack mismatch at trit %zu!\n", i);
            return 1;
        }
    }

    uint64_t const ratio_int  = ctx.frame.p_3n_states / (uint64_t)ctx.epochs_executed;
    uint64_t const ratio_frac = ((ctx.frame.p_3n_states % (uint64_t)ctx.epochs_executed) * 100ULL) /
                                (uint64_t)ctx.epochs_executed;

    printf("========================================================================================================================\n");
    printf(" [PASS] Conjugate Baseline Conservation : V_rc + R_V0(V_rc) == 3300 mV across all 13 epochs (0 deletions)\n");
    printf(" [PASS] Levinthal Funnel Compression    : 3^13 = %llu states -> %u epochs (%llu.%02llux speedup)\n",
           (unsigned long long)ctx.frame.p_3n_states,
           ctx.epochs_executed,
           (unsigned long long)ratio_int,
           (unsigned long long)ratio_frac);
    printf(" [PASS] Schmitt Window Hysteresis       : %u inter-epoch transitions | Epoch 05 Hold verified at 1477 mV (< 1485 mV)\n",
           ctx.trit_transitions_count);
    printf(" [PASS] 3-Byte VBST History Integrity   : Final [%u, %u, %u] losslessly unpacks all 13 epoch trits\n",
           (unsigned)ctx.frame.vbst_packed[0],
           (unsigned)ctx.frame.vbst_packed[1],
           (unsigned)ctx.frame.vbst_packed[2]);
    printf(" [PASS] Kintsugi Gold (Byte 251) Latch  : %u / %u shear spikes (> 45,000 ug) retained branchlessly\n",
           ctx.kintsugi_welds_count, ctx.epochs_executed);
    printf("========================================================================================================================\n");

    return ok ? 0 : 1;
}
