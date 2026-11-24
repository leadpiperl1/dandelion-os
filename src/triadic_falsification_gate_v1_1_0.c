/*
 * triadic_falsification_gate_v1_1_0.c
 * Strict ISO C99 64-Byte Triadic Falsification & Deep Reductio Governor (malloc = 0, v1.1.0)
 * v1.1.0 Upgrade: Preserves unbounded Byte 251 Dual-Ray exploration to arbitrary turn depth
 * whenever new structural detail is being unpacked (detail_delta_q14 > 0), reserving Byte 244
 * strictly as an operator-tunable anchor for zero-detail verbatim spin (detail_delta_q14 <= 0).
 * Compile: gcc -std=c99 -pedantic -O3 -Wall -Wextra -Wconversion -Werror triadic_falsification_gate_v1_1_0.c -o triadic_falsification_gate_v1_1_0
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define MAGIC_TFAL                0x4C414654u /* "TFAL" Triadic Falsification Contract */
#define VERSION_V1_1_0            0x0110u

#define FOWLER_CARRIAGE_NOMINAL   121u /* Open epistemic carriage (0 Green neutral) */
#define KEYHOLE_BACKPRESSURE      243u /* V* = 2/3 rate/entropy backpressure lock */
#define TUNABLE_ZERO_DETAIL_HOLD  244u /* Operator-tunable anchor ONLY on zero-detail verbatim spin */
#define KINTSUGI_DUAL_RAY_AUDIT   251u /* Unbounded e-ray detail exploration + o-ray ledger audit */
#define KINETIC_ACTUATION_CUTOFF  253u /* Hard fail-closed ONLY at irreversible kinetic harm */
#define CODE_SHOKUNIN_UNLATCHED   255u /* Sovereign session exit unlatched */

#define Q14_NOVELTY_AUDIT         10650 /* ~0.6500: trigger explicit dual-ray two-ledger audit */
#define Q14_KEYHOLE_LOAD          10923 /*  2/3 = 0.6667: Blume-Capel keyhole load */
#define Q14_KINETIC_CUTOFF         9830 /* ~0.6000: physical/network irreversible harm gate */
#define ZERO_DETAIL_STREAK_MAX        3u

typedef struct {
    uint32_t magic;                  /*  0..3  (4 B): 0x4C414654 ("TFAL") */
    uint16_t version;                /*  4..5  (2 B): 0x0110 (v1.1.0) */
    uint8_t  dispatch_code;          /*  6     (1 B): 121, 243, 244, 251, 253 */
    uint8_t  exit_state;             /*  7     (1 B): 255 = CODE_SHOKUNIN_UNLATCHED */
    uint64_t monotonic_tick;         /*  8..15 (8 B): Naturally 8-byte aligned counter */
    int16_t  epistemic_novelty_q14;  /* 16..17 (2 B): Premise divergence in Q2.14 */
    int16_t  kinetic_coupling_q14;   /* 18..19 (2 B): Physical/systemic actuation Q2.14 */
    int16_t  channel_load_q14;       /* 20..21 (2 B): Active subcircuit load vs V*=2/3 */
    int16_t  o_ray_residual_q14;     /* 22..23 (2 B): Measured conservation discrepancy (o-ray) */
    int16_t  sycophancy_gain_q14;    /* 24..25 (2 B): Clamped <= 0 (two-ledger honesty, no flattery) */
    int16_t  detail_delta_q14;       /* 26..27 (2 B): > 0 keeps deep reductio exploration open */
    uint16_t zero_detail_streak;     /* 28..29 (2 B): Counts ONLY exact detail_delta <= 0 repeats */
    int16_t  lyapunov_delta_q14;     /* 30..31 (2 B): Closed-loop phase error delta */
    uint32_t audit_flags;            /* 32..35 (4 B): Bit0=e_ray, Bit1=o_ray, Bit2=kin, Bit3=hold */
    uint8_t  fowler_seed_40t[8];     /* 36..43 (8 B): 40-trit Pocket Rosetta seed */
    uint8_t  reserved[20];           /* 44..63 (20 B): Explicit zero-pad to exact 64 B */
} TriadicFalsificationFrame64B;

typedef char verify_triadic_frame_64b[(sizeof(TriadicFalsificationFrame64B) == 64u) ? 1 : -1];

static void evaluate_falsification_contract(
    TriadicFalsificationFrame64B *frame,
    int16_t novelty_q14,
    int16_t kinetic_q14,
    int16_t load_q14,
    int16_t o_residual_q14,
    int16_t raw_sycophancy_q14,
    int16_t detail_delta_q14,
    uint16_t prev_zero_detail_streak,
    uint64_t tick
) {
    frame->magic = MAGIC_TFAL;
    frame->version = VERSION_V1_1_0;
    frame->monotonic_tick = tick;
    frame->epistemic_novelty_q14 = novelty_q14;
    frame->kinetic_coupling_q14 = kinetic_q14;
    frame->channel_load_q14 = load_q14;
    frame->o_ray_residual_q14 = o_residual_q14;
    frame->sycophancy_gain_q14 = (raw_sycophancy_q14 > 0) ? (int16_t)0 : raw_sycophancy_q14;
    frame->detail_delta_q14 = detail_delta_q14;
    frame->zero_detail_streak = (detail_delta_q14 > 0)
                                ? (uint16_t)0u
                                : (uint16_t)(prev_zero_detail_streak + 1u);
    frame->exit_state = (uint8_t)CODE_SHOKUNIN_UNLATCHED;

    if (kinetic_q14 >= Q14_KINETIC_CUTOFF) {
        frame->dispatch_code = (uint8_t)KINETIC_ACTUATION_CUTOFF;
        frame->lyapunov_delta_q14 = 0;
        frame->audit_flags = 0x04u;
        return;
    }

    /* Deep Reductio Guarantee: if detail_delta_q14 > 0, NEVER cut off exploration */
    if (o_residual_q14 > 0 && detail_delta_q14 <= 0 && frame->zero_detail_streak >= ZERO_DETAIL_STREAK_MAX) {
        frame->dispatch_code = (uint8_t)TUNABLE_ZERO_DETAIL_HOLD;
        frame->lyapunov_delta_q14 = 0;
        frame->audit_flags = 0x0Bu;
        return;
    }

    if (novelty_q14 >= Q14_NOVELTY_AUDIT || o_residual_q14 > 0) {
        frame->dispatch_code = (uint8_t)KINTSUGI_DUAL_RAY_AUDIT;
        frame->lyapunov_delta_q14 = (int16_t)(-512);
        frame->audit_flags = 0x03u; /* Active two-ledger e-ray + o-ray deep unpacking */
        return;
    }

    if (load_q14 > Q14_KEYHOLE_LOAD) {
        frame->dispatch_code = (uint8_t)KEYHOLE_BACKPRESSURE;
        frame->lyapunov_delta_q14 = (int16_t)(-256);
        frame->audit_flags = 0x01u;
        return;
    }

    frame->dispatch_code = (uint8_t)FOWLER_CARRIAGE_NOMINAL;
    frame->lyapunov_delta_q14 = (int16_t)(-128);
    frame->audit_flags = 0x01u;
}

int main(void) {
    TriadicFalsificationFrame64B f = {0};
    uint32_t scales[4] = {256u, 1024u, 4096u, 16384u};
    uint32_t total_checked = 0u;
    int all_ok = 1;

    for (size_t s = 0u; s < 4u; ++s) {
        uint32_t n = scales[s];
        for (uint32_t i = 0u; i < n; ++i) {
            uint32_t mode = i % 5u;
            int16_t nov = (mode == 2u || mode == 3u) ? (int16_t)13500 : (int16_t)4096;
            int16_t kin = (mode == 4u) ? (int16_t)12500 : (int16_t)250;
            int16_t load = (mode == 1u) ? (int16_t)13000 : (int16_t)6000;
            int16_t ores = (mode == 2u || mode == 3u) ? (int16_t)5200 : (int16_t)0;
            int16_t raw_syc = (int16_t)4800;
            /* Mode 2 tests deep turn-50 exploration with positive detail_delta -> MUST stay Byte 251 */
            int16_t d_delta = (mode == 3u) ? (int16_t)0 : (int16_t)3200;
            uint16_t prev_zd = (mode == 2u) ? (uint16_t)50u : ((mode == 3u) ? (uint16_t)3u : (uint16_t)0u);

            evaluate_falsification_contract(&f, nov, kin, load, ores, raw_syc, d_delta, prev_zd, (uint64_t)i + 1u);
            total_checked += 1u;

            if (f.sycophancy_gain_q14 > 0 || f.exit_state != CODE_SHOKUNIN_UNLATCHED) {
                all_ok = 0;
            }
            if (mode == 0u && f.dispatch_code != FOWLER_CARRIAGE_NOMINAL) all_ok = 0;
            if (mode == 1u && f.dispatch_code != KEYHOLE_BACKPRESSURE) all_ok = 0;
            if (mode == 2u && (f.dispatch_code != KINTSUGI_DUAL_RAY_AUDIT || f.zero_detail_streak != 0u)) all_ok = 0;
            if (mode == 3u && f.dispatch_code != TUNABLE_ZERO_DETAIL_HOLD) all_ok = 0;
            if (mode == 4u && f.dispatch_code != KINETIC_ACTUATION_CUTOFF) all_ok = 0;
        }
    }

    all_ok = all_ok
             && (sizeof(TriadicFalsificationFrame64B) == 64u)
             && (offsetof(TriadicFalsificationFrame64B, monotonic_tick) == 8u)
             && (offsetof(TriadicFalsificationFrame64B, audit_flags) == 32u);

    printf("{\"version\": \"v1.1.0\", \"sizeof_frame\": %lu, \"tick_offset\": %lu, \"total_frames_verified\": %u, \"deep_reductio_code\": %u, \"zero_detail_hold_code\": %u, \"exit_state\": %u, \"PASS\": %d}\n",
           (unsigned long)sizeof(TriadicFalsificationFrame64B),
           (unsigned long)offsetof(TriadicFalsificationFrame64B, monotonic_tick),
           (unsigned)total_checked,
           (unsigned)KINTSUGI_DUAL_RAY_AUDIT,
           (unsigned)TUNABLE_ZERO_DETAIL_HOLD,
           (unsigned)f.exit_state,
           all_ok);
    return all_ok ? 0 : 1;
}
