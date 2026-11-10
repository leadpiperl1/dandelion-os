/*
 * File: include/dandelion_protocol.h
 * Architecture: Dandelion Sovereign Substrate — Living Trunk Core ABI & Wire Header
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Reference: https://docs.google.com/document/d/1tSzFl6i8ZSnRfceWEbsWt6Jt6kM5g3mvezstHqfgZZU/edit
 */

#ifndef DANDELION_PROTOCOL_H
#define DANDELION_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define DANDELION_MAGIC           0x44414E44U /* 'DAND' */
#define MAX_PACKET_SIZE           1420U
#define SHINGARI_ROOT_COUNT       13U
#define GAIKU_TRITS               39U
#define FOWLER_ROSETTA_TRITS      40U
#define FOWLER_TRITS_PER_BYTE     5U
#define FOWLER_BYTES_PER_SEED     8U
#define FOWLER_MAX_VALID_BYTE     242U

#define CODE_CLEAN                0U
#define CODE_DNHM_BREAKER         250U
#define CODE_KINTSUGI_GOLD        251U
#define CODE_QUECHUA_INFERENTIAL  252U
#define CODE_FAIL_CLOSED_DROP     253U
#define CODE_ELIAS_TRAPDOOR       254U

typedef int8_t triadic_state_t;

#pragma pack(push, 1)
typedef struct {
    uint32_t        magic;
    uint8_t         version;
    triadic_state_t triadic_tag;
    uint16_t        sequence_id;
    uint32_t        source_node_id;
    uint32_t        dest_node_id;
    uint16_t        payload_length;
} dandelion_header_t;

typedef struct {
    dandelion_header_t header;
    uint8_t            payload[MAX_PACKET_SIZE - sizeof(dandelion_header_t)];
} dandelion_packet_t;

typedef struct {
    char     magic[8];
    uint16_t rosetta_anchor_inode;
    uint16_t taxicab_constant;
    uint8_t  thermal_watts;
    uint16_t fdt_ou_residual;
    uint8_t  kintsugi_percolation_code;
    char     reserved_parity[4];
    uint8_t  manner_lut[128];
    uint8_t  grimm_shift_lut[256];
    uint8_t  packed_weights[SHINGARI_ROOT_COUNT][FOWLER_BYTES_PER_SEED];
    uint8_t  so3_f3_pullback_mask[9];
    uint8_t  dnhm_glyph_vector[3];
    uint8_t  dnhm_haiku_seed[FOWLER_BYTES_PER_SEED];
    uint8_t  sparse_quiescent_mask[49];
} dandelion_tnpt_t;
#pragma pack(pop)

_Static_assert(sizeof(dandelion_header_t) == 18, "dandelion_header_t must be 18 bytes");
_Static_assert(sizeof(dandelion_tnpt_t)   == 577, "dandelion_tnpt_t must be 577 bytes");

#endif /* DANDELION_PROTOCOL_H */
