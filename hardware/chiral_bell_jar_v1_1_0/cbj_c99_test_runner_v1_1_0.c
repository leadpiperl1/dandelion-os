/*
 * cbj_c99_test_runner_v1_1_0.c
 * Deterministic Unit-Test Suite for Chiral Bell Jar 64B/32B Safety Interlocks,
 * CAD Q2.14 Phase-Trim LUT, and Kembali 64 KiB Zero-Copy Ring Hysteresis.
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243) | Version: v1.1.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define CBJ_TEST_FLOAT_REF 1
#include "cbj_cad_trim_lut_v1_1_0.h"

#define CBJ_GPIO_EXCITON_OUT    0u
#define CBJ_GPIO_FAIL_CLOSED_N  5u
#define CBJ_GPIO_LEG_DONE      10u

#define CBJ_CODE_NEUTRAL       121u
#define CBJ_CODE_STORM_HYST    242u
#define CBJ_CODE_KEYHOLE_BP    243u
#define CBJ_CODE_DARK_VORTEX   251u
#define CBJ_CODE_SAFETY_TRIP   253u

typedef struct {
    uint32_t magic_sync;
    uint32_t frame_seq;
    uint32_t erlang_dt_us;
    uint16_t trit_pos_mask;
    uint16_t trit_neg_mask;
    uint16_t vortex_null_mask;
    int16_t  chiral_wind_q8;
    int16_t  hk_adc[6];
    int16_t  phi_rad_q13[13];
    uint8_t  vbst_trits[3];
    uint8_t  kintsugi_code;
    uint16_t crc16_ccitt;
} cbj_spi_dma_frame_64b_t;

typedef struct {
    uint8_t  magic[4];
    uint32_t frame_seq;
    uint32_t erlang_dt_us;
    int16_t  chiral_wind_q8;
    uint16_t vortex_null_mask;
    int16_t  hk_adc[6];
    uint8_t  vbst_trits[3];
    uint8_t  kintsugi_code;
} stigmergic_ble_frame_t;

typedef struct {
    cbj_spi_dma_frame_64b_t slots[1024];
} cbj_kembali_ring_64k_t;

_Static_assert(sizeof(cbj_spi_dma_frame_64b_t) == 64u,    "64B frame ABI mismatch");
_Static_assert(sizeof(stigmergic_ble_frame_t)  == 32u,    "32B DND3 ABI mismatch");
_Static_assert(sizeof(cbj_kembali_ring_64k_t)  == 65536u, "64KiB Kembali ring mismatch");

extern void cbj_apply_cad_iq_trim_q14(uint8_t k, int16_t i_in, int16_t q_in,
                                      int16_t *i_out, int16_t *q_out);
extern bool cbj_process_104hz_frame(stigmergic_ble_frame_t *out_dnd3);

static cbj_spi_dma_frame_64b_t g_mock_spi_tx;
static bool     g_mock_spi_ok         = true;
static uint32_t g_fail_closed_low_cnt = 0u;
static uint32_t g_exciton_pulse_cnt   = 0u;
static uint32_t g_leg_done_pulse_cnt  = 0u;

static cbj_kembali_ring_64k_t g_kembali_ring __attribute__((aligned(4096)));

static void mock_reset_counters(void) {
    g_mock_spi_ok         = true;
    g_fail_closed_low_cnt = 0u;
    g_exciton_pulse_cnt   = 0u;
    g_leg_done_pulse_cnt  = 0u;
}

void cbj_hal_gpio_set_low(uint32_t pin) {
    if (pin == CBJ_GPIO_FAIL_CLOSED_N) {
        ++g_fail_closed_low_cnt;
    }
}

void cbj_hal_gpio_pulse(uint32_t pin) {
    if (pin == CBJ_GPIO_EXCITON_OUT)   { ++g_exciton_pulse_cnt; }
    if (pin == CBJ_GPIO_LEG_DONE)      { ++g_leg_done_pulse_cnt; }
}

bool cbj_hal_spi_dma_transfer_64b(cbj_spi_dma_frame_64b_t *rx_dst) {
    if (!g_mock_spi_ok) {
        return false;
    }
    memcpy(rx_dst, &g_mock_spi_tx, sizeof(cbj_spi_dma_frame_64b_t));
    return true;
}

static uint16_t ref_crc16_ccitt(const uint8_t *buf, size_t len) {
    uint16_t crc = 0xFFFFu;
    for (size_t i = 0u; i < len; ++i) {
        crc ^= (uint16_t)((uint16_t)buf[i] << 8);
        for (uint8_t b = 0u; b < 8u; ++b) {
            crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ 0x1021u)
                                  : (uint16_t)(crc << 1);
        }
    }
    return crc;
}

static void init_nominal_stimulus(cbj_spi_dma_frame_64b_t *f, uint32_t seq) {
    memset(f, 0, sizeof(*f));
    f->magic_sync    = 0x43424A31u;
    f->frame_seq     = seq;
    f->erlang_dt_us  = 9615u;
    f->hk_adc[0]     = 18000;
    f->hk_adc[1]     = 1200;
    f->hk_adc[2]     = 14000;
    f->hk_adc[3]     = 4200;
    f->hk_adc[4]     = 22938;
    f->hk_adc[5]     = 15500;
    f->kintsugi_code = CBJ_CODE_NEUTRAL;
    f->crc16_ccitt   = ref_crc16_ccitt((const uint8_t *)f, 62u);
}

static uint8_t cbj_kembali_storm_gate(uint8_t raw_code, uint64_t *win64,
                                      uint16_t *hyst_cnt) {
    if (raw_code == CBJ_CODE_SAFETY_TRIP) {
        *hyst_cnt = 104u;
        return CBJ_CODE_SAFETY_TRIP;
    }
    const uint64_t active_bit = (raw_code == CBJ_CODE_DARK_VORTEX) ? 1ULL : 0ULL;
    *win64 = (*win64 << 1) | active_bit;

    uint64_t x = *win64;
    uint8_t pop = 0u;
    while (x != 0ULL) {
        x &= (x - 1ULL);
        ++pop;
    }

    if (pop >= 43u) {
        if (*hyst_cnt < 104u) {
            ++(*hyst_cnt);
            return CBJ_CODE_STORM_HYST;
        }
        return CBJ_CODE_KEYHOLE_BP;
    }
    if (*hyst_cnt > 0u) {
        --(*hyst_cnt);
        return CBJ_CODE_STORM_HYST;
    }
    return raw_code;
}

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "[FAIL] %s (line %d)\n", (msg), __LINE__); \
        return 1; \
    } \
} while (0)

int main(void) {
    stigmergic_ble_frame_t dnd3;

    ASSERT_TRUE((((uintptr_t)&g_kembali_ring) & 4095u) == 0u, "64KiB ring 4K alignment");
    printf("[PASS] T1 ABI Parity: 64B SPI DMA, 32B DND3, 65,536B page-aligned Kembali ring\n");

    mock_reset_counters();
    init_nominal_stimulus(&g_mock_spi_tx, 1u);
    for (uint8_t k = 0u; k < 13u; ++k) {
        double phi = -3.141592653589793 + (2.0 * 3.141592653589793 * (double)k) / 13.0;
        g_mock_spi_tx.phi_rad_q13[k] = (int16_t)lrint(phi * 8192.0);
    }
    g_mock_spi_tx.crc16_ccitt = ref_crc16_ccitt((const uint8_t *)&g_mock_spi_tx, 62u);
    ASSERT_TRUE(cbj_process_104hz_frame(&dnd3), "Nominal CCW frame execution");
    ASSERT_TRUE(dnd3.vbst_trits[0] == 121u && dnd3.vbst_trits[1] == 121u &&
                dnd3.vbst_trits[2] == 121u, "All-zero trits -> Byte 121 neutral");
    ASSERT_TRUE(dnd3.chiral_wind_q8 == 1608, "CCW W=+1 winding == +1608 Q8.8");
    ASSERT_TRUE(g_fail_closed_low_cnt == 0u && g_exciton_pulse_cnt == 1u, "Exciton advance");

    for (uint8_t k = 0u; k < 13u; ++k) {
        g_mock_spi_tx.phi_rad_q13[k] = (int16_t)(-g_mock_spi_tx.phi_rad_q13[k]);
    }
    g_mock_spi_tx.trit_pos_mask = 0x1FFFu;
    g_mock_spi_tx.crc16_ccitt = ref_crc16_ccitt((const uint8_t *)&g_mock_spi_tx, 62u);
    ASSERT_TRUE(cbj_process_104hz_frame(&dnd3), "CW anti-soliton frame execution");
    ASSERT_TRUE(dnd3.chiral_wind_q8 == -1609, "CW W=-1 winding == -1609 Q8.8");
    ASSERT_TRUE(dnd3.vbst_trits[0] == 242u && dnd3.vbst_trits[1] == 242u &&
                dnd3.vbst_trits[2] == 134u, "VBST ceiling strictly <= 242");
    printf("[PASS] T2 Fowler-VBST Ceiling (<=242) & Telescoping Winding (+1608 / -1609)\n");

    mock_reset_counters();
    init_nominal_stimulus(&g_mock_spi_tx, 3u);
    g_mock_spi_tx.vortex_null_mask = (uint16_t)(1u << 7);
    g_mock_spi_tx.crc16_ccitt = ref_crc16_ccitt((const uint8_t *)&g_mock_spi_tx, 62u);
    ASSERT_TRUE(cbj_process_104hz_frame(&dnd3), "Dark-vortex frame accepted");
    ASSERT_TRUE(dnd3.kintsugi_code == CBJ_CODE_DARK_VORTEX, "Code 251 latched");
    ASSERT_TRUE(g_fail_closed_low_cnt == 0u && g_exciton_pulse_cnt == 1u, "No false RF mute");
    printf("[PASS] T3 Dark-Vortex Diagnostic (Code 251) Preserves Exciton Circulation\n");

    const uint8_t fault_ch[4]   = { 1u,    3u,     4u,     5u    };
    const int16_t fault_val[4]  = { 6555,  16385,  19659,  26870 };
    for (uint8_t f = 0u; f < 4u; ++f) {
        mock_reset_counters();
        init_nominal_stimulus(&g_mock_spi_tx, 10u + f);
        g_mock_spi_tx.hk_adc[fault_ch[f]] = fault_val[f];
        g_mock_spi_tx.crc16_ccitt = ref_crc16_ccitt((const uint8_t *)&g_mock_spi_tx, 62u);
        ASSERT_TRUE(cbj_process_104hz_frame(&dnd3), "Fault frame parsed cleanly");
        ASSERT_TRUE(dnd3.kintsugi_code == CBJ_CODE_SAFETY_TRIP, "Code 253 safety trip");
        ASSERT_TRUE(g_fail_closed_low_cnt == 1u, "GPIO 5 fail-closed asserted immediately");
        ASSERT_TRUE(g_exciton_pulse_cnt == 0u && g_leg_done_pulse_cnt == 1u, "Exciton halted");
    }
    printf("[PASS] T4 4-Vector Safety Interlocks (VSWR, Townsend, B0, Temp -> Code 253)\n");

    mock_reset_counters();
    memset(&dnd3, 0xAA, sizeof(dnd3));
    init_nominal_stimulus(&g_mock_spi_tx, 20u);
    g_mock_spi_tx.phi_rad_q13[6] ^= 0x0001;
    ASSERT_TRUE(!cbj_process_104hz_frame(&dnd3), "Corrupted CRC rejected");
    ASSERT_TRUE(g_fail_closed_low_cnt == 1u && dnd3.kintsugi_code == 0xAAu, "DND3 untouched");
    printf("[PASS] T5 Single-Bit SPI CRC Corruption Rejected Fail-Closed\n");

    uint64_t lcg = 42ULL;
    int32_t max_lsb_err = 0;
    for (uint8_t k = 0u; k < 13u; ++k) {
        const double c_ref = cos(g_cbj_ref_dtheta_rad[k]);
        const double s_ref = sin(g_cbj_ref_dtheta_rad[k]);
        for (uint16_t n = 0u; n < 100u; ++n) {
            lcg = lcg * 6364136223846793005ULL + 1442695040888963407ULL;
            int16_t i_in = (int16_t)((int32_t)((lcg >> 32) & 0x7FFFu) - 16384);
            lcg = lcg * 6364136223846793005ULL + 1442695040888963407ULL;
            int16_t q_in = (int16_t)((int32_t)((lcg >> 32) & 0x7FFFu) - 16384);
            int16_t i_out = 0, q_out = 0;
            cbj_apply_cad_iq_trim_q14(k, i_in, q_in, &i_out, &q_out);

            int32_t i_exp = (int32_t)lrint((double)i_in * c_ref - (double)q_in * s_ref);
            int32_t q_exp = (int32_t)lrint((double)i_in * s_ref + (double)q_in * c_ref);
            int32_t ei = (i_out > i_exp) ? (i_out - i_exp) : (i_exp - i_out);
            int32_t eq = (q_out > q_exp) ? (q_out - q_exp) : (q_exp - q_out);
            if (ei > max_lsb_err) { max_lsb_err = ei; }
            if (eq > max_lsb_err) { max_lsb_err = eq; }
        }
    }
    ASSERT_TRUE(max_lsb_err <= 2, "Q2.14 CAD rotation error <= 2 LSB");
    printf("[PASS] T6 Q2.14 CAD Trim Sweep (1,300 vectors, max_err=%d LSB)\n", max_lsb_err);

    uint64_t win64 = 0ULL;
    uint16_t hyst  = 0u;
    uint32_t false_storms = 0u;
    for (uint16_t slot = 0u; slot < 1024u; ++slot) {
        init_nominal_stimulus(&g_kembali_ring.slots[slot], slot);
        uint8_t raw = ((slot % 4u) == 0u) ? CBJ_CODE_DARK_VORTEX : CBJ_CODE_NEUTRAL;
        uint8_t gated = cbj_kembali_storm_gate(raw, &win64, &hyst);
        if (gated == CBJ_CODE_KEYHOLE_BP) {
            ++false_storms;
        }
    }
    ASSERT_TRUE(false_storms == 0u, "Zero false keyhole storms under sparse alpha kicks");
    ASSERT_TRUE(cbj_kembali_storm_gate(CBJ_CODE_SAFETY_TRIP, &win64, &hyst) == CBJ_CODE_SAFETY_TRIP,
                "0-tick Code 253 preempt bypasses storm hysteresis");
    printf("[PASS] T7 Kembali 64 KiB Batch Ring: 0/1,024 false storms, 0-tick Code 253 preempt\n");

    return 0;
}
