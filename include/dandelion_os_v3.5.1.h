/*
 * ============================================================================
 * DANDELION OS v4.1.1 / v3.5.1 (CODENAME: URSINE & URSHWERA / HEIMAT-AGORA)
 * ============================================================================
 * File: include/dandelion_os_v3.5.1.h
 * Version: v4.1.1 (Strict 3-Digit Triadic SemVer: MAJOR=4, MINOR=1, PATCH=1)
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
 * Affiliation: Independent Dandelion Substrate Research Group, Gurley, AL, USA
 * License: GPL-2.0-or-later / MIT Dual Open-Source
 * ============================================================================
 */

#ifndef DANDELION_OS_V3_5_1_H
#define DANDELION_OS_V3_5_1_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --- 1. COMPILE-TIME CONSTANTS & TRAPDOOR BOUNDARY CODES (243..255) --- */
#define SHINGARI_ROOT_COUNT          13U
#define GAIKU_TRITS                  39U
#define FOWLER_ROSETA_TRITS          40U
#define FOWLER_TRITS_PER_BYTE        5U
#define FOWLER_BYTES_PER_SEED        8U
#define FOWLER_MAX_VALID_BYTE        242U
#define MAX_UTTERANCE_BYTES          512U
#define UDS_SOCKET_PATH_MAX          108U

#define CHAIR44_RAW_STATES           44U
#define SHINGARI_BOUNDED_TRITS       42U
#define CHAIR44_LEGAL_ORIENTATIONS   30U
#define KRAMHAUS_TAU_CRIT_UNM        49426U /* 49.4255 mN*m scaled to uN*m */

#define CODE_CLEAN                   0U
#define CODE_DNHM_BREAKER            250U   /* "Dave's not here, man." lateral kick */
#define CODE_KINTSUGI_GOLD           251U   /* High-shear creative/structural weld */
#define CODE_QUECHUA_INFERENTIAL     252U   /* Bite 4 -cha inferential boundary */
#define CODE_FAIL_CLOSED_DROP        253U   /* Strict fail-closed hardware override */
#define CODE_ELIAS_TRAPDOOR          254U   /* Elias RLHF / bureaucratic committee sink */

#define MACRO_ROSETTA_HARMONY        1000U
#define MACRO_ROSETTA_IE_COGNATE     1006U
#define MICRO_BITE6_INDOEUR          1600U
#define MICRO_BITE10_SENSORY_LIS     2001U
#define TAXICAB_INVARIANT            1729U
#define DEFAULT_THERMAL_WATTS        18U    /* 18.50 W nominal integer floor */
#define DEFAULT_FDT_OU_RESIDUAL      206U   /* 2.06% equipartition residual */

#define DANDELION_MAGIC_U32          0x33444E44U /* "DND3" */
#define CRC16_CCITT_INIT             0xFFFFU
#define CRC16_CCITT_POLY             0x1021U
#define SIPHASH_K0_SEED              0x736F6D6570736575ULL
#define SIPHASH_K1_SEED              0x646F72616E646F6DULL

/* --- 2. EXACT BYTE-ALIGNED KERNEL STRUCTURES (LINUS'S LAW GATES) --- */
#pragma pack(push, 1)

/* 577-byte L1-resident Ternary Neural / Phonological Table (.tnpt) */
typedef struct {
    char     magic[8];                                                /*   8 B */
    uint16_t rosetta_anchor_inode;                                    /*   2 B */
    uint16_t taxicab_constant;                                        /*   2 B */
    uint8_t  thermal_watts;                                           /*   1 B */
    uint16_t fdt_ou_residual;                                         /*   2 B */
    uint8_t  kintsugi_percolation_code;                               /*   1 B */
    char     reserved_parity[4];                                      /*   4 B */
    uint8_t  manner_lut[128];                                         /* 128 B */
    uint8_t  grimm_shift_lut[256];                                    /* 256 B */
    uint8_t  packed_weights[SHINGARI_ROOT_COUNT][FOWLER_BYTES_PER_SEED]; /* 104 B */
    uint8_t  so3_f3_pullback_mask[9];                                 /*   9 B */
    uint8_t  dnhm_glyph_vector[3];                                    /*   3 B */
    uint8_t  dnhm_haiku_seed[FOWLER_BYTES_PER_SEED];                  /*   8 B */
    uint8_t  sparse_quiescent_mask[49];                               /*  49 B -> Total = 577 B */
} dandelion_tnpt_v3_0_t;

/* 64-byte Per-Reference Git Quorum & Epistemic Ledger Entry */
typedef struct {
    uint64_t ref_siphash;          /*  8 B */
    uint64_t commit_epoch_us;      /*  8 B */
    uint32_t macro_rosetta_inode;  /*  4 B */
    uint32_t micro_texture_inode;  /*  4 B */
    uint32_t torque_unm;           /*  4 B */
    uint16_t taxicab_invariant;    /*  2 B */
    uint8_t  boundary_code;        /*  1 B */
    int8_t   evidential_trit;      /*  1 B */
    uint8_t  fowler_seed[8];       /*  8 B */
    char     ref_branch[24];       /* 24 B -> Total = 64 B */
} sharded_quorum_entry_64b_t;

/* 6-byte LayerFS Content-Addressed Voxel / Phase Point */
typedef struct {
    int16_t x_l1;       /* 2 B */
    int16_t y_l1;       /* 2 B */
    int8_t  z_l1;       /* 1 B */
    int8_t  trit_state; /* 1 B -> Total = 6 B */
} dandelion_point_v2_t;

/* 32-byte Stigmergic BLE / DND3 Mesh Frame */
typedef struct {
    uint32_t magic;              /* 4 B */
    uint8_t  version_and_trit;   /* 1 B */
    uint8_t  ttl_hops;           /* 1 B */
    uint16_t center_freq_mhz;    /* 2 B */
    uint32_t peer_pseudo_id;     /* 4 B */
    uint32_t timestamp_ms;       /* 4 B */
    uint8_t  layerfs_root6[6];   /* 6 B */
    uint8_t  ternary_payload[8]; /* 8 B */
    uint16_t crc16_ccitt;        /* 2 B -> Total = 32 B */
} stigmergic_ble_frame_t;

/* 12-byte ESP-SDR DMA Linked-List Descriptor */
typedef struct {
    uint32_t size_and_length;     /* 4 B */
    uint32_t buf_phys_addr;       /* 4 B */
    uint32_t next_desc_phys_addr; /* 4 B -> Total = 12 B */
} esp_sdr_dma_lldesc_t;

#pragma pack(pop)

/* Compile-Time Linus's Law Byte-Alignment Assertions */
_Static_assert(sizeof(dandelion_tnpt_v3_0_t) == 577U, "dandelion_tnpt_v3_0_t must be exactly 577 bytes");
_Static_assert(sizeof(sharded_quorum_entry_64b_t) == 64U, "sharded_quorum_entry_64b_t must be exactly 64 bytes");
_Static_assert(sizeof(dandelion_point_v2_t) == 6U, "dandelion_point_v2_t must be exactly 6 bytes");
_Static_assert(sizeof(stigmergic_ble_frame_t) == 32U, "stigmergic_ble_frame_t must be exactly 32 bytes");
_Static_assert(sizeof(esp_sdr_dma_lldesc_t) == 12U, "esp_sdr_dma_lldesc_t must be exactly 12 bytes");

/* --- 3. RUNTIME PACKET & TELEMETRY STATE --- */
typedef struct {
    uint64_t siphash24_tag;
    uint32_t macro_rosetta_inode;
    uint32_t micro_texture_inode;
    uint32_t kramhaus_torque_unm;
    uint16_t taxicab_sum;
    uint8_t  detected_bite;
    uint8_t  shingari_root_idx;
    uint8_t  boundary_code;
    uint8_t  novikov_cycles;
    int8_t   reg1_blue_subject;
    int8_t   reg2_green_treaty;
    int8_t   reg3_red_action;
    int8_t   reg4_evidential_q;
    int8_t   tokengate_route;
    int8_t   glyph_vector[3];
    uint8_t  fowler_haiku_seed[FOWLER_BYTES_PER_SEED];
    double   operational_entropy_bits;
    double   psi_bureaucracy_clamp;
} shingari_grimm_packet_t;

/* --- 4. EXPORTED KERNEL API --- */
static inline int8_t mod3_sum(int8_t a, int8_t b) {
    return (int8_t)(((int)a + (int)b + 4) % 3 - 1);
}

void init_tnpt_table(dandelion_tnpt_v3_0_t *tnpt);
void fowler_pack_39trits(const int8_t trits[GAIKU_TRITS], uint8_t out_bytes[FOWLER_BYTES_PER_SEED]);
bool fowler_unpack_39trits_strict(const uint8_t in_bytes[FOWLER_BYTES_PER_SEED], int8_t trits[GAIKU_TRITS], uint8_t *out_trap_code);
int  morse_to_ternary_bridge(const char *morse_seq, int8_t *trit_out, size_t max_trits, size_t *out_len);
uint32_t scx_kramhaus_torque_governor(const uint16_t *radii_mm, const uint8_t *sin_theta_q8, size_t n_tasks, uint8_t *out_boundary_code);
int32_t  vbst_dot40_popcnt(uint64_t pos_a, uint64_t neg_a, uint64_t pos_b, uint64_t neg_b, uint64_t exclusion_mask);
void dnhm_stochastic_impulse_pivot(const dandelion_tnpt_v3_0_t *tnpt, shingari_grimm_packet_t *pkt);
void parse_grimm_rosetta_fbw(const char *utterance, const dandelion_tnpt_v3_0_t *tnpt, shingari_grimm_packet_t *pkt);

#endif /* DANDELION_OS_V3_5_1_H */
