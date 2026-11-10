/*
 * File: include/argon_methylation_engine.h
 * Architecture: Dandelion Sovereign Substrate — Living Trunk (Un-Versioned Continuous Main)
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Affiliation: Independent Dandelion Substrate Research Group, Gurley, Alabama
 * Target Host: Professor Plum (custom-fedora-garden-rt, PREEMPT_RT, 104.0 Hz = 9,615,384 ns)
 */

#ifndef ARGON_METHYLATION_ENGINE_H
#define ARGON_METHYLATION_ENGINE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ARGON_V0_BASELINE_MV       1650   /* Active metabolic resting midpoint (mV) */
#define ARGON_VMAX_RAIL_MV         3300   /* Full-scale rail supply ceiling (mV)    */
#define ARGON_DELTA_VH_MV           165   /* Schmitt hysteresis half-width (mV)     */

#define ARGON_UPPER_ENTER_MV       1980   /* V_0 + 2 * Delta_V_H (+1 Source Enter)  */
#define ARGON_UPPER_EXIT_MV        1815   /* V_0 + 1 * Delta_V_H (+1 Source Exit)   */
#define ARGON_LOWER_ENTER_MV       1320   /* V_0 - 2 * Delta_V_H (-1 Sink Enter)    */
#define ARGON_LOWER_EXIT_MV        1485   /* V_0 - 1 * Delta_V_H (-1 Sink Exit)     */

#define ARGON_RC_SHIFT_K              3   /* Decay factor 2^-3 = 1/8 (gamma = 7/8)  */
#define ARGON_EPIG_SHIFT              2   /* Epigenetic bias scaling 2^-2 = 1/4     */

#define ARGON_VBST_RADIX_MAX       242U   /* 3^5 - 1 = 242 maximum packed trit byte */
#define ARGON_VBST_NEUTRAL_BYTE    121U   /* 0x79: sum_{j=0..4} 1 * 3^j (all 0s)    */
#define ARGON_VBST_BYTES_LEN         3U   /* 3 bytes = 15 trits (13 active + 2 pad) */

#define ARGON_SHEAR_TH_UG        45000U   /* Unsmoothed shear threshold (micro-g)   */
#define ARGON_KINTSUGI_GOLD_BYTE   251U   /* 0xFB: Structural weld trapdoor byte    */
#define ARGON_NOMINAL_TRAPDOOR       0U   /* 0x00: Nominal un-tripped trapdoor      */
#define ARGON_CPG_13BIT_MASK    0x1FFFU   /* 13-bit chiral CpG mask (bits 0..12)    */

#define ARGON_EPOCHS_COUNT          13U   /* 13 deterministic folding epochs        */
#define ARGON_SCALE_N_MIN            2U   /* Initial simplicial scale n = 2         */
#define ARGON_SCALE_N_MAX           14U   /* Terminal simplicial scale n = 14       */
#define ARGON_LEVINTHAL_3_13  1594323ULL  /* 3^13 unmethylated conformations        */

#define ARGON_TELLURIC_PERIOD_NS 9615384ULL
#define ARGON_SOLITON_SLIP_NS    1800000ULL

typedef enum {
    ARGON_TRIT_SINK    = -1,
    ARGON_TRIT_NEUTRAL =  0,
    ARGON_TRIT_SOURCE  =  1
} ArgonTritState;

typedef struct __attribute__((aligned(64))) {
    uint64_t pos_rail_64;        /*  8 B | Offset  0: Dual-rail +1 Proton Source bitmask      */
    uint64_t neg_rail_64;        /*  8 B | Offset  8: Dual-rail -1 Electron Sink bitmask      */
    uint64_t p_3n_states;        /*  8 B | Offset 16: P = 3^(n-1) bit-shifted state count     */
    int32_t  v_rc_mv;            /*  4 B | Offset 24: Active RC accumulator potential (mV)    */
    int32_t  v_conj_mv;          /*  4 B | Offset 28: Conjugate mirror R_V0(V_rc) = 3300-V_rc */
    int32_t  m_epig_mv;          /*  4 B | Offset 32: Rolling epigenetic CpG hysteresis (mV)  */
    uint32_t taxicab_l1_norm;    /*  4 B | Offset 36: Branchless Manhattan L1 norm d_1(p,q)   */
    uint32_t shear_tau_ug;       /*  4 B | Offset 40: Unsmoothed shear impulse tau (micro-g)  */
    uint16_t methyl_cpg_mask;    /*  2 B | Offset 44: 13-bit chiral CpG methylation mask      */
    uint8_t  vbst_packed[3];     /*  3 B | Offset 46: 13-trit VBST (5 trits/byte, 0..242)     */
    int8_t   window_trit;        /*  1 B | Offset 49: Schmitt comparator state {-1, 0, +1}    */
    uint8_t  trapdoor_byte;      /*  1 B | Offset 50: 0 = Nominal, 251 = Kintsugi Gold Weld   */
    uint8_t  epoch_scale_n;      /*  1 B | Offset 51: Simplicial scale n in [2..14]           */
    uint8_t  open_circle_pad[12];/* 12 B | Offset 52: 1/3 Open-Circle thermodynamic reserve   */
} ArgonMethylatedFrame64B;

_Static_assert(sizeof(ArgonMethylatedFrame64B) == 64,
               "ArgonMethylatedFrame64B must occupy exactly 1 L1D cache line (64 bytes)");
_Static_assert(_Alignof(ArgonMethylatedFrame64B) == 64,
               "ArgonMethylatedFrame64B must be 64-byte aligned");
_Static_assert(offsetof(ArgonMethylatedFrame64B, pos_rail_64)     ==  0, "Offset mismatch: pos_rail_64");
_Static_assert(offsetof(ArgonMethylatedFrame64B, neg_rail_64)     ==  8, "Offset mismatch: neg_rail_64");
_Static_assert(offsetof(ArgonMethylatedFrame64B, p_3n_states)     == 16, "Offset mismatch: p_3n_states");
_Static_assert(offsetof(ArgonMethylatedFrame64B, v_rc_mv)         == 24, "Offset mismatch: v_rc_mv");
_Static_assert(offsetof(ArgonMethylatedFrame64B, v_conj_mv)       == 28, "Offset mismatch: v_conj_mv");
_Static_assert(offsetof(ArgonMethylatedFrame64B, m_epig_mv)       == 32, "Offset mismatch: m_epig_mv");
_Static_assert(offsetof(ArgonMethylatedFrame64B, taxicab_l1_norm) == 36, "Offset mismatch: taxicab_l1_norm");
_Static_assert(offsetof(ArgonMethylatedFrame64B, shear_tau_ug)    == 40, "Offset mismatch: shear_tau_ug");
_Static_assert(offsetof(ArgonMethylatedFrame64B, methyl_cpg_mask) == 44, "Offset mismatch: methyl_cpg_mask");
_Static_assert(offsetof(ArgonMethylatedFrame64B, vbst_packed)     == 46, "Offset mismatch: vbst_packed");
_Static_assert(offsetof(ArgonMethylatedFrame64B, window_trit)     == 49, "Offset mismatch: window_trit");
_Static_assert(offsetof(ArgonMethylatedFrame64B, trapdoor_byte)   == 50, "Offset mismatch: trapdoor_byte");
_Static_assert(offsetof(ArgonMethylatedFrame64B, epoch_scale_n)   == 51, "Offset mismatch: epoch_scale_n");
_Static_assert(offsetof(ArgonMethylatedFrame64B, open_circle_pad) == 52, "Offset mismatch: open_circle_pad");

typedef struct {
    uint8_t  epoch_idx;
    uint8_t  scale_n;
    int32_t  delta_v_in_mv;
    int32_t  m_epig_mv;
    int32_t  dx;
    int32_t  dy;
    int32_t  dz;
    uint32_t shear_tau_ug;
    uint16_t methyl_cpg_mask;
} ArgonEpochInput;

typedef struct {
    ArgonMethylatedFrame64B frame;
    uint32_t kintsugi_welds_count;
    uint32_t trit_transitions_count;
    uint32_t epochs_executed;
    bool     conservation_verified;
} ArgonEngineContext;

static inline uint32_t argon_branchless_abs_i32(int32_t dx) {
    int32_t const mask = dx >> 31;
    return (uint32_t)((dx + mask) ^ mask);
}

static inline uint32_t argon_branchless_l1_norm(int32_t dx, int32_t dy, int32_t dz) {
    return argon_branchless_abs_i32(dx)
         + argon_branchless_abs_i32(dy)
         + argon_branchless_abs_i32(dz);
}

static inline uint64_t argon_step_base3_shift(uint64_t p_prev) {
    return (p_prev << 1U) + p_prev;
}

static inline int32_t argon_conjugate_mirror_mv(int32_t v_rc_mv) {
    return (ARGON_V0_BASELINE_MV << 1) - v_rc_mv;
}

static inline int32_t argon_clamp_rail_mv(int32_t v_mv) {
    int32_t const low_clamped = v_mv & ~(v_mv >> 31);
    int32_t const diff_high   = ARGON_VMAX_RAIL_MV - low_clamped;
    return low_clamped + (diff_high & (diff_high >> 31));
}

static inline int32_t argon_rc_step_mv(int32_t v_rc_prev_mv,
                                       int32_t delta_v_in_mv,
                                       int32_t m_epig_mv) {
    int32_t const decay_term = (v_rc_prev_mv - ARGON_V0_BASELINE_MV) >> ARGON_RC_SHIFT_K;
    int32_t const epig_term  = m_epig_mv >> ARGON_EPIG_SHIFT;
    int32_t const v_next     = v_rc_prev_mv - decay_term + delta_v_in_mv + epig_term;
    return argon_clamp_rail_mv(v_next);
}

static inline int8_t argon_schmitt_eval(int8_t prev_trit, int32_t v_rc_mv) {
    int32_t const from_zero = (int32_t)(prev_trit == 0) *
                              ((int32_t)(v_rc_mv >= ARGON_UPPER_ENTER_MV) -
                               (int32_t)(v_rc_mv <= ARGON_LOWER_ENTER_MV));
    int32_t const from_pos  = (int32_t)(prev_trit == 1) *
                              (int32_t)(v_rc_mv > ARGON_UPPER_EXIT_MV);
    int32_t const from_neg  = (int32_t)(prev_trit == -1) *
                              (-(int32_t)(v_rc_mv < ARGON_LOWER_EXIT_MV));
    return (int8_t)(from_zero + from_pos + from_neg);
}

static inline uint8_t argon_pow3_u8(uint8_t j) {
    static uint8_t const pow3_lut[5] = { 1U, 3U, 9U, 27U, 81U };
    return pow3_lut[j % 5U];
}

static inline void argon_vbst_pack_trit(uint8_t vbst_packed[3],
                                        uint8_t trit_idx,
                                        int8_t  trit_val) {
    if (trit_idx < 15U) {
        uint8_t const byte_idx = (uint8_t)(trit_idx / 5U);
        uint8_t const pos_j    = (uint8_t)(trit_idx % 5U);
        int32_t const delta    = (int32_t)trit_val * (int32_t)argon_pow3_u8(pos_j);
        vbst_packed[byte_idx]  = (uint8_t)((int32_t)vbst_packed[byte_idx] + delta);
    }
}

static inline int8_t argon_vbst_unpack_trit(uint8_t const vbst_packed[3],
                                            uint8_t       trit_idx) {
    uint8_t const byte_idx = (uint8_t)(trit_idx / 5U);
    uint8_t const pos_j    = (uint8_t)(trit_idx % 5U);
    uint8_t const divisor  = argon_pow3_u8(pos_j);
    uint8_t const digit    = (uint8_t)((vbst_packed[byte_idx] / divisor) % 3U);
    return (int8_t)((int32_t)digit - 1);
}

static inline uint8_t argon_latch_kintsugi_shear(uint32_t  shear_tau_ug,
                                                 uint32_t *weld_counter) {
    uint32_t const is_spike = (uint32_t)(shear_tau_ug > ARGON_SHEAR_TH_UG);
    *weld_counter += is_spike;
    return (uint8_t)(is_spike * ARGON_KINTSUGI_GOLD_BYTE);
}

void argon_engine_init(ArgonEngineContext *ctx);
void argon_engine_step_epoch(ArgonEngineContext *ctx, ArgonEpochInput const *input);
bool argon_engine_verify_frame(ArgonMethylatedFrame64B const *frame);
bool argon_engine_run_13_epochs(ArgonEngineContext *ctx, ArgonMethylatedFrame64B history_out[ARGON_EPOCHS_COUNT]);

#ifdef __cplusplus
}
#endif

#endif /* ARGON_METHYLATION_ENGINE_H */
