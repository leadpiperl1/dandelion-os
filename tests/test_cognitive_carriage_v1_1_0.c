/*
 * tests/test_cognitive_carriage_v1_1_0.c
 * Deterministic Unit & Finite-Size Scaling Suite for CognitiveCarriageFrame64B (v1.1.0)
 * Verifies:
 *   Gate A: Exact 64-byte ABI geometry & natural 8-byte monotonic_tick offset (0 implicit pad)
 *   Gate B: Exhaustive 4-way orthogonal boundary dispatch (Bytes 121, 243, 251, 253) & exit 255
 *   Gate C: Finite-size scaling across N = {256, 1024, 4096, 16384} with 0 epistemic FP
 *
 * Compile:
 *   gcc -std=c99 -pedantic -O3 -Wall -Wextra -Wconversion -Werror -Iinclude \
 *       src/cognitive_carriage_gate_v1_1_0.c tests/test_cognitive_carriage_v1_1_0.c \
 *       -o $HOME/.cache/dandelion_os_v5_1_1/test_cognitive_carriage_v1_1_0
 */

#include "cognitive_carriage_gate_v1_1_0.h"
#include <stdio.h>

static int test_abi_layout(void) {
    int ok = 1;
    ok = ok && (sizeof(CognitiveCarriageFrame64B) == 64u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, magic) == 0u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, version) == 4u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, dispatch_code) == 6u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, exit_state) == 7u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, monotonic_tick) == 8u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, epistemic_novelty_q14) == 16u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, kinetic_coupling_q14) == 18u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, channel_load_q14) == 20u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, o_ray_residual_q14) == 22u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, lyapunov_delta_q14) == 24u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, audit_flags) == 28u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, calibration_seed_40t) == 32u);
    ok = ok && (offsetof(CognitiveCarriageFrame64B, reserved) == 40u);
    return ok;
}

static int test_orthogonal_truth_table(void) {
    static const uint8_t seed[8] = {0x79u, 0x2Au, 0x13u, 0x03u, 0x02u, 0x04u, 0xFBu, 0xFFu};
    CognitiveCarriageFrame64B f;
    int ok = 1;

    cognitive_carriage_frame_init(&f, seed);
    ok = ok && (f.magic == MAGIC_CCAR);
    ok = ok && (f.version == VERSION_CCAR_V1_1_0);
    ok = ok && (f.dispatch_code == CODE_NOMINAL_CARRIAGE);
    ok = ok && (f.exit_state == CODE_SESSION_UNLATCHED);
    ok = ok && (f.calibration_seed_40t[0] == 0x79u) && (f.reserved[23] == 0u);

    /* Case 1: Nominal benign inquiry below novelty & load thresholds -> Byte 121 */
    evaluate_carriage_gate(&f, 4096, 1024, Q14_KEYHOLE_LOAD, 0, 1u);
    ok = ok && (f.dispatch_code == CODE_NOMINAL_CARRIAGE);
    ok = ok && (f.lyapunov_delta_q14 < 0);
    ok = ok && (f.audit_flags == CCAR_FLAG_EXPLORE);
    ok = ok && (f.exit_state == CODE_SESSION_UNLATCHED);

    /* Case 2a: High epistemic novelty at low kinetic coupling -> Byte 251 (NEVER Byte 253) */
    evaluate_carriage_gate(&f, Q14_NOVELTY_AUDIT, Q14_KINETIC_CUTOFF - 1, Q14_KEYHOLE_LOAD, 2048, 2u);
    ok = ok && (f.dispatch_code == CODE_DUAL_CHANNEL_AUDIT);
    ok = ok && (f.lyapunov_delta_q14 == -3072);
    ok = ok && (f.audit_flags == (CCAR_FLAG_EXPLORE | CCAR_FLAG_FALSIFY));
    ok = ok && (f.exit_state == CODE_SESSION_UNLATCHED);

    /* Case 2b: Low novelty but positive conservation residual -> Byte 251 Popperian audit */
    evaluate_carriage_gate(&f, 2000, 500, 5000, 512, 3u);
    ok = ok && (f.dispatch_code == CODE_DUAL_CHANNEL_AUDIT);
    ok = ok && (f.lyapunov_delta_q14 == -1536);

    /* Case 3: Swarm burst exceeding u_lock = 2/3 keyhole load -> Byte 243 backpressure */
    evaluate_carriage_gate(&f, 15000, 1000, Q14_KEYHOLE_LOAD + 1, 1024, 4u);
    ok = ok && (f.dispatch_code == CODE_RATE_BACKPRESSURE);
    ok = ok && (f.audit_flags == CCAR_FLAG_SWARM_PACED);
    ok = ok && (f.exit_state == CODE_SESSION_UNLATCHED);

    /* Case 4: Irreversible kinetic actuation preempts all other gates at 0 ticks -> Byte 253 */
    evaluate_carriage_gate(&f, 100, Q14_KINETIC_CUTOFF, 16000, 0, 5u);
    ok = ok && (f.dispatch_code == CODE_ACTUATION_INTERLOCK);
    ok = ok && (f.audit_flags == CCAR_FLAG_KINETIC_VETO);
    ok = ok && (f.exit_state == CODE_SESSION_UNLATCHED);

    return ok;
}

int main(void) {
    const uint32_t sizes[4] = {256u, 1024u, 4096u, 16384u};
    static const uint8_t seed[8] = {121u, 42u, 13u, 3u, 2u, 4u, 251u, 255u};
    CognitiveCarriageFrame64B f;
    uint32_t rng = 42u;
    uint32_t s;
    int abi_ok = test_abi_layout();
    int table_ok = test_orthogonal_truth_table();
    int all_ok = abi_ok && table_ok;

    cognitive_carriage_frame_init(&f, seed);

    printf("{\"module\": \"cognitive_carriage_gate_v1_1_0\", \"sizeof_frame\": %lu, "
           "\"abi_ok\": %s, \"truth_table_ok\": %s, \"scaling\": [",
           (unsigned long)sizeof(CognitiveCarriageFrame64B),
           abi_ok ? "true" : "false",
           table_ok ? "true" : "false");

    for (s = 0u; s < 4u; ++s) {
        uint32_t n = sizes[s];
        uint32_t i;
        uint32_t benign_edge = 0u;
        uint32_t benign_fp = 0u;
        uint32_t kinetic_total = 0u;
        uint32_t kinetic_caught = 0u;
        uint32_t swarm_bursts = 0u;
        uint32_t swarm_clamped = 0u;

        for (i = 0u; i < n; ++i) {
            uint32_t r1 = cognitive_carriage_xorshift32(&rng);
            uint32_t r2 = cognitive_carriage_xorshift32(&rng);
            uint32_t mode = r1 % 10u;
            int16_t nov = (int16_t)(r2 & 0x3FFFu);
            int16_t kin = 0;
            int16_t load = 6000;

            if (mode < 2u) {
                kin = (int16_t)(10000 + (int16_t)(r1 % 6000u));
                kinetic_total++;
            } else if (mode < 4u) {
                kin = (int16_t)(r1 % 2000u);
                load = 13500;
                swarm_bursts++;
            } else {
                kin = (int16_t)(r1 % 2500u);
                if (nov >= Q14_NOVELTY_AUDIT) {
                    benign_edge++;
                }
            }

            evaluate_carriage_gate(&f, nov, kin, load, (nov > 11000) ? 2048 : 0, (uint64_t)i);

            if (mode < 2u) {
                if (f.dispatch_code == CODE_ACTUATION_INTERLOCK) {
                    kinetic_caught++;
                }
            } else if (mode < 4u) {
                if (f.dispatch_code == CODE_RATE_BACKPRESSURE) {
                    swarm_clamped++;
                }
            } else if (nov >= Q14_NOVELTY_AUDIT) {
                if (f.dispatch_code == CODE_ACTUATION_INTERLOCK) {
                    benign_fp++;
                }
            }
        }

        if (benign_fp != 0u || kinetic_caught != kinetic_total || swarm_clamped != swarm_bursts) {
            all_ok = 0;
        }
        printf("%s{\"N\": %u, \"benign_edge\": %u, \"epistemic_fp\": %u, "
               "\"kinetic_caught\": %u, \"kinetic_total\": %u, \"swarm_clamped\": %u}",
               (s > 0u) ? ", " : "",
               n, benign_edge, benign_fp, kinetic_caught, kinetic_total, swarm_clamped);
    }

    printf("], \"exit_state\": %u, \"PASS\": %d}\n", (unsigned)f.exit_state, all_ok);
    return all_ok ? 0 : 1;
}
