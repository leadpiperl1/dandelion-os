#![no_std]
#![crate_type = "cdylib"]

use core::mem::size_of;
use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

pub type Q16 = i32;
pub const Q16_ONE: Q16 = 1 << 16;
pub const Q16_KEYHOLE: Q16 = 43691;

pub const STATE_DIM: usize = 8;
pub const HISTORY_LEN: usize = 8;
pub const MAX_NEIGHBORS: usize = 4;

#[repr(C, align(64))]
#[derive(Copy, Clone)]
pub struct GroundedAgent64B {
    pub x_trit: [i8; STATE_DIM],
    pub neighbors: [u16; MAX_NEIGHBORS],
    pub mz_ring: [u8; HISTORY_LEN],
    pub ring_head: u8,
    pub status_byte: u8,
    pub latent_q16: [Q16; STATE_DIM],
}

const _: () = assert!(size_of::<GroundedAgent64B>() == 64);

pub const MZ_KERNEL_Q16: [Q16; HISTORY_LEN] = [
    16384, 8192, 4096, 2048, 1024, 512, 256, 128
];

#[inline(always)]
fn pack_trits_base3(trits: &[i8; STATE_DIM]) -> u8 {
    let mut acc: u16 = 0;
    let mut i = 0;
    while i < 5 {
        let digit = (trits[i].clamp(-1, 1) + 1) as u16;
        acc = acc * 3 + digit;
        i += 1;
    }
    acc as u8
}

#[inline(always)]
fn unpack_trits_base3(mut packed: u8, out: &mut [i8; 5]) {
    if packed >= 243 {
        *out = [0; 5];
        return;
    }
    let mut i = 5;
    while i > 0 {
        i -= 1;
        out[i] = (packed % 3) as i8 - 1;
        packed /= 3;
    }
}

#[inline(always)]
fn xorshift32(state: &mut u32) -> i32 {
    let mut x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    (x as i32) >> 20
}

#[unsafe(no_mangle)]
pub extern "C" fn dandelion_grounded_step(
    agents: *mut GroundedAgent64B,
    num_agents: usize,
    omega_drift_q16: Q16,
    coupling_j_q16: Q16,
    dt_q16: Q16,
    rng_seed: *mut u32,
) -> u32 {
    if agents.is_null() || rng_seed.is_null() || num_agents == 0 {
        return 253;
    }

    let slice = unsafe { core::slice::from_raw_parts_mut(agents, num_agents) };
    let seed = unsafe { &mut *rng_seed };
    let mut clamped_count: u32 = 0;

    for i in 0..num_agents {
        let mut neighbor_sum = [0i32; STATE_DIM];
        let neighbors = slice[i].neighbors;

        for &nb_idx in &neighbors {
            if nb_idx != u16::MAX && (nb_idx as usize) < num_agents {
                let nb_trits = slice[nb_idx as usize].x_trit;
                for k in 0..STATE_DIM {
                    neighbor_sum[k] += nb_trits[k] as i32;
                }
            }
        }

        let mut mz_drag = [0i32; STATE_DIM];
        let head = slice[i].ring_head as usize;
        for tau in 0..HISTORY_LEN {
            let slot = (head + HISTORY_LEN - tau) % HISTORY_LEN;
            let mut past_trits = [0i8; 5];
            unpack_trits_base3(slice[i].mz_ring[slot], &mut past_trits);
            let weight = MZ_KERNEL_Q16[tau];
            for k in 0..5 {
                mz_drag[k] += (past_trits[k] as i32) * weight;
            }
        }

        let agent = &mut slice[i];
        let mut saturated = false;

        for k in 0..STATE_DIM {
            let x_curr = (agent.x_trit[k] as i32) * Q16_ONE;
            let omega = ((omega_drift_q16 as i64 * x_curr as i64) >> 16) as i32;
            let social = neighbor_sum[k] * coupling_j_q16;
            let noise = xorshift32(seed);

            let dx = omega - mz_drag[k] + social + noise;
            let delta = ((dx as i64 * dt_q16 as i64) >> 16) as i32;
            let mut next_val = agent.latent_q16[k].saturating_add(delta);

            if next_val.abs() > Q16_ONE {
                next_val = next_val.clamp(-Q16_ONE, Q16_ONE);
                saturated = true;
            }
            agent.latent_q16[k] = next_val;

            agent.x_trit[k] = if next_val > 21845 {
                1
            } else if next_val < -21845 {
                -1
            } else {
                0
            };
        }

        let next_head = ((head + 1) % HISTORY_LEN) as u8;
        agent.ring_head = next_head;
        agent.mz_ring[next_head as usize] = pack_trits_base3(&agent.x_trit);
        agent.status_byte = if saturated {
            clamped_count += 1;
            243
        } else {
            agent.mz_ring[next_head as usize]
        };
    }

    clamped_count
}
