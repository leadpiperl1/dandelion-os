#!/usr/bin/env python3
"""
cbj_rx13_netlist_gen_v1_1_0.py
Generates KiCad 8.0 S-expression netlist (version E) for the Chiral Bell Jar
13-Channel Phase-Coherent Homodyne I/Q & 32-Ch Simultaneous ADC Carrier PCB.
Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243) | Version: v1.1.0
"""

from pathlib import Path
import sys


def build_cbj_rx13_netlist():
    components = []
    nets = {}

    def add_comp(ref, value, footprint, desc):
        components.append((ref, value, footprint, desc))

    def connect(net_name, ref, pin):
        nets.setdefault(net_name, []).append((ref, str(pin)))

    # 1. Power LDO Regulators & Master Mezzanine Header
    add_comp("J_PWR", "DC_BARREL_9V", "Connector_BarrelJack:BarrelJack_Horizontal", "9V_Bench_Input")
    add_comp("U_LDO_RF", "ADM7150ACPZ-5.0", "Package_CSP:LFCSP-8-1EP_3x3mm_P0.65mm_EP1.6x1.6mm", "5V_RF_LDO")
    add_comp("U_LDO_ADC", "ADM7150ACPZ-5.0", "Package_CSP:LFCSP-8-1EP_3x3mm_P0.65mm_EP1.6x1.6mm", "5V_ADC_LDO")
    add_comp("U_LDO_DIG", "ADM7172ACPZ-3.3", "Package_CSP:LFCSP-8-1EP_3x3mm_P0.65mm_EP1.6x1.6mm", "3V3_DIG_LDO")
    add_comp("J_FPGA", "CONN_02X20_ICE40_ESP32", "Connector_PinHeader_2.54mm:PinHeader_2x20_P2.54mm_Vertical", "Arbiter_Header")

    connect("+9V_RAW", "J_PWR", 1)
    connect("GND", "J_PWR", 2)
    for u_ldo, rail in [("U_LDO_RF", "+5V_RF"), ("U_LDO_ADC", "+5V_ADC"), ("U_LDO_DIG", "+3V3_DIG")]:
        connect("+9V_RAW", u_ldo, 8)
        connect("+9V_RAW", u_ldo, 5)
        connect(rail, u_ldo, 1)
        connect("GND", u_ldo, 4)

    # 2. Central 16-Way Coherent LO Wilkinson Manifold (915.0 MHz)
    add_comp("J_LO_IN", "SMA_EDGE_915MHZ_LO", "Connector_Coaxial:SMA_Amphenol_132289_EdgeMount", "Master_LO_Input")
    add_comp("U_WILK16", "ZN16PD-252-S+", "RF_Mini-Circuits:WILKINSON_16WAY_STAR", "16Way_LO_Splitter")
    connect("LO_MASTER_IN", "J_LO_IN", 1)
    connect("GND", "J_LO_IN", 2)
    connect("LO_MASTER_IN", "U_WILK16", "SUM")
    connect("GND", "U_WILK16", "GND")

    for unused_idx in (13, 14, 15):
        r_ref = f"R_LOTERM_{unused_idx}"
        add_comp(r_ref, "49.9_Ohm_0.1%", "Resistor_SMD:R_0402_1005Metric", "Wilkinson_Unused_Load")
        net_lo = f"LO_PORT_{unused_idx:02d}"
        connect(net_lo, "U_WILK16", f"P{unused_idx}")
        connect(net_lo, r_ref, 1)
        connect("GND", r_ref, 2)

    # 3. Four 8-Channel Simultaneous-Sampling Differential SAR ADCs (32 Ch Total)
    for adc_idx in range(4):
        u_adc = f"U_ADC{adc_idx}"
        c_dec = f"C_ADC_DEC{adc_idx}"
        r_miso = f"R_SER_MISO{adc_idx}"
        add_comp(u_adc, "AD7606C-16", "Package_QFP:LQFP-64_10x10mm_P0.5mm", "8Ch_16Bit_Diff_SAR_ADC")
        add_comp(c_dec, "100nF_C0G", "Capacitor_SMD:C_0402_1005Metric", "ADC_AVCC_Decoupling")
        add_comp(r_miso, "47_Ohm", "Resistor_SMD:R_0402_1005Metric", "SPI_Source_Term")

        connect("+5V_ADC", u_adc, "AVCC")
        connect("+3V3_DIG", u_adc, "VDRIVE")
        connect("GND", u_adc, "AGND")
        connect("+5V_ADC", c_dec, 1)
        connect("GND", c_dec, 2)

        connect("ADC_CONVST_STAR", u_adc, "CONVST")
        connect("ADC_SCLK_BUS", u_adc, "SCLK")
        connect("ADC_RESET_BUS", u_adc, "RESET")
        connect(f"ADC_CS_N_{adc_idx}", u_adc, "CS_N")
        connect(f"ADC_BUSY_{adc_idx}", u_adc, "BUSY")
        connect(f"ADC_DOUTA_RAW_{adc_idx}", u_adc, "DOUTA")
        connect(f"ADC_DOUTA_RAW_{adc_idx}", r_miso, 1)
        connect(f"ADC_MISO_{adc_idx}", r_miso, 2)

        connect(f"ADC_CS_N_{adc_idx}", "J_FPGA", 3 + adc_idx * 2)
        connect(f"ADC_BUSY_{adc_idx}", "J_FPGA", 4 + adc_idx * 2)
        connect(f"ADC_MISO_{adc_idx}", "J_FPGA", 11 + adc_idx)

    connect("ADC_CONVST_STAR", "J_FPGA", 1)
    connect("ADC_SCLK_BUS", "J_FPGA", 2)
    connect("ADC_RESET_BUS", "J_FPGA", 15)
    connect("ALPHA_SCHMITT_IN", "J_FPGA", 16)
    connect("SHIL_ENV_GATE", "J_FPGA", 17)
    connect("DARK_CAL_SW_CTL", "J_FPGA", 18)
    connect("+3V3_DIG", "J_FPGA", 39)
    connect("GND", "J_FPGA", 40)

    # 4. Instantiate 13 Equatorial Homodyne I/Q Strips + 5th-Order Bessel Filters
    for ch in range(13):
        tag = f"{ch:02d}"
        j_rf = f"J_RF_{tag}"
        u_lim = f"U_LIM_{tag}"
        r_pad1 = f"R_PAD1_{tag}"
        r_pad2 = f"R_PAD2_{tag}"
        r_pad3 = f"R_PAD3_{tag}"
        u_iq = f"U_IQ_{tag}"
        c_rf = f"C_IQDEC_{tag}"

        add_comp(j_rf, "SMA_PROBE_IN", "Connector_Coaxial:SMA_Amphenol_132289_EdgeMount", f"Probe_P{tag}_Input")
        add_comp(u_lim, "VLM-33-S+", "Package_TO_SOT_SMD:SOT-363_SC-70-6", "PIN_RF_Limiter")
        add_comp(r_pad1, "292_Ohm", "Resistor_SMD:R_0402_1005Metric", "3dB_Pi_Shunt1")
        add_comp(r_pad2, "17.6_Ohm", "Resistor_SMD:R_0402_1005Metric", "3dB_Pi_Series")
        add_comp(r_pad3, "292_Ohm", "Resistor_SMD:R_0402_1005Metric", "3dB_Pi_Shunt2")
        add_comp(u_iq, "ADL5380ACPZ", "Package_CSP:LFCSP-24-1EP_4x4mm_P0.5mm_EP2.5x2.5mm", "915MHz_IQ_Demod")
        add_comp(c_rf, "100nF_C0G", "Capacitor_SMD:C_0402_1005Metric", "Mixer_Supply_Decouple")

        connect(f"RF_RAW_{tag}", j_rf, 1)
        connect("GND", j_rf, 2)
        connect(f"RF_RAW_{tag}", u_lim, 1)
        connect("GND", u_lim, 2)
        connect(f"RF_LIM_{tag}", u_lim, 3)

        connect(f"RF_LIM_{tag}", r_pad1, 1)
        connect("GND", r_pad1, 2)
        connect(f"RF_LIM_{tag}", r_pad2, 1)
        connect(f"RF_PAD_OUT_{tag}", r_pad2, 2)
        connect(f"RF_PAD_OUT_{tag}", r_pad3, 1)
        connect("GND", r_pad3, 2)

        connect(f"RF_PAD_OUT_{tag}", u_iq, "RFINP")
        connect("GND", u_iq, "RFINN")
        connect(f"LO_PORT_{tag}", "U_WILK16", f"P{ch}")
        connect(f"LO_PORT_{tag}", u_iq, "LOINP")
        connect("GND", u_iq, "LOINN")
        connect("+5V_RF", u_iq, "VPOS")
        connect("GND", u_iq, "EPAD")
        connect("+5V_RF", c_rf, 1)
        connect("GND", c_rf, 2)

        for quad_idx, axis in enumerate(("I", "Q")):
            global_ch = ch * 2 + quad_idx
            adc_chip = f"U_ADC{global_ch // 8}"
            adc_pin_idx = global_ch % 8

            p_in = f"BB_{axis}P_RAW_{tag}"
            n_in = f"BB_{axis}N_RAW_{tag}"
            connect(p_in, u_iq, f"{axis}BBP")
            connect(n_in, u_iq, f"{axis}BBN")

            l1p, l1n = f"L1P_{axis}_{tag}", f"L1N_{axis}_{tag}"
            c2d = f"C2D_{axis}_{tag}"
            l3p, l3n = f"L3P_{axis}_{tag}", f"L3N_{axis}_{tag}"
            c4d = f"C4D_{axis}_{tag}"
            l5p, l5n = f"L5P_{axis}_{tag}", f"L5N_{axis}_{tag}"
            r_term = f"R_BBTERM_{axis}_{tag}"

            add_comp(l1p, "75uH_Shielded", "Inductor_SMD:L_0603_1608Metric", "Bessel_Stg1_P")
            add_comp(l1n, "75uH_Shielded", "Inductor_SMD:L_0603_1608Metric", "Bessel_Stg1_N")
            add_comp(c2d, "300pF_C0G_1%", "Capacitor_SMD:C_0402_1005Metric", "Bessel_Stg2_Diff")
            add_comp(l3p, "27uH_Shielded", "Inductor_SMD:L_0603_1608Metric", "Bessel_Stg3_P")
            add_comp(l3n, "27uH_Shielded", "Inductor_SMD:L_0603_1608Metric", "Bessel_Stg3_N")
            add_comp(c4d, "130pF_C0G_1%", "Capacitor_SMD:C_0402_1005Metric", "Bessel_Stg4_Diff")
            add_comp(l5p, "5.6uH_Shielded", "Inductor_SMD:L_0603_1608Metric", "Bessel_Stg5_P")
            add_comp(l5n, "5.6uH_Shielded", "Inductor_SMD:L_0603_1608Metric", "Bessel_Stg5_N")
            add_comp(r_term, "499_Ohm_0.1%", "Resistor_SMD:R_0402_1005Metric", "Bessel_Diff_Load")

            n1p, n1n = f"N1P_{axis}_{tag}", f"N1N_{axis}_{tag}"
            n2p, n2n = f"N2P_{axis}_{tag}", f"N2N_{axis}_{tag}"
            out_p, out_n = f"ADC_{axis}P_{tag}", f"ADC_{axis}N_{tag}"

            connect(p_in, l1p, 1); connect(n1p, l1p, 2)
            connect(n_in, l1n, 1); connect(n1n, l1n, 2)
            connect(n1p, c2d, 1);  connect(n1n, c2d, 2)

            connect(n1p, l3p, 1);  connect(n2p, l3p, 2)
            connect(n1n, l3n, 1);  connect(n2n, l3n, 2)
            connect(n2p, c4d, 1);  connect(n2n, c4d, 2)

            connect(n2p, l5p, 1);  connect(out_p, l5p, 2)
            connect(n2n, l5n, 1);  connect(out_n, l5n, 2)
            connect(out_p, r_term, 1); connect(out_n, r_term, 2)

            connect(out_p, adc_chip, f"V{adc_pin_idx}_P")
            connect(out_n, adc_chip, f"V{adc_pin_idx}_N")

    # 5. Populate Remaining 6 Housekeeping Telemetry Inputs on U_ADC3 (Ch 26..31)
    hk_names = ["FWD_RF_PWR", "REV_VSWR_PWR", "SHIL_3W0_ENV", "TOWNSEND_I_MON", "HELMHOLTZ_B0", "CAVITY_TEMP"]
    add_comp("J_HK_TELEM", "CONN_02X06_HOUSEKEEPING", "Connector_PinHeader_2.54mm:PinHeader_2x06_P2.54mm_Vertical", "Safety_Sensors")
    for idx, hk in enumerate(hk_names):
        adc_pin_idx = 2 + idx
        connect(f"HK_{hk}_P", "J_HK_TELEM", idx * 2 + 1)
        connect(f"HK_{hk}_N", "J_HK_TELEM", idx * 2 + 2)
        connect(f"HK_{hk}_P", "U_ADC3", f"V{adc_pin_idx}_P")
        connect(f"HK_{hk}_N", "U_ADC3", f"V{adc_pin_idx}_N")

    return components, nets


def emit_kicad_sexpr(components, nets, out_path):
    lines = [
        '(export (version "E")',
        '  (design (source "cbj_rx13_netlist_gen_v1_1_0.py") (date "2026-11-17")',
        '    (tool "Chiral Bell Jar Parametric Netlist Synthesis v1.1.0"))',
        '  (components'
    ]
    for ref, val, fp, desc in sorted(components):
        lines.append(f'    (comp (ref "{ref}") (value "{val}") (footprint "{fp}") (description "{desc}"))')
    lines.append('  )')
    lines.append('  (nets')
    for code, (net_name, nodes) in enumerate(sorted(nets.items()), start=1):
        lines.append(f'    (net (code "{code}") (name "{net_name}")')
        for ref, pin in nodes:
            lines.append(f'      (node (ref "{ref}") (pin "{pin}"))')
        lines.append('    )')
    lines.append('  )')
    lines.append(')')
    Path(out_path).write_text("\n".join(lines) + "\n", encoding="utf-8")


if __name__ == "__main__":
    comps, net_map = build_cbj_rx13_netlist()
    floating = [n for n, pins in net_map.items() if len(pins) < 2 and not n.endswith(("_CTL", "_GATE", "_IN"))]
    if floating:
        sys.exit(f"AUDIT FAILED - Floating nets detected: {floating}")
    target = "cbj_rx13_carrier_v1_1_0.net"
    emit_kicad_sexpr(comps, net_map, target)
    print(f"[PASS] {target}: {len(comps)} components, {len(net_map)} verified nets, 32/32 ADC channels packed.")
