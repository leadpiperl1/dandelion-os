#![no_std]
#![deny(unsafe_op_in_unsafe_fn)]

use core::mem::{offset_of, size_of};

#[repr(C, packed)]
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct DandelionPointV2 {
    pub x_l1: i16,
    pub y_l1: i16,
    pub z_l1: i8,
    pub trit_state: i8,
}

#[repr(C)]
#[derive(Copy, Clone, Debug)]
pub struct StigmergicBleFrame {
    pub magic: [u8; 4],
    pub fowler_seed_8b: [u8; 8],
    pub sender_node_id: u32,
    pub hop_ttl: u16,
    pub trapdoor_status: u8,
    pub crc8_parity: u8,
    pub payload_trits: [u8; 12],
}

#[repr(C, align(64))]
#[derive(Copy, Clone, Debug)]
pub struct ShardedQuorumEntry64B {
    pub ref_hash: u64,
    pub macro_rosetta_p: u32,
    pub micro_texture_q: u32,
    pub pos_mask_40t: u64,
    pub neg_mask_40t: u64,
    pub kintsugi_shear_mdeg: u32,
    pub thermal_mc: u32,
    pub reserved_open_circle: [u8; 24],
}

#[repr(C, packed)]
#[derive(Copy, Clone)]
pub struct DandelionTnpt {
    pub header_magic: [u8; 4],
    pub version_semver: [u8; 3],
    pub keyhole_duty_flag: u8,
    pub lut4_weights: [u8; 512],
    pub rosetta_pins: [u8; 48],
    pub tail_kintsugi_crc: u8,
}

const _: () = assert!(size_of::<DandelionPointV2>() == 6);
const _: () = assert!(size_of::<StigmergicBleFrame>() == 32);
const _: () = assert!(size_of::<ShardedQuorumEntry64B>() == 64);
const _: () = assert!(size_of::<DandelionTnpt>() == 577);
const _: () = assert!(offset_of!(StigmergicBleFrame, fowler_seed_8b) == 4);

const MASK_40_TRITS: u64 = (1u64 << 40) - 1;

#[no_mangle]
pub extern "C" fn vbst_dot40_popcnt(
    a_pos: u64,
    a_neg: u64,
    b_pos: u64,
    b_neg: u64,
) -> i32 {
    let ap = a_pos & MASK_40_TRITS;
    let an = a_neg & MASK_40_TRITS;
    let bp = b_pos & MASK_40_TRITS;
    let bn = b_neg & MASK_40_TRITS;
    let same_sign = (ap & bp) | (an & bn);
    let opp_sign = (ap & bn) | (an & bp);
    (same_sign.count_ones() as i32) - (opp_sign.count_ones() as i32)
}
