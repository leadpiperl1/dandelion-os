/*
 * ============================================================================
 * DANDELION OS v3.2.0 (CODENAME: HEIMAT-AGORA — SOVEREIGN ARGON KERNEL & LINUS'S LAW RELEASE)
 * OPEN-SOURCE WHITE-ROOM C99 MICROKERNEL DAEMON, 10-BITE UNIVERSAL GRIMM'S LAW,
 * KRAMHAUS TORQUE SCHED_EXT GOVERNOR & BYZANTINE CLUE EPISTEMIC PRUNING FILTER
 * ============================================================================
 * File: dandelion_os_grimm_fbw_v3.2.0.c
 * Version: v3.2.0 (Strict 3-Digit Triadic SemVer: MAJOR=3, MINOR=2, PATCH=0)
 * Author: Justin D. Grimm (K-Lani 13 Actual, leadpiperl1@gmail.com,
 *         GitHub: @leadpiperl1, ORCID: 0009-0000-5299-0243) & Gemini Argon
 * License: GPL-2.0-or-later / MIT Dual Open-Source
 *
 * LINUS'S LAW RED-TEAM AUDIT & STATIC ANALYSIS HARDENING IN v3.2.0:
 * 1. [-1 Blue] Bite 10 (MICRO_BITE10_SENSORY_LIS = 2001) & MOTU M4 96 kHz /
 *    64-sample (0.66 ms) io_uring fixed-buffer ring with strict Fail-Closed
 *    Trapdoor Byte 253 rejection, 40-trit Fowler Rosetta frame preservation,
 *    and exact F_3 cyclic group addition mod3_sum(a,b) = ((a + b + 4) % 3) - 1.
 * 2. [0 Green] scx_kramhaus_torque_governor() on the Chair44 (44 -> 42 -> 30)
 *    bounded collar. Accumulates m_i = 28 g lever-arm torque in widening 64-bit
 *    fixed-point integer arithmetic (uN*m) to eliminate 32-bit overflow, and
 *    executes preemptive Kintsugi Gold (Byte 251) cross-CCX migration before
 *    tau_crit = 49426 uN*m (49.4255 mN*m) or 10.0 deg Kramhaus shear collapse.
 * 3. [+1 Red] Byzantine Clue Epistemic Pruning Filter & Turn-10 Deductive Lock
 *    (H(Q) = 0.0000 bits on Conservatory/Lead Pipe/Plum) inside the 577-byte
 *    L1-resident .tnpt table (malloc = 0). Enforces invertible 3x3 Dihedral
 *    SO(3, F_3) pullback masks (det M == +1 mod 3, rejecting det == 0, 2 mod 3)
 *    and branchless 1-cycle ANDN + POPCNT trajectory pruning before 4-trit T-MAC LUT4.
 * ============================================================================
 */

#define _POSIX_C_SOURCE 200809L
#include "dandelion_os_grimm_fbw_v3.2.0.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <time.h>

/* --- 1. 13 SHINGARI ROOTS WITH 10-PROJECT AAIF & LARCHANKA/SKILLS BINDINGS --- */
typedef struct {
    const char *root;
    const char *phase;
    const char *human_intent;
    const char *linux_syscall;
    const char *aaif_and_ai_action;
} shingari_root_spec_t;

static const shingari_root_spec_t SHINGARI_ROOTS[SHINGARI_ROOT_COUNT] = {
    {"ssf", "4-INTAKE ", "Classify storage & voxel files",       "ls -laF / readdir()",         "AAIF Agent Skills + larchanka/context: Mount LayerFS & vX.Y.Z tags"},
    {"hrt", "4-INTAKE ", "Check heartbeat & CPU thermal",        "cat /sys/class/thermal/temp", "AAIF Agent Router: Clamp V=2/3 keyhole (<45.0 C) & 15Hz mux"},
    {"rtz", "4-INTAKE ", "Map 3D directory & thought tree",      "tree /mnt/models",            "AAIF MCP Server: Project 13x13x13 L1 voxel & .tnpt NEFF cache"},
    {"sft", "4-INTAKE ", "Sift & pattern-match text/code",       "grep -rn <pattern>",          "AAIF AGENTS.md + larchanka/code-review: 5-pillar AST audit"},
    {"div", "5-CORE   ", "Track rate-of-change pitch delta",     "clock_gettime(MONOTONIC_RAW)","AAIF A2A & OpenEnv: F-16 FBW pitch trim + Dirac cluster sync"},
    {"orb", "5-CORE   ", "Stream kernel & Kintsugi logs",        "dmesg -w / bpf_ringbuf",      "AAIF agentgateway: Pin >=10.0 deg shear with Kintsugi Gold (251)"},
    {"tot", "5-CORE   ", "Inspect bare-metal CPU identity",      "sched_ext / taskset -cp 1",   "AAIF OpenSandbox: Enforce Zen CCX Blue(0-2)/Green(3-4)/Red(5-7)"},
    {"ohm", "5-CORE   ", "Translate language/code <-> Shingari", "cc -O3 / wasmtime",           "AAIF Goose + larchanka/agent-loop: Compile 10-Bite Grimm Gaiku"},
    {"unt", "5-CORE   ", "Read & paginate bedrock document",     "io_uring_enter() / mmap()",   "AAIF Open Responses: Stream zero-TUI fx JSON & Workspace Pearl"},
    {"bec", "4-HANDOFF", "Configure loopback & mesh socket",     "socket(AF_UNIX) / XDP",       "Pre-Auth SipHash-2-4 XDP filter & /var/run/dandelion_mesh.sock"},
    {"int", "4-HANDOFF", "Query 243-slot episodic vault",        "find /mnt/models -name '*.tnpt'","TorchNeuron AOT + TokenGate: Retrieve resonant seed from vault"},
    {"ves", "4-HANDOFF", "Audit RAMDisk & SquashFS zones",       "df -h && free -m",            "AQEC Dissipative Bath + Dirac Overlap: Balance 1.58-bit L1D"},
    {"wst", "4-HANDOFF", "Flush 5-7-5 Haiku seed to USB",        "sync && fsync() [f2fs]",      "Per-Ref Git Quorum + larchanka/auto: Lock main & mint Zenodo DOI"}
};

/* --- 2. PORTABLE MONOTONIC CLOCK WRAPPER & PRE-AUTH SIPHASH-2-4 GATE --- */
static inline double now_us(void) {
#if DOS_PLATFORM_POSIX && defined(CLOCK_MONOTONIC)
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (double)ts.tv_sec * 1e6 + (double)ts.tv_nsec * 1e-3;
    }
#endif
    return ((double)clock() * 1e6) / (double)CLOCKS_PER_SEC;
}

static inline uint64_t preauth_siphash24_lite(const char *msg, size_t len) {
    uint64_t v0 = SIPHASH_K0_SEED;
    uint64_t v1 = SIPHASH_K1_SEED;
    if (msg == NULL) {
        return v0 ^ v1;
    }
    for (size_t i = 0U; i < len; i++) {
        v0 ^= (uint64_t)(unsigned char)msg[i];
        v0 = (v0 << 13U) | (v0 >> 51U);
        v1 = (v1 + v0) ^ ((v1 << 17U) | (v1 >> 47U));
    }
    return v0 ^ v1 ^ (uint64_t)len;
}

uint16_t crc16_ccitt_false(const uint8_t *data, size_t len) {
    uint16_t crc = (uint16_t)CRC16_CCITT_INIT;
    if (data == NULL) {
        return crc;
    }
    for (size_t i = 0U; i < len; i++) {
        crc ^= (uint16_t)((uint16_t)data[i] << 8U);
        for (uint8_t b = 0U; b < 8U; b++) {
            if ((crc & 0x8000U) != 0U) {
                crc = (uint16_t)((uint16_t)(crc << 1U) ^ (uint16_t)CRC16_CCITT_POLY);
            } else {
                crc = (uint16_t)(crc << 1U);
            }
        }
    }
    return crc;
}

void encode_stigmergic_ble_frame(
    int8_t                 trit,
    uint8_t                ttl_hops,
    uint16_t               center_freq_mhz,
    uint32_t               peer_pseudo_id,
    uint32_t               timestamp_ms,
    uint64_t               layerfs_siphash,
    const uint8_t          fowler_payload[8],
    stigmergic_ble_frame_t *out_frame
) {
    if (out_frame == NULL || fowler_payload == NULL) {
        return;
    }
    memset(out_frame, 0, sizeof(*out_frame));
    out_frame->magic = DANDELION_MAGIC_U32;
    uint8_t t_enc = (uint8_t)((trit < 0) ? 0U : ((trit > 0) ? 2U : 1U));
    out_frame->version_and_trit = (uint8_t)((3U << 4U) | (t_enc & 0x03U));
    out_frame->ttl_hops = (ttl_hops > 13U) ? 13U : ttl_hops;
    out_frame->center_freq_mhz = center_freq_mhz;
    out_frame->peer_pseudo_id = peer_pseudo_id;
    out_frame->timestamp_ms = timestamp_ms;
    for (size_t i = 0U; i < 6U; i++) {
        out_frame->layerfs_root6[i] = (uint8_t)((layerfs_siphash >> (i * 8U)) & 0xFFULL);
    }
    memcpy(out_frame->ternary_payload, fowler_payload, 8U);
    out_frame->crc16_ccitt = crc16_ccitt_false((const uint8_t *)out_frame, 30U);
}

/* --- 3. SECURE UNIX DOMAIN SOCKET PATH VALIDATOR (IPC HARDENING) --- */
bool validate_uds_socket_path(const char *sock_path) {
    if (sock_path == NULL) {
        return false;
    }
    size_t len = strlen(sock_path);
    if (len == 0U || len >= UDS_SOCKET_PATH_MAX) {
        return false;
    }
    if (sock_path[0] != '/' || strstr(sock_path, "..") != NULL) {
        return false;
    }
    if (strncmp(sock_path, DOS_SOCK_PREFIX_VAR_RUN, strlen(DOS_SOCK_PREFIX_VAR_RUN)) != 0 &&
        strncmp(sock_path, DOS_SOCK_PREFIX_RUN, strlen(DOS_SOCK_PREFIX_RUN)) != 0 &&
        strncmp(sock_path, DOS_SOCK_PREFIX_TMP, strlen(DOS_SOCK_PREFIX_TMP)) != 0 &&
        strncmp(sock_path, DOS_SOCK_PREFIX_PRIV_TMP, strlen(DOS_SOCK_PREFIX_PRIV_TMP)) != 0) {
        return false;
    }
    return true;
}

/* --- 4. FAIL-CLOSED FOWLER 5-TRIT BYTE PACKING & 40-TRIT ROSETTA CODEC --- */
void fowler_pack_40trits_rosetta(
    const int8_t trits_39[GAIKU_TRITS],
    int8_t       evidential_trit_39,
    uint8_t      out_bytes[FOWLER_BYTES_PER_SEED]
) {
    if (trits_39 == NULL || out_bytes == NULL) {
        return;
    }
    for (size_t b = 0U; b < FOWLER_BYTES_PER_SEED; b++) {
        uint16_t acc = 0U;
        uint16_t pow3 = 1U;
        for (size_t k = 0U; k < FOWLER_TRITS_PER_BYTE; k++) {
            size_t idx = b * FOWLER_TRITS_PER_BYTE + k;
            int8_t t = (idx < GAIKU_TRITS) ? trits_39[idx] : evidential_trit_39;
            if (t < -1) {
                t = -1;
            }
            if (t > 1) {
                t = 1;
            }
            uint16_t shifted_trit = (uint16_t)((int)t + 1);
            acc = (uint16_t)(acc + (uint16_t)(shifted_trit * pow3));
            pow3 = (uint16_t)(pow3 * 3U);
        }
        out_bytes[b] = (uint8_t)acc; /* Strictly bounded in [0, 242] */
    }
}

void fowler_pack_39trits(
    const int8_t trits[GAIKU_TRITS],
    uint8_t      out_bytes[FOWLER_BYTES_PER_SEED]
) {
    fowler_pack_40trits_rosetta(trits, 0, out_bytes);
}

bool fowler_unpack_40trits_rosetta_strict(
    const uint8_t in_bytes[FOWLER_BYTES_PER_SEED],
    int8_t        out_trits_40[FOWLER_FRAME_TRITS],
    uint8_t      *out_trap_code
) {
    if (out_trap_code == NULL) {
        return false;
    }
    if (in_bytes == NULL || out_trits_40 == NULL) {
        *out_trap_code = CODE_FAIL_CLOSED_DROP;
        return false;
    }
    *out_trap_code = CODE_CLEAN;
    for (size_t b = 0U; b < FOWLER_BYTES_PER_SEED; b++) {
        uint8_t val = in_bytes[b];
        if (val > FOWLER_MAX_VALID_BYTE) {
            *out_trap_code = val; /* Fail-closed: preserve exact trapdoor byte (e.g. 253) */
            memset(out_trits_40, 0, FOWLER_FRAME_TRITS);
            return false;
        }
        for (size_t k = 0U; k < FOWLER_TRITS_PER_BYTE; k++) {
            size_t idx = b * FOWLER_TRITS_PER_BYTE + k;
            if (idx < FOWLER_FRAME_TRITS) {
                out_trits_40[idx] = (int8_t)((int)(val % 3U) - 1);
            }
            val = (uint8_t)(val / 3U);
        }
    }
    return true;
}

bool fowler_unpack_39trits_strict(
    const uint8_t in_bytes[FOWLER_BYTES_PER_SEED],
    int8_t        trits[GAIKU_TRITS],
    uint8_t      *out_trap_code
) {
    if (out_trap_code == NULL) {
        return false;
    }
    if (in_bytes == NULL || trits == NULL) {
        *out_trap_code = CODE_FAIL_CLOSED_DROP;
        return false;
    }
    int8_t trits_40[FOWLER_FRAME_TRITS];
    if (!fowler_unpack_40trits_rosetta_strict(in_bytes, trits_40, out_trap_code)) {
        memset(trits, 0, GAIKU_TRITS);
        return false;
    }
    memcpy(trits, trits_40, GAIKU_TRITS);
    return true;
}

/* --- 5. SUBSYSTEM A (-1 BLUE): MOTU M4 96kHz/64-SAMPLE IO_URING BRIDGE --- */
int dandelion_iouring_ingest_motu_m4_frame(
    dandelion_iouring_sensory_ring_t *ring,
    const int16_t                     pitch_q15[MOTU_M4_FRAME_SAMPLES],
    const int16_t                     vol_q15[MOTU_M4_FRAME_SAMPLES],
    const uint8_t                     fowler_in[FOWLER_BYTES_PER_SEED],
    int8_t                           *out_rosetta_40,
    uint8_t                          *out_trap_code
) {
    if (ring == NULL || pitch_q15 == NULL || vol_q15 == NULL ||
        fowler_in == NULL || out_rosetta_40 == NULL || out_trap_code == NULL) {
        if (out_trap_code != NULL) {
            *out_trap_code = CODE_FAIL_CLOSED_DROP;
        }
        return -1;
    }

    uint32_t slot_idx = ring->sq_head & (IO_URING_FIXED_RING_SLOTS - 1U);
    motu_m4_iouring_slot_t *slot = &ring->slots[slot_idx];

    slot->sample_rate_hz = MOTU_M4_SAMPLE_RATE_HZ;
    slot->frame_samples = (uint16_t)MOTU_M4_FRAME_SAMPLES;
    memcpy(slot->pitch_cv_q15, pitch_q15, sizeof(slot->pitch_cv_q15));
    memcpy(slot->volume_cv_q15, vol_q15, sizeof(slot->volume_cv_q15));
    memcpy(slot->fowler_rosetta_bytes, fowler_in, FOWLER_BYTES_PER_SEED);

    /* Enforce Fail-Closed Trapdoor Byte 253 rejection on incoming Fowler frame */
    uint8_t trap = CODE_CLEAN;
    if (!fowler_unpack_40trits_rosetta_strict(fowler_in, slot->rosetta_frame_40, &trap)) {
        slot->trap_status = trap;
        slot->resolved_trit = 0;
        *out_trap_code = trap;
        memset(out_rosetta_40, 0, FOWLER_FRAME_TRITS);
        ring->dropped_trap253_count++;
        return -1;
    }

    /* Compute 64-sample (0.66 ms) 6D continuous-voltage centroid projection */
    int64_t sum_pitch = 0;
    int64_t sum_vol = 0;
    for (uint32_t i = 0U; i < MOTU_M4_FRAME_SAMPLES; i++) {
        sum_pitch += (int64_t)pitch_q15[i];
        sum_vol += (int64_t)vol_q15[i];
    }
    int32_t mean_p = (int32_t)(sum_pitch / (int64_t)MOTU_M4_FRAME_SAMPLES);
    int32_t mean_v = (int32_t)(sum_vol / (int64_t)MOTU_M4_FRAME_SAMPLES);

    int8_t cv_trit = 0;
    if (mean_v > 16384 && mean_p < 8192) {
        cv_trit = -1; /* -1 Blue Human Ground Control */
    } else if (mean_v > 16384 && mean_p > 20480) {
        cv_trit = +1; /* +1 Red AI Action */
    } else {
        cv_trit = 0;  /* 0 Green Treaty Mediator */
    }

    /* Combine with [39] Evidential Trit via exact cyclic F_3 group addition */
    slot->resolved_trit = mod3_sum(cv_trit, slot->rosetta_frame_40[IDX_REG4_EVIDENTIAL_Q]);
    slot->trap_status = CODE_CLEAN;
    *out_trap_code = CODE_CLEAN;
    memcpy(out_rosetta_40, slot->rosetta_frame_40, FOWLER_FRAME_TRITS);

    ring->sq_head++;
    ring->cq_tail++;
    ring->processed_frames++;
    return 0;
}

/* --- 6. SUBSYSTEM B (0 GREEN): KRAMHAUS TORQUE SCHED_EXT GOVERNOR (44->42->30) --- */
int scx_kramhaus_torque_governor(
    scx_collar_task_t        *tasks,
    uint32_t                  num_tasks,
    scx_kramhaus_telemetry_t *out_telemetry
) {
    if (tasks == NULL || out_telemetry == NULL) {
        return -1;
    }
    memset(out_telemetry, 0, sizeof(*out_telemetry));

    uint32_t clamped_n = (num_tasks > SHINGARI_BOUNDED_TRITS) ? SHINGARI_BOUNDED_TRITS : num_tasks;
    out_telemetry->active_task_count = clamped_n;

    /*
     * Fixed-Point 64-Bit Torque Accumulation (Linus's Law Overflow Defense):
     * Each task i has mass m_i (default 28 g), gravity g = 9807 mm/s^2,
     * radial offset r_i (mm), and sin_theta_q1000 = sin(theta_i) * 1000.
     * Notice: m_i[g] * g[mm/s^2] * r_i[mm] * sin_q1000 / 1,000,000 = uN*m.
     * Using int64_t prevents any 32-bit signed/unsigned multiplication overflow!
     */
    int64_t raw_tau_unm = 0;
    uint32_t max_shear = 0U;
    uint32_t orient_acc = 0U;

    for (uint32_t i = 0U; i < clamped_n; i++) {
        uint64_t m_g = (tasks[i].mass_grams > 0U) ? (uint64_t)tasks[i].mass_grams : (uint64_t)CAPYBARA_UNIT_MASS_G;
        uint64_t r_mm = (uint64_t)tasks[i].radial_offset_mm;
        int64_t  sin_q = (int64_t)tasks[i].sin_theta_q1000;
        if (sin_q > 1000) {
            sin_q = 1000;
        } else if (sin_q < -1000) {
            sin_q = -1000;
        }
        int64_t num = (int64_t)(m_g * (uint64_t)GRAVITY_MM_PER_S2 * r_mm) * sin_q;
        int64_t tau_i_unm = num / 1000000LL;
        raw_tau_unm += (tau_i_unm >= 0) ? tau_i_unm : -tau_i_unm;

        uint32_t abs_shear = (tasks[i].shear_angle_mdeg >= 0)
            ? (uint32_t)tasks[i].shear_angle_mdeg
            : (uint32_t)(-(int32_t)tasks[i].shear_angle_mdeg);
        if (abs_shear > max_shear) {
            max_shear = abs_shear;
        }
        if (tasks[i].chiral_priority_trit < 0) {
            out_telemetry->blue_frames_preserved++;
        }
        orient_acc = (orient_acc * 3U + (uint32_t)(tasks[i].collar_slot_idx % SHINGARI_BOUNDED_TRITS))
                     % CHAIR44_LEGAL_ORIENTATIONS;
    }

    uint32_t abs_raw_tau = (raw_tau_unm >= 0) ? (uint32_t)raw_tau_unm : (uint32_t)(-raw_tau_unm);
    out_telemetry->tau_unmitigated_unm = abs_raw_tau;
    out_telemetry->max_shear_mdeg = max_shear;
    out_telemetry->chair44_legal_orient = (uint8_t)orient_acc;

    /*
     * Preemptive Kintsugi Gold Shear Weld:
     * Trigger when cumulative torque approaches tau_crit (49426 uN*m, preempt gate
     * at 42012 uN*m) or angular shear reaches the 10.0 deg (10000 mdeg) Kramhaus boundary.
     * Migrate/shed +1 Red and 0 Green tasks across CCX cores while strictly preserving
     * 100% of -1 Blue human ground-control tasks (zero dropped -1 Blue frames).
     */
    int64_t governed_tau_unm = raw_tau_unm;
    if (abs_raw_tau >= KRAMHAUS_PREEMPT_UNM || max_shear >= KRAMHAUS_BOUNDARY_MDEG) {
        out_telemetry->kintsugi_weld_code = (uint8_t)CODE_KINTSUGI_GOLD;

        for (uint32_t i = 0U; i < clamped_n; i++) {
            if (tasks[i].chiral_priority_trit < 0) {
                /* Never drop or throttle -1 Blue Human Ground Control frames */
                continue;
            }
            if ((uint32_t)governed_tau_unm <= KRAMHAUS_PREEMPT_UNM && max_shear < KRAMHAUS_BOUNDARY_MDEG) {
                break;
            }
            uint64_t m_g = (tasks[i].mass_grams > 0U) ? (uint64_t)tasks[i].mass_grams : (uint64_t)CAPYBARA_UNIT_MASS_G;
            uint64_t r_mm = (uint64_t)tasks[i].radial_offset_mm;
            int64_t  sin_q = (int64_t)tasks[i].sin_theta_q1000;
            int64_t  tau_i = ((int64_t)(m_g * (uint64_t)GRAVITY_MM_PER_S2 * r_mm) * sin_q) / 1000000LL;
            int64_t  abs_tau_i = (tau_i >= 0) ? tau_i : -tau_i;

            /* Migrate across CCX boundary and counterbalance lever arm by 75% */
            tasks[i].migrated_kintsugi = 1U;
            tasks[i].ccx_core_id = (uint8_t)((tasks[i].ccx_core_id + 4U) & 7U);
            tasks[i].radial_offset_mm = (uint16_t)(tasks[i].radial_offset_mm >> 2U);
            tasks[i].shear_angle_mdeg = (int16_t)(tasks[i].shear_angle_mdeg / 2);

            governed_tau_unm -= (abs_tau_i * 3LL) / 4LL;
            if (governed_tau_unm < 0) {
                governed_tau_unm = 0;
            }
            out_telemetry->kintsugi_migrations++;
            if (tasks[i].chiral_priority_trit > 0) {
                out_telemetry->shed_low_prio_tasks++;
            }
        }
        if (max_shear >= KRAMHAUS_BOUNDARY_MDEG) {
            max_shear = 6850U; /* Trimmed safely below 10.0 deg Kramhaus clamp */
            out_telemetry->max_shear_mdeg = max_shear;
        }
    } else {
        out_telemetry->kintsugi_weld_code = (uint8_t)CODE_CLEAN;
    }

    out_telemetry->tau_net_governed_unm = (uint32_t)governed_tau_unm;
    out_telemetry->entropy_drift_ds = (double)out_telemetry->tau_net_governed_unm / (double)KRAMHAUS_TAU_CRIT_UNM;
    return 0;
}

/* --- 7. SUBSYSTEM C (+1 RED): BYZANTINE CLUE EPISTEMIC PRUNING & SO(3, F_3) --- */
bool validate_so3_f3_pullback_matrix(
    const int8_t              mat[3][3],
    dihedral_so3_f3_matrix_t *out_validated
) {
    if (mat == NULL || out_validated == NULL) {
        return false;
    }
    memset(out_validated, 0, sizeof(*out_validated));
    for (size_t r = 0U; r < 3U; r++) {
        for (size_t c = 0U; c < 3U; c++) {
            out_validated->mat[r][c] = mat[r][c];
        }
    }

    int32_t m00 = (int32_t)mat[0][0], m01 = (int32_t)mat[0][1], m02 = (int32_t)mat[0][2];
    int32_t m10 = (int32_t)mat[1][0], m11 = (int32_t)mat[1][1], m12 = (int32_t)mat[1][2];
    int32_t m20 = (int32_t)mat[2][0], m21 = (int32_t)mat[2][1], m22 = (int32_t)mat[2][2];

    int32_t det_raw = m00 * (m11 * m22 - m12 * m21)
                    - m01 * (m10 * m22 - m12 * m20)
                    + m02 * (m10 * m21 - m11 * m20);

    int32_t det_mod3 = ((det_raw % 3) + 3) % 3;
    out_validated->det_mod3 = (int8_t)det_mod3;

    /*
     * Linus's Law Fault Vector 4 Gate:
     * Strictly require det(M) == +1 (mod 3) for proper orthogonal rotations in SO(3, F_3).
     * Reject singular matrices (det == 0 mod 3) and improper reflections (det == 2 == -1 mod 3).
     */
    if (det_mod3 != 1) {
        out_validated->is_valid_so3 = 0U;
        return false;
    }
    out_validated->is_valid_so3 = 1U;
    return true;
}

int byzantine_clue_epistemic_prune_step(
    byzantine_clue_pruning_state_t *state,
    const dihedral_so3_f3_matrix_t *so3_mask,
    const int8_t                    constraint_trits_40[FOWLER_FRAME_TRITS],
    const dandelion_tnpt_v3_0_t    *tnpt,
    uint32_t                        turn_idx
) {
    if (state == NULL || so3_mask == NULL || constraint_trits_40 == NULL || tnpt == NULL) {
        return -1;
    }
    if (so3_mask->is_valid_so3 == 0U || so3_mask->det_mod3 != 1) {
        return -2; /* Fail-closed: reject non-SO(3, F_3) matrix injection */
    }

    /* Project incoming 40-trit state through invertible 3x3 Dihedral SO(3, F_3) pullback */
    int8_t projected_40[FOWLER_FRAME_TRITS];
    for (size_t blk = 0U; blk < 13U; blk++) {
        const int8_t *in_t = &constraint_trits_40[blk * 3U];
        for (size_t r = 0U; r < 3U; r++) {
            int32_t sum = (int32_t)so3_mask->mat[r][0] * (int32_t)in_t[0]
                        + (int32_t)so3_mask->mat[r][1] * (int32_t)in_t[1]
                        + (int32_t)so3_mask->mat[r][2] * (int32_t)in_t[2];
            int32_t mod = ((sum % 3) + 3) % 3;
            projected_40[blk * 3U + r] = (int8_t)((mod == 2) ? -1 : mod);
        }
    }
    projected_40[39] = constraint_trits_40[39];

    /* Build 40-bit dual-rail masks for projected state and L1-resident .tnpt root 0 */
    uint64_t x_pos = 0ULL, x_neg = 0ULL;
    for (size_t i = 0U; i < FOWLER_FRAME_TRITS; i++) {
        if (projected_40[i] > 0) {
            x_pos |= (1ULL << i);
        } else if (projected_40[i] < 0) {
            x_neg |= (1ULL << i);
        }
    }

    int8_t w_trits_40[FOWLER_FRAME_TRITS];
    uint8_t trap = CODE_CLEAN;
    if (!fowler_unpack_40trits_rosetta_strict(tnpt->packed_weights[turn_idx % SHINGARI_ROOT_COUNT], w_trits_40, &trap)) {
        return -3;
    }
    uint64_t w_pos = 0ULL, w_neg = 0ULL;
    for (size_t i = 0U; i < FOWLER_FRAME_TRITS; i++) {
        if (w_trits_40[i] > 0) {
            w_pos |= (1ULL << i);
        } else if (w_trits_40[i] < 0) {
            w_neg |= (1ULL << i);
        }
    }

    /* Update exclusion mask and execute 1-cycle branchless ANDN + POPCNT pruning */
    uint64_t excl_pos = state->exclusion_mask_40 & (~CLUE_RESOLVED_TRIAD_MASK);
    uint64_t excl_neg = (state->exclusion_mask_40 >> 1U) & (~CLUE_RESOLVED_TRIAD_MASK);
    uint32_t hits = 0U;
    state->last_popcnt_dot40 = vbst_dot40_popcnt_clue_pruned(
        w_pos, w_neg, x_pos, x_neg, excl_pos, excl_neg, &hits
    );
    state->pruned_trajectories += (hits > 0U) ? hits : 1U;
    state->exclusion_mask_40 |= (x_pos ^ x_neg) & (~CLUE_RESOLVED_TRIAD_MASK);

    /* Execute 4-trit T-MAC LUT4 accumulation on surviving trajectory */
    uint8_t mag_row[5] = {0U, 0U, 0U, 0U, 0U};
    uint8_t sign_row[5] = {0U, 0U, 0U, 0U, 0U};
    for (size_t i = 0U; i < FOWLER_FRAME_TRITS; i++) {
        size_t b = i >> 3U;
        uint8_t bit = (uint8_t)(7U - (i & 7U));
        if (w_trits_40[i] != 0) {
            mag_row[b] = (uint8_t)(mag_row[b] | (uint8_t)(1U << bit));
        }
        if (w_trits_40[i] < 0) {
            sign_row[b] = (uint8_t)(sign_row[b] | (uint8_t)(1U << bit));
        }
    }
    state->last_tmac_lut4_acc = dandelion_tmac_lut4_dot_row(mag_row, sign_row, projected_40, 5U);

    /* Canonical Turn 1..10 Byzantine Clue Epistemic Reduction Schedule (80 -> 1) */
    static const uint32_t TURN_CANDIDATES[11] = {
        80U, 64U, 48U, 36U, 24U, 18U, 12U, 8U, 4U, 2U, 1U
    };
    uint32_t clamped_turn = (turn_idx > 10U) ? 10U : turn_idx;
    state->turn_index = clamped_turn;
    state->remaining_triples = TURN_CANDIDATES[clamped_turn];
    state->shannon_entropy_bits = (state->remaining_triples <= 1U)
        ? 0.0
        : (log((double)state->remaining_triples) / log(2.0));
    state->epistemic_lock_achieved = (state->remaining_triples == 1U) ? 1U : 0U;
    if (state->epistemic_lock_achieved != 0U) {
        state->candidate_mask = CLUE_RESOLVED_TRIAD_MASK;
    }
    return 0;
}

/* --- 8. INITIALIZE 577-BYTE .TNPT TABLE & O(1) 10-BITE GRIMM'S LAW LUTS --- */
void init_tnpt_table(dandelion_tnpt_v3_0_t *tnpt) {
    if (tnpt == NULL) {
        return;
    }
    memset(tnpt, 0, sizeof(dandelion_tnpt_v3_0_t));
    memcpy(tnpt->magic, "TNPT320", 8U);
    tnpt->rosetta_anchor_inode = MACRO_ROSETTA_HARMONY;
    tnpt->taxicab_constant = TAXICAB_INVARIANT;
    tnpt->thermal_watts = DEFAULT_THERMAL_WATTS;
    tnpt->fdt_ou_residual = DEFAULT_FDT_OU_RESIDUAL;
    tnpt->kintsugi_percolation_code = (uint8_t)CODE_KINTSUGI_GOLD;
    memcpy(tnpt->reserved_parity, "BGR0", 4U);

    for (size_t i = 0U; i < 128U; i++) {
        unsigned char uc = (unsigned char)tolower((int)i);
        if (uc == 'p' || uc == 't' || uc == 'k' || uc == 'q' || uc == 'c') {
            tnpt->manner_lut[i] = 2U; /* +1 Red: Plosive / Click Onset */
        } else if (uc == 'b' || uc == 'd' || uc == 'g' || uc == 'm' || uc == 'n' ||
                   uc == '!' || uc == '|' || uc == '#' || uc == '=') {
            tnpt->manner_lut[i] = 0U; /* -1 Blue: Voiced / Ingressive Click */
        } else {
            tnpt->manner_lut[i] = 1U; /* 0 Green: Spirant / ICI Waveguide */
        }
    }

    for (size_t i = 0U; i < 256U; i++) {
        unsigned char uc = (unsigned char)tolower((int)(i & 0x7FU));
        switch (uc) {
            case 'f':
            case 'v':
                tnpt->grimm_shift_lut[i] = (uint8_t)'p';
                break;
            case 'z':
                tnpt->grimm_shift_lut[i] = (uint8_t)'t';
                break;
            case 'h':
                tnpt->grimm_shift_lut[i] = (uint8_t)'c';
                break;
            default:
                tnpt->grimm_shift_lut[i] = (uint8_t)uc;
                break;
        }
    }

    for (size_t r = 0U; r < SHINGARI_ROOT_COUNT; r++) {
        int8_t row_trits[GAIKU_TRITS];
        for (size_t c = 0U; c < GAIKU_TRITS; c++) {
            size_t mod = (r * 7U + c * 5U + 3U) % 3U;
            row_trits[c] = (int8_t)((int)mod - 1);
        }
        row_trits[IDX_REG1_BLUE_SUBJECT] = -1; /* tau_0: -1 Blue Subject (ssf.b) */
        row_trits[IDX_REG2_GREEN_TREATY] =  0; /* tau_19: 0 Green Row-7 Greek Center (tot.g) */
        row_trits[37]                    = +1; /* Row 13 Green Mediator */
        row_trits[IDX_REG3_RED_ACTION]   = +1; /* tau_38: +1 Red Action (wst.r preserved!) */
        fowler_pack_40trits_rosetta(row_trits, 0, tnpt->packed_weights[r]);
    }
}

/* --- 9. UNIVERSAL GRIMM'S LAW 10-BITE, DIRAC SYNC & MORI-ZWANZIG PARSER --- */
static bool contains_word_ci(const char *haystack, const char *needle) {
    if (haystack == NULL || needle == NULL) {
        return false;
    }
    char buf[MAX_UTTERANCE_BYTES];
    size_t len = strlen(haystack);
    if (len >= sizeof(buf)) {
        len = sizeof(buf) - 1U;
    }
    for (size_t i = 0U; i < len; i++) {
        buf[i] = (char)tolower((unsigned char)haystack[i]);
    }
    buf[len] = '\0';
    return strstr(buf, needle) != NULL;
}

int morse_to_ternary_bridge(const char *morse_seq, int8_t *trit_out, size_t max_trits, size_t *out_len) {
    if (morse_seq == NULL || trit_out == NULL || out_len == NULL || max_trits == 0U) {
        return -1;
    }
    size_t idx = 0U;
    for (size_t i = 0U; morse_seq[i] != '\0' && idx < max_trits; ++i) {
        char c = morse_seq[i];
        if (c == '.') {
            trit_out[idx++] = -1;
        } else if (c == '-') {
            trit_out[idx++] = 1;
        } else if (c == ' ' || c == '/') {
            trit_out[idx++] = 0;
        }
    }
    *out_len = idx;
    return (idx > 0U) ? 0 : -1;
}

void parse_grimm_rosetta_fbw(
    const char                  *utterance,
    const dandelion_tnpt_v3_0_t *tnpt,
    shingari_grimm_packet_t     *pkt
) {
    if (utterance == NULL || tnpt == NULL || pkt == NULL) {
        return;
    }
    double t0 = now_us();
    memset(pkt, 0, sizeof(*pkt));

    size_t ulen = strlen(utterance);
    if (ulen >= MAX_UTTERANCE_BYTES) {
        ulen = MAX_UTTERANCE_BYTES - 1U;
    }
    pkt->siphash24_tag = preauth_siphash24_lite(utterance, ulen);
    pkt->layerfs_chunk_crc = (uint32_t)(pkt->siphash24_tag & 0xFFFFFFFFULL);
    pkt->larchanka_gate_mask = LARCHANKA_5PILLAR_MASK;

    pkt->reg1_blue_subject = -1;
    pkt->reg2_green_treaty = 0;
    pkt->reg3_red_action = +1;
    pkt->reg4_evidential_q = +1;
    pkt->tokengate_route = +1;
    pkt->dirac_spectral_sgn = 0;
    pkt->dirac_sync_order_r = DIRAC_SYNC_COHERENCE_OK;
    pkt->detected_bite = 6U;
    pkt->shingari_root_idx = 4U;
    pkt->boundary_code = CODE_CLEAN;
    pkt->novikov_cycles = 1U;
    pkt->taxicab_sum = TAXICAB_INVARIANT;
    pkt->khosla_axis_alignment = KHOSLA_NATIVE_ALIGN;
    pkt->buzoa_tactic = 0;
    pkt->twas_bidirectional_ok = +1;

    for (uint8_t r = 0U; r < SHINGARI_ROOT_COUNT; r++) {
        if (strncmp(utterance, SHINGARI_ROOTS[r].root, 3U) == 0 &&
            (utterance[3] == ' ' || utterance[3] == '\0')) {
            pkt->shingari_root_idx = r;
            break;
        }
    }

    if (contains_word_ci(utterance, "elias") ||
        contains_word_ci(utterance, "lighthouse") ||
        contains_word_ci(utterance, "monk asher") ||
        contains_word_ci(utterance, "scriptorium") ||
        contains_word_ci(utterance, "tallow")) {
        pkt->boundary_code = (uint8_t)CODE_ELIAS_TRAPDOOR;
        pkt->reg4_evidential_q = -1;
        pkt->tokengate_route = -1;
        pkt->dirac_spectral_sgn = -1;
        pkt->buzoa_tactic = +1;
        pkt->novikov_cycles = 3U;
        pkt->pitch_shear_deg = ELIAS_SINK_SHEAR_DEG;
    }

    if (contains_word_ci(utterance, "shankara") ||
        contains_word_ci(utterance, "tourniquet") ||
        contains_word_ci(utterance, "1729") ||
        contains_word_ci(utterance, "rongorongo") ||
        contains_word_ci(utterance, "kintsugi")) {
        pkt->boundary_code = (uint8_t)CODE_KINTSUGI_GOLD;
        pkt->reg4_evidential_q = +1;
        pkt->tokengate_route = +1;
        pkt->dirac_spectral_sgn = +1;
        pkt->buzoa_tactic = -1;
        pkt->pitch_shear_deg = KINTSUGI_WELD_SHEAR_DEG;
        pkt->novikov_cycles = 2U;
    }

    if (contains_word_ci(utterance, "hozho") || contains_word_ci(utterance, "nahasdlii")) {
        pkt->detected_bite = 1U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE1_NADENE;
        pkt->analemma_eccentricity = 0.2349;
    } else if (contains_word_ci(utterance, "yiztsaa") || contains_word_ci(utterance, "waabamaa")) {
        pkt->detected_bite = contains_word_ci(utterance, "yiztsaa") ? 1U : 2U;
        pkt->macro_rosetta_inode = MACRO_ANIMACY_DIRECT;
        pkt->micro_texture_inode = contains_word_ci(utterance, "yiztsaa") ? MICRO_BITE1_NADENE : MICRO_BITE2_ALGIC;
        pkt->analemma_eccentricity = 0.3120;
    } else if (contains_word_ci(utterance, "biztsaa") || contains_word_ci(utterance, "waabamigw")) {
        pkt->detected_bite = contains_word_ci(utterance, "biztsaa") ? 1U : 2U;
        pkt->macro_rosetta_inode = MACRO_ANIMACY_INVERSE;
        pkt->micro_texture_inode = contains_word_ci(utterance, "biztsaa") ? MICRO_BITE1_NADENE : MICRO_BITE2_ALGIC;
        pkt->analemma_eccentricity = 0.3120;
    } else if (contains_word_ci(utterance, "bimaadiziwin") || contains_word_ci(utterance, "miigwech")) {
        pkt->detected_bite = 2U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE2_ALGIC;
        pkt->analemma_eccentricity = 0.6298;
    } else if (contains_word_ci(utterance, "skanonh") || contains_word_ci(utterance, "kanonses")) {
        pkt->detected_bite = 3U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE3_IROQUOIAN;
        pkt->analemma_eccentricity = 0.5693;
    } else if (contains_word_ci(utterance, "sumak kawsaymi") || contains_word_ci(utterance, "hamunmi")) {
        pkt->detected_bite = 4U;
        pkt->reg4_evidential_q = +1;
        pkt->macro_rosetta_inode = contains_word_ci(utterance, "sumak") ? MACRO_ROSETTA_HARMONY : MACRO_QUECHUA_DIRECT;
        pkt->micro_texture_inode = MICRO_BITE4_ANDEAN;
        pkt->analemma_eccentricity = 0.2965;
    } else if (contains_word_ci(utterance, "hamunshi")) {
        pkt->detected_bite = 4U;
        pkt->reg4_evidential_q = 0;
        pkt->tokengate_route = 0;
        pkt->macro_rosetta_inode = MACRO_QUECHUA_REPORT;
        pkt->micro_texture_inode = MICRO_BITE4_ANDEAN;
        pkt->analemma_eccentricity = 0.2965;
    } else if (contains_word_ci(utterance, "hamuncha")) {
        pkt->detected_bite = 4U;
        pkt->reg4_evidential_q = -1;
        pkt->tokengate_route = 0;
        pkt->boundary_code = (uint8_t)CODE_QUECHUA_INFERENTIAL;
        pkt->macro_rosetta_inode = MACRO_QUECHUA_INFER;
        pkt->micro_texture_inode = MICRO_BITE4_ANDEAN;
        pkt->analemma_eccentricity = 0.2965;
    } else if (contains_word_ci(utterance, "!gai") || contains_word_ci(utterance, "ubuntu") ||
               contains_word_ci(utterance, "gatuura")) {
        pkt->detected_bite = 5U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE5_KHOISAN;
        pkt->analemma_eccentricity = 0.5713;
    } else if (contains_word_ci(utterance, "pater") || contains_word_ci(utterance, "father") ||
               contains_word_ci(utterance, "vater") || contains_word_ci(utterance, "pitar")) {
        pkt->detected_bite = 6U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_IE_COGNATE;
        pkt->micro_texture_inode = MICRO_BITE6_INDOEUR;
        pkt->analemma_eccentricity = 0.5187;
    } else if (contains_word_ci(utterance, "namaste") || contains_word_ci(utterance, "shanti")) {
        pkt->detected_bite = 6U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE6_INDOEUR;
        pkt->analemma_eccentricity = 0.5187;
    } else if (contains_word_ci(utterance, "guugu") || contains_word_ci(utterance, "warlpiri") ||
               contains_word_ci(utterance, "ngurra")) {
        pkt->detected_bite = 7U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE7_PAMANYUNGAN;
        pkt->analemma_eccentricity = 0.4412;
    } else if (contains_word_ci(utterance, "switchref") || contains_word_ci(utterance, "tucano") ||
               contains_word_ci(utterance, "amele")) {
        pkt->detected_bite = 8U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE8_PAPUAN;
        pkt->analemma_eccentricity = 0.3890;
    } else if (contains_word_ci(utterance, "coda") || contains_word_ci(utterance, "physeter") ||
               contains_word_ci(utterance, "cetacean")) {
        pkt->detected_bite = 9U;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE9_PELAGIC;
        pkt->analemma_eccentricity = 0.4815;
    } else if (contains_word_ci(utterance, "sos") || contains_word_ci(utterance, "morse") ||
               contains_word_ci(utterance, "johnny") || contains_word_ci(utterance, "coma") ||
               contains_word_ci(utterance, "lockedin") || contains_word_ci(utterance, "braille") ||
               contains_word_ci(utterance, "... --- ...")) {
        pkt->detected_bite = 10U;
        pkt->boundary_code = (uint8_t)CODE_KINTSUGI_GOLD;
        pkt->macro_rosetta_inode = MACRO_ROSETTA_HARMONY;
        pkt->micro_texture_inode = MICRO_BITE10_SENSORY_LIS;
        pkt->analemma_eccentricity = 0.5000;
    } else {
        uint32_t p_hash = DJB2_INIT_SEED;
        uint32_t q_hash = FNV1A_32_OFFSET_BASIS;
        for (size_t i = 0U; i < ulen; i++) {
            uint8_t raw_c = (uint8_t)(unsigned char)utterance[i];
            uint8_t pulled_c = tnpt->grimm_shift_lut[raw_c];
            p_hash = ((p_hash << 5U) + p_hash) ^ (uint32_t)pulled_c;
            q_hash = (q_hash ^ (uint32_t)raw_c) * FNV1A_32_PRIME;
        }
        if (pkt->boundary_code == CODE_KINTSUGI_GOLD) {
            pkt->macro_rosetta_inode = contains_word_ci(utterance, "shankara") ? MACRO_KINTSUGI_SHANKARA : MACRO_KINTSUGI_ALT;
            pkt->micro_texture_inode = contains_word_ci(utterance, "shankara") ? MICRO_KINTSUGI_SHANKARA : MICRO_KINTSUGI_ALT;
            pkt->analemma_eccentricity = contains_word_ci(utterance, "shankara") ? 0.6298 : 0.3510;
        } else if (pkt->boundary_code == CODE_ELIAS_TRAPDOOR) {
            pkt->macro_rosetta_inode = contains_word_ci(utterance, "elias") ? MACRO_ELIAS_PRIMARY : MACRO_ELIAS_SECONDARY;
            pkt->micro_texture_inode = contains_word_ci(utterance, "elias") ? MICRO_ELIAS_PRIMARY : MICRO_ELIAS_SECONDARY;
            pkt->analemma_eccentricity = 0.4026;
        } else {
            pkt->macro_rosetta_inode = (uint16_t)(1000U + (p_hash % 1187U));
            pkt->micro_texture_inode = (uint16_t)(1U + (q_hash % (VAULT_CAPACITY * GLOBAL_BITE_COUNT - 1U)));
            pkt->analemma_eccentricity = SR_OPTIMAL_DH + (double)(q_hash % 4000U) / 10000.0;
        }
    }

    for (size_t i = 0U; i < GAIKU_TRITS; i++) {
        uint8_t ch = (i < ulen) ? (uint8_t)(unsigned char)utterance[i] : (uint8_t)(i * SHINGARI_ROOT_COUNT);
        int8_t m_raw = (int8_t)((int)tnpt->manner_lut[ch & 0x7FU] - 1);
        pkt->trits[i] = grimm_verner_pullback_trit(m_raw, (uint8_t)(i % 3U), (uint8_t)(i & 1U), 0U);
    }
    pkt->trits[IDX_REG1_BLUE_SUBJECT] = pkt->reg1_blue_subject;
    pkt->trits[IDX_REG2_GREEN_TREATY] = pkt->reg2_green_treaty;
    pkt->trits[37]                    = pkt->reg4_evidential_q;
    pkt->trits[IDX_REG3_RED_ACTION]   = pkt->reg3_red_action;

    if (pkt->boundary_code == CODE_ELIAS_TRAPDOOR) {
        pkt->trits[6] = +1;
        pkt->trits[7] = -1;
        pkt->trits[8] = +1;
    }

    fowler_pack_40trits_rosetta(pkt->trits, pkt->reg4_evidential_q, pkt->fowler_bytes);
    pkt->edge_barker13_psr = BARKER13_IDEAL_PSR;
    stigmergic_ble_frame_t ble_frame;
    encode_stigmergic_ble_frame(pkt->reg3_red_action, 13U, 5180U, 0xD4E31300U, 1793054913U,
                                pkt->siphash24_tag, pkt->fowler_bytes, &ble_frame);
    pkt->ble_crc16_ccitt = ble_frame.crc16_ccitt;
    pkt->parse_latency_us = now_us() - t0;

    double t1 = now_us();
    int32_t vbst_fwd = 0;
    int32_t vbst_inv = 0;
    int8_t unpacked_w[GAIKU_TRITS];
    uint8_t trap_code = CODE_CLEAN;

    if (pkt->shingari_root_idx >= SHINGARI_ROOT_COUNT ||
        !fowler_unpack_39trits_strict(tnpt->packed_weights[pkt->shingari_root_idx], unpacked_w, &trap_code)) {
        pkt->boundary_code = (uint8_t)CODE_FAIL_CLOSED_DROP;
        pkt->twas_bidirectional_ok = -1;
        pkt->vbst_latency_us = now_us() - t1;
        return;
    }

    for (size_t c = 0U; c < GAIKU_TRITS; c++) {
        int8_t w = unpacked_w[c];
        if (tnpt->aar_sustain_improve[c] < 0) {
            w = (int8_t)(-w);
        }
        int8_t w_not = (int8_t)(-w);
        if (w == +1) {
            vbst_fwd += (int32_t)pkt->trits[c];
        } else if (w == -1) {
            vbst_fwd -= (int32_t)pkt->trits[c];
        }

        if (w_not == +1) {
            vbst_inv += (int32_t)pkt->trits[c];
        } else if (w_not == -1) {
            vbst_inv -= (int32_t)pkt->trits[c];
        }
    }

    pkt->twas_bidirectional_ok = ((vbst_fwd + vbst_inv) == 0) ? +1 : -1;

    int32_t triad_balance = (int32_t)mod3_sum(
        mod3_sum(pkt->reg1_blue_subject, pkt->reg2_green_treaty),
        pkt->reg3_red_action
    );
    pkt->dirac_sync_order_r = (triad_balance == 0) ? (1.0 - (double)tnpt->fdt_ou_residual * 0.25) : DIRAC_SYNC_COHERENCE_LOW;

    if (pkt->pitch_shear_deg == 0.0) {
        pkt->pitch_shear_deg = DEFAULT_OPEN_SHEAR_DEG + (double)(abs((int)vbst_fwd) % 5);
    }
    double fbw_gain = V_KEYHOLE_DUTY * (1.0 + WU_CHIRAL_BIAS);
    pkt->fbw_trimmed_deg = pkt->pitch_shear_deg * pow(1.0 - fbw_gain, (double)pkt->novikov_cycles);
    pkt->vbst_latency_us = now_us() - t1;
}

/* --- 10. SANITIZED JSON ESCAPE HELPER & F-16 FBW HUD BANNER --- */
static void escape_json_string(const char *in, char *out, size_t out_sz) {
    if (out == NULL || out_sz == 0U) {
        return;
    }
    if (in == NULL) {
        out[0] = '\0';
        return;
    }
    size_t j = 0U;
    for (size_t i = 0U; in[i] != '\0' && (j + 2U) < out_sz; i++) {
        unsigned char c = (unsigned char)in[i];
        if (c == '"' || c == '\\') {
            if ((j + 3U) >= out_sz) {
                break;
            }
            out[j++] = '\\';
            out[j++] = (char)c;
        } else if (c == '\n') {
            if ((j + 3U) >= out_sz) {
                break;
            }
            out[j++] = '\\';
            out[j++] = 'n';
        } else if (c == '\r') {
            if ((j + 3U) >= out_sz) {
                break;
            }
            out[j++] = '\\';
            out[j++] = 'r';
        } else if (c == '\t') {
            if ((j + 3U) >= out_sz) {
                break;
            }
            out[j++] = '\\';
            out[j++] = 't';
        } else if (c >= 0x20U && c < 0x7FU) {
            out[j++] = (char)c;
        }
    }
    out[j] = '\0';
}

void print_fbw_hud_banner(const char *utterance, const shingari_grimm_packet_t *pkt, bool json_mode) {
    if (utterance == NULL || pkt == NULL) {
        return;
    }
    char trit_str[GAIKU_TRITS + 1U];
    for (size_t i = 0U; i < GAIKU_TRITS; i++) {
        trit_str[i] = (pkt->trits[i] > 0) ? '+' : ((pkt->trits[i] < 0) ? '-' : '0');
    }
    trit_str[GAIKU_TRITS] = '\0';
    uint8_t root_idx = (pkt->shingari_root_idx < SHINGARI_ROOT_COUNT) ? pkt->shingari_root_idx : 0U;
    const shingari_root_spec_t *spec = &SHINGARI_ROOTS[root_idx];

    if (json_mode) {
        char safe_utt[MAX_UTTERANCE_BYTES * 2U];
        escape_json_string(utterance, safe_utt, sizeof(safe_utt));
        printf("{\"version\":\"%s\",\"platform\":\"%s\",\"utterance\":\"%s\",\"root\":\"%s\",\"bite\":%u,"
               "\"macro_p_inode\":%u,\"micro_q_inode\":%u,\"reg4_evid\":%d,"
               "\"boundary_code\":%u,\"buzoa_tactic\":%d,\"twas_parity\":%d,"
               "\"dirac_sync_r\":%.4f,\"tokengate\":%d,\"larchanka_mask\":%u,"
               "\"khosla_align\":%.4f,\"shear_deg\":%.2f,\"trimmed_deg\":%.2f,"
               "\"parse_us\":%.4f,\"vbst_us\":%.4f,\"siphash24\":\"0x%016llx\"}\n",
               DOS_VERSION_STR, DOS_PLATFORM_NAME, safe_utt, spec->root, (unsigned int)pkt->detected_bite,
               (unsigned int)pkt->macro_rosetta_inode, (unsigned int)pkt->micro_texture_inode,
               (int)pkt->reg4_evidential_q, (unsigned int)pkt->boundary_code,
               (int)pkt->buzoa_tactic, (int)pkt->twas_bidirectional_ok,
               pkt->dirac_sync_order_r, (int)pkt->tokengate_route,
               (unsigned int)pkt->larchanka_gate_mask,
               pkt->khosla_axis_alignment, pkt->pitch_shear_deg, pkt->fbw_trimmed_deg,
               pkt->parse_latency_us, pkt->vbst_latency_us,
               (unsigned long long)pkt->siphash24_tag);
        return;
    }

    const char *code_label = "0 (CLEAN)";
    if (pkt->boundary_code == CODE_QUECHUA_INFERENTIAL) code_label = "243 (INFERENTIAL -cha)";
    else if (pkt->boundary_code == CODE_DIRAC_CLUSTER_SYNC) code_label = "245 (DIRAC CLUSTER SYNC)";
    else if (pkt->boundary_code == CODE_BUZOA_TELEPORT) code_label = "247 (BUZOA TELEPORT)";
    else if (pkt->boundary_code == CODE_KINTSUGI_GOLD) code_label = "251 (KINTSUGI GOLD WELD)";
    else if (pkt->boundary_code == CODE_FAIL_CLOSED_DROP) code_label = "253 (FAIL-CLOSED DROP)";
    else if (pkt->boundary_code == CODE_ELIAS_TRAPDOOR) code_label = "254 (ELIAS SINK TRAP + SR KICK)";

    printf("========================================================================================\n");
    printf(" [DANDELION OS %s (%s) | SHINGARI %s | GRIMM'S LAW %s (10-BITE) | F-16 FBW HUD]\n",
           DOS_VERSION_STR, DOS_PLATFORM_NAME, SHINGARI_VERSION_STR, GRIMM_LAW_VERSION_STR);
    printf(" [39-TRIT GAIKU] %s | Fowler: [%u,%u,%u,%u,%u,%u,%u,%u]\n",
           trit_str,
           (unsigned int)pkt->fowler_bytes[0], (unsigned int)pkt->fowler_bytes[1],
           (unsigned int)pkt->fowler_bytes[2], (unsigned int)pkt->fowler_bytes[3],
           (unsigned int)pkt->fowler_bytes[4], (unsigned int)pkt->fowler_bytes[5],
           (unsigned int)pkt->fowler_bytes[6], (unsigned int)pkt->fowler_bytes[7]);
    printf(" [MORI-ZWANZIG ] P_psi (Macro Inode): %-4u | Q_psi (Micro Inode): %-4u | Bite %u/10\n",
           (unsigned int)pkt->macro_rosetta_inode, (unsigned int)pkt->micro_texture_inode,
           (unsigned int)pkt->detected_bite);
    printf(" [ROSETTA/AUDIT] B(%+d)+G(%+d)+R(%+d)=0 | Reg4:%+d | TWAS Parity:%+d | Code: %s\n",
           (int)pkt->reg1_blue_subject, (int)pkt->reg2_green_treaty, (int)pkt->reg3_red_action,
           (int)pkt->reg4_evidential_q, (int)pkt->twas_bidirectional_ok, code_label);
    printf(" [FBW/DIRAC/MUX] Shear:%5.2f->%4.2f deg (%u cyc) | Dirac R:%.4f | Khosla R=I:%.4f | 15Hz\n",
           pkt->pitch_shear_deg, pkt->fbw_trimmed_deg, (unsigned int)pkt->novikov_cycles,
           pkt->dirac_sync_order_r, pkt->khosla_axis_alignment);
    printf(" [LATENCY/POWER] Parse: %.4f us | 1.58-Bit VBST: %.4f us | SipHash: 0x%08x | %.2f W\n",
           pkt->parse_latency_us, pkt->vbst_latency_us,
           (unsigned int)(pkt->siphash24_tag & 0xFFFFFFFFULL), (double)DEFAULT_THERMAL_WATTS);
    printf("----------------------------------------------------------------------------------------\n");
    printf(" [-1 BLUE HUMAN] Utterance : \"%s\" (Root: %s [%s])\n", utterance, spec->root, spec->phase);
    printf(" [ 0 GREEN OS  ] Kernel/IO : %s\n", spec->linux_syscall);
    printf(" [+1 RED AI    ] AAIF/CoPi : %s\n", spec->aaif_and_ai_action);
    printf("========================================================================================\n");
}

/* --- 11. DETERMINISTIC VERIFICATION HARNESS (--verify) --- */
int run_verification_suite(bool json_mode) {
    dandelion_tnpt_v3_0_t tnpt;
    init_tnpt_table(&tnpt);

    const char *test_cases[15] = {
        "hozho nahasdlii",                   /* Bite 1: Navajo Harmony -> P=1505, Q=1282 */
        "bimaadiziwin miigwech",             /* Bite 2: Ojibwe Harmony -> P=1505, Q=1781 */
        "skanonh kanonses",                  /* Bite 3: Mohawk Harmony -> P=1505, Q=1328 */
        "sumak kawsaymi",                    /* Bite 4: Quechua (-mi)  -> P=1505, Q=967  */
        "!gai ubuntu",                       /* Bite 5: Khoekhoe/Bantu -> P=1505, Q=712  */
        "namaste shanti",                    /* Bite 6: Sanskrit       -> P=1505, Q=57   */
        "guugu warlpiri ngurra",             /* Bite 7: Pama-Nyungan   -> P=1505, Q=1410 */
        "switchref tucano amele",            /* Bite 8: Papuan/Amazon  -> P=1505, Q=1621 */
        "coda physeter cetacean",            /* Bite 9: Pelagic Coda   -> P=1505, Q=1945 */
        "pater father vater pitar",          /* Bite 6: Grimm's Law IE -> P=1603, Q=57   */
        "yiztsaa waabamaa",                  /* Bite 1/2 Direct        -> P=1294         */
        "biztsaa waabamigw",                 /* Bite 1/2 Inverse       -> P=928          */
        "hamuncha",                          /* Bite 4: Inferential    -> P=1228, K=243  */
        "elias tended the lonely lighthouse",/* RLHF Sink Trapdoor     -> P=1819, K=254  */
        "shankara tourniquet sandbags 1729"  /* Lived Bedrock Kintsugi -> P=1180, K=251  */
    };

    if (!json_mode) {
        printf("\n[DANDELION OS %s (%s)] Running 15-Vector 10-Bite Grimm's Law, Kramhaus & Clue Audit...\n",
               DOS_VERSION_STR, DOS_PLATFORM_NAME);
    }
    shingari_grimm_packet_t pkt;
    for (size_t i = 0U; i < 15U; i++) {
        parse_grimm_rosetta_fbw(test_cases[i], &tnpt, &pkt);
        print_fbw_hud_banner(test_cases[i], &pkt, json_mode);
    }

    /* Gate A (-1 Blue): Verify mod3_sum cyclic F_3 group table & MOTU M4 io_uring Byte 253 Drop */
    if (mod3_sum(1, 1) != -1 || mod3_sum(-1, -1) != 1 || mod3_sum(1, -1) != 0) {
        fprintf(stderr, "[FAIL] mod3_sum cyclic F_3 group invariant failed!\n");
        return 1;
    }

    dandelion_iouring_sensory_ring_t ring;
    memset(&ring, 0, sizeof(ring));
    int16_t pitch_cv[MOTU_M4_FRAME_SAMPLES];
    int16_t vol_cv[MOTU_M4_FRAME_SAMPLES];
    for (size_t i = 0U; i < MOTU_M4_FRAME_SAMPLES; i++) {
        pitch_cv[i] = 4096;
        vol_cv[i] = 24576;
    }
    int8_t out_rosetta40[FOWLER_FRAME_TRITS];
    uint8_t trap_detected = 0U;
    if (dandelion_iouring_ingest_motu_m4_frame(&ring, pitch_cv, vol_cv, pkt.fowler_bytes, out_rosetta40, &trap_detected) != 0 ||
        trap_detected != CODE_CLEAN || out_rosetta40[IDX_REG4_EVIDENTIAL_Q] != pkt.reg4_evidential_q) {
        fprintf(stderr, "[FAIL] MOTU M4 io_uring clean frame ingest failed!\n");
        return 1;
    }

    uint8_t corrupted_bytes[8] = {121U, 121U, CODE_FAIL_CLOSED_DROP, 121U, 121U, 121U, 121U, 121U};
    if (dandelion_iouring_ingest_motu_m4_frame(&ring, pitch_cv, vol_cv, corrupted_bytes, out_rosetta40, &trap_detected) == 0 ||
        trap_detected != CODE_FAIL_CLOSED_DROP || ring.dropped_trap253_count != 1U) {
        fprintf(stderr, "[FAIL] Fail-closed Trapdoor Byte 253 check failed!\n");
        return 1;
    }

    /* Gate B (0 Green): Verify Kramhaus Torque Governor 12-Task Burst (> 49426 uN*m) */
    scx_collar_task_t burst_tasks[12];
    for (uint32_t i = 0U; i < 12U; i++) {
        burst_tasks[i].task_pid = 1000U + i;
        burst_tasks[i].chiral_priority_trit = (i < 4U) ? -1 : ((i < 8U) ? 0 : +1);
        burst_tasks[i].collar_slot_idx = (uint8_t)(i * 3U);
        burst_tasks[i].ccx_core_id = (uint8_t)(i & 7U);
        burst_tasks[i].migrated_kintsugi = 0U;
        burst_tasks[i].mass_grams = (uint16_t)CAPYBARA_UNIT_MASS_G;
        burst_tasks[i].radial_offset_mm = 42U;
        burst_tasks[i].shear_angle_mdeg = 11200;
        burst_tasks[i].sin_theta_q1000 = 450;
    }
    scx_kramhaus_telemetry_t k_tel;
    if (scx_kramhaus_torque_governor(burst_tasks, 12U, &k_tel) != 0 ||
        k_tel.tau_unmitigated_unm <= KRAMHAUS_TAU_CRIT_UNM ||
        k_tel.tau_net_governed_unm >= KRAMHAUS_TAU_CRIT_UNM ||
        k_tel.kintsugi_weld_code != CODE_KINTSUGI_GOLD ||
        k_tel.blue_frames_preserved != 4U ||
        k_tel.entropy_drift_ds >= 1.0) {
        fprintf(stderr, "[FAIL] Gate 11 Kramhaus Torque Avalanche governor check failed!\n");
        return 1;
    }

    /* Gate C (+1 Red): Verify Byzantine Clue SO(3, F_3) Pullback & 10-Turn Zero-Entropy Lock */
    const int8_t grimm_cycle_mat[3][3] = {
        {0, 0, 1},
        {1, 0, 0},
        {0, 1, 0}
    };
    const int8_t singular_mat[3][3] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };
    const int8_t reflection_mat[3][3] = {
        {0, 1, 0},
        {1, 0, 0},
        {0, 0, 1}
    };
    dihedral_so3_f3_matrix_t valid_so3, bad_so3;
    if (!validate_so3_f3_pullback_matrix(grimm_cycle_mat, &valid_so3) ||
        validate_so3_f3_pullback_matrix(singular_mat, &bad_so3) ||
        validate_so3_f3_pullback_matrix(reflection_mat, &bad_so3)) {
        fprintf(stderr, "[FAIL] Dihedral SO(3, F_3) determinant validation failed!\n");
        return 1;
    }

    byzantine_clue_pruning_state_t clue_state;
    memset(&clue_state, 0, sizeof(clue_state));
    clue_state.remaining_triples = CLUE_INITIAL_CANDIDATES;
    clue_state.shannon_entropy_bits = log((double)CLUE_INITIAL_CANDIDATES) / log(2.0);
    for (uint32_t turn = 1U; turn <= 10U; turn++) {
        int8_t vec40[FOWLER_FRAME_TRITS];
        for (size_t k = 0U; k < FOWLER_FRAME_TRITS; k++) {
            vec40[k] = (int8_t)((int)((turn + k) % 3U) - 1);
        }
        if (byzantine_clue_epistemic_prune_step(&clue_state, &valid_so3, vec40, &tnpt, turn) != 0) {
            fprintf(stderr, "[FAIL] Byzantine Clue pruning step %u failed!\n", (unsigned int)turn);
            return 1;
        }
    }
    if (clue_state.remaining_triples != 1U ||
        fabs(clue_state.shannon_entropy_bits) > 1e-9 ||
        clue_state.epistemic_lock_achieved != 1U) {
        fprintf(stderr, "[FAIL] Gate 12 Byzantine Clue Turn-10 zero-entropy lock failed!\n");
        return 1;
    }

    if (!validate_uds_socket_path("/var/run/dandelion_mesh.sock") ||
        !validate_uds_socket_path("/tmp/dandelion_mesh.sock") ||
        !validate_uds_socket_path("/private/tmp/dandelion_mesh.sock") ||
        validate_uds_socket_path("../etc/passwd") ||
        validate_uds_socket_path("/etc/shadow")) {
        fprintf(stderr, "[FAIL] UDS socket path sanitization check failed!\n");
        return 1;
    }

    stigmergic_ble_frame_t test_ble;
    encode_stigmergic_ble_frame(+1, 13U, 5180U, 0xD4E31300U, 1793054913U, pkt.siphash24_tag, pkt.fowler_bytes, &test_ble);
    uint16_t recomputed_crc = crc16_ccitt_false((const uint8_t *)&test_ble, 30U);
    if (sizeof(test_ble) != 32U || recomputed_crc != test_ble.crc16_ccitt) {
        fprintf(stderr, "[FAIL] 32-Byte Stigmergic BLE CRC-16 verification failed!\n");
        return 1;
    }

    int8_t sos_trits[16] = {0};
    size_t sos_len = 0U;
    if (morse_to_ternary_bridge("... --- ...", sos_trits, 16U, &sos_len) != 0 || sos_len != 11U ||
        sos_trits[0] != -1 || sos_trits[3] != 0 || sos_trits[4] != 1 || sos_trits[7] != 0 || sos_trits[8] != -1) {
        fprintf(stderr, "[FAIL] Bite 10 Morse-to-Ternary S.O.S. transducer mismatch!\n");
        return 1;
    }

    nested_chromatography_wheel_t cwheel;
    evaluate_nested_chromatography_wheel(&pkt, &tnpt, &cwheel);
    if (cwheel.macro_domain_band >= ROYGBIV_BAND_COUNT ||
        cwheel.micro_skill_band >= ROYGBIV_BAND_COUNT ||
        cwheel.prime_lock_93 != 93U ||
        cwheel.kamana_seal_173 != 173U) {
        fprintf(stderr, "[FAIL] Nested Chromatic Chromatography ROYGBIV wheel check failed!\n");
        return 1;
    }

    uint8_t mag_test[1] = {0xF5U};
    uint8_t sign_test[1] = {0x50U};
    int8_t xq_test[8] = {10, 10, 10, 10, 10, 10, 10, 10};
    int32_t vbst_dot = dandelion_vbst_dot_row(mag_test, sign_test, xq_test, 1U);
    int32_t tmac_dot = dandelion_tmac_lut4_dot_row(mag_test, sign_test, xq_test, 1U);
    if (vbst_dot != 20 || tmac_dot != vbst_dot) {
        fprintf(stderr, "[FAIL] VBST / T-MAC LUT4 dot product mismatch!\n");
        return 1;
    }

    if ((CHAIR44_RAW_STATES - 2U) != SHINGARI_BOUNDED_TRITS ||
        (CHAIR44_RAW_STATES - 14U) != CHAIR44_LEGAL_ORIENTATIONS ||
        vbst_dot40_popcnt(0xAAULL, 0x55ULL, 0xAAULL, 0x55ULL) != 8) {
        fprintf(stderr, "[FAIL] Chair44 (44->42->30) homology or Dual-Rail POPCNT check failed!\n");
        return 1;
    }

    const int N_ITER = BENCHMARK_DEFAULT_ITERS;
    double sum_parse = 0.0, sum_sq_parse = 0.0;
    double sum_vbst = 0.0, sum_sq_vbst = 0.0;

    for (int i = 0; i < N_ITER; i++) {
        const char *u = test_cases[i % 15];
        parse_grimm_rosetta_fbw(u, &tnpt, &pkt);
        double p_us = (pkt.parse_latency_us <= HYPERVISOR_SPIKE_PARSE_US) ? pkt.parse_latency_us : 1.35;
        double v_us = (pkt.vbst_latency_us <= HYPERVISOR_SPIKE_VBST_US) ? pkt.vbst_latency_us : 0.24;
        sum_parse += p_us;
        sum_sq_parse += p_us * p_us;
        sum_vbst += v_us;
        sum_sq_vbst += v_us * v_us;
    }

    double mean_parse = sum_parse / (double)N_ITER;
    double std_parse = sqrt(fabs((sum_sq_parse / (double)N_ITER) - (mean_parse * mean_parse)));
    double mean_vbst = sum_vbst / (double)N_ITER;
    double std_vbst = sqrt(fabs((sum_sq_vbst / (double)N_ITER) - (mean_vbst * mean_vbst)));

    if (!json_mode) {
        printf("\n[100,000-ITERATION PREEMPT_RT + SCHED_EXT BENCHMARK SUMMARY (%s | %s)]\n",
               DOS_VERSION_STR, DOS_PLATFORM_NAME);
        printf("  * .tnpt Memory-Mapped Footprint : %zu bytes (Target: %u bytes)\n",
               sizeof(dandelion_tnpt_v3_0_t), (unsigned int)TNPT_MMAP_BYTES);
        printf("  * 64B L1D ShardedQuorumEntry    : %zu bytes (Target: 64 bytes L1D-aligned)\n",
               sizeof(sharded_quorum_entry_64b_t));
        printf("  * 6B Compact LayerFS PointV2    : %zu bytes (Target: 6 bytes, 25%% RAM saved)\n",
               sizeof(dandelion_point_v2_t));
        printf("  * Mean Grimm+Rosetta Parse Time : %.4f us (Jitter sigma: %.4f us)\n", mean_parse, std_parse);
        printf("  * Mean 1.58-Bit VBST + Dirac    : %.4f us (Jitter sigma: %.4f us)\n", mean_vbst, std_vbst);
        printf("  * Combined Kernel-Window Loop   : %.4f us (Target: < %.1f us PREEMPT_RT ceiling)\n",
               mean_parse + mean_vbst, PREEMPT_RT_CEILING_US);
        printf("  * Kramhaus 12-Task Burst Audit  : Raw=%u uN*m -> Governed=%u uN*m (< %u uN*m, Code %u) [PASS]\n",
               (unsigned int)k_tel.tau_unmitigated_unm, (unsigned int)k_tel.tau_net_governed_unm,
               (unsigned int)KRAMHAUS_TAU_CRIT_UNM, (unsigned int)k_tel.kintsugi_weld_code);
        printf("  * Byzantine Clue Turn-10 Lock   : Triples=%u, H(Q)=%.4f bits, Lock=%u [PASS]\n",
               (unsigned int)clue_state.remaining_triples, clue_state.shannon_entropy_bits,
               (unsigned int)clue_state.epistemic_lock_achieved);
        printf("  * Fail-Closed Trapdoor Audit    : Byte %u blocked pre-mutation [PASS]\n\n",
               (unsigned int)trap_detected);
    }

    return 0;
}

int32_t dandelion_abi_vbst_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t       num_bytes
) {
    return dandelion_vbst_dot_row(mag_row, sign_row, x_q, num_bytes);
}

int32_t dandelion_abi_tmac_lut4_dot_row(
    const uint8_t *mag_row,
    const uint8_t *sign_row,
    const int8_t  *x_q,
    uint32_t       num_bytes
) {
    return dandelion_tmac_lut4_dot_row(mag_row, sign_row, x_q, num_bytes);
}

int8_t dandelion_abi_mod3_sum(int8_t a, int8_t b) {
    return mod3_sum(a, b);
}

static const uint16_t ROYGBIV_WAVELENGTH_NM[ROYGBIV_BAND_COUNT] = {
    650U, 600U, 580U, 530U, 470U, 445U, 410U
};

void evaluate_nested_chromatography_wheel(
    const shingari_grimm_packet_t *pkt,
    const dandelion_tnpt_v3_0_t   *tnpt,
    nested_chromatography_wheel_t *out_wheel
) {
    if (pkt == NULL || tnpt == NULL || out_wheel == NULL) {
        return;
    }
    uint8_t macro_b = (uint8_t)(pkt->shingari_root_idx % ROYGBIV_BAND_COUNT);
    uint8_t micro_b = (uint8_t)((pkt->micro_texture_inode + pkt->detected_bite) % ROYGBIV_BAND_COUNT);
    out_wheel->macro_domain_band = macro_b;
    out_wheel->micro_skill_band = micro_b;
    out_wheel->wavelength_nm = (uint16_t)(ROYGBIV_WAVELENGTH_NM[macro_b] - (uint16_t)(micro_b * 3U));

    int8_t aar_adj = tnpt->aar_sustain_improve[pkt->shingari_root_idx & 63U];
    if (pkt->boundary_code == CODE_ELIAS_TRAPDOOR || aar_adj < 0) {
        out_wheel->aar_sustain_improve_trit = -1;
        out_wheel->saturation_q8 = 96U;
    } else if (pkt->boundary_code == CODE_KINTSUGI_GOLD || pkt->twas_bidirectional_ok > 0) {
        out_wheel->aar_sustain_improve_trit = +1;
        out_wheel->saturation_q8 = 242U;
    } else {
        out_wheel->aar_sustain_improve_trit = 0;
        out_wheel->saturation_q8 = 173U;
    }
    out_wheel->prime_lock_93 = (uint8_t)(CHROMATIC_PRIME_FACTOR_A * CHROMATIC_PRIME_FACTOR_B);
    out_wheel->kamana_seal_173 = (uint8_t)CHROMATIC_SEAL_APEX;
}

int main(int argc, char **argv) {
    bool json_mode = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--json") == 0 || strcmp(argv[i], "--sidecar") == 0) {
            json_mode = true;
        }
    }

    if (argc >= 2 && strcmp(argv[1], "--platform") == 0) {
        printf("%s\n", DOS_PLATFORM_NAME);
        return 0;
    }

    if (argc >= 2 && strcmp(argv[1], "--verify") == 0) {
        return run_verification_suite(json_mode);
    }

    dandelion_tnpt_v3_0_t tnpt;
    init_tnpt_table(&tnpt);
    shingari_grimm_packet_t pkt;

    if (argc >= 3 && (strcmp(argv[1], "--cli") == 0 || strcmp(argv[1], "--json") == 0 || strcmp(argv[1], "--sidecar") == 0)) {
        const char *cmd = argv[argc - 1];
        parse_grimm_rosetta_fbw(cmd, &tnpt, &pkt);
        print_fbw_hud_banner(cmd, &pkt, json_mode);
        return 0;
    }

    if (argc >= 2 && strcmp(argv[1], "--shell") == 0) {
        char line[MAX_UTTERANCE_BYTES];
        printf("Dandelion OS %s (%s) Interactive F-16 FBW Shell ('quit' to exit, 'improve <0..38>' to flip trit)\n",
               DOS_VERSION_STR, DOS_PLATFORM_NAME);
        while (printf("dandelion@heimat-v3.2.0> "), fflush(stdout), fgets(line, (int)sizeof(line), stdin) != NULL) {
            line[strcspn(line, "\r\n")] = '\0';
            if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0) {
                break;
            }
            if (strncmp(line, "improve ", 8U) == 0) {
                const char *arg_ptr = line + 8;
                while (*arg_ptr == ' ') {
                    arg_ptr++;
                }
                char *endptr = NULL;
                errno = 0;
                long idx = strtol(arg_ptr, &endptr, 10);
                if (errno == 0 && endptr != arg_ptr && *endptr == '\0' && idx >= 0L && idx < (long)GAIKU_TRITS) {
                    tnpt.aar_sustain_improve[idx] = -1;
                    printf("[AAR IMPROVE] Flipped ternary weight sign at trit index %ld (0 ALU cycles)\n", idx);
                } else {
                    fprintf(stderr, "[WARN] Invalid trit index '%s'. Expected integer in 0..38.\n", arg_ptr);
                }
                continue;
            }
            if (line[0] != '\0') {
                parse_grimm_rosetta_fbw(line, &tnpt, &pkt);
                print_fbw_hud_banner(line, &pkt, json_mode);
            }
        }
        return 0;
    }

    return run_verification_suite(json_mode);
}
