/**
 * @file dandelion_dma_uring.h
 * @brief Dandelion OS v4.2.0 - Zero-Copy DMA-BUF and io_uring Pipeline Header
 * @author Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 */

#ifndef DANDELION_DMA_URING_H
#define DANDELION_DMA_URING_H

#include <stdint.h>
#include <stddef.h>

#define DANDELION_URING_QUEUE_DEPTH 64
#define DANDELION_CACHE_LINE_SIZE   64

typedef struct __attribute__((aligned(64))) {
    int32_t  dmabuf_fd;        // Signed file descriptor to support < 0 error checks
    uint32_t map_flags;
    uint64_t physical_address;
    uint64_t virtual_address;
    size_t   buffer_size;
    uint32_t reference_count;
    uint8_t  reserved[20];     // Adjusted padding to maintain 64-byte alignment
} dandelion_dmabuf_desc_t;

typedef struct __attribute__((aligned(64))) {
    int32_t  ring_fd;
    uint32_t head_offset;
    uint32_t tail_offset;
    uint32_t ring_mask;
    uint32_t ring_entries;
    uint32_t flags;
    uint8_t  pad[36];
} dandelion_iouring_ring_t;

int dandelion_dmabuf_init_ring(dandelion_iouring_ring_t *ring, uint32_t entries);
int dandelion_dmabuf_zero_copy_submit(dandelion_iouring_ring_t *ring, dandelion_dmabuf_desc_t *desc);
void dandelion_dmabuf_teardown(dandelion_iouring_ring_t *ring);

#endif /* DANDELION_DMA_URING_H */
