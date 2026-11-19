/*
 * cbj_esp32c5_daemon_v1_1_0.c
 * Chiral Bell Jar 104 Hz SPI DMA Telemetry, CAD Phase-Trim & Safety Interlock
 * Target: Waveshare ESP32-C5 RISC-V (Node A Leg 1 & Node C Leg 2) | ISO C99
 * Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243) | Version: v1.1.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#if defined(__has_include)
#  if __has_include("cbj_cad_trim_lut_v1_1_0.h")
#    include "cbj_cad_trim_lut_v1_1_0.h"
#  endif
#endif

/* --- 1. Hardware Pinout Contract --- */
#define CBJ_GPIO_EXCITON_OUT    0u  /* CCW 13-lap exciton ring output        */
#define CBJ_GPIO_EXCITON_IN     1u  /* CCW 13-lap exciton ring input         */
#define CBJ_GPIO_LEG_START      2u  /* Handshake start to iCE40              */
#define CBJ_GPIO_LEG_BUSY       3u  /* Handshake busy from iCE40             */
#define CBJ_GPIO_ERLANG_ISR     4u  /* 104 Hz modulo-13 Am-241 frame strobe  */
#define CBJ_GPIO_FAIL_CLOSED_N  5u  /* Active-low hardware RF/HV mute line   */
#define CBJ_GPIO_SPI_SCK        6u  /* 47 Ohm series-damped 20 MHz SPI SCK   */
#define CBJ_GPIO_SPI_CS_N       7u  /* Active-low SPI chip select            */
#define CBJ_GPIO_SPI_MOSI       8u  /* 47 Ohm series-damped SPI MOSI         */
#define CBJ_GPIO_SPI_MISO       9u  /* iCE40 -> ESP32-C5 DMA MISO            */
#define CBJ_GPIO_LEG_DONE      10u  /* Frame audit complete strobe           */

/* --- 2. Out-of-Band Fowler-VBST Diagnostic Codes (Bytes 242..255) --- */
#define CBJ_CODE_NEUTRAL       121u /* Nominal 5-trit all-equilibrium state  */
#define CBJ_CODE_STORM_HYST    242u /* 104-tick hysteresis hold              */
#define CBJ_CODE_KEYHOLE_BP    243u /* Blume-Capel V = 2/3 backpressure      */
#define CBJ_CODE_DARK_VORTEX   251u /* Standing-wave amplitude null detected */
#define CBJ_CODE_SAFETY_TRIP   253u /* Fail-closed VSWR or Townsend trip     */

/* Safety Thresholds (AD7606C-16 +/-2.5V scale: 1 LSB = 76.29 uV) */
#define CBJ_MAX_VSWR_REV_LSB   6554  /* > 0.50 V reflected RF -> plasma arc  */
#define CBJ_MAX_TOWNSEND_LSB  16384  /* > 25 uA anode current -> glow onset  */
#define CBJ_MIN_HELMHOLTZ_LSB 19660  /* < 24.0 mT -> chiral B0 loss          */
#define CBJ_MAX_TEMP_LSB      26869  /* > 41.0 C cavity wall thermal ceiling */

/* --- 3. Compile-Time Verified ABI Structs (Zero Implicit Padding) --- */
typedef struct {
    uint32_t magic_sync;       /* 0..3   : 0x43424A31u ("CBJ1")              */
    uint32_t frame_seq;        /* 4..7   : Monotonic 104 Hz Erlang sequence  */
    uint32_t erlang_dt_us;     /* 8..11  : Microseconds for 13 Am-241 alphas */
    uint16_t trit_pos_mask;    /* 12..13 : Bit k=1 if probe k in +1 basin    */
    uint16_t trit_neg_mask;    /* 14..15 : Bit k=1 if probe k in -1 basin    */
    uint16_t vortex_null_mask; /* 16..17 : Bit k=1 if I^2 + Q^2 < R_NULL_SQ  */
    int16_t  chiral_wind_q8;   /* 18..19 : Signed Q8.8 ring winding rate     */
    int16_t  hk_adc[6];        /* 20..31 : [FWD, REV, SHIL, HV_I, B0, TEMP]  */
    int16_t  phi_rad_q13[13];  /* 32..57 : Probe phases in rad * 8192        */
    uint8_t  vbst_trits[3];    /* 58..60 : 13 trits packed 5-trits/byte      */
    uint8_t  kintsugi_code;    /* 61     : Diagnostic status (121 / 251/253) */
    uint16_t crc16_ccitt;      /* 62..63 : CCITT-FALSE frame integrity check */
} cbj_spi_dma_frame_64b_t;

typedef struct {
    uint8_t  magic[4];         /* 0..3   : {'D', 'N', 'D', '3'}              */
    uint32_t frame_seq;        /* 4..7   : Monotonic frame sequence          */
    uint32_t erlang_dt_us;     /* 8..11  : Erlang-13 nuclear interval (us)   */
    int16_t  chiral_wind_q8;   /* 12..13 : Chiral soliton winding metric     */
    uint16_t vortex_null_mask; /* 14..15 : 13-bit dark-vortex bitmap         */
    int16_t  hk_adc[6];        /* 16..27 : 6 safety housekeeping channels    */
    uint8_t  vbst_trits[3];    /* 28..30 : Fowler-VBST packed 13-trit state  */
    uint8_t  kintsugi_code;    /* 31     : Status / fail-closed code         */
} stigmergic_ble_frame_t;

_Static_assert(sizeof(cbj_spi_dma_frame_64b_t) == 64u, "CBJ SPI DMA frame must equal 64 bytes");
_Static_assert(sizeof(stigmergic_ble_frame_t)  == 32u, "DND3 telemetry frame must equal 32 bytes");
_Static_assert(offsetof(cbj_spi_dma_frame_64b_t, phi_rad_q13) == 32u, "Cache-half split mismatch");

/* --- 4. CAD-Coupled Geometric Trim Table --- */
typedef struct {
    int16_t cos_q14;
    int16_t sin_q14;
    int16_t dphi_q13;
} cbj_cad_port_trim_t;

#ifndef CBJ_CAD_TRIM_LUT_INIT
#define CBJ_CAD_TRIM_LUT_INIT { \
    {16384, 0, 0}, {16384, 0, 0}, {16384, 0, 0}, {16384, 0, 0}, \
    {16384, 0, 0}, {16384, 0, 0}, {16384, 0, 0}, {16384, 0, 0}, \
    {16384, 0, 0}, {16384, 0, 0}, {16384, 0, 0}, {16384, 0, 0}, \
    {16384, 0, 0} \
}
#endif

static const cbj_cad_port_trim_t g_cad_port_trim[13] = CBJ_CAD_TRIM_LUT_INIT;

static cbj_spi_dma_frame_64b_t g_dma_rx_ring[2] __attribute__((aligned(32)));
static volatile uint8_t        g_active_bank = 0u;

extern void cbj_hal_gpio_set_low(uint32_t pin);
extern void cbj_hal_gpio_pulse(uint32_t pin);
extern bool cbj_hal_spi_dma_transfer_64b(cbj_spi_dma_frame_64b_t *rx_dst);

void cbj_apply_cad_iq_trim_q14(uint8_t k, int16_t i_in, int16_t q_in,
                               int16_t *i_out, int16_t *q_out) {
    const int32_t c = g_cad_port_trim[k].cos_q14;
    const int32_t s = g_cad_port_trim[k].sin_q14;
    *i_out = (int16_t)(((int32_t)i_in * c - (int32_t)q_in * s + 8192) >> 14);
    *q_out = (int16_t)(((int32_t)i_in * s + (int32_t)q_in * c + 8192) >> 14);
}

static void cbj_pack_fowler_vbst13(uint16_t pos_m, uint16_t neg_m, uint8_t out[3]) {
    static const uint8_t pow3[5] = {1u, 3u, 9u, 27u, 81u};
    for (uint8_t b = 0u; b < 3u; ++b) {
        uint8_t acc = 0u;
        for (uint8_t m = 0u; m < 5u; ++m) {
            const uint8_t idx = (uint8_t)(b * 5u + m);
            uint8_t u_trit = 1u;
            if (idx < 13u) {
                if ((pos_m >> idx) & 1u)      { u_trit = 2u; }
                else if ((neg_m >> idx) & 1u) { u_trit = 0u; }
            }
            acc = (uint8_t)(acc + u_trit * pow3[m]);
        }
        out[b] = acc;
    }
}

static uint16_t cbj_crc16_ccitt(const uint8_t *buf, size_t len) {
    uint16_t crc = 0xFFFFu;
    for (size_t i = 0u; i < len; ++i) {
        crc ^= (uint16_t)((uint16_t)buf[i] << 8);
        for (uint8_t bit = 0u; bit < 8u; ++bit) {
            crc = (crc & 0x8000u) ? (uint16_t)((crc << 1) ^ 0x1021u)
                                  : (uint16_t)(crc << 1);
        }
    }
    return crc;
}

bool cbj_process_104hz_frame(stigmergic_ble_frame_t *out_dnd3) {
    cbj_spi_dma_frame_64b_t * const rx = &g_dma_rx_ring[g_active_bank & 1u];
    g_active_bank ^= 1u;

    if (!cbj_hal_spi_dma_transfer_64b(rx)) {
        return false;
    }

    const uint16_t calc_crc = cbj_crc16_ccitt((const uint8_t *)rx, 62u);
    if ((rx->magic_sync != 0x43424A31u) || (calc_crc != rx->crc16_ccitt)) {
        cbj_hal_gpio_set_low(CBJ_GPIO_FAIL_CLOSED_N);
        return false;
    }

    uint8_t status = CBJ_CODE_NEUTRAL;
    const bool plasma_arc   = (rx->hk_adc[1] > CBJ_MAX_VSWR_REV_LSB);
    const bool glow_runaway = (rx->hk_adc[3] > CBJ_MAX_TOWNSEND_LSB);
    const bool b0_dropout   = (rx->hk_adc[4] < CBJ_MIN_HELMHOLTZ_LSB);
    const bool over_temp    = (rx->hk_adc[5] > CBJ_MAX_TEMP_LSB);

    if (plasma_arc || glow_runaway || b0_dropout || over_temp) {
        cbj_hal_gpio_set_low(CBJ_GPIO_FAIL_CLOSED_N);
        status = CBJ_CODE_SAFETY_TRIP;
    } else if ((rx->vortex_null_mask & 0x1FFFu) != 0u) {
        status = CBJ_CODE_DARK_VORTEX;
    }

    int32_t winding_sum = 0;
    int16_t phi_trimmed[13];
    for (uint8_t k = 0u; k < 13u; ++k) {
        phi_trimmed[k] = (int16_t)(rx->phi_rad_q13[k] + g_cad_port_trim[k].dphi_q13);
    }
    for (uint8_t k = 0u; k < 13u; ++k) {
        const uint8_t next_k = (uint8_t)((k + 1u == 13u) ? 0u : (k + 1u));
        int32_t dphi = (int32_t)phi_trimmed[next_k] - (int32_t)phi_trimmed[k];
        if (dphi >  25736) { dphi -= 51472; }
        if (dphi < -25736) { dphi += 51472; }
        winding_sum += dphi;
    }

    out_dnd3->magic[0]         = 'D';
    out_dnd3->magic[1]         = 'N';
    out_dnd3->magic[2]         = 'D';
    out_dnd3->magic[3]         = '3';
    out_dnd3->frame_seq        = rx->frame_seq;
    out_dnd3->erlang_dt_us     = rx->erlang_dt_us;
    out_dnd3->chiral_wind_q8   = (int16_t)(winding_sum >> 5);
    out_dnd3->vortex_null_mask = (uint16_t)(rx->vortex_null_mask & 0x1FFFu);
    for (uint8_t c = 0u; c < 6u; ++c) {
        out_dnd3->hk_adc[c] = rx->hk_adc[c];
    }
    cbj_pack_fowler_vbst13(rx->trit_pos_mask, rx->trit_neg_mask, out_dnd3->vbst_trits);
    out_dnd3->kintsugi_code    = status;

    if (status != CBJ_CODE_SAFETY_TRIP) {
        cbj_hal_gpio_pulse(CBJ_GPIO_EXCITON_OUT);
    }
    cbj_hal_gpio_pulse(CBJ_GPIO_LEG_DONE);
    return true;
}
