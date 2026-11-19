#![no_std]
//! DOS-PEARL-v3.5.0: Balanced Ternary Ergospheric Health-Check Damper
//! Author: Justin D. Grimm | Independent Dandelion Substrate Research Group
//! ORCID: 0009-0000-5299-0243 | Version: 3.5.0
//!
//! Replaces binary Heaviside `/healthz` node eviction with a 3-state Blume-Capel
//! regulator (`-1` Hardware Quarantine, `0` Green Backpressure/Bulk-Shed, `+1` Active)
//! pinned at the Vonnegut u_lock = 2/3 keyhole and coprime 13x19 probe scheduling.

pub const FOWLER_NEUTRAL_BYTE: u8 = 121;
pub const KEYHOLE_TRAPDOOR_BYTE: u8 = 243;
pub const KINTSUGI_GOLD_BYTE: u8 = 251;

pub const COPRIME_STRIDE_A: u32 = 13;
pub const COPRIME_MODULUS_B: u32 = 19;

/// Fixed-point milli-units (1000 = 1.000 capacity ceiling; 667 = 2/3 keyhole)
pub const U_LOCK_MILLI: u16 = 667;
pub const U_GREEN_ENTER_MILLI: u16 = 780;

#[repr(i8)]
#[derive(Copy, Clone, Debug, PartialEq, Eq)]
pub enum TritHealthState {
    Quarantine = -1, // Hardware/ECC fault ONLY; never triggered by queue latency
    Backpressure = 0, // 0-Green slack state: shed bulk sync, keep TLS & core alive
    Active = 1,       // +1-Red full execution below saturation
}

#[repr(C)]
#[derive(Copy, Clone, Debug)]
pub struct NodeTelemetry {
    pub node_id: u32,
    pub state: TritHealthState,
    pub diagnostic_byte: u8,
    pub bulk_accept_permille: u16,
}

/// Evaluates whether `node_id` is scheduled for a lightweight health check on `tick`
/// using pairwise coprime (13, 19) stride de-aliasing to prevent probe stampedes.
#[inline]
pub const fn is_coprime_probe_due(node_id: u32, tick: u32) -> bool {
    ((node_id.wrapping_mul(COPRIME_STRIDE_A)).wrapping_add(tick)) % COPRIME_MODULUS_B == 0
}

/// Pure deterministic state transition preventing superradiant eviction cascades.
#[inline]
pub const fn step_ternary_health(
    current: TritHealthState,
    node_id: u32,
    load_milli: u16,
    hw_ecc_fault: bool,
) -> NodeTelemetry {
    if hw_ecc_fault {
        return NodeTelemetry {
            node_id,
            state: TritHealthState::Quarantine,
            diagnostic_byte: KINTSUGI_GOLD_BYTE,
            bulk_accept_permille: 0,
        };
    }

    let next_state = match current {
        TritHealthState::Quarantine => TritHealthState::Backpressure, // Warm re-entry via 0-Green!
        TritHealthState::Active => {
            if load_milli > U_GREEN_ENTER_MILLI {
                TritHealthState::Backpressure
            } else {
                TritHealthState::Active
            }
        }
        TritHealthState::Backpressure => {
            if load_milli <= U_LOCK_MILLI {
                TritHealthState::Active
            } else {
                TritHealthState::Backpressure
            }
        }
    };

    let (diag, bulk_permille) = match next_state {
        TritHealthState::Active => (FOWLER_NEUTRAL_BYTE, 1000),
        TritHealthState::Backpressure => (KEYHOLE_TRAPDOOR_BYTE, 150),
        TritHealthState::Quarantine => (KINTSUGI_GOLD_BYTE, 0),
    };

    NodeTelemetry {
        node_id,
        state: next_state,
        diagnostic_byte: diag,
        bulk_accept_permille: bulk_permille,
    }
}
