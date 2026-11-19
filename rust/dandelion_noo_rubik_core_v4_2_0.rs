// =============================================================================
// FILE: dandelion_noo_rubik_core_v4_2_0.rs
// VERSION: v4.2.0 (Strict 3-Digit SemVer — Supersedes v4.1.2 Core Substrate)
// AUTHOR: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
// AFFILIATION: Independent Dandelion Substrate Research Group, Gurley, AL
// SUBSTRATE: 0 Green — Dandelion OS & Shingari Core (#![no_std], #[repr(C)])
// TARGET: profplum (Linux 6.12+ PREEMPT_RT, DMA-BUF io_uring, 18.50 W Envelope)
// =============================================================================
// Build on profplum:
//   rustc --edition=2021 --crate-type=cdylib -C opt-level=3 -C panic=abort \
//     dandelion_noo_rubik_core_v4_2_0.rs -o build/libdandelion_os_v4_2_0.so
// =============================================================================

#![no_std]
#![deny(unsafe_op_in_unsafe_fn)]
#![deny(improper_ctypes_definitions)]

use core::mem::{offset_of, size_of};

// -----------------------------------------------------------------------------
// 1. Canonical Metrological & Trapdoor Constants
// -----------------------------------------------------------------------------
pub const FOWLER_FRAME_TRITS: usize = 40;
pub const FOWLER_BYTES_PER_SEED: usize = 8;
pub const FOWLER_MAX_VALID_BYTE: u8 = 242; // 3^5 - 1 = 242 (94.92% byte density)
pub const MASK_40_TRITS: u64 = (1u64 << 40) - 1;

pub const CODE_CLEAN: u8 = 0;
pub const CODE_DNHM_BREAKER: u8 = 250;
pub const CODE_KINTSUGI_GOLD: u8 = 251;
pub const CODE_QUECHUA_INFERENTIAL: u8 = 252;
pub const CODE_FAIL_CLOSED_DROP: u8 = 253;
pub const CODE_ELIAS_TRAPDOOR: u8 = 254;
pub const CODE_SHOKUNIN_UNLATCHED: u8 = 255;

pub const RUBIK_MAX_ELEMENT_ORDER: u32 = 1260;
pub const RUBIK_BLE_TTL_HORIZON: u16 = 258;
pub const THERMAL_CEILING_MC: u32 = 41_000; // 41.000 C Landauer envelope ceiling

// -----------------------------------------------------------------------------
// 2. Mandatory C99 / #[repr(C)] Byte-Parity Structures (Zero Implicit Padding)
// -----------------------------------------------------------------------------

/// 6-Byte LayerFS Content-Addressed Relational Lattice Point
#[repr(C, packed)]
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct DandelionPointV2 {
    pub x_l1: i16,
    pub y_l1: i16,
    pub z_l1: i8,
    pub trit_state: i8, // {-1 Blue Subject, 0 Green Object, +1 Red Action}
}

/// 32-Byte Stigmergic BLE & Mesh Relay Frame ("DND3")
#[repr(C)]
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct StigmergicBleFrame {
    pub magic: [u8; 4],          // b"DND3" (0..4)
    pub fowler_seed_8b: [u8; 8], // 40 balanced trits packed at 5 trits/byte (4..12)
    pub sender_node_id: u32,     // (12..16)
    pub hop_ttl: u16,            // Bounded by finite orbital horizon 258 (16..18)
    pub trapdoor_status: u8,     // 0 = Clean, 251 = Kintsugi Gold weld (18..19)
    pub crc8_parity: u8,         // (19..20)
    pub payload_trits: [u8; 12], // (20..32)
}

/// 64-Byte L1 Cache-Line Aligned Shingari Triadic Deed & DMA-BUF Descriptor
/// Ordered descending by alignment (u64 -> u32 -> u16 -> u8) for zero implicit padding.
#[repr(C, align(64))]
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct ShingariPacket64 {
    pub ref_hash: u64,                 // 0..8
    pub pos_mask_40t: u64,             // 8..16: Dual-Rail +1 bitplane
    pub neg_mask_40t: u64,             // 16..24: Dual-Rail -1 bitplane
    pub noo_tick_ns: u64,              // 24..32: Monotonic 3D operational clock tick
    pub macro_rosetta_p: u32,          // 32..36: Mori-Zwanzig P-subspace inode (1505)
    pub micro_texture_q: u32,          // 36..40: Mori-Zwanzig Q-subspace inode (2001)
    pub dmabuf_fd: i32,                // 40..44: Zero-copy kernel DMA-BUF descriptor
    pub thermal_mc: u32,               // 44..48: Die temp in millidegrees C (<= 41000)
    pub io_uring_buf_idx: u16,         // 48..50: IORING_OP_READ_FIXED / WRITE_FIXED slot
    pub kintsugi_code: u8,             // 50..51: 0 = Clean, 251 = Kintsugi shear weld
    pub root_id: u8,                   // 51..52: 1..=13 Shingari [4-5-4] ISA root
    pub haptic_forehead_rx: [i8; 3],   // 52..55: Joe Bonham 3-trit return pulse
    pub reserved_open_circle: [u8; 9], // 55..64: Explicit Open-Circle alignment seal
}

/// 577-Byte L1D-Resident Ternary Neural & Group Pullback Table (.tnpt)
/// Exact byte sum: 4 + 3 + 1 + 512 + 48 + 8 + 1 = 577 bytes.
#[repr(C, packed)]
#[derive(Copy, Clone)]
pub struct DandelionTnptV4 {
    pub header_magic: [u8; 4],    // 0..4:     b"TNPT"
    pub version_semver: [u8; 3],  // 4..7:     [4, 2, 0]
    pub keyhole_duty_flag: u8,    // 7..8:     0xAA -> V* = 2/3 Blume-Capel lock
    pub lut4_weights: [u8; 512],  // 8..520:   4-trit T-MAC / VBST lookup table
    pub rosetta_pins: [u8; 48],   // 520..568: 6 x 8-byte Fowler seeds
    pub dnhm_haiku_seed: [u8; 8], // 568..576: 40-trit temporal haiku Fowler seed
    pub tail_kintsugi_crc: u8,    // 576..577: Byte 577 parity seal
}

/// 64-Byte L1 Cache-Line Aligned Modified Rubik's Nootemporal State Engine
/// Encodes G = (Z_3^7 x Z_2^11) |x ((A_8 x A_12) |x Z_2) inside S_48
/// with zero implicit padding across Rust, ISO C99, and Julia 1.12.
#[repr(C, align(64))]
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub struct RubikNooState64B {
    pub corner_perm: [u8; 8],       // 0..8:   0..7 corner cubie indices
    pub corner_trit: [i8; 8],       // 8..16:  {-1, 0, +1} Z_3 twist; sum == 0 (mod 3)
    pub edge_perm: [u8; 12],        // 16..28: 0..11 edge cubie indices
    pub edge_sign: [i8; 12],        // 28..40: {-1, +1} Z_2 signed monomial; prod == +1
    pub fowler_cocycle_8b: [u8; 8], // 40..48: 20 cocycle trits + 20 parity trits packed
    pub noo_step_count: u64,        // 48..56: Irreversible operational process counter
    pub radar_sync_offset_ns: i32,  // 56..60: Einstein 1905 2-way clock offset
    pub frustration_l1: u16,        // 60..62: Non-zero cocycle + displacement tension
    pub kintsugi_status: u8,        // 62..63: 0 = Clean, 251 = Cocycle healed
    pub parity_sign: i8,            // 63..64: +1 even quarter-turn parity, -1 odd
}

// Compile-Time Static ABI Assertions
const _: () = assert!(size_of::<DandelionPointV2>() == 6);
const _: () = assert!(size_of::<StigmergicBleFrame>() == 32);
const _: () = assert!(size_of::<ShingariPacket64>() == 64);
const _: () = assert!(size_of::<DandelionTnptV4>() == 577);
const _: () = assert!(size_of::<RubikNooState64B>() == 64);
const _: () = assert!(offset_of!(StigmergicBleFrame, fowler_seed_8b) == 4);
const _: () = assert!(offset_of!(ShingariPacket64, noo_tick_ns) == 24);
const _: () = assert!(offset_of!(RubikNooState64B, noo_step_count) == 48);

// -----------------------------------------------------------------------------
// 3. Exact 3D Chiral Rubik Generator Tables (Derived from {-1,0,+1}^3 Lattice)
// -----------------------------------------------------------------------------

const CORNER_SRC: [[u8; 8]; 6] = [
    [3, 0, 1, 2, 4, 5, 6, 7], // U: 0->1->2->3->0
    [0, 1, 2, 3, 5, 6, 7, 4], // D: 4->7->6->5->4 (CW from -Y)
    [0, 2, 6, 3, 4, 1, 5, 7], // L: 1->5->6->2->1 (CW from -X)
    [4, 1, 2, 0, 7, 5, 6, 3], // R: 0->3->7->4->0 (CW from +X)
    [1, 5, 2, 3, 0, 4, 6, 7], // F: 0->4->5->1->0 (CW from +Z)
    [0, 1, 3, 7, 4, 5, 2, 6], // B: 3->2->6->7->3 (CW from -Z)
];

const CORNER_DELTA: [[i8; 8]; 6] = [
    [ 0,  0,  0,  0,  0,  0,  0,  0], // U: preserves Y-axis reference
    [ 0,  0,  0,  0,  0,  0,  0,  0], // D: preserves Y-axis reference
    [ 0,  1, -1,  0,  0, -1,  1,  0], // L: sum = 0
    [-1,  0,  0,  1,  1,  0,  0, -1], // R: sum = 0
    [ 1, -1,  0,  0, -1,  1,  0,  0], // F: sum = 0
    [ 0,  0,  1, -1,  0,  0, -1,  1], // B: sum = 0
];

const EDGE_SRC: [[u8; 12]; 6] = [
    [ 3,  0,  1,  2,  4,  5,  6,  7,  8,  9, 10, 11], // U
    [ 0,  1,  2,  3,  5,  6,  7,  4,  8,  9, 10, 11], // D
    [ 0, 10,  2,  3,  4,  9,  6,  7,  8,  1,  5, 11], // L
    [ 0,  1,  2,  8,  4,  5,  6, 11,  7,  9, 10,  3], // R
    [ 9,  1,  2,  3,  8,  5,  6,  7,  0,  4, 10, 11], // F
    [ 0,  1, 11,  3,  4,  5, 10,  7,  8,  9,  2,  6], // B
];

const EDGE_SIGN_MUL: [[i8; 12]; 6] = [
    [ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1], // U
    [ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1], // D
    [ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1], // L
    [ 1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1], // R
    [-1,  1,  1,  1, -1,  1,  1,  1, -1, -1,  1,  1], // F: 4 flips, prod = +1
    [ 1,  1, -1,  1,  1,  1, -1,  1,  1,  1, -1, -1], // B: 4 flips, prod = +1
];

#[inline(always)]
fn wrap_bal3(v: i8) -> i8 {
    let mut r = v % 3;
    if r > 1 {
        r -= 3;
    } else if r < -1 {
        r += 3;
    }
    r
}

// -----------------------------------------------------------------------------
// 4. Exported Bare-Metal C-ABI Kernels (`extern "C"`, Zero Heap Allocation)
// -----------------------------------------------------------------------------

/// Multiplication-Free 40-Trit Dual-Rail POPCNT Inner Product
#[no_mangle]
pub extern "C" fn vbst_dot40_popcnt(
    w_pos: u64,
    w_neg: u64,
    x_pos: u64,
    x_neg: u64,
) -> i32 {
    let wp = w_pos & MASK_40_TRITS;
    let wn = w_neg & MASK_40_TRITS;
    let xp = x_pos & MASK_40_TRITS;
    let xn = x_neg & MASK_40_TRITS;

    let same = (wp & xp) | (wn & xn);
    let diff = (wp & xn) | (wn & xp);
    (same.count_ones() as i32) - (diff.count_ones() as i32)
}

/// Packs 40 balanced trits {-1, 0, +1} into 8 Fowler radix-3 bytes (0..=242).
#[no_mangle]
pub unsafe extern "C" fn fowler_pack_40trits(
    trits_ptr: *const i8,
    out_bytes_ptr: *mut u8,
) -> u8 {
    if trits_ptr.is_null() || out_bytes_ptr.is_null() {
        return CODE_FAIL_CLOSED_DROP;
    }
    let trits = unsafe { core::slice::from_raw_parts(trits_ptr, FOWLER_FRAME_TRITS) };
    let out = unsafe { core::slice::from_raw_parts_mut(out_bytes_ptr, FOWLER_BYTES_PER_SEED) };

    let mut status = CODE_CLEAN;
    let mut b = 0usize;
    while b < FOWLER_BYTES_PER_SEED {
        let mut val: u16 = 0;
        let mut mult: u16 = 1;
        let mut k = 0usize;
        while k < 5 {
            let raw = trits[b * 5 + k];
            let clamped = if raw < -1 {
                status = CODE_KINTSUGI_GOLD;
                -1
            } else if raw > 1 {
                status = CODE_KINTSUGI_GOLD;
                1
            } else {
                raw
            };
            val += ((clamped + 1) as u16) * mult;
            mult *= 3;
            k += 1;
        }
        out[b] = val as u8;
        b += 1;
    }
    status
}

/// Unpacks 8 Fowler bytes into 40 balanced trits, routing trapdoors 243..=255.
#[no_mangle]
pub unsafe extern "C" fn fowler_unpack_40trits_strict(
    in_bytes_ptr: *const u8,
    out_trits_ptr: *mut i8,
) -> u8 {
    if in_bytes_ptr.is_null() || out_trits_ptr.is_null() {
        return CODE_FAIL_CLOSED_DROP;
    }
    let inp = unsafe { core::slice::from_raw_parts(in_bytes_ptr, FOWLER_BYTES_PER_SEED) };
    let out = unsafe { core::slice::from_raw_parts_mut(out_trits_ptr, FOWLER_FRAME_TRITS) };

    let mut b = 0usize;
    while b < FOWLER_BYTES_PER_SEED {
        let byte_val = inp[b];
        if byte_val > FOWLER_MAX_VALID_BYTE {
            let mut z = 0usize;
            while z < FOWLER_FRAME_TRITS {
                out[z] = 0;
                z += 1;
            }
            return byte_val;
        }
        let mut rem = byte_val;
        let mut k = 0usize;
        while k < 5 {
            out[b * 5 + k] = ((rem % 3) as i8) - 1;
            rem /= 3;
            k += 1;
        }
        b += 1;
    }
    CODE_CLEAN
}

/// Einstein 1905 Operational Two-Way Radar Clock Synchronizer (Zero Float)
#[no_mangle]
pub unsafe extern "C" fn dandelion_noo_einstein_sync_ns(
    t_tx_local_ns: u64,
    t_rx_local_ns: u64,
    t_reflect_remote_ns: u64,
    out_offset_ns: *mut i64,
) -> u64 {
    if t_rx_local_ns < t_tx_local_ns {
        if !out_offset_ns.is_null() {
            unsafe { *out_offset_ns = 0 };
        }
        return 0;
    }
    let round_trip_ns = t_rx_local_ns - t_tx_local_ns;
    let midpoint_ns = t_tx_local_ns + (round_trip_ns >> 1);
    if !out_offset_ns.is_null() {
        let offset = (t_reflect_remote_ns as i128) - (midpoint_ns as i128);
        unsafe { *out_offset_ns = offset as i64 };
    }
    round_trip_ns >> 1
}

#[inline]
fn refresh_rubik_telemetry(st: &mut RubikNooState64B) -> u8 {
    let mut twist_sum: i16 = 0;
    let mut edge_prod: i8 = 1;
    let mut l1_frust: u16 = 0;
    let mut cocycle_trits = [0i8; FOWLER_FRAME_TRITS];

    let mut i = 0usize;
    while i < 8 {
        twist_sum += st.corner_trit[i] as i16;
        l1_frust += st.corner_trit[i].unsigned_abs() as u16;
        if st.corner_perm[i] != (i as u8) {
            l1_frust += 1;
        }
        cocycle_trits[i] = st.corner_trit[i];
        i += 1;
    }

    let mut e = 0usize;
    while e < 12 {
        edge_prod *= st.edge_sign[e];
        if st.edge_sign[e] < 0 {
            l1_frust += 1;
        }
        if st.edge_perm[e] != (e as u8) {
            l1_frust += 1;
        }
        cocycle_trits[8 + e] = if st.edge_sign[e] < 0 { -1 } else { 0 };
        e += 1;
    }

    let mut status = CODE_CLEAN;
    let rem = twist_sum.rem_euclid(3);
    if rem != 0 {
        st.corner_trit[7] = wrap_bal3(st.corner_trit[7] - (rem as i8));
        cocycle_trits[7] = st.corner_trit[7];
        status = CODE_KINTSUGI_GOLD;
    }
    if edge_prod < 0 {
        st.edge_sign[11] = -st.edge_sign[11];
        cocycle_trits[19] = if st.edge_sign[11] < 0 { -1 } else { 0 };
        status = CODE_KINTSUGI_GOLD;
    }

    st.frustration_l1 = l1_frust;
    st.kintsugi_status = status;
    unsafe {
        let _ = fowler_pack_40trits(
            cocycle_trits.as_ptr(),
            st.fowler_cocycle_8b.as_mut_ptr(),
        );
    }
    status
}

/// Initializes a solved 64-byte Modified Rubik's Nootemporal State.
#[no_mangle]
pub unsafe extern "C" fn rubik_noo_init_solved(state_ptr: *mut RubikNooState64B) {
    if state_ptr.is_null() {
        return;
    }
    let st = unsafe { &mut *state_ptr };
    let mut i = 0usize;
    while i < 8 {
        st.corner_perm[i] = i as u8;
        st.corner_trit[i] = 0;
        i += 1;
    }
    let mut e = 0usize;
    while e < 12 {
        st.edge_perm[e] = e as u8;
        st.edge_sign[e] = 1;
        e += 1;
    }
    st.fowler_cocycle_8b = [0u8; 8];
    st.noo_step_count = 0;
    st.radar_sync_offset_ns = 0;
    st.frustration_l1 = 0;
    st.kintsugi_status = CODE_CLEAN;
    st.parity_sign = 1;
    let _ = refresh_rubik_telemetry(st);
}

/// Applies move `move_code` (0..5 = CW {U,D,L,R,F,B}; 6..11 = 180 {U2..B2};
/// 12..17 = CCW {U'..B'}) in balanced ternary wreath coordinates.
#[no_mangle]
pub unsafe extern "C" fn rubik_noo_step_generator(
    state_ptr: *mut RubikNooState64B,
    move_code: u8,
) -> u8 {
    if state_ptr.is_null() || move_code >= 18 {
        return CODE_FAIL_CLOSED_DROP;
    }
    let st = unsafe { &mut *state_ptr };
    let base_face = (move_code % 6) as usize;
    let quarter_turns = (move_code / 6) + 1;

    let mut q = 0u8;
    while q < quarter_turns {
        let old_cp = st.corner_perm;
        let old_ct = st.corner_trit;
        let old_ep = st.edge_perm;
        let old_es = st.edge_sign;

        let mut c = 0usize;
        while c < 8 {
            let src = CORNER_SRC[base_face][c] as usize;
            st.corner_perm[c] = old_cp[src];
            st.corner_trit[c] = wrap_bal3(old_ct[src] + CORNER_DELTA[base_face][c]);
            c += 1;
        }

        let mut e = 0usize;
        while e < 12 {
            let src = EDGE_SRC[base_face][e] as usize;
            st.edge_perm[e] = old_ep[src];
            st.edge_sign[e] = old_es[src] * EDGE_SIGN_MUL[base_face][e];
            e += 1;
        }
        st.parity_sign = -st.parity_sign;
        q += 1;
    }

    st.noo_step_count = st.noo_step_count.wrapping_add(1);
    refresh_rubik_telemetry(st)
}

/// Computes the exact closed periodic orbit order of a macro sequence on bare metal.
#[no_mangle]
pub unsafe extern "C" fn rubik_noo_orbit_period(
    seq_ptr: *const u8,
    seq_len: usize,
    max_periods: u32,
) -> u32 {
    if seq_ptr.is_null() || seq_len == 0 {
        return 0;
    }
    let seq = unsafe { core::slice::from_raw_parts(seq_ptr, seq_len) };
    let mut st = RubikNooState64B {
        corner_perm: [0; 8],
        corner_trit: [0; 8],
        edge_perm: [0; 12],
        edge_sign: [1; 12],
        fowler_cocycle_8b: [0; 8],
        noo_step_count: 0,
        radar_sync_offset_ns: 0,
        frustration_l1: 0,
        kintsugi_status: CODE_CLEAN,
        parity_sign: 1,
    };
    unsafe { rubik_noo_init_solved(&mut st) };

    let mut period: u32 = 0;
    while period < max_periods {
        let mut k = 0usize;
        while k < seq_len {
            unsafe {
                let _ = rubik_noo_step_generator(&mut st, seq[k]);
            }
            k += 1;
        }
        period += 1;
        if st.frustration_l1 == 0 {
            return period;
        }
    }
    0
}

#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {
        core::hint::spin_loop();
    }
}
