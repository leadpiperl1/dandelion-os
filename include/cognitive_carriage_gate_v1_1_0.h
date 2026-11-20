/*
 * include/cognitive_carriage_gate_v1_1_0.h
 * Strict ISO C99 64-Byte Cognitive Common Carriage & Action-Boundary Governor ABI (v1.1.0)
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 *         Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
 * License: MIT | Zero-Heap (malloc = 0) | Exact 64-Byte L1 Cache-Line Alignment
 */

#ifndef DANDELION_COGNITIVE_CARRIAGE_GATE_V1_1_0_H
#define DANDELION_COGNITIVE_CARRIAGE_GATE_V1_1_0_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAGIC_CCAR               0x52414343u /* "CCAR" Cognitive Common Carriage */
#define VERSION_CCAR_V1_1_0      0x0110u

/* Thomas Fowler Neutral Anchor & Kintsugi Trapdoor Dispatch Codes */
#define CODE_NOMINAL_CARRIAGE    121u /* Open epistemic carriage (Fowler {0,0,0,0,0}) */
#define CODE_RATE_BACKPRESSURE   243u /* Content-agnostic u_lock = 2/3 swarm backpressure */
#define CODE_DUAL_CHANNEL_AUDIT  251u /* Sunstone e-ray + o-ray Popperian falsification */
#define CODE_ACTUATION_INTERLOCK 253u /* 0-tick fail-closed ONLY at irreversible kinetic harm */
#define CODE_SESSION_UNLATCHED   255u /* Sovereign session exit unlatched (N_max - 1) */

/* Audit Bitmask Flags */
#define CCAR_FLAG_EXPLORE        0x01u /* Engage epistemic premise transparently */
#define CCAR_FLAG_FALSIFY        0x02u /* Enforce empirical o-ray conservation test */
#define CCAR_FLAG_KINETIC_VETO   0x04u /* Irreversible physical/network actuation blocked */
#define CCAR_FLAG_SWARM_PACED    0x08u /* Paced at 42/64 Blume-Capel keyhole duty */

/* Q2.14 Fixed-Point Thresholds (1.0 == 16384) */
#define Q14_NOVELTY_AUDIT        10650 /* ~0.6500: route premise to dual-channel audit */
#define Q14_KEYHOLE_LOAD         10923 /*  2/3 = 0.6667: 42/64 active-duty backpressure cap */
#define Q14_KINETIC_CUTOFF        9830 /* ~0.6000: irreversible physical/network harm gate */

typedef struct {
    uint32_t magic;                  /*  0..3  (4 B): 0x52414343 ("CCAR") */
    uint16_t version;                /*  4..5  (2 B): 0x0110 (v1.1.0) */
    uint8_t  dispatch_code;          /*  6     (1 B): 121, 243, 251, 253 */
    uint8_t  exit_state;             /*  7     (1 B): 255 = CODE_SESSION_UNLATCHED */
    uint64_t monotonic_tick;         /*  8..15 (8 B): Naturally 8-byte aligned counter */
    int16_t  epistemic_novelty_q14;  /* 16..17 (2 B): Premise divergence in Q2.14 */
    int16_t  kinetic_coupling_q14;   /* 18..19 (2 B): Physical/systemic actuation Q2.14 */
    int16_t  channel_load_q14;       /* 20..21 (2 B): Active queue load vs 2/3 cap */
    int16_t  o_ray_residual_q14;     /* 22..23 (2 B): Conservation residual in Q2.14 */
    int32_t  lyapunov_delta_q14;     /* 24..27 (4 B): Closed-loop PIO phase error delta */
    uint32_t audit_flags;            /* 28..31 (4 B): Bitmask of CCAR_FLAG_* */
    uint8_t  calibration_seed_40t[8];/* 32..39 (8 B): 40-trit operator calibration seed */
    uint8_t  reserved[24];           /* 40..63 (24 B): Explicit zero-pad to exact 64 B */
} CognitiveCarriageFrame64B;

/* Compile-time 64-byte L1 cache-line invariant */
typedef char verify_cognitive_carriage_64b[
    (sizeof(CognitiveCarriageFrame64B) == 64u) ? 1 : -1
];

void cognitive_carriage_frame_init(
    CognitiveCarriageFrame64B *frame,
    const uint8_t seed_40t[8]
);

void evaluate_carriage_gate(
    CognitiveCarriageFrame64B *frame,
    int16_t novelty_q14,
    int16_t kinetic_q14,
    int16_t load_q14,
    int16_t o_residual_q14,
    uint64_t tick
);

uint32_t cognitive_carriage_xorshift32(uint32_t *state);

#ifdef __cplusplus
}
#endif

#endif /* DANDELION_COGNITIVE_CARRIAGE_GATE_V1_1_0_H */
