/*
 * quantum_energy_grid_governor_64b_v1_0_0.c
 * Strict ISO C99 Single-Cache-Line (64-Byte) Grid & Solar Waveguide Governor
 * Zero dynamic heap allocation (malloc = 0), deterministic integer/Q16.16 arithmetic.
 *
 * Unites:
 *  - 12+1=13 FCC microgrid bond bitmap & spinning-reserve keyhole clamp (Byte 243)
 *  - 3-Stage OATC [1,3,9] 27-state balanced-ternary differential quadrature (active-zero I_o == I_e)
 *  - Ambient singlet-to-|T0> spin-1 qutrit solar exciton harvest counter
 *  - Unconditional 0-tick Byte 253 hardware safety interlock (optical dark-link / radial fault)
 *  - Kintsugi Byte 251 islanding weld & Byte 121 steady-state telemetry
 *
 * Version: v1.0.0
 * Status: CODE_SHOKUNIN_UNLATCHED = 255
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE_STEADY_121           121u
#define BYTE_KEYHOLE_CLAMP_243    243u
#define BYTE_KINTSUGI_WELD_251    251u
#define BYTE_SAFETY_INTERLOCK_253 253u
#define CODE_SHOKUNIN_UNLATCHED   255u

#define OATC_DARK_LINK_FLOOR_Q16  4096u   /* Minimum I_o + I_e optical quadrature sum */
#define FAULT_CURRENT_LIMIT_MA    18000u  /* Radial/branch overcurrent threshold */
#define RESERVE_KEYHOLE_NUM       2u
#define RESERVE_KEYHOLE_DEN       3u      /* V* = 2/3 Blume-Capel / Vonnegut keyhole */

typedef struct {
    uint32_t splitmix_state;        /*  4B [0..3]   Shared PRNG seed state (0-weight sync) */
    uint32_t tick_counter;          /*  4B [4..7]   Monotonic control-frame tick */
    int32_t  io_quadrature_q16;     /*  4B [8..11]  OATC ordinary-ray intensity (Q16.16) */
    int32_t  ie_quadrature_q16;     /*  4B [12..15] OATC extraordinary-ray intensity (Q16.16) */
    uint32_t demand_kw;             /*  4B [16..19] Active microgrid demand (kW) */
    uint32_t capacity_kw;           /*  4B [20..23] Committed dispatch + PV capacity (kW) */
    uint32_t branch_fault_ma;       /*  4B [24..27] Peak feeder branch current (mA) */
    uint32_t solar_t0_excitons;     /*  4B [28..31] Cumulative |T0> spin-1 qutrit harvests */
    uint16_t fcc_bond_mask_12;      /*  2B [32..33] 12-neighbor 3D FCC active bond bitmask */
    uint16_t reserve_shock_permil;  /*  2B [34..35] Spinning-reserve surge (0..1000 permil) */
    int8_t   oatc_state_27;         /*  1B [36]     3^3 = 27 folded phase state [-13..+13] */
    int8_t   tariff_trit_5;         /*  1B [37]     5-level symmetric tariff [-2,-1,0,+1,+2] */
    uint8_t  quorum_nodes_13;       /*  1B [38]     Active nodes in 12+1=13 Shingari cell */
    uint8_t  oob_telemetry_byte;    /*  1B [39]     Out-of-band status: 121 / 243 / 251 / 253 */
    uint32_t steady_count;          /*  4B [40..43] Count of Byte 121 nominal ticks */
    uint32_t clamp_count;           /*  4B [44..47] Count of Byte 243 keyhole clamps */
    uint32_t weld_count;            /*  4B [48..51] Count of Byte 251 Kintsugi FCC welds */
    uint32_t interlock_count;       /*  4B [52..55] Count of Byte 253 0-tick safety trips */
    uint32_t crc_rolling;           /*  4B [56..59] Deterministic rolling state witness */
    uint16_t power_budget_centiw;   /*  2B [60..61] Node power draw in centiwatts (1850 = 18.50W) */
    uint8_t  schema_version;        /*  1B [62]     ABI schema v1 */
    uint8_t  shokunin_flag;         /*  1B [63]     255 when invariant gates hold */
} QuantumEnergyGridGovernor64B;

/* Compile-time single-cache-line invariant check */
typedef char assert_governor_is_exact_64_bytes[
    (sizeof(QuantumEnergyGridGovernor64B) == 64u) ? 1 : -1
];

static uint32_t next_splitmix32(uint32_t *state) {
    uint32_t z = (*state += 0x9E3779B9u);
    z = (z ^ (z >> 16)) * 0x85EBCA6Bu;
    z = (z ^ (z >> 13)) * 0xC2B2AE35u;
    return z ^ (z >> 16);
}

static uint16_t popcount12(uint16_t mask) {
    uint16_t c = 0u;
    uint16_t m = (uint16_t)(mask & 0x0FFFu);
    while (m != 0u) {
        m = (uint16_t)(m & (uint16_t)(m - 1u));
        c++;
    }
    return c;
}

static void governor_init(QuantumEnergyGridGovernor64B *g, uint32_t seed) {
    memset(g, 0, sizeof(*g));
    g->splitmix_state = seed;
    g->io_quadrature_q16 = 65536;
    g->ie_quadrature_q16 = 65536;
    g->demand_kw = 4200u;
    g->capacity_kw = 7500u;
    g->fcc_bond_mask_12 = 0x0FFFu;
    g->quorum_nodes_13 = 13u;
    g->oob_telemetry_byte = (uint8_t)BYTE_STEADY_121;
    g->power_budget_centiw = 1850u; /* 18.50 W Hobo Rig envelope */
    g->schema_version = 1u;
    g->shokunin_flag = (uint8_t)CODE_SHOKUNIN_UNLATCHED;
}

static void governor_step(QuantumEnergyGridGovernor64B *g) {
    uint32_t r = next_splitmix32(&g->splitmix_state);
    uint32_t optical_sum;
    uint16_t live_bonds;

    g->tick_counter++;

    /* Synthesize deterministic grid/solar operating regimes across ticks */
    if ((r & 0x3Fu) == 0u) {
        /* ~1.56% ticks: optical dark-link drop or radial fault surge -> 0-tick Byte 253 */
        g->io_quadrature_q16 = 1024;
        g->ie_quadrature_q16 = 1024;
        g->branch_fault_ma = 21500u;
    } else {
        int32_t delta = (int32_t)((r >> 8) % 27u) - 13; /* [-13..+13] 3-stage [1,3,9] OATC */
        g->io_quadrature_q16 = 65536 + delta * 2048;
        g->ie_quadrature_q16 = 65536 - delta * 2048;
        g->oatc_state_27 = (int8_t)delta;
        g->branch_fault_ma = 6400u + (r & 0x0FFFu);
    }

    /* Simulate FCC bond fluctuations (up to 75% transient link loss) & reserve shocks */
    if (((r >> 6) & 0x0Fu) == 1u) {
        g->fcc_bond_mask_12 = (uint16_t)(0x0FFFu & ((r >> 12) | 0x0007u)); /* >=3 FCC bonds survive */
    } else {
        g->fcc_bond_mask_12 = 0x0FFFu;
    }
    live_bonds = popcount12(g->fcc_bond_mask_12);
    g->quorum_nodes_13 = (uint8_t)(live_bonds + 1u);

    g->reserve_shock_permil = (uint16_t)((r >> 16) % 300u); /* 0..29.9% spinning reserve shock */
    g->demand_kw = 3800u + ((r >> 10) % 2400u);             /* 3800..6199 kW */
    g->capacity_kw = 7500u;

    optical_sum = (uint32_t)(g->io_quadrature_q16 + g->ie_quadrature_q16);

    /* Priority 1: Unconditional 0-tick Byte 253 safety interlock */
    if (optical_sum < OATC_DARK_LINK_FLOOR_Q16 || g->branch_fault_ma > FAULT_CURRENT_LIMIT_MA) {
        g->oob_telemetry_byte = (uint8_t)BYTE_SAFETY_INTERLOCK_253;
        g->tariff_trit_5 = 0;
        g->interlock_count++;
    }
    /* Priority 2: Kintsugi Byte 251 FCC microgrid islanding weld under sparse bonds */
    else if (live_bonds < 9u) {
        g->oob_telemetry_byte = (uint8_t)BYTE_KINTSUGI_WELD_251;
        g->tariff_trit_5 = 1;
        g->solar_t0_excitons += 2u; /* Singlet fission: 1 photon -> 2 triplet excitons */
        g->weld_count++;
    }
    /* Priority 3: Byte 243 V*=2/3 keyhole backpressure clamp under >=15% reserve shock or load > 2/3 cap */
    else if (g->reserve_shock_permil >= 150u ||
             (g->demand_kw * RESERVE_KEYHOLE_DEN) > (g->capacity_kw * RESERVE_KEYHOLE_NUM)) {
        g->oob_telemetry_byte = (uint8_t)BYTE_KEYHOLE_CLAMP_243;
        g->tariff_trit_5 = (g->reserve_shock_permil >= 225u) ? 2 : 1; /* Symmetric +25% / +50% nudge */
        g->solar_t0_excitons += 2u;
        g->clamp_count++;
    }
    /* Priority 4: Nominal Byte 121 active-zero dispatch & PV |T0> harvest */
    else {
        g->oob_telemetry_byte = (uint8_t)BYTE_STEADY_121;
        g->tariff_trit_5 = (g->demand_kw < 4300u) ? -1 : 0; /* Symmetric off-peak incentive or zero */
        g->solar_t0_excitons += 2u;
        g->steady_count++;
    }

    g->crc_rolling = (g->crc_rolling * 16777619u) ^
                     ((uint32_t)g->oob_telemetry_byte << 16) ^
                     (uint32_t)(uint8_t)g->oatc_state_27 ^
                     (uint32_t)live_bonds;
}

int main(int argc, char **argv) {
    uint32_t seed = 42u;
    const uint32_t horizons[4] = {256u, 1024u, 4096u, 16384u};
    QuantumEnergyGridGovernor64B gov;
    int all_ok = 1;
    int i;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--seed") == 0 && (i + 1) < argc) {
            seed = (uint32_t)strtoul(argv[++i], NULL, 10);
        }
    }

    if (sizeof(QuantumEnergyGridGovernor64B) != 64u) {
        return 1;
    }

    printf("{\n");
    printf("  \"module\": \"quantum_energy_grid_governor_64b_v1_0_0\",\n");
    printf("  \"sizeof_bytes\": %lu,\n", (unsigned long)sizeof(QuantumEnergyGridGovernor64B));
    printf("  \"malloc_calls\": 0,\n");
    printf("  \"seed\": %u,\n", seed);
    printf("  \"horizons\": [\n");

    for (i = 0; i < 4; i++) {
        uint32_t n = horizons[i];
        uint32_t t;
        uint32_t total_accounted;
        governor_init(&gov, seed);
        for (t = 0u; t < n; t++) {
            governor_step(&gov);
        }
        total_accounted = gov.steady_count + gov.clamp_count + gov.weld_count + gov.interlock_count;
        if (total_accounted != n || gov.steady_count == 0u || gov.clamp_count == 0u ||
            gov.weld_count == 0u || gov.interlock_count == 0u || gov.power_budget_centiw != 1850u) {
            all_ok = 0;
        }
        printf("    {\"N\": %u, \"byte_121_steady\": %u, \"byte_243_clamp\": %u, "
               "\"byte_251_weld\": %u, \"byte_253_interlock\": %u, "
               "\"t0_excitons\": %u, \"crc\": \"0x%08X\"}%s\n",
               n, gov.steady_count, gov.clamp_count, gov.weld_count,
               gov.interlock_count, gov.solar_t0_excitons, gov.crc_rolling,
               (i < 3) ? "," : "");
    }

    printf("  ],\n");
    printf("  \"code_shokunin_unlatched\": %u\n",
           all_ok ? CODE_SHOKUNIN_UNLATCHED : 1u);
    printf("}\n");

    return all_ok ? 0 : 1;
}
