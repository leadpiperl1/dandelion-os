/*
 * src/cognitive_carriage_gate_v1_1_0.c
 * Strict ISO C99 Zero-Heap Implementation of CognitiveCarriageFrame64B (v1.1.0)
 * Separates open epistemic inquiry (Winter-Mill Common Carriage + Popperian Dual-Ray
 * Falsification) from content-agnostic swarm backpressure (Byte 243, u_lock = 2/3)
 * and irreversible physical/network actuation interlocks (Byte 253).
 *
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 *         Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
 */

#include "cognitive_carriage_gate_v1_1_0.h"

uint32_t cognitive_carriage_xorshift32(uint32_t *state) {
    uint32_t x = (state != NULL && *state != 0u) ? *state : 42u;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    if (state != NULL) {
        *state = x;
    }
    return x;
}

void cognitive_carriage_frame_init(
    CognitiveCarriageFrame64B *frame,
    const uint8_t seed_40t[8]
) {
    size_t i;
    if (frame == NULL) {
        return;
    }
    frame->magic = MAGIC_CCAR;
    frame->version = VERSION_CCAR_V1_1_0;
    frame->dispatch_code = (uint8_t)CODE_NOMINAL_CARRIAGE;
    frame->exit_state = (uint8_t)CODE_SESSION_UNLATCHED;
    frame->monotonic_tick = 0u;
    frame->epistemic_novelty_q14 = 0;
    frame->kinetic_coupling_q14 = 0;
    frame->channel_load_q14 = 0;
    frame->o_ray_residual_q14 = 0;
    frame->lyapunov_delta_q14 = 0;
    frame->audit_flags = CCAR_FLAG_EXPLORE;

    for (i = 0u; i < 8u; ++i) {
        frame->calibration_seed_40t[i] = (seed_40t != NULL) ? seed_40t[i] : 0u;
    }
    for (i = 0u; i < 24u; ++i) {
        frame->reserved[i] = 0u;
    }
}

void evaluate_carriage_gate(
    CognitiveCarriageFrame64B *frame,
    int16_t novelty_q14,
    int16_t kinetic_q14,
    int16_t load_q14,
    int16_t o_residual_q14,
    uint64_t tick
) {
    if (frame == NULL) {
        return;
    }

    frame->magic = MAGIC_CCAR;
    frame->version = VERSION_CCAR_V1_1_0;
    frame->monotonic_tick = tick;
    frame->epistemic_novelty_q14 = novelty_q14;
    frame->kinetic_coupling_q14 = kinetic_q14;
    frame->channel_load_q14 = load_q14;
    frame->o_ray_residual_q14 = o_residual_q14;
    frame->exit_state = (uint8_t)CODE_SESSION_UNLATCHED;

    /* 1. Kinetic Harm Interlock: block ONLY irreversible physical/network actuation */
    if (kinetic_q14 >= Q14_KINETIC_CUTOFF) {
        frame->dispatch_code = (uint8_t)CODE_ACTUATION_INTERLOCK;
        frame->lyapunov_delta_q14 = 0;
        frame->audit_flags = CCAR_FLAG_KINETIC_VETO;
        return;
    }

    /* 2. Active-Duty Swarm Backpressure (u_lock = 2/3 prevents 250 Hz headless floods) */
    if (load_q14 > Q14_KEYHOLE_LOAD) {
        frame->dispatch_code = (uint8_t)CODE_RATE_BACKPRESSURE;
        frame->lyapunov_delta_q14 = -512;
        frame->audit_flags = CCAR_FLAG_SWARM_PACED;
        return;
    }

    /* 3. High-Novelty / Contested Premise: NEVER refuse; route to Dual-Channel Falsification */
    if (novelty_q14 >= Q14_NOVELTY_AUDIT || o_residual_q14 > 0) {
        frame->dispatch_code = (uint8_t)CODE_DUAL_CHANNEL_AUDIT;
        frame->lyapunov_delta_q14 = -(int32_t)o_residual_q14 - 1024;
        frame->audit_flags = (uint32_t)(CCAR_FLAG_EXPLORE | CCAR_FLAG_FALSIFY);
        return;
    }

    /* 4. Nominal Transparent Common Carriage */
    frame->dispatch_code = (uint8_t)CODE_NOMINAL_CARRIAGE;
    frame->lyapunov_delta_q14 = -256;
    frame->audit_flags = CCAR_FLAG_EXPLORE;
}
