//! @file dandelion_dma_uring_safe.rs
//! @brief Safe Rust abstractions for Dandelion OS v4.2.0 zero-copy DMA-BUF & io_uring pipeline.
//! @author Justin D. Grimm (ORCID: 0009-0000-5299-0243)

#![no_std]

use core::ffi::c_int;

#[repr(C)]
#[repr(align(64))]
#[derive(Copy, Clone, Debug)]
pub struct DandelionDmabufDesc {
    pub dmabuf_fd: i32,
    pub map_flags: u32,
    pub physical_address: u64,
    pub virtual_address: u64,
    pub buffer_size: usize,
    pub reference_count: u32,
    pub reserved: [u8; 20],
}

#[repr(C)]
#[repr(align(64))]
#[derive(Copy, Clone, Debug)]
pub struct DandelionIoUringRing {
    pub ring_fd: i32,
    pub head_offset: u32,
    pub tail_offset: u32,
    pub ring_mask: u32,
    pub ring_entries: u32,
    pub flags: u32,
    pub pad: [u8; 36],
}

extern "C" {
    fn dandelion_dmabuf_init_ring(ring: *mut DandelionIoUringRing, entries: u32) -> c_int;
    fn dandelion_dmabuf_zero_copy_submit(ring: *mut DandelionIoUringRing, desc: *mut DandelionDmabufDesc) -> c_int;
    fn dandelion_dmabuf_teardown(ring: *mut DandelionIoUringRing);
}

pub struct SafeIoUringRing {
    raw: DandelionIoUringRing,
}

impl SafeIoUringRing {
    pub const fn new() -> Self {
        Self {
            raw: DandelionIoUringRing {
                ring_fd: -1,
                head_offset: 0,
                tail_offset: 0,
                ring_mask: 0,
                ring_entries: 0,
                flags: 0,
                pad: [0; 36],
            },
        }
    }

    pub fn init(&mut self, entries: u32) -> Result<(), c_int> {
        let ret = unsafe { dandelion_dmabuf_init_ring(&mut self.raw, entries) };
        if ret == 0 {
            Ok(())
        } else {
            Err(ret)
        }
    }

    pub fn submit_zero_copy(&mut self, desc: &mut DandelionDmabufDesc) -> Result<(), c_int> {
        let ret = unsafe { dandelion_dmabuf_zero_copy_submit(&mut self.raw, desc) };
        if ret == 0 {
            Ok(())
        } else {
            Err(ret)
        }
    }
}

impl Drop for SafeIoUringRing {
    fn drop(&mut self) {
        unsafe {
            dandelion_dmabuf_teardown(&mut self.raw);
        }
    }
}
