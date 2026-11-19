#![no_std] Rust 64-Byte KMS Frame & Surprisal Gate (kembali_kms_gate.rs)
#![no_std]


pub const CODE_HYSTERESIS_CLAMP: u8 = 242;
pub const CODE_KEYHOLE_SATURATED: u8 = 243;
pub const CODE_KINTSUGI_SHEAR: u8 = 251;
pub const CODE_GROUND_PREEMPT: u8 = 253;
pub const CODE_SHOKUNIN_UNLATCHED: u8 = 255;


#[repr(C, align(64))]
#[derive(Clone, Copy)]
pub struct KembaliCarbonFrame64B {
    pub tick_index: u64,            // 8 B: 104.0 Hz master phase tick
    pub sp3_axes_q16: [i32; 4],     // 16 B: tetrahedral Carbon sp3 projections (sum <= 4 LSB)
    pub is_geom_gain_q16: u32,      // 4 B: geometric-mean Itakura-Saito ratio G_geom
    pub eta_drag_q16: i32,          // 4 B: -1 Blue KMS longitudinal drift M1(v)
    pub kappa_diff_q16: u32,        // 4 B:  0 Green soft cavity/HTL variance M2(v)
    pub tail_r_q16: u32,            // 4 B: +1 Red hard-collision braking tail R(v)
    pub kms_tilt_q16: i32,          // 4 B: universal KMS asymmetry shift v_env / T_eff
    pub popcnt_window_64: u64,      // 8 B: 64-tick V = 2/3 keyhole bitmask
    pub fowler_superchair: [u8; 9], // 9 B: 42-trit Fowler Superchair payload
    pub cooldown_ticks: u8,         // 1 B: 0..104 Eli Whitney 1.000 s cooldown
    pub gate_code: u8,              // 1 B: triadic gate dispatch code
    pub exit_latch: u8,             // 1 B: 255 = N_max - 1 unlatched exit
}


const _: () = assert!(core::mem::size_of::<KembaliCarbonFrame64B>() == 64);


#[inline(always)]
pub fn evaluate_kms_frame(frame: &mut KembaliCarbonFrame64B, dk_q16: i32) -> u8 {
    // 0-tick fail-closed Ground Preempt (Byte 253) bypasses cooldown immediately
    if frame.gate_code == CODE_GROUND_PREEMPT {
        frame.cooldown_ticks = 0;
        frame.exit_latch = CODE_SHOKUNIN_UNLATCHED;
        return CODE_GROUND_PREEMPT;
    }


    // Carbon sp3 tetrahedral zero-sum parity check (<= 4 LSB)
    let sp3_sum: i64 = (frame.sp3_axes_q16[0] as i64)
        + (frame.sp3_axes_q16[1] as i64)
        + (frame.sp3_axes_q16[2] as i64)
        + (frame.sp3_axes_q16[3] as i64);
    if sp3_sum.abs() > 4 {
        frame.gate_code = CODE_KINTSUGI_SHEAR;
        return CODE_KINTSUGI_SHEAR;
    }


    // Asymmetric KMS tail surprisal in Q16.16 nats:
    // Passive braking (dk >= 0): S_brake = R(v) * dk  (gate > 7.8 nats = 511181 Q16)
    // Active contra-drift (dk < 0): S_boost = (R(v) + v/T) * |dk|  (gate > 6.2 nats = 406323 Q16)
    let is_candidate = if dk_q16 >= 0 {
        let s_q16 = ((frame.tail_r_q16 as i64) * (dk_q16 as i64)) >> 16;
        s_q16 > 511_181
    } else {
        let r_boost_q16 = (frame.tail_r_q16 as i64) + (frame.kms_tilt_q16.abs() as i64);
        let s_q16 = (r_boost_q16 * (dk_q16.unsigned_abs() as i64)) >> 16;
        s_q16 > 406_323
    };


    let out = if is_candidate {
        if frame.cooldown_ticks == 0 {
            frame.cooldown_ticks = 104;
            CODE_KEYHOLE_SATURATED
        } else {
            CODE_HYSTERESIS_CLAMP
        }
    } else {
        0
    };


    if frame.cooldown_ticks > 0 && out != CODE_KEYHOLE_SATURATED {
        frame.cooldown_ticks -= 1;
    }
    frame.gate_code = out;
    frame.exit_latch = CODE_SHOKUNIN_UNLATCHED;
    out
}
