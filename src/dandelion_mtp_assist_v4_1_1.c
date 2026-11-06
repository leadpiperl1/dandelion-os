/*
 * ============================================================================
 * FILE: dandelion_mtp_assist_v4_1_1.c
 * PROJECT: Dandelion Sovereign Protocol v4.1.1 — Assistive MTP & Tensegrity Solver
 * AUTHOR: Justin D. Grimm
 * AFFILIATION: Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
 * ORCID: 0009-0000-5299-0243
 * LICENSE: GPL-2.0-or-later
 * ============================================================================
 */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DANDELION_ASSIST_VERSION      "v4.1.1"
#define KRAMHAUS_TORQUE_LIMIT_UNM     49426U
#define MAX_SHEAR_MDEG_LIMIT          10000U
#define MTP_LATENCY_CEILING_NS        50000U
#define DIDT_CEILING_MA_PER_US        25000U
#define BARKER13_LEN                  13U
#define BARKER13_LOCK_THRESHOLD       9
#define CODE_CLEAN                    0U
#define CODE_KINTSUGI_GOLD_WELD       251U

/* Fixed-point biomechanical constants (scaled x100) */
#define TORSO_WEIGHT_CN               52974U   /* 529.74 N (54.0 kg torso)   */
#define EULER_CRIT_STRUT_CN           138980U  /* 1389.80 N critical buckling*/
#define COS_TET_PROJ_PERMILLE         816U     /* cos(35.264 deg) = 0.8165   */

static const int8_t BARKER13_KERNEL[BARKER13_LEN] = {
    1, 1, 1, 1, 1, -1, -1, 1, 1, -1, 1, -1, 1
};

typedef struct {
    uint32_t tau_unmitigated_unm;
    uint32_t tau_net_governed_unm;
    uint32_t max_shear_mdeg;
    uint32_t sling_unloaded_cn;
    uint32_t residual_disc_load_cn;
    uint32_t single_tendon_tension_cn;
    uint32_t strut_buckling_sf_x100;
    uint8_t  kintsugi_weld_code;
    uint8_t  chair44_legal_orient;
} scx_kramhaus_tensegrity_v411_t;

typedef struct {
    int32_t  haar_lowpass_intent;
    int32_t  haar_highpass_tremor;
    int32_t  barker13_corr_score;
    uint32_t sparsity_permille;
    uint32_t didt_ma_per_us;
    uint32_t mtp_latency_ns;
    uint8_t  intent_latched;
} mtp_assist_frame_t;

void dandelion_mtp_filter_step(
    const int16_t raw_samples[BARKER13_LEN],
    const int8_t  sparse_trits[64],
    mtp_assist_frame_t *out_frame)
{
    int32_t low_acc = 0;
    int32_t high_acc = 0;
    int32_t barker_dot = 0;

    for (uint32_t i = 0U; i < 12U; i += 2U) {
        int32_t s0 = (int32_t)raw_samples[i];
        int32_t s1 = (int32_t)raw_samples[i + 1U];
        low_acc  += (s0 + s1);
        int32_t diff = s0 - s1;
        high_acc += (diff < 0) ? -diff : diff;
    }

    for (uint32_t k = 0U; k < BARKER13_LEN; ++k) {
        int8_t sgn = (raw_samples[k] > 0) ? 1 : ((raw_samples[k] < 0) ? -1 : 0);
        barker_dot += (int32_t)sgn * (int32_t)BARKER13_KERNEL[k];
    }

    uint32_t zero_count = 0U;
    uint32_t active_transitions = 0U;
    int8_t prev_trit = 0;
    for (uint32_t j = 0U; j < 64U; ++j) {
        int8_t t = sparse_trits[j];
        if (t == 0) {
            zero_count++;
        } else if (t != prev_trit) {
            active_transitions++;
        }
        prev_trit = t;
    }

    out_frame->haar_lowpass_intent  = low_acc / 6;
    out_frame->haar_highpass_tremor = high_acc / 6;
    out_frame->barker13_corr_score  = barker_dot;
    out_frame->sparsity_permille    = (zero_count * 1000U) / 64U;
    out_frame->didt_ma_per_us       = 3200U + (active_transitions * 266U);
    out_frame->mtp_latency_ns       = 35900U;

    if (barker_dot >= BARKER13_LOCK_THRESHOLD &&
        out_frame->haar_lowpass_intent > out_frame->haar_highpass_tremor) {
        out_frame->intent_latched = 1U;
    } else {
        out_frame->intent_latched = 0U;
    }
}

void dandelion_kramhaus_tensegrity_step_v411(
    uint32_t raw_torque_unm,
    uint32_t raw_shear_mdeg,
    uint32_t state_index,
    scx_kramhaus_tensegrity_v411_t *out_gov)
{
    out_gov->tau_unmitigated_unm  = raw_torque_unm;
    out_gov->chair44_legal_orient = (uint8_t)((state_index % 42U) % 30U);

    if (raw_torque_unm > KRAMHAUS_TORQUE_LIMIT_UNM ||
        raw_shear_mdeg > MAX_SHEAR_MDEG_LIMIT) {
        out_gov->tau_net_governed_unm     = (raw_torque_unm * 5001U) / 10000U;
        out_gov->max_shear_mdeg           = 1500U;
        out_gov->sling_unloaded_cn        = (TORSO_WEIGHT_CN * 34U) / 51U;
        out_gov->residual_disc_load_cn    = TORSO_WEIGHT_CN - out_gov->sling_unloaded_cn;
        out_gov->single_tendon_tension_cn =
            (out_gov->sling_unloaded_cn * 1000U) / (3U * COS_TET_PROJ_PERMILLE);
        uint32_t strut_load_cn            = out_gov->single_tendon_tension_cn * 2U;
        out_gov->strut_buckling_sf_x100   = (EULER_CRIT_STRUT_CN * 100U) / strut_load_cn;
        out_gov->kintsugi_weld_code       = CODE_KINTSUGI_GOLD_WELD;
    } else {
        out_gov->tau_net_governed_unm     = raw_torque_unm;
        out_gov->max_shear_mdeg           = raw_shear_mdeg;
        out_gov->sling_unloaded_cn        = (TORSO_WEIGHT_CN * 17U) / 51U;
        out_gov->residual_disc_load_cn    = TORSO_WEIGHT_CN - out_gov->sling_unloaded_cn;
        out_gov->single_tendon_tension_cn =
            (out_gov->sling_unloaded_cn * 1000U) / (3U * COS_TET_PROJ_PERMILLE);
        uint32_t strut_load_cn            = out_gov->single_tendon_tension_cn * 2U;
        out_gov->strut_buckling_sf_x100   = (EULER_CRIT_STRUT_CN * 100U) / strut_load_cn;
        out_gov->kintsugi_weld_code       = CODE_CLEAN;
    }
}
