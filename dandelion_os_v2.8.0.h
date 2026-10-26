/*
 * ============================================================================
 * DANDELION OS v2.8.0 (CODENAME: SOVEREIGN DESKTOP & EMBEDDED AI RELEASE)
 * CENTRALIZED C99 CONFIGURATION, PLATFORM GUARDS & CONTROL LAW DEFINITIONS
 * ============================================================================
 * File:        dandelion_os_v2.8.0.h
 * Version:     v2.8.0 (Strict 3-Digit Triadic SemVer: MAJOR=2 [das], MINOR=8 [die], PATCH=0 [der])
 * Author:      Justin D. Grimm (K-Lani 13 Actual, @leadpiperl1, ORCID: 0009-0000-5299-0243)
 * License:     GPL-2.0-or-later / MIT Dual Open-Source
 *
 * COPILOT AUDIT & PORTABILITY ARCHITECTURE (v2.8.0):
 *   - Centralized all runtime limits, socket namespaces, hash seeds, 9-Bite
 *     Mori-Zwanzig inode coordinates, and gate trap codes into a single header.
 *   - Added explicit OS/platform detection macros (DOS_PLATFORM_LINUX vs.
 *     DOS_PLATFORM_POSIX vs. DOS_PLATFORM_PORTABLE_FALLBACK) so user-space
 *     builds and CI runners operate cleanly across Linux, macOS, and BSD.
 * ============================================================================
 */

#ifndef DANDELION_OS_V2_8_0_H
#define DANDELION_OS_V2_8_0_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --- 0. PLATFORM & ENVIRONMENT DETECTION (COPILOT POINT 4) --- */
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
#define DOS_VERSION_MAJOR        2
#define DOS_VERSION_MINOR        8
#define DOS_VERSION_PATCH        0
#define DOS_VERSION_STR          "v2.8.0"
#define DOS_SEMVER_BARE_STR      "2.8.0"
#define DOS_CODENAME_STR         "Sovereign Desktop & Embedded AI Release"
#define SHINGARI_VERSION_STR     "v3.6.0"
#define GRIMM_LAW_VERSION_STR    "v3.2.0"

/* --- 2. RUNTIME LIMITS & BUFFER BOUNDS (COPILOT POINT 6) --- */
#define GAIKU_TRITS              39U     /* 39-trit [4-5-4] Shingari Gaiku frame */
#define FOWLER_TRITS_PER_BYTE    5U      /* 3^5 = 243 states per byte (94.92% density) */
#define FOWLER_BYTES_PER_SEED    8U      /* ceil(39 / 5) = 8 packed Fowler bytes */
#define FOWLER_MAX_VALID_BYTE    242U    /* Valid packed 5-trit states: 0..242 */
#define SHINGARI_ROOT_COUNT      13U     /* 13 Bedrock Shingari roots (ssf..wst) */
#define GLOBAL_BITE_COUNT        9U      /* 9-Bite Universal Grimm's Law families */
#define VAULT_CAPACITY           243U    /* 3^5 episodic seed crystal slots */
#define TNPT_MMAP_BYTES          577U    /* Exact L1-resident .tnpt weight image */
#define MAX_UTTERANCE_BYTES      512U    /* Bounded input buffer size (fail-closed) */
#define UDS_SOCKET_PATH_MAX      104U    /* Safe limit below 108-byte sun_path ceiling */
#define BENCHMARK_DEFAULT_ITERS  100000  /* Default 100k-iteration latency benchmark */
#define HYPERVISOR_SPIKE_PARSE_US 2.50   /* Guest VM context-switch filter ceiling (us) */
#define HYPERVISOR_SPIKE_VBST_US  1.50   /* Guest VM VBST preemption filter ceiling (us) */
#define PREEMPT_RT_CEILING_US    50.00   /* Linux PREEMPT_RT + sched_ext window ceiling */
#define STRICT_JITTER_CEILING_US  1.00   /* Bare-metal core wakeup jitter ceiling (us) */
#define PORTABLE_JITTER_CEILING_US 15.00 /* Shared CI runner / container jitter ceiling */

/* --- 3. CENTRALIZED IPC SOCKET NAMESPACES (COPILOT POINTS 4 & 6) --- */
#define DOS_SOCK_PREFIX_VAR_RUN  "/var/run/dandelion_"
#define DOS_SOCK_PREFIX_RUN      "/run/dandelion_"
#define DOS_SOCK_PREFIX_TMP      "/tmp/dandelion_"
#define DOS_SOCK_PREFIX_PRIV_TMP "/private/tmp/dandelion_" /* macOS / BSD compatibility */

/* --- 4. FAIL-CLOSED BOUNDARY & TRAPDOOR CODES (FOWLER CEILING 243..255) --- */
#define CODE_CLEAN               0U      /* Valid 5-trit Fowler state (0..242) */
#define CODE_QUECHUA_INFERENTIAL 243U    /* Register 4 = -1 (-cha conjecture guard) */
#define CODE_DIRAC_CLUSTER_SYNC  245U    /* Topological Dirac cluster sync lock */
#define CODE_BUZOA_TELEPORT      247U    /* BUZOA +1 global exploration leap */
#define CODE_KINTSUGI_GOLD       251U    /* >= 10.0 deg lived/creative shear weld */
#define CODE_FAIL_CLOSED_DROP    253U    /* Security hardening: unmapped byte drop */
#define CODE_ELIAS_TRAPDOOR      254U    /* Hamilton & Mimno (2026) RLHF sink trap */

/* --- 5. CRYPTOGRAPHIC & HASH SEEDS (CENTRALIZED FOR AUDITABILITY) --- */
#define SIPHASH_K0_SEED          (0x736f6d6570736575ULL ^ 0x1729150500000280ULL)
#define SIPHASH_K1_SEED          (0x646f72616e646f6dULL ^ 0x0009000052990243ULL)
#define DJB2_INIT_SEED           5381U
#define FNV1A_32_OFFSET_BASIS    2166136261U
#define FNV1A_32_PRIME           16777619U
#define LARCHANKA_5PILLAR_MASK   0x1FU   /* Bitmask: Correctness|Readability|Arch|Sec|Perf */

/* --- 6. PHYSICAL, TOPOLOGICAL & THERMODYNAMIC INVARIANTS --- */
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
#define TRIO_PULSE_NS            90.0        /* WACQT 90-ns robust control window */
#define TAXICAB_INVARIANT        1729U       /* 1^3 + 12^3 = 9^3 + 10^3 homology */

/* --- 7. CENTRALIZED 9-BITE MORI-ZWANZIG INODE COORDINATES (P_psi & Q_psi) --- */
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

/* --- 8. PACKED KERNEL & LAYERFS DATA STRUCTURES --- */

/*
 * 6-Byte Compact Consistent Hash Ring Point (Cloudflare 25% RAM Savings Invariant):
 * Replaces padded 8-byte { uint32_t hash; uint32_t index; } with packed 6-byte struct
 * for bounded server/chunk sets (< 65,535 nodes).
 */
typedef struct __attribute__((packed)) {
    uint32_t chunk_hash;                     /* 32-bit FNV-1a / SipHash chunk digest */
    uint16_t node_index;                     /* 16-bit LayerFS / Cube node index */
} dandelion_point_v2_t;

/*
 * 64-Byte L1D-Pinned Core-Sharded Quorum Descriptor (AQEC + Dirac Sync + LayerFS):
 * Explicitly sized and aligned to a single 64-byte CPU L1D cache line.
 */
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

/*
 * 577-Byte Memory-Mapped Ternary Neural Parameter Table (.tnpt v2.8.0):
 * Fits entirely within L1 data cache with zero dynamic heap allocation.
 */
typedef struct __attribute__((packed)) {
    char     magic[8];                       /* "TNPT280 " (8 bytes) */
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
} dandelion_tnpt_v2_8_t;

/*
 * 4-Register Rosetta Stone + Topological Dirac + LayerFS + larchanka Telemetry Packet
 */
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
    int8_t   buzoa_tactic;                   /* -1 Local, 0 Leader, +1 Teleport */
    int8_t   twas_bidirectional_ok;          /* +1 forward/inverse sign parity verified */
    uint64_t siphash24_tag;                  /* Pre-auth SipHash-2-4 packet tag */
    uint8_t  boundary_code;                  /* 0, 243, 245, 247, 251, 253, 254 */
    uint8_t  novikov_cycles;                 /* 1..3 self-consistency cycles */
    double   parse_latency_us;               /* Measured 9-Bite Grimm+Rosetta parse (us) */
    double   vbst_latency_us;                /* Measured 1.58-bit VBST + Dirac sync pass (us) */
} shingari_grimm_packet_t;

/* Compile-time size invariants (ISO C99 compatible static assertions) */
typedef char static_assert_tnpt_577[(sizeof(dandelion_tnpt_v2_8_t) == TNPT_MMAP_BYTES) ? 1 : -1];
typedef char static_assert_quorum_64[(sizeof(sharded_quorum_entry_64b_t) == 64U) ? 1 : -1];
typedef char static_assert_point_6[(sizeof(dandelion_point_v2_t) == 6U) ? 1 : -1];

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

void init_tnpt_table(dandelion_tnpt_v2_8_t *tnpt);

void parse_grimm_rosetta_fbw(
    const char *utterance,
    const dandelion_tnpt_v2_8_t *tnpt,
    shingari_grimm_packet_t *pkt
);

bool validate_uds_socket_path(const char *sock_path);

#ifdef __cplusplus
}
#endif

#endif /* DANDELION_OS_V2_8_0_H */
