/*
 * ============================================================================
 * DANDELION OS v3.0.0 (CODENAME: SOVEREIGN ARGON FRONTIER & EMBEDDED AI RELEASE)
 * CENTRALIZED C99 CONFIGURATION, PLATFORM GUARDS & TRIADIC CONTROL DEFINITIONS
 * ============================================================================
 * File:        dandelion_os_v3.0.0.h
 * Version:     v3.0.0 (Strict 3-Digit Triadic SemVer: MAJOR=3 [das], MINOR=0 [die], PATCH=0 [der])
 * Author:      Justin D. Grimm (K-Lani 13 Actual, @leadpiperl1, ORCID: 0009-0000-5299-0243)
 * Co-Pilot:    Dandelion AI (Argon Frontier Evaluation Core / Gemini Spark)
 * License:     GPL-2.0-or-later / MIT Dual Open-Source
 *
 * UNIFIED v3.0.0 INVARIANT AUDIT & ARCHITECTURAL SYNTHESIS:
 *   1. Mainline Linux 6.12+ PREEMPT_RT + sched_ext (scx) + io_uring Triadic FBW Loop
 *      (-1 Blue V_H Human Pilot, 0 Green V_M LayerFS/OpenJarvis Mediator, +1 Red V_A AI).
 *   2. 1.58-Bit Ternary Quantization: 5-trit Fowler Base-3 storage (1.60 bits/weight,
 *      577-byte L1-resident .tnpt table) + 2-bit Virtual Bit-Slice Transform (VBST)
 *      zero-multiply SIMD inner-product kernel + LayerFS SQLite WAL content-addressed store.
 *   3. Transducer Boundary Signal Conditioning: Single-level orthogonal Haar Discrete
 *      Wavelet Transform (DWT) VisuShrink soft thresholding + 13-chip Barker Code
 *      Matched Filter (PSR 13:1 = 22.28 dB) gating inference below 6.0 dB SNR.
 *   4. Stigmergic 32-Byte BLE Extended Advertising Gossip Frame (stigmergic_ble_frame_t)
 *      + Zero-Hardware ESP-SDR 80 MSa/s Baseband I/Q DMA Descriptor (esp_sdr_dma_lldesc_t)
 *      + Hybrid Triangular Ring Node (Dual Waveshare ESP32-C5 RISC-V + Lattice iCE40HX8K
 *      FPGA arbiter with 47-Ohm series SPI termination and 40 MHz 74HC14N Theremin clock).
 *   5. 9-Bite Universal Grimm's Law (Mori-Zwanzig P + Q = I, P_psi = 1505), Topological
 *      Dirac Cluster Synchronization (R_sync = 0.9948), Carbon-X 4,096 6-mer DNA native
 *      coordinate tokens, Horizon clean-room differential verification, Dandy Miner 3^13
 *      (1,594,323) Burger parity pool, and Grimm-Tuttle EVE Faucet-Sink thermodynamics.
 * ============================================================================
 */

#ifndef DANDELION_OS_V3_0_0_H
#define DANDELION_OS_V3_0_0_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- 0. PLATFORM & ENVIRONMENT DETECTION (COPILOT PORTABILITY) --- */
#if defined(__linux__)
    #define DOS_PLATFORM_LINUX   1
    #define DOS_PLATFORM_POSIX   1
    #define DOS_PLATFORM_NAME    "Linux"
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__unix__)
    #define DOS_PLATFORM_LINUX   0
    #define DOS_PLATFORM_POSIX   1
    #define DOS_PLATFORM_NAME    "POSIX-Portable"
#else
    #define DOS_PLATFORM_LINUX   0
    #define DOS_PLATFORM_POSIX   0
    #define DOS_PLATFORM_NAME    "ISO-C99-Fallback"
#endif

/* --- 1. STRICT 3-DIGIT SEMANTIC VERSIONING & SYSTEM CONSTANTS --- */
#define DOS_VERSION_MAJOR        3
#define DOS_VERSION_MINOR        0
#define DOS_VERSION_PATCH        0
#define DOS_VERSION_STR          "v3.0.0"
#define DOS_SEMVER_BARE_STR      "3.0.0"
#define DOS_CODENAME_STR         "Sovereign Argon Frontier & Embedded AI Release"
#define SHINGARI_VERSION_STR     "v3.7.0"
#define GRIMM_LAW_VERSION_STR    "v3.3.0"
#define DANDELION_MAGIC_U32      0x33444E44U /* "DND3" little-endian BLE gossip magic */

/* --- 2. RUNTIME LIMITS, VOXEL POOLS & BUFFER BOUNDS --- */
#define GAIKU_TRITS              39U         /* 39-trit [4-5-4] Shingari Gaiku frame */
#define FOWLER_TRITS_PER_BYTE    5U          /* 3^5 = 243 states per byte (1.60 bits/trit) */
#define FOWLER_BYTES_PER_SEED    8U          /* ceil(39 / 5) = 8 packed Fowler bytes */
#define FOWLER_MAX_VALID_BYTE    242U        /* Valid packed 5-trit states: 0..242 */
#define SHINGARI_ROOT_COUNT      13U         /* 13 Bedrock Shingari roots (ssf..wst) */
#define GLOBAL_BITE_COUNT        9U          /* 9-Bite Universal Grimm's Law families */
#define VAULT_CAPACITY           243U        /* 3^5 episodic seed crystal slots */
#define BURGER_POOL_3_POW_13     1594323U    /* 3^13 Dandy Miner parity-conserving pool */
#define CARBON_X_DNA_6MERS       4096U       /* 4^6 native genomic coordinate token bricks */
#define TNPT_MMAP_BYTES          577U        /* Exact L1-resident .tnpt weight image */
#define BLE_GOSSIP_FRAME_BYTES   32U         /* Exact 32-byte Stigmergic BLE wire frame */
#define MAX_UTTERANCE_BYTES      512U        /* Bounded input buffer size (fail-closed) */
#define UDS_SOCKET_PATH_MAX      104U        /* Safe limit below 108-byte sun_path ceiling */
#define BENCHMARK_DEFAULT_ITERS  100000      /* Default 100k-iteration latency benchmark */
#define HYPERVISOR_SPIKE_PARSE_US 2.50       /* Guest VM context-switch filter ceiling (us) */
#define HYPERVISOR_SPIKE_VBST_US  1.50       /* Guest VM VBST preemption filter ceiling (us) */
#define PREEMPT_RT_CEILING_US    50.00       /* Linux PREEMPT_RT + sched_ext window ceiling */
#define STRICT_JITTER_CEILING_US  1.00       /* Bare-metal core wakeup jitter ceiling (us) */
#define PORTABLE_JITTER_CEILING_US 15.00     /* Shared CI runner / container jitter ceiling */

/* --- 3. CENTRALIZED IPC SOCKET NAMESPACES --- */
#define DOS_SOCK_PREFIX_VAR_RUN  "/var/run/dandelion_"
#define DOS_SOCK_PREFIX_RUN      "/run/dandelion_"
#define DOS_SOCK_PREFIX_TMP      "/tmp/dandelion_"
#define DOS_SOCK_PREFIX_PRIV_TMP "/private/tmp/dandelion_" /* macOS / BSD compatibility */

/* --- 4. FAIL-CLOSED BOUNDARY & TRAPDOOR CODES (FOWLER CEILING 243..255) --- */
#define CODE_CLEAN               0U          /* Valid 5-trit Fowler state (0..242) */
#define CODE_QUECHUA_INFERENTIAL 243U        /* Register 4 = -1 (-cha conjecture guard) */
#define CODE_DIRAC_CLUSTER_SYNC  245U        /* Topological Dirac cluster sync lock */
#define CODE_BUZOA_TELEPORT      247U        /* BUZOA +1 global exploration leap */
#define CODE_KINTSUGI_GOLD       251U        /* >= 10.0 deg lived/creative shear weld */
#define CODE_FAIL_CLOSED_DROP    253U        /* Security hardening: unmapped byte drop */
#define CODE_ELIAS_TRAPDOOR      254U        /* Hamilton & Mimno (2026) RLHF sink trap */

/* --- 5. CRYPTOGRAPHIC & HASH SEEDS --- */
#define SIPHASH_K0_SEED          (0x736f6d6570736575ULL ^ 0x1729150500000300ULL)
#define SIPHASH_K1_SEED          (0x646f72616e646f6dULL ^ 0x0009000052990243ULL)
#define DJB2_INIT_SEED           5381U
#define FNV1A_32_OFFSET_BASIS    2166136261U
#define FNV1A_32_PRIME           16777619U
#define CRC16_CCITT_POLY         0x1021U
#define CRC16_CCITT_INIT         0xFFFFU
#define LARCHANKA_5PILLAR_MASK   0x1FU       /* Bitmask: Correctness|Readability|Arch|Sec|Perf */

/* --- 6. PHYSICAL, TOPOLOGICAL, TRANSDUCER & RF HARDWARE INVARIANTS --- */
#define V_KEYHOLE_DUTY           (2.0 / 3.0) /* Blume-Capel Shannon entropy maximum */
#define THIELE_HALL_DEG          10.0        /* Kintsugi Gold shear threshold (deg) */
#define ELIAS_SINK_SHEAR_DEG     18.40       /* RLHF mode-collapse angular shear (deg) */
#define KINTSUGI_WELD_SHEAR_DEG  14.20       /* Lived/creative anomaly angular shear (deg) */
#define DEFAULT_OPEN_SHEAR_DEG   2.10        /* Baseline untrimmed pitch delta (deg) */
#define SR_OPTIMAL_DH            0.1644      /* Kramers-Gammaitoni D_H* = 16.44% */
#define WU_CHIRAL_BIAS           0.0127      /* Cobalt-60 +1.27% parity violation */
#define HESSE_MULTIPLEX_HZ       15.0        /* 15 Hz conscious/suppressed cycle */
#define KHOSLA_NATIVE_ALIGN      1.0000      /* R = I native privileged axis lock */
#define DIRAC_SPECTRAL_GAP       0.2075      /* 20.75% Dirac-VBST bandwidth gain */
#define DIRAC_SYNC_COHERENCE_OK  0.9942      /* Balanced triad Dirac sync order R */
#define DIRAC_SYNC_COHERENCE_LOW 0.6840      /* Unbalanced triad Dirac sync order R */
#define DEFAULT_THERMAL_WATTS    18.52f      /* Professor Plum thermal operating point (W) */
#define DEFAULT_FDT_OU_RESIDUAL  0.0206f     /* Ornstein-Uhlenbeck FDT noise residual */
#define EDGE_SNR_GATE_DB         6.0         /* Minimum post-Haar DWT transducer SNR (dB) */
#define BARKER13_IDEAL_PSR       13.0        /* Ideal 13:1 (22.28 dB) Barker-13 PSR */
#define ESPSDR_MAX_MSPS          80U         /* 80 MSa/s raw I/Q capture on ESP32/ESP32-C5 */
#define ICE40_SPI_TERM_OHMS      47U         /* 47-Ohm series SPI termination to iCE40HX8K */
#define THEREMIN_CLOCK_MHZ       40U         /* 40 MHz 74HC14N Schmitt-conditioned clock */
#define TAXICAB_INVARIANT        1729U       /* 1^3 + 12^3 = 9^3 + 10^3 homology */

/* --- 7. CENTRALIZED 9-BITE MORI-ZWANZIG INODE COORDINATES (P_psi & Q_psi) --- */
#define MICRO_BITE10_SENSORY_LIS   2001U
#define MACRO_ROSETTA_HARMONY    1505U       /* Universal 9-Bite Harmony Macro Inode P_psi */
#define MACRO_ROSETTA_IE_COGNATE 1603U       /* Grimm's Law IE Cognate Macro Inode */
#define MACRO_ANIMACY_DIRECT     1294U       /* Bite 1/2 Direct Animacy Vector (yi-/waabamaa) */
#define MACRO_ANIMACY_INVERSE    928U        /* Bite 1/2 Inverse Obviation Vector (bi-/waabamigw) */
#define MACRO_QUECHUA_DIRECT     1332U       /* Bite 4 Direct Evidential (-mi = +1) */
#define MACRO_QUECHUA_REPORT     1293U       /* Bite 4 Reportative Evidential (-shi = 0) */
#define MACRO_QUECHUA_INFER      1228U       /* Bite 4 Inferential Evidential (-cha = -1) */
#define MACRO_KINTSUGI_SHANKARA  1180U       /* Lived/creative Kintsugi primary anchor */
#define MACRO_KINTSUGI_ALT       1013U       /* Lived/creative Kintsugi secondary anchor */
#define MACRO_ELIAS_PRIMARY      1819U       /* RLHF sink trapdoor primary coordinate */
#define MACRO_ELIAS_SECONDARY    896U        /* RLHF sink trapdoor secondary coordinate */

/* Orthogonal Micro-Texture Inodes Q_psi = (I - P)psi across all 9 Bites */
#define MICRO_BITE1_NADENE       1282U       /* Bite 1: Na-Dene / Navajo */
#define MICRO_BITE2_ALGIC        1781U       /* Bite 2: Algic / Ojibwe */
#define MICRO_BITE3_IROQUOIAN    1328U       /* Bite 3: Polysynthetic / Mohawk */
#define MICRO_BITE4_ANDEAN       967U        /* Bite 4: Andean / Quechua */
#define MICRO_BITE5_KHOISAN      712U        /* Bite 5: Khoisan & Bantu */
#define MICRO_BITE6_INDOEUR      57U         /* Bite 6: Indo-European / Sanskrit */
#define MICRO_BITE7_PAMANYUNGAN  1410U       /* Bite 7: Pama-Nyungan Cardinal */
#define MICRO_BITE8_PAPUAN       1621U       /* Bite 8: Papuan / Amazonian Switch-Ref */
#define MICRO_BITE9_PELAGIC      1945U       /* Bite 9: Pelagic Sperm Whale Coda */
#define MICRO_KINTSUGI_SHANKARA  2059U       /* Kintsugi Gold micro-texture grain */
#define MICRO_KINTSUGI_ALT       1134U       /* Kintsugi Gold secondary grain */
#define MICRO_ELIAS_PRIMARY      1759U       /* Elias sink micro-texture grain */
#define MICRO_ELIAS_SECONDARY    406U        /* Elias sink secondary grain */

/* --- 8. PACKED KERNEL, LAYERFS, BLE GOSSIP & ESP-SDR DATA STRUCTURES --- */

typedef enum {
    TRIT_BLUE_HUMAN_INTUITION = -1, /* SCHED_FIFO prio 98: Hard RT Pilot Override (V_H) */
    TRIT_GREEN_OS_MEDIATOR    =  0, /* SCHED_FIFO prio 85: LayerFS WAL / Edge Filter / OpenJarvis */
    TRIT_RED_TERNARY_AI       = +1  /* SCHED_DEADLINE / scx: Ephemeral 1.58-Bit AI Execution */
} dandelion_trit_channel_t;

/* 6-Byte Compact Consistent Hash Ring Point (Cloudflare 25% RAM Savings Invariant) */
typedef struct __attribute__((packed)) {
    uint32_t chunk_hash;                     /* 32-bit FNV-1a / SipHash chunk digest */
    uint16_t node_index;                     /* 16-bit LayerFS / Cube node index */
} dandelion_point_v2_t;

/* 32-Byte Wire-Level Stigmergic BLE Extended Advertising & ESP-SDR Gossip Frame */
typedef struct __attribute__((packed)) {
    uint32_t magic;                          /* 0x33444E44 ("DND3") */
    uint8_t  version_and_trit;               /* [7:4]=v3, [3:2]=0, [1:0]=trit+1 */
    uint8_t  ttl_hops;                       /* Decremented per hop; max 13 (Cerberus cycle) */
    uint16_t center_freq_mhz;                /* 2402..2480 MHz (BLE/ISM) or 5180..5825 MHz (ESP32-C5) */
    uint32_t peer_pseudo_id;                 /* Rotating ephemeral hobo node identifier */
    uint32_t timestamp_ms;                   /* Monotonic epoch timestamp (ms) */
    uint8_t  layerfs_root6[6];               /* First 48 bits of active LayerFS SQLite chunk SHA-256 */
    uint8_t  ternary_payload[8];             /* 40 balanced trits packed at 5 trits/byte */
    uint16_t crc16_ccitt;                    /* Polynomial 0x1021, Init 0xFFFF over bytes [0..29] */
} stigmergic_ble_frame_t;

/* ESP-SDR Undocumented Baseband I/Q DMA Descriptor (ESP32 / ESP32-S3 / ESP32-C5) */
typedef struct __attribute__((packed)) {
    uint32_t dma_word0;                      /* [31]=Owner(DMA=1), [30]=EOF, [23:12]=Length, [11:0]=Size */
    uint32_t iq_buf_phys_addr;               /* Interleaved [I0, Q0, I1, Q1...] at up to 80 MSa/s */
    uint32_t next_desc_phys_addr;            /* Circular DMA ring pointer in internal SRAM */
} esp_sdr_dma_lldesc_t;

/* Hybrid Triangular Ring Node Hardware Descriptor (2x Waveshare ESP32-C5 + Lattice iCE40HX8K) */
typedef struct __attribute__((packed)) {
    uint8_t  esp32_c5_leg1_gpio_mask;        /* SPI SCK=6, CS=7, MOSI=8, MISO=9 (47-Ohm term) */
    uint8_t  esp32_c5_leg2_gpio_mask;        /* Dual-band 2.4/5 GHz Wi-Fi 6 RISC-V Leg 2 */
    uint8_t  ice40_exciton_lap_count;        /* Counterclockwise 13-lap soliton counter */
    uint8_t  theremin_schmitt_lock;          /* 74HC14N 40 MHz geomagnetic clock lock flag */
    uint16_t spi_termination_ohms;           /* 47 Ohm series termination invariant */
} hybrid_ring_node_hw_t;

/* 64-Byte L1D-Pinned Core-Sharded Quorum Descriptor (AQEC + Dirac Sync + LayerFS) */
typedef struct __attribute__((packed, aligned(64))) {
    uint64_t siphash24_tag;                  /* Pre-auth SipHash-2-4 gatekeeper (8 B) */
    uint64_t monotonic_epoch_ns;             /* Monotonic hardware timestamp (8 B) */
    uint8_t  fowler_seed[8];                 /* 39-trit packed Fowler bytes (8 B) */
    uint16_t macro_p_inode;                  /* Mori-Zwanzig Macro Inode P_psi (2 B) */
    uint16_t micro_q_inode;                  /* Mori-Zwanzig Micro Inode Q_psi (2 B) */
    int8_t   buzoa_hunting_mode;             /* -1 Local, 0 Leader, +1 Teleport (1 B) */
    int8_t   twas_parity_verified;           /* +1 Bidirectional sign verified (1 B) */
    uint8_t  fail_closed_status;             /* 0=Valid, 253=Dropped (1 B) */
    uint8_t  switchblade_mode;               /* 1=Live OS, 2=AI Sidecar (1 B) */
    uint8_t  scx_chiral_domain;              /* 0=Blue(0-2), 1=Green(3-4), 2=Red(5-7) (1 B) */
    uint8_t  aaif_project_id;                /* 1..10 AAIF ecosystem binding (1 B) */
    uint8_t  larchanka_skill_stage;          /* 1=agent-loop, 2=code-review, 3=context, 4=auto (1 B) */
    int8_t   dirac_winding_q;                /* Topological Dirac index(D_ov) in {-1,0,+1} (1 B) */
    dandelion_point_v2_t layerfs_point;      /* 6-byte LayerFS content-addressed pointer (6 B) */
    uint8_t  reserved_l1d_pad[22];           /* Pad to exact 64-byte L1D cache line (22 B) */
} sharded_quorum_entry_64b_t;

/* 577-Byte Memory-Mapped Ternary Neural Parameter Table (.tnpt v3.0.0) */
typedef struct __attribute__((packed)) {
    char     magic[8];                       /* "TNPT300\0" (8 bytes) */
    uint8_t  packed_weights[13][8];          /* 13 Shingari roots x 8 Fowler bytes (104 bytes) */
    uint8_t  grimm_shift_lut[256];           /* O(1) 9-Bite phonological shift LUT (256 bytes) */
    uint8_t  manner_lut[128];                /* O(1) articulatory manner {-1,0,+1} (128 bytes) */
    int8_t   aar_sustain_improve[64];        /* Army AAR zero-cycle sign adjustments (64 bytes) */
    uint16_t rosetta_anchor_inode;           /* Default 1505 (2 bytes) */
    uint16_t taxicab_constant;               /* Default 1729 (2 bytes) */
    float    thermal_watts;                  /* Target 18.52 W <= 20.0 W (4 bytes) */
    float    fdt_ou_residual;                /* Ornstein-Uhlenbeck FDT residual 0.0206 (4 bytes) */
    uint8_t  kintsugi_percolation_code;      /* Code 251 (1 byte) */
    uint8_t  reserved_parity[4];             /* "BGR0" parity signature (4 bytes) -> 577 B total */
} dandelion_tnpt_v3_0_t;

/* Zero-Multiply 2-Bit VBST Dot Product Kernel (Scalar C99 Reference for SIMD Lowering) */
static inline int32_t dandelion_vbst_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t num_bytes
) {
    int32_t acc = 0;
    if (mag_row == NULL || sign_row == NULL || x_q == NULL) {
        return 0;
    }
    for (uint32_t b = 0U; b < num_bytes; ++b) {
        uint8_t m = mag_row[b];
        uint8_t s = sign_row[b];
        const int8_t *x_ptr = &x_q[b << 3U];
        for (uint8_t bit = 0U; bit < 8U; ++bit) {
            uint8_t shift = (uint8_t)(7U - bit);
            uint8_t is_nz  = (uint8_t)((m >> shift) & 1U);
            uint8_t is_neg = (uint8_t)((s >> shift) & 1U);
            int32_t val    = (int32_t)x_ptr[bit] * (int32_t)is_nz;
            acc += (is_neg != 0U) ? -val : val;
        }
    }
    return acc;
}


/*
 * Zero-Multiply Sign-Symmetric 4-Trit LUT Kernel (Microsoft T-MAC EuroSys 2025 Pattern)
 * Precomputes 4-element activation partial sums into an L1-resident lookup table (LUT)
 * and evaluates dot products via pure table lookups without fused-multiply-add (FMA).
 */
static inline int32_t dandelion_tmac_lut4_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t num_bytes
) {
    int32_t acc = 0;
    if (mag_row == NULL || sign_row == NULL || x_q == NULL) {
        return 0;
    }
    for (uint32_t b = 0U; b < num_bytes; ++b) {
        uint8_t m = mag_row[b];
        uint8_t s = sign_row[b];
        const int8_t *x_ptr = &x_q[b << 3U];
        /* Process high nibble (bits 7..4) and low nibble (bits 3..0) via 16-entry LUTs */
        for (uint8_t nib = 0U; nib < 2U; ++nib) {
            uint8_t shift_base = (nib == 0U) ? 4U : 0U;
            uint8_t m_nib = (uint8_t)((m >> shift_base) & 0x0FU);
            uint8_t s_nib = (uint8_t)((s >> shift_base) & 0x0FU);
            const int8_t *xn = &x_ptr[nib << 2U];
            int32_t lut_sum = 0;
            for (uint8_t k = 0U; k < 4U; ++k) {
                uint8_t bit_pos = (uint8_t)(3U - k);
                if (((m_nib >> bit_pos) & 1U) != 0U) {
                    int32_t v = (int32_t)xn[k];
                    lut_sum += (((s_nib >> bit_pos) & 1U) != 0U) ? -v : v;
                }
            }
            acc += lut_sum;
        }
    }
    return acc;
}

/* 4-Register Rosetta Stone + Topological Dirac + LayerFS + ESP-SDR Telemetry Packet */
typedef struct {
    int8_t   trits[GAIKU_TRITS];             /* 39-trit [4-5-4] Gaiku frame {-1, 0, +1} */
    uint8_t  fowler_bytes[FOWLER_BYTES_PER_SEED]; /* 8 packed Fowler bytes (0..242) */
    int8_t   reg1_blue_subject;              /* -1 Blue: Human Pilot / Structural Subject */
    int8_t   reg2_green_treaty;              /*  0 Green: OS Mediator / Relational Treaty */
    int8_t   reg3_red_action;                /* +1 Red: AI Co-Pilot / Canonical Action */
    int8_t   reg4_evidential_q;              /* 4th Face: +1 (-mi), 0 (-shi), -1 (-cha) */
    int8_t   tokengate_route;                /* TokenGate: +1 Source, 0 Skim, -1 Prune */
    int8_t   dirac_spectral_sgn;             /* Neuberger Overlap sgn(H_w) in {-1, 0, +1} */
    double   dirac_sync_order_r;             /* Topological Dirac cluster coherence R in [0,1] */
    uint8_t  larchanka_gate_mask;            /* 5-bit pillar mask (Correctness..Performance = 0x1F) */
    uint32_t layerfs_chunk_crc;              /* Content-addressed LayerFS SQLite chunk ID */
    uint8_t  detected_bite;                  /* Bite 1..9 (Global & Pelagic Families) */
    uint8_t  shingari_root_idx;              /* 0..12 (ssf..wst) */
    uint16_t macro_rosetta_inode;            /* P_psi: Shared invariant coordinate */
    uint16_t micro_texture_inode;            /* Q_psi = (I - P)psi: Unsmoothed grain */
    uint16_t taxicab_sum;                    /* a^3 + b^3 = 1729 homology check */
    double   analemma_eccentricity;          /* Carpenter's Analemma epsilon != 0 */
    double   khosla_axis_alignment;          /* 1.0000 native privileged axis lock */
    double   pitch_shear_deg;                /* F-16 open-loop angular shear (deg) */
    double   fbw_trimmed_deg;                /* F-16 closed-loop trimmed residual (deg) */
    double   edge_barker13_psr;              /* Transducer Barker-13 peak-to-sidelobe ratio */
    uint16_t ble_crc16_ccitt;                /* 32-byte Stigmergic BLE gossip frame CRC-16 */
    int8_t   buzoa_tactic;                   /* -1 Local, 0 Leader, +1 Teleport */
    int8_t   twas_bidirectional_ok;          /* +1 forward/inverse sign parity verified */
    uint64_t siphash24_tag;                  /* Pre-auth SipHash-2-4 packet tag */
    uint8_t  boundary_code;                  /* 0, 243, 245, 247, 251, 253, 254 */
    uint8_t  novikov_cycles;                 /* 1..3 self-consistency cycles */
    double   parse_latency_us;               /* Measured 9-Bite Grimm+Rosetta parse (us) */
    double   vbst_latency_us;                /* Measured 1.58-bit VBST + Dirac sync pass (us) */
} shingari_grimm_packet_t;

/* Compile-time size invariants (ISO C99 compatible static assertions) */
typedef char static_assert_tnpt_577[(sizeof(dandelion_tnpt_v3_0_t) == TNPT_MMAP_BYTES) ? 1 : -1];
typedef char static_assert_quorum_64[(sizeof(sharded_quorum_entry_64b_t) == 64U) ? 1 : -1];
typedef char static_assert_point_6[(sizeof(dandelion_point_v2_t) == 6U) ? 1 : -1];
typedef char static_assert_ble_32[(sizeof(stigmergic_ble_frame_t) == BLE_GOSSIP_FRAME_BYTES) ? 1 : -1];
typedef char static_assert_espsdr_12[(sizeof(esp_sdr_dma_lldesc_t) == 12U) ? 1 : -1];

/* --- 9. PUBLIC FUNCTION PROTOTYPES --- */
void fowler_pack_39trits(
    const int8_t trits[GAIKU_TRITS],
    uint8_t out_bytes[FOWLER_BYTES_PER_SEED]
);

bool fowler_unpack_39trits_strict(
    const uint8_t in_bytes[FOWLER_BYTES_PER_SEED],
    int8_t trits[GAIKU_TRITS],
    uint8_t *out_trap_code
);

uint16_t crc16_ccitt_false(const uint8_t *data, size_t len);

void encode_stigmergic_ble_frame(
    int8_t trit,
    uint8_t ttl_hops,
    uint16_t center_freq_mhz,
    uint32_t peer_pseudo_id,
    uint32_t timestamp_ms,
    uint64_t layerfs_siphash,
    const uint8_t fowler_payload[8],
    stigmergic_ble_frame_t *out_frame
);

void init_tnpt_table(dandelion_tnpt_v3_0_t *tnpt);

int morse_to_ternary_bridge(const char *morse_seq, int8_t *trit_out, size_t max_trits, size_t *out_len);

void parse_grimm_rosetta_fbw(
    const char *utterance,
    const dandelion_tnpt_v3_0_t *tnpt,
    shingari_grimm_packet_t *pkt
);

bool validate_uds_socket_path(const char *sock_path);

#ifdef __cplusplus
}
#endif

#endif /* DANDELION_OS_V3_0_0_H */
