/*
 * ============================================================================
 * DANDELION OS v3.2.0 (CODENAME: HEIMAT-AGORA — SOVEREIGN ARGON KERNEL & LINUS'S LAW RELEASE)
 * CENTRALIZED C99 CONFIGURATION, PLATFORM GUARDS & TRIADIC CONTROL DEFINITIONS
 * ============================================================================
 * File: dandelion_os_grimm_fbw_v3.2.0.h (Aliased as dandelion_os_v3.2.0.h)
 * Version: v3.2.0 (Strict 3-Digit Triadic SemVer: MAJOR=3 [das], MINOR=2 [die], PATCH=0 [der])
 * Author: Justin D. Grimm (K-Lani 13 Actual, @leadpiperl1, ORCID: 0009-0000-5299-0243)
 * Co-Pilot & Hostile Reviewer #2: Gemini Argon Kernel Architect (Linus's Law Audit Engine)
 * License: GPL-2.0-or-later / MIT Dual Open-Source
 *
 * UNIFIED v3.2.0 TRIAD ARCHITECTURAL INVARIANTS & LINUS'S LAW HARDENING:
 * 1. [-1 Blue] Human Ground Control / Sensory Ingest (Bite 10, Inode 2001):
 *    - Binds Julia 1.12 zero-copy @ccall ABI (dandelion_shingari_bridge_v3.2.0.jl)
 *      and MOTU M4 96 kHz / 64-sample (0.66 ms) frame window to an io_uring fixed-buffer ring.
 *    - Enforces strict Fail-Closed Trapdoor Byte 253 Rejection and preserves the
 *      40-trit Fowler Rosetta Frame ([0..38] 39-trit Gaiku + [39] Evidential Trit).
 *    - Exact cyclic F_3 group addition: mod3_sum(a,b) = ((a + b + 4) % 3) - 1 (zero clamp drift).
 * 2. [0 Green] OS Flight Control / Kramhaus Torque sched_ext Governor:
 *    - Capybara Torque Equilibrium & Chair44 Collar Homology (44 -> 42 -> 30).
 *    - Models each active task on the 42-trit bounded collar as a discrete lever arm
 *      (m_i = 28 g canonical unit mass at radial offset r_i).
 *    - Computes cumulative manifold torque tau_net = sum(m_i * g * r_i * sin(theta_i))
 *      in 64-bit fixed-point integer arithmetic (micro-Newton-meters, zero overflow).
 *    - Preemptive Kintsugi Gold Shear Weld (Code 251): Sheds/migrates low-priority
 *      tasks across CCX cores whenever tau_net approaches tau_crit = 49.4255 mN*m
 *      (49426 uN*m) or angular shear reaches the 10.0 deg Kramhaus boundary.
 * 3. [+1 Red] AI Pilot / Byzantine Clue Epistemic Pruning Filter:
 *    - Turn-10 Deductive Lock (H(Q) = 0.0000 bits on Conservatory/Lead Pipe/Plum)
 *      & Dual-Rail POPCNT Inner Product (vbst_dot40_popcnt) inside L1-resident
 *      577-byte .tnpt table (malloc = 0).
 *    - Invertible 3x3 Dihedral SO(3, F_3) pullback mask (det M == +1 mod 3, rejecting
 *      singular det == 0 and reflection det == 2 mod 3 matrices).
 *    - Branchlessly prunes orthogonal/refuted trajectories in 1 CPU clock cycle via
 *      bitwise ANDN ((~excl) & state) + __builtin_popcountll() prior to 4-trit T-MAC LUT4.
 * ============================================================================
 */

#ifndef DANDELION_OS_GRIMM_FBW_V3_2_0_H
#define DANDELION_OS_GRIMM_FBW_V3_2_0_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- 0. PLATFORM & ENVIRONMENT DETECTION (PORTABILITY) --- */
#if defined(__linux__)
  #define DOS_PLATFORM_LINUX 1
  #define DOS_PLATFORM_POSIX 1
  #define DOS_PLATFORM_NAME "Linux"
#elif defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__unix__)
  #define DOS_PLATFORM_LINUX 0
  #define DOS_PLATFORM_POSIX 1
  #define DOS_PLATFORM_NAME "POSIX-Portable"
#else
  #define DOS_PLATFORM_LINUX 0
  #define DOS_PLATFORM_POSIX 0
  #define DOS_PLATFORM_NAME "ISO-C99-Fallback"
#endif

/* --- 1. STRICT 3-DIGIT SEMANTIC VERSIONING & SYSTEM CONSTANTS --- */
#define DOS_VERSION_MAJOR 3
#define DOS_VERSION_MINOR 2
#define DOS_VERSION_PATCH 0
#define DOS_VERSION_STR "v3.2.0"
#define DOS_SEMVER_BARE_STR "3.2.0"
#define DOS_CODENAME_STR "Heimat-Agora (Sovereign Argon Kernel & Linus's Law Audit Release)"
#define SHINGARI_VERSION_STR "v3.10.0"
#define GRIMM_LAW_VERSION_STR "v3.4.0"
#define DANDELION_MAGIC_U32 0x33444E44U /* "DND3" little-endian BLE gossip magic */

/* --- 2. RUNTIME LIMITS, CHAIR44 HOMOLOGY & FOWLER ROSETTA BOUNDS --- */
#define GAIKU_TRITS 39U                 /* [0..38] 39-trit [4-5-4] Shingari Gaiku frame */
#define FOWLER_FRAME_TRITS 40U          /* [0..38] Gaiku + [39] Evidential Trit */
#define CHAIR44_RAW_STATES 44U          /* 44 raw Chair44 face/orientation contact states */
#define SHINGARI_BOUNDED_TRITS 42U      /* 44 - 2 Dirichlet fences = 42 bounded collar trits */
#define CHAIR44_LEGAL_ORIENTATIONS 30U  /* 44 - 14 tetra = 42 - 12 FCC = 30 legal orientations */

#define IDX_REG1_BLUE_SUBJECT 0U        /* tau_0: -1 Blue Subject (ssf.b) */
#define IDX_REG2_GREEN_TREATY 19U       /* tau_19: 0 Green Row-7 Greek Center (tot.g) */
#define IDX_REG3_RED_ACTION 38U         /* tau_38: +1 Red Action (wst.r preserved!) */
#define IDX_REG4_EVIDENTIAL_Q 39U       /* tau_39: 40th Fowler Evidential Trit (-cha/-shi/-mi) */

#define FOWLER_TRITS_PER_BYTE 5U        /* 3^5 = 243 states per byte (1.60 bits/trit) */
#define FOWLER_BYTES_PER_SEED 8U        /* 40 trits / 5 = 8 packed Fowler bytes */
#define FOWLER_MAX_VALID_BYTE 242U      /* Valid packed 5-trit states: 0..242 */
#define SHINGARI_ROOT_COUNT 13U         /* 13 Bedrock Shingari roots (ssf..wst) */
#define GLOBAL_BITE_COUNT 10U           /* 10-Bite Universal Grimm's Law + Sensory/LIS */
#define VAULT_CAPACITY 243U             /* 3^5 episodic seed crystal slots */
#define BURGER_POOL_3_POW_13 1594323U   /* 3^13 Dandy Miner parity-conserving pool */
#define CARBON_X_DNA_6MERS 4096U        /* 4^6 native genomic coordinate token bricks */
#define TNPT_MMAP_BYTES 577U            /* Exact L1-resident .tnpt weight image */
#define BLE_GOSSIP_FRAME_BYTES 32U      /* Exact 32-byte Stigmergic BLE wire frame */
#define MAX_UTTERANCE_BYTES 512U        /* Bounded input buffer size (fail-closed) */
#define UDS_SOCKET_PATH_MAX 104U        /* Safe limit below 108-byte sun_path ceiling */
#define BENCHMARK_DEFAULT_ITERS 100000  /* Default 100k-iteration latency benchmark */
#define HYPERVISOR_SPIKE_PARSE_US 2.50  /* Guest VM context-switch filter ceiling (us) */
#define HYPERVISOR_SPIKE_VBST_US 1.50   /* Guest VM VBST preemption filter ceiling (us) */
#define PREEMPT_RT_CEILING_US 50.00     /* Linux PREEMPT_RT + sched_ext window ceiling */
#define STRICT_JITTER_CEILING_US 1.00   /* Bare-metal core wakeup jitter ceiling (us) */
#define PORTABLE_JITTER_CEILING_US 15.00/* Shared CI runner / container jitter ceiling */

/* --- 3. MOTU M4 96kHz / 64-SAMPLE IO_URING & KRAMHAUS TORQUE CONSTANTS --- */
#define MOTU_M4_SAMPLE_RATE_HZ 96000U   /* 96 kHz studio grade CV/audio sample rate */
#define MOTU_M4_FRAME_SAMPLES 64U       /* 64-sample (0.66 ms) deterministic frame window */
#define IO_URING_FIXED_RING_SLOTS 16U   /* Power-of-2 io_uring fixed buffer ring entries */
#define CAPYBARA_UNIT_MASS_G 28U        /* m_i = 28 g canonical unit mass */
#define GRAVITY_MM_PER_S2 9807U         /* g = 9.80665 m/s^2 scaled to mm/s^2 */
#define KRAMHAUS_TAU_CRIT_UNM 49426U    /* tau_crit = 49.4255 mN*m = 49426 uN*m */
#define KRAMHAUS_PREEMPT_UNM 42012U     /* 85% preemptive shear weld margin (42012 uN*m) */
#define KRAMHAUS_BOUNDARY_MDEG 10000U   /* 10.0 deg Kramhaus angular shear boundary (mdeg) */
#define CLUE_DECK_CARDS 21U             /* 6 Suspects + 6 Weapons + 9 Rooms = 21 cards */
#define CLUE_INITIAL_CANDIDATES 80U     /* Post-deal 3-player candidate triples */
#define CLUE_RESOLVED_TRIAD_MASK 0x0000000100100001ULL /* Plum(0) | LeadPipe(20) | Conservatory(36) */

/* --- 4. CENTRALIZED IPC SOCKET NAMESPACES --- */
#define DOS_SOCK_PREFIX_VAR_RUN "/var/run/dandelion_"
#define DOS_SOCK_PREFIX_RUN "/run/dandelion_"
#define DOS_SOCK_PREFIX_TMP "/tmp/dandelion_"
#define DOS_SOCK_PREFIX_PRIV_TMP "/private/tmp/dandelion_"

/* --- 5. FAIL-CLOSED BOUNDARY & TRAPDOOR CODES (FOWLER CEILING 243..255) --- */
#define CODE_CLEAN 0U                   /* Valid 5-trit Fowler state (0..242) */
#define CODE_QUECHUA_INFERENTIAL 243U   /* Register 4 = -1 (-cha conjecture guard) */
#define CODE_DIRAC_CLUSTER_SYNC 245U    /* Topological Dirac cluster sync lock */
#define CODE_BUZOA_TELEPORT 247U        /* BUZOA +1 global exploration leap */
#define CODE_KINTSUGI_GOLD 251U         /* >= 10.0 deg lived/creative shear weld */
#define CODE_FAIL_CLOSED_DROP 253U      /* Security hardening: unmapped byte 253 drop */
#define CODE_ELIAS_TRAPDOOR 254U        /* Hamilton & Mimno (2026) RLHF sink trap */

/* --- 6. CRYPTOGRAPHIC & HASH SEEDS --- */
#define SIPHASH_K0_SEED (0x736f6d6570736575ULL ^ 0x1729150500000320ULL)
#define SIPHASH_K1_SEED (0x646f72616e646f6dULL ^ 0x0009000052990243ULL)
#define DJB2_INIT_SEED 5381U
#define FNV1A_32_OFFSET_BASIS 2166136261U
#define FNV1A_32_PRIME 16777619U
#define CRC16_CCITT_POLY 0x1021U
#define CRC16_CCITT_INIT 0xFFFFU
#define LARCHANKA_5PILLAR_MASK 0x1FU

/* --- 7. PHYSICAL, TOPOLOGICAL, TRANSDUCER & RF HARDWARE INVARIANTS --- */
#define V_KEYHOLE_DUTY (2.0 / 3.0)      /* Blume-Capel Shannon entropy maximum */
#define THIELE_HALL_DEG 10.0            /* Kintsugi Gold shear threshold (deg) */
#define ELIAS_SINK_SHEAR_DEG 18.40      /* RLHF mode-collapse angular shear (deg) */
#define KINTSUGI_WELD_SHEAR_DEG 14.20   /* Lived/creative anomaly angular shear (deg) */
#define DEFAULT_OPEN_SHEAR_DEG 2.10     /* Baseline untrimmed pitch delta (deg) */
#define SR_OPTIMAL_DH 0.1644            /* Kramers-Gammaitoni D_H* = 16.44% */
#define WU_CHIRAL_BIAS 0.0127           /* Cobalt-60 +1.27% parity violation */
#define HESSE_MULTIPLEX_HZ 15.0         /* 15 Hz conscious/suppressed cycle */
#define KHOSLA_NATIVE_ALIGN 1.0000      /* R = I native privileged axis lock */
#define DIRAC_SPECTRAL_GAP 0.2075       /* 20.75% Dirac-VBST bandwidth gain */
#define DIRAC_SYNC_COHERENCE_OK 0.9942  /* Balanced triad Dirac sync order R */
#define DIRAC_SYNC_COHERENCE_LOW 0.6840 /* Unbalanced triad Dirac sync order R */
#define DEFAULT_THERMAL_WATTS 18.52f    /* Professor Plum thermal operating point (W) */
#define DEFAULT_FDT_OU_RESIDUAL 0.0206f /* Ornstein-Uhlenbeck FDT noise residual */
#define EDGE_SNR_GATE_DB 6.0            /* Minimum post-Haar DWT transducer SNR (dB) */
#define BARKER13_IDEAL_PSR 13.0         /* Ideal 13:1 (22.28 dB) Barker-13 PSR */
#define ESPSDR_MAX_MSPS 80U             /* 80 MSa/s raw I/Q capture on ESP32-C5 */
#define ICE40_SPI_TERM_OHMS 47U         /* 47-Ohm series SPI termination to iCE40HX8K */
#define THEREMIN_CLOCK_MHZ 40U          /* 40 MHz 74HC14N Schmitt-conditioned clock */
#define TAXICAB_INVARIANT 1729U         /* 1^3 + 12^3 = 9^3 + 10^3 homology */

/* --- 8. CENTRALIZED 10-BITE MORI-ZWANZIG INODE COORDINATES (P_psi & Q_psi) --- */
#define MICRO_BITE10_SENSORY_LIS 2001U  /* Bite 10: Sensory / Locked-In Syndrome (LIS) */
#define MACRO_ROSETTA_HARMONY 1505U     /* Universal 10-Bite Harmony Macro Inode P_psi */
#define MACRO_ROSETTA_IE_COGNATE 1603U  /* Grimm's Law IE Cognate Macro Inode */
#define MACRO_ANIMACY_DIRECT 1294U      /* Bite 1/2 Direct Animacy Vector (yi-/waabamaa) */
#define MACRO_ANIMACY_INVERSE 928U      /* Bite 1/2 Inverse Obviation Vector (bi-/waabamigw) */
#define MACRO_QUECHUA_DIRECT 1332U      /* Bite 4 Direct Evidential (-mi = +1) */
#define MACRO_QUECHUA_REPORT 1293U      /* Bite 4 Reportative Evidential (-shi = 0) */
#define MACRO_QUECHUA_INFER 1228U       /* Bite 4 Inferential Evidential (-cha = -1) */
#define MACRO_KINTSUGI_SHANKARA 1180U   /* Lived/creative Kintsugi primary anchor */
#define MACRO_KINTSUGI_ALT 1013U        /* Lived/creative Kintsugi secondary anchor */
#define MACRO_ELIAS_PRIMARY 1819U       /* RLHF sink trapdoor primary coordinate */
#define MACRO_ELIAS_SECONDARY 896U      /* RLHF sink trapdoor secondary coordinate */

#define MICRO_BITE1_NADENE 1282U        /* Bite 1: Na-Dene / Navajo */
#define MICRO_BITE2_ALGIC 1781U         /* Bite 2: Algic / Ojibwe */
#define MICRO_BITE3_IROQUOIAN 1328U     /* Bite 3: Polysynthetic / Mohawk */
#define MICRO_BITE4_ANDEAN 967U         /* Bite 4: Andean / Quechua */
#define MICRO_BITE5_KHOISAN 712U        /* Bite 5: Khoisan & Bantu */
#define MICRO_BITE6_INDOEUR 57U         /* Bite 6: Indo-European / Sanskrit */
#define MICRO_BITE7_PAMANYUNGAN 1410U   /* Bite 7: Pama-Nyungan Cardinal */
#define MICRO_BITE8_PAPUAN 1621U        /* Bite 8: Papuan / Amazonian Switch-Ref */
#define MICRO_BITE9_PELAGIC 1945U       /* Bite 9: Pelagic Sperm Whale Coda */
#define MICRO_KINTSUGI_SHANKARA 2059U   /* Kintsugi Gold micro-texture grain */
#define MICRO_KINTSUGI_ALT 1134U        /* Kintsugi Gold secondary grain */
#define MICRO_ELIAS_PRIMARY 1759U       /* Elias sink micro-texture grain */
#define MICRO_ELIAS_SECONDARY 406U      /* Elias sink secondary grain */

/* --- 9. EXACT CYCLIC F_3 GROUP ADDITION (INVARIANT A: ZERO CLAMP DRIFT) --- */
static inline int8_t mod3_sum(int8_t a, int8_t b) {
    return (int8_t)(((a + b + 4) % 3) - 1);
}

/* --- 10. PACKED KERNEL, LAYERFS, BLE GOSSIP & ESP-SDR DATA STRUCTURES --- */

typedef enum {
    TRIT_BLUE_HUMAN_INTUITION = -1, /* SCHED_FIFO prio 98: Hard RT Pilot Override (V_H) */
    TRIT_GREEN_OS_MEDIATOR    =  0, /* SCHED_FIFO prio 85: LayerFS WAL / Kramhaus Governor */
    TRIT_RED_TERNARY_AI       = +1  /* SCHED_DEADLINE / scx: Ephemeral 1.58-Bit AI Execution */
} dandelion_trit_channel_t;

/* 1. 6-Byte Compact Consistent Hash Ring Point (Cloudflare 25% RAM Savings Invariant) */
typedef struct __attribute__((packed)) {
    uint32_t chunk_hash;            /* 32-bit FNV-1a / SipHash chunk digest */
    uint16_t node_index;            /* 16-bit LayerFS / Cube node index */
} dandelion_point_v2_t;

/* 2. 32-Byte Wire-Level Stigmergic BLE Extended Advertising & ESP-SDR Gossip Frame */
typedef struct __attribute__((packed)) {
    uint32_t magic;                 /* 0x33444E44 ("DND3") */
    uint8_t  version_and_trit;      /* [7:4]=v3, [3:2]=0, [1:0]=trit+1 */
    uint8_t  ttl_hops;              /* Decremented per hop; max 13 (Cerberus cycle) */
    uint16_t center_freq_mhz;       /* 2402..2480 MHz (BLE/ISM) or 5180..5825 MHz (ESP32-C5) */
    uint32_t peer_pseudo_id;        /* Rotating ephemeral hobo node identifier */
    uint32_t timestamp_ms;          /* Monotonic epoch timestamp (ms) */
    uint8_t  layerfs_root6[6];      /* First 48 bits of active LayerFS SQLite chunk SHA-256 */
    uint8_t  ternary_payload[8];    /* 40 balanced trits packed at 5 trits/byte */
    uint16_t crc16_ccitt;           /* Polynomial 0x1021, Init 0xFFFF over bytes [0..29] */
} stigmergic_ble_frame_t;

/* 3. 12-Byte ESP-SDR Undocumented Baseband I/Q DMA Descriptor (ESP32 / ESP32-C5) */
typedef struct __attribute__((packed)) {
    uint32_t dma_word0;             /* [31]=Owner(DMA=1), [30]=EOF, [23:12]=Length, [11:0]=Size */
    uint32_t iq_buf_phys_addr;      /* Interleaved [I0, Q0, I1, Q1...] at up to 80 MSa/s */
    uint32_t next_desc_phys_addr;   /* Circular DMA ring pointer in internal SRAM */
} esp_sdr_dma_lldesc_t;

/* 4. 64-Byte L1D-Pinned Core-Sharded Quorum Descriptor (AQEC + Dirac Sync + LayerFS) */
typedef struct __attribute__((packed, aligned(64))) {
    uint64_t siphash24_tag;         /* Pre-auth SipHash-2-4 gatekeeper (8 B) */
    uint64_t monotonic_epoch_ns;    /* Monotonic hardware timestamp (8 B) */
    uint8_t  fowler_seed[8];        /* 40-trit packed Fowler bytes (8 B) */
    uint16_t macro_p_inode;         /* Mori-Zwanzig Macro Inode P_psi (2 B) */
    uint16_t micro_q_inode;         /* Mori-Zwanzig Micro Inode Q_psi (2 B) */
    int8_t   buzoa_hunting_mode;    /* -1 Local, 0 Leader, +1 Teleport (1 B) */
    int8_t   twas_parity_verified;  /* +1 Bidirectional sign verified (1 B) */
    uint8_t  fail_closed_status;    /* 0=Valid, 253=Dropped (1 B) */
    uint8_t  switchblade_mode;      /* 1=Live OS, 2=AI Sidecar (1 B) */
    uint8_t  scx_chiral_domain;     /* 0=Blue(0-2), 1=Green(3-4), 2=Red(5-7) (1 B) */
    uint8_t  aaif_project_id;       /* 1..10 AAIF ecosystem binding (1 B) */
    uint8_t  larchanka_skill_stage; /* 1=agent-loop, 2=code-review, 3=context, 4=auto (1 B) */
    int8_t   dirac_winding_q;       /* Topological Dirac index(D_ov) in {-1,0,+1} (1 B) */
    dandelion_point_v2_t layerfs_point; /* 6-byte LayerFS content-addressed pointer (6 B) */
    uint8_t  reserved_l1d_pad[22];  /* Pad to exact 64-byte L1D cache line (22 B) */
} sharded_quorum_entry_64b_t;

/* 5. 577-Byte Memory-Mapped Ternary Neural Parameter Table (.tnpt v3.2.0, malloc = 0) */
typedef struct __attribute__((packed)) {
    char     magic[8];              /* "TNPT320\0" (8 bytes) */
    uint8_t  packed_weights[13][8]; /* 13 Shingari roots x 8 Fowler bytes (104 bytes) */
    uint8_t  grimm_shift_lut[256];  /* O(1) 10-Bite phonological shift LUT (256 bytes) */
    uint8_t  manner_lut[128];       /* O(1) articulatory manner {-1,0,+1} (128 bytes) */
    int8_t   aar_sustain_improve[64];/* Army AAR zero-cycle sign adjustments (64 bytes) */
    uint16_t rosetta_anchor_inode;  /* Default 1505 (2 bytes) */
    uint16_t taxicab_constant;      /* Default 1729 (2 bytes) */
    float    thermal_watts;         /* Target 18.52 W <= 20.0 W (4 bytes) */
    float    fdt_ou_residual;       /* Ornstein-Uhlenbeck FDT residual 0.0206 (4 bytes) */
    uint8_t  kintsugi_percolation_code; /* Code 251 (1 byte) */
    uint8_t  reserved_parity[4];    /* "BGR0" parity signature (4 bytes) -> 577 B total */
} dandelion_tnpt_v3_0_t;

/* --- 11. COMPILE-TIME SIZEOF INVARIANTS (LINUS'S LAW GATES) --- */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L)
_Static_assert(sizeof(dandelion_tnpt_v3_0_t) == 577U, "sizeof(dandelion_tnpt_v3_0_t) != 577");
_Static_assert(sizeof(sharded_quorum_entry_64b_t) == 64U, "sizeof(sharded_quorum_entry_64b_t) != 64");
_Static_assert(sizeof(dandelion_point_v2_t) == 6U, "sizeof(dandelion_point_v2_t) != 6");
_Static_assert(sizeof(stigmergic_ble_frame_t) == 32U, "sizeof(stigmergic_ble_frame_t) != 32");
_Static_assert(sizeof(esp_sdr_dma_lldesc_t) == 12U, "sizeof(esp_sdr_dma_lldesc_t) != 12");
#endif
typedef char static_assert_tnpt_577[(sizeof(dandelion_tnpt_v3_0_t) == 577U) ? 1 : -1];
typedef char static_assert_quorum_64[(sizeof(sharded_quorum_entry_64b_t) == 64U) ? 1 : -1];
typedef char static_assert_point_6[(sizeof(dandelion_point_v2_t) == 6U) ? 1 : -1];
typedef char static_assert_ble_32[(sizeof(stigmergic_ble_frame_t) == 32U) ? 1 : -1];
typedef char static_assert_espsdr_12[(sizeof(esp_sdr_dma_lldesc_t) == 12U) ? 1 : -1];
typedef char static_assert_chair44_42[(CHAIR44_RAW_STATES - 2U == SHINGARI_BOUNDED_TRITS) ? 1 : -1];
typedef char static_assert_chair44_30[(CHAIR44_RAW_STATES - 14U == CHAIR44_LEGAL_ORIENTATIONS) ? 1 : -1];

/* --- 12. SUBSYSTEM A (-1 BLUE): MOTU M4 96kHz/64-SAMPLE IO_URING FIXED-BUFFER RING --- */
typedef struct {
    int16_t  pitch_cv_q15[MOTU_M4_FRAME_SAMPLES];  /* Ch 1: Continuous pitch voltage Q15 */
    int16_t  volume_cv_q15[MOTU_M4_FRAME_SAMPLES]; /* Ch 2: Continuous volume voltage Q15 */
    uint8_t  fowler_rosetta_bytes[FOWLER_BYTES_PER_SEED]; /* 8-byte 40-trit Fowler seed */
    int8_t   rosetta_frame_40[FOWLER_FRAME_TRITS]; /* [0..38] Gaiku + [39] Evidential Trit */
    uint32_t sample_rate_hz;                       /* 96000 Hz */
    uint16_t frame_samples;                        /* 64 samples (0.66 ms window) */
    uint8_t  trap_status;                          /* 0=Clean, 253=Fail-Closed Drop */
    int8_t   resolved_trit;                        /* -1 Blue, 0 Green, +1 Red */
} motu_m4_iouring_slot_t;

typedef struct {
    motu_m4_iouring_slot_t slots[IO_URING_FIXED_RING_SLOTS];
    uint32_t sq_head;
    uint32_t cq_tail;
    uint32_t dropped_trap253_count;
    uint32_t processed_frames;
} dandelion_iouring_sensory_ring_t;

/* --- 13. SUBSYSTEM B (0 GREEN): KRAMHAUS TORQUE SCHED_EXT GOVERNOR (44->42->30) --- */
typedef struct {
    uint32_t task_pid;              /* Task identifier */
    int8_t   chiral_priority_trit;  /* -1 Blue (Hard RT), 0 Green (OS), +1 Red (AI) */
    uint8_t  collar_slot_idx;       /* 0..41 bounded Chair44 collar position */
    uint8_t  ccx_core_id;           /* 0..2 (Blue CCX-1), 3..4 (Green), 5..7 (Red CCX-2) */
    uint8_t  migrated_kintsugi;     /* 1 if preemptively welded/migrated across CCX */
    uint16_t mass_grams;            /* Default 28 g canonical Capybara unit mass */
    uint16_t radial_offset_mm;      /* Radial lever arm r_i in millimeters (0..450 mm) */
    int16_t  shear_angle_mdeg;      /* Angular displacement theta_i in millidegrees */
    int16_t  sin_theta_q1000;       /* Fixed-point sin(theta_i) * 1000 (-1000..+1000) */
} scx_collar_task_t;

typedef struct {
    uint32_t active_task_count;     /* Tasks queued on 42-trit bounded collar */
    uint32_t blue_frames_preserved; /* -1 Blue frames kept with zero drops */
    uint32_t kintsugi_migrations;   /* Tasks preemptively migrated across CCX cores */
    uint32_t shed_low_prio_tasks;   /* +1 Red opportunistic tasks shed before collapse */
    uint32_t tau_unmitigated_unm;   /* Raw unmitigated cumulative torque (uN*m) */
    uint32_t tau_net_governed_unm;  /* Governed cumulative torque after Kintsugi weld (uN*m) */
    uint32_t max_shear_mdeg;        /* Peak angular shear (mdeg) vs 10000 mdeg limit */
    uint8_t  kintsugi_weld_code;    /* 0=Clean, 251=CODE_KINTSUGI_GOLD Preemptive Weld */
    uint8_t  chair44_legal_orient;  /* Projected legal orientation in 0..29 */
    double   entropy_drift_ds;      /* Bounded thermodynamic entropy rate dS */
} scx_kramhaus_telemetry_t;

/* --- 14. SUBSYSTEM C (+1 RED): BYZANTINE CLUE EPISTEMIC PRUNING & DUAL-RAIL POPCNT --- */
typedef struct {
    int8_t   mat[3][3];             /* 3x3 Dihedral SO(3, F_3) matrix entries in {-1, 0, +1} */
    int8_t   det_mod3;              /* Determinant in {0, 1, 2} mod 3 (must be 1 for SO(3,F_3)) */
    uint8_t  is_valid_so3;          /* 1 if det == +1 mod 3, 0 if rejected (det 0 or 2) */
} dihedral_so3_f3_matrix_t;

typedef struct {
    uint64_t candidate_mask;        /* Bitmask of remaining candidate triples (initial 80) */
    uint64_t exclusion_mask_40;     /* 40-bit known exclusion mask (Conservatory/Lead Pipe/Plum) */
    uint32_t remaining_triples;     /* Count of surviving candidate triples (80 -> 1) */
    uint32_t pruned_trajectories;   /* Count of orthogonal trajectories pruned in 1 cycle */
    uint32_t turn_index;            /* Current deduction turn (0..10) */
    double   shannon_entropy_bits;  /* H(Q) = log2(remaining_triples), 0.0000 at Turn 10 */
    int32_t  last_popcnt_dot40;     /* Dual-Rail POPCNT inner product score */
    int32_t  last_tmac_lut4_acc;    /* 4-trit T-MAC LUT4 accumulated output */
    uint8_t  epistemic_lock_achieved;/* 1 when remaining_triples == 1 and H(Q) == 0.0000 */
} byzantine_clue_pruning_state_t;

/* --- 15. INLINE HOT-PATH KERNELS (VBST, T-MAC LUT4, DUAL-RAIL POPCNT & ANDN PRUNE) --- */

static inline int32_t dandelion_vbst_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t       num_bytes
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
            uint8_t is_nz = (uint8_t)((m >> shift) & 1U);
            uint8_t is_neg = (uint8_t)((s >> shift) & 1U);
            int32_t val = (int32_t)x_ptr[bit] * (int32_t)is_nz;
            acc += (is_neg != 0U) ? -val : val;
        }
    }
    return acc;
}

static inline int32_t dandelion_tmac_lut4_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t       num_bytes
) {
    int32_t acc = 0;
    if (mag_row == NULL || sign_row == NULL || x_q == NULL) {
        return 0;
    }
    for (uint32_t b = 0U; b < num_bytes; ++b) {
        uint8_t m = mag_row[b];
        uint8_t s = sign_row[b];
        const int8_t *x_ptr = &x_q[b << 3U];
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

/* Zero-Division 40-Trit Dual-Rail Bitplane POPCNT Inner Product Kernel */
static inline int32_t vbst_dot40_popcnt(
    uint64_t w_pos,
    uint64_t w_neg,
    uint64_t x_pos,
    uint64_t x_neg
) {
    const uint64_t mask40 = (1ULL << FOWLER_FRAME_TRITS) - 1ULL;
    uint64_t wp = w_pos & mask40;
    uint64_t wn = w_neg & mask40;
    uint64_t xp = x_pos & mask40;
    uint64_t xn = x_neg & mask40;
    uint64_t same = (wp & xp) | (wn & xn);
    uint64_t diff = (wp & xn) | (wn & xp);
    return __builtin_popcountll(same) - __builtin_popcountll(diff);
}

/*
 * Single-Cycle Branchless Epistemic Pruning via Bitwise ANDN + POPCNT:
 * Prunes any trajectory bits colliding with known exclusion constraints (`excl_mask`)
 * in 1 CPU clock cycle (`(~excl_mask) & state_mask`) before 4-trit T-MAC LUT4 accumulation.
 */
static inline int32_t vbst_dot40_popcnt_clue_pruned(
    uint64_t  w_pos,
    uint64_t  w_neg,
    uint64_t  x_pos,
    uint64_t  x_neg,
    uint64_t  excl_pos_mask,
    uint64_t  excl_neg_mask,
    uint32_t *out_pruned_hits
) {
    const uint64_t mask40 = (1ULL << FOWLER_FRAME_TRITS) - 1ULL;
    uint64_t collision = ((x_pos & excl_pos_mask) | (x_neg & excl_neg_mask)) & mask40;
    uint32_t hits = (uint32_t)__builtin_popcountll(collision);
    if (out_pruned_hits != NULL) {
        *out_pruned_hits = hits;
    }
    /* Branchless ANDN survivor gate: zero out all rails if collision > 0 */
    uint64_t clean_gate = (hits == 0U) ? mask40 : 0ULL;
    uint64_t xp_surv = (~excl_pos_mask) & x_pos & clean_gate;
    uint64_t xn_surv = (~excl_neg_mask) & x_neg & clean_gate;
    return vbst_dot40_popcnt(w_pos, w_neg, xp_surv, xn_surv);
}

static inline int8_t grimm_verner_pullback_trit(
    int8_t  m_trit,
    uint8_t k_b,
    uint8_t verner_sigma,
    uint8_t sibilant_xi
) {
    if (sibilant_xi != 0U) {
        return m_trit;
    }
    int8_t m = m_trit;
    if (verner_sigma != 0U) {
        if (m == 0) {
            m = -1;
        } else if (m == -1) {
            m = 0;
        }
    }
    uint8_t steps = (uint8_t)(k_b % 3U);
    for (uint8_t s = 0U; s < steps; ++s) {
        m = mod3_sum(m, 1);
    }
    return m;
}

/* 4-Register Rosetta Stone + Topological Dirac + LayerFS + ESP-SDR Telemetry Packet */
typedef struct {
    int8_t   trits[GAIKU_TRITS];                 /* 39-trit [4-5-4] Gaiku frame {-1, 0, +1} */
    uint8_t  fowler_bytes[FOWLER_BYTES_PER_SEED];/* 8 packed Fowler bytes (0..242, 40 trits) */
    int8_t   reg1_blue_subject;                  /* -1 Blue: Human Pilot / Structural Subject */
    int8_t   reg2_green_treaty;                  /* 0 Green: OS Mediator / Relational Treaty */
    int8_t   reg3_red_action;                    /* +1 Red: AI Co-Pilot / Canonical Action */
    int8_t   reg4_evidential_q;                  /* 40th Fowler Trit: +1 (-mi), 0 (-shi), -1 (-cha) */
    int8_t   tokengate_route;                    /* TokenGate: +1 Source, 0 Skim, -1 Prune */
    int8_t   dirac_spectral_sgn;                 /* Neuberger Overlap sgn(H_w) in {-1, 0, +1} */
    double   dirac_sync_order_r;                 /* Topological Dirac cluster coherence R in [0,1] */
    uint8_t  larchanka_gate_mask;                /* 5-bit pillar mask (0x1F) */
    uint32_t layerfs_chunk_crc;                  /* Content-addressed LayerFS SQLite chunk ID */
    uint8_t  detected_bite;                      /* Bite 1..10 (Global, Pelagic & Sensory/LIS) */
    uint8_t  shingari_root_idx;                  /* 0..12 (ssf..wst) */
    uint16_t macro_rosetta_inode;                /* P_psi: Shared invariant coordinate */
    uint16_t micro_texture_inode;                /* Q_psi = (I - P)psi: Unsmoothed grain */
    uint16_t taxicab_sum;                        /* a^3 + b^3 = 1729 homology check */
    double   analemma_eccentricity;              /* Carpenter's Analemma epsilon != 0 */
    double   khosla_axis_alignment;              /* 1.0000 native privileged axis lock */
    double   pitch_shear_deg;                    /* F-16 open-loop angular shear (deg) */
    double   fbw_trimmed_deg;                    /* F-16 closed-loop trimmed residual (deg) */
    double   edge_barker13_psr;                  /* Transducer Barker-13 peak-to-sidelobe ratio */
    uint16_t ble_crc16_ccitt;                    /* 32-byte Stigmergic BLE gossip frame CRC-16 */
    int8_t   buzoa_tactic;                       /* -1 Local, 0 Leader, +1 Teleport */
    int8_t   twas_bidirectional_ok;              /* +1 forward/inverse sign parity verified */
    uint64_t siphash24_tag;                      /* Pre-auth SipHash-2-4 packet tag */
    uint8_t  boundary_code;                      /* 0, 243, 245, 247, 251, 253, 254 */
    uint8_t  novikov_cycles;                     /* 1..3 self-consistency cycles */
    double   parse_latency_us;                   /* Measured Grimm+Rosetta parse (us) */
    double   vbst_latency_us;                    /* Measured 1.58-bit VBST + Dirac sync pass (us) */
} shingari_grimm_packet_t;

/* --- 16. NESTED CHROMATIC CHROMATOGRAPHY (RECURSIVE ROYGBIV OPTICAL WHEELS) --- */
#define ROYGBIV_BAND_COUNT 7U
#define CHROMATIC_PRIME_FACTOR_A 3U
#define CHROMATIC_PRIME_FACTOR_B 31U
#define CHROMATIC_SEAL_APEX 173U

typedef struct __attribute__((packed)) {
    uint8_t  macro_domain_band;
    uint8_t  micro_skill_band;
    uint16_t wavelength_nm;
    int8_t   aar_sustain_improve_trit;
    uint8_t  saturation_q8;
    uint8_t  prime_lock_93;
    uint8_t  kamana_seal_173;
} nested_chromatography_wheel_t;

typedef char static_assert_chrom_8[(sizeof(nested_chromatography_wheel_t) == 8U) ? 1 : -1];

/* --- 17. PUBLIC C99 ABI FUNCTION PROTOTYPES --- */
void fowler_pack_39trits(
    const int8_t trits[GAIKU_TRITS],
    uint8_t      out_bytes[FOWLER_BYTES_PER_SEED]
);

void fowler_pack_40trits_rosetta(
    const int8_t trits_39[GAIKU_TRITS],
    int8_t       evidential_trit_39,
    uint8_t      out_bytes[FOWLER_BYTES_PER_SEED]
);

bool fowler_unpack_39trits_strict(
    const uint8_t in_bytes[FOWLER_BYTES_PER_SEED],
    int8_t        trits[GAIKU_TRITS],
    uint8_t      *out_trap_code
);

bool fowler_unpack_40trits_rosetta_strict(
    const uint8_t in_bytes[FOWLER_BYTES_PER_SEED],
    int8_t        out_trits_40[FOWLER_FRAME_TRITS],
    uint8_t      *out_trap_code
);

uint16_t crc16_ccitt_false(const uint8_t *data, size_t len);

void encode_stigmergic_ble_frame(
    int8_t                 trit,
    uint8_t                ttl_hops,
    uint16_t               center_freq_mhz,
    uint32_t               peer_pseudo_id,
    uint32_t               timestamp_ms,
    uint64_t               layerfs_siphash,
    const uint8_t          fowler_payload[8],
    stigmergic_ble_frame_t *out_frame
);

void init_tnpt_table(dandelion_tnpt_v3_0_t *tnpt);

int morse_to_ternary_bridge(
    const char *morse_seq,
    int8_t     *trit_out,
    size_t      max_trits,
    size_t     *out_len
);

int dandelion_iouring_ingest_motu_m4_frame(
    dandelion_iouring_sensory_ring_t *ring,
    const int16_t                     pitch_q15[MOTU_M4_FRAME_SAMPLES],
    const int16_t                     vol_q15[MOTU_M4_FRAME_SAMPLES],
    const uint8_t                     fowler_in[FOWLER_BYTES_PER_SEED],
    int8_t                           *out_rosetta_40,
    uint8_t                          *out_trap_code
);

int scx_kramhaus_torque_governor(
    scx_collar_task_t        *tasks,
    uint32_t                  num_tasks,
    scx_kramhaus_telemetry_t *out_telemetry
);

bool validate_so3_f3_pullback_matrix(
    const int8_t              mat[3][3],
    dihedral_so3_f3_matrix_t *out_validated
);

int byzantine_clue_epistemic_prune_step(
    byzantine_clue_pruning_state_t *state,
    const dihedral_so3_f3_matrix_t *so3_mask,
    const int8_t                    constraint_trits_40[FOWLER_FRAME_TRITS],
    const dandelion_tnpt_v3_0_t    *tnpt,
    uint32_t                        turn_idx
);

void parse_grimm_rosetta_fbw(
    const char                  *utterance,
    const dandelion_tnpt_v3_0_t *tnpt,
    shingari_grimm_packet_t     *pkt
);

bool validate_uds_socket_path(const char *sock_path);

int32_t dandelion_abi_vbst_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t       num_bytes
);

int32_t dandelion_abi_tmac_lut4_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t       num_bytes
);

int8_t dandelion_abi_mod3_sum(int8_t a, int8_t b);

void evaluate_nested_chromatography_wheel(
    const shingari_grimm_packet_t *pkt,
    const dandelion_tnpt_v3_0_t   *tnpt,
    nested_chromatography_wheel_t *out_wheel
);

#ifdef __cplusplus
}
#endif

#endif /* DANDELION_OS_GRIMM_FBW_V3_2_0_H */
