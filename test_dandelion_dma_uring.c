/**
 * @file test_dandelion_dma_uring.c
 * @brief Unit test harness for Dandelion OS v4.2.0 zero-copy DMA-BUF & io_uring pipeline.
 * @author Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 */

#include "dandelion_dma_uring.h"
#include <stdio.h>
#include <assert.h>
#include <errno.h>

void test_ring_initialization(void) {
    printf("[TEST] Running test_ring_initialization...\n");
    dandelion_iouring_ring_t ring;

    int ret = dandelion_dmabuf_init_ring(&ring, 16);
    assert(ret == 0);
    assert(ring.ring_entries == 16);
    assert(ring.ring_mask == 15);
    assert(ring.flags == 0x01);

    ret = dandelion_dmabuf_init_ring(&ring, 10);
    assert(ret == -EINVAL);

    ret = dandelion_dmabuf_init_ring(&ring, 0);
    assert(ret == -EINVAL);

    printf("[PASS] test_ring_initialization passed successfully.\n");
}

void test_zero_copy_submission_and_ring_full(void) {
    printf("[TEST] Running test_zero_copy_submission_and_ring_full...\n");
    dandelion_iouring_ring_t ring;
    dandelion_dmabuf_desc_t desc;

    int ret = dandelion_dmabuf_init_ring(&ring, 4);
    assert(ret == 0);

    desc.dmabuf_fd = 42;
    desc.map_flags = 0x02;
    desc.physical_address = 0x10000000;
    desc.virtual_address = 0x7fff0000;
    desc.buffer_size = 4096;
    desc.reference_count = 0;

    ret = dandelion_dmabuf_zero_copy_submit(&ring, &desc);
    assert(ret == 0);
    assert(desc.reference_count == 1);

    ret = dandelion_dmabuf_zero_copy_submit(&ring, &desc);
    assert(ret == 0);

    ret = dandelion_dmabuf_zero_copy_submit(&ring, &desc);
    assert(ret == 0);

    ret = dandelion_dmabuf_zero_copy_submit(&ring, &desc);
    assert(ret == -EAGAIN);

    printf("[PASS] test_zero_copy_submission_and_ring_full passed successfully.\n");
}

void test_teardown(void) {
    printf("[TEST] Running test_teardown...\n");
    dandelion_iouring_ring_t ring;
    dandelion_dmabuf_init_ring(&ring, 8);

    dandelion_dmabuf_teardown(&ring);
    assert(ring.ring_fd == -1);
    assert(ring.ring_entries == 0);
    assert(ring.flags == 0);

    printf("[PASS] test_teardown passed successfully.\n");
}

int main(void) {
    printf("=== STARTING DANDELION OS DMA-URING UNIT TESTS ===\n");
    test_ring_initialization();
    test_zero_copy_submission_and_ring_full();
    test_teardown();
    printf("=== ALL DMA-URING UNIT TESTS PASSED CLEANLY ===\n");
    return 0;
}
