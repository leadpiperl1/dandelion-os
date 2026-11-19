Chiral Bell Jar Standing-Wave Processor — Reproducibility & Hardware-CAD-RTL Release (v1.1.0)
Author: Justin D. Grimm (ORCID: 0009-0000-5299-0243)
Affiliation: Independent Dandelion Substrate Research Group, Gurley, Alabama 35748, USA
Version: v1.1.0 | License: CC-BY-4.0 | Date: November 17, 2026
1. Overview & Physical Operating Point
This repository bundle contains the self-contained mathematical manuscript, mechanical vacuum vessel CAD, 13-channel homodyne receiver PCB netlist generator, freestanding ISO C99 RISC-V safety-interlock firmware, and synthesizable Lattice iCE40HX8K-CT256 Verilog RTL for a partial-vacuum radio-frequency (RF) standing-wave balanced-ternary processor (s_i in {-1, 0, +1}):


* Spherical $\text{TM}_{101}$ Cavity: Inner radius $a = 0.1431\text{ m}$ ($12.27\text{ L}$ internal volume), electroplated $25\text{ }\mu\text{m}$ OFHC copper over annealed non-magnetic 304L stainless steel, resonant at $f_0 = 915.0\text{ MHz}$ ISM band ($u_{11} = 2.7437$) with a $72.00^\circ$ ($z = +44.22\text{ mm}$) non-equatorial Viton/fingerstock parting plane that preserves a monolithic, seam-free 13-port equatorial bowl.
* Ramsauer-Townsend Argon Buffer: Static 5N Argon fill at $1.067\text{ kPa}$ ($8.0\text{ Torr}$), centering electron elastic scattering near the $0.23\text{ eV}$ Ramsauer-Townsend minimum ($\nu_m / \omega_0 = 0.0153$, loaded $Q_L = 1,850$, peak continuous $E = 8.13\text{ kV/m}$ with $9.4\text{ dB}$ sub-breakdown margin).
* Coherent $3\omega_0$ SHIL Ternary Quantization: Direct frequency tripling ($2,745.0\text{ MHz}$) off the master $915.0\text{ MHz}$ local oscillator carves three drift-free $120^\circ$ attractors ${-2\pi/3, 0, +2\pi/3}$ above the ternary Landauer floor $k_B T_g \ln 3$.
* Nuclear Erlang-13 Timing & Gyrotropic Parity Bias: An NRC-exempt $37.0\text{ kBq}$ ($1.0\text{ }\mu\text{Ci}$) $^{241}\text{Am}$ alpha source at the magic angle ($\theta_m = 54.74^\circ$, $P_2(\cos\theta_m) = 0$) drives a modulo-13 hardware frame strobe at $104.0\text{ Hz}$ ($\mu_\tau = 9.615\text{ ms}$), while an axial $37.0\text{ kBq}$ $^{60}\text{Co}$ beta source inside a $28.0\text{ mT}$ Helmholtz guide field breaks dihedral $D_3 \to C_3$ symmetry via a $10.50^\circ$ non-reciprocal Sakaguchi phase lag.
2. Complete Release Manifest (v1.1.0)
File
	Domain
	Verified Invariant / Output
	chiral_bell_jar_manuscript_v1_1_0.tex
	APS revtex4-2 Paper
	Pure SI metrology, Page-1 Falsification Box, inline TikZ schematic
	cbj_vessel_v1_1_0.scad
	Parametric 3D CAD
	21-port vessel, $72.0^\circ$ dome split, Helmholtz pair, CBJ_CAD_LUT echo
	cbj_rx13_netlist_gen_v1_1_0.py
	KiCad 8.0 PCB Gen
	Emits cbj_rx13_carrier_v1_1_0.net (13x ADL5380, 4x AD7606C-16)
	verify_cbj_synergy_v1_1_0.py
	CAD-to-C99 Bridge
	Audits flange/coil clearances; emits cbj_cad_trim_lut_v1_1_0.h (--seed 42)
	cbj_esp32c5_daemon_v1_1_0.c
	ISO C99 Firmware
	64 B SPI DMA, 32 B DND3, Q2.14 CAD trim, 0-tick Code 253 trip
	cbj_c99_test_runner_v1_1_0.c
	C11 Unit Test Suite
	Tests T1–T7 including 65,536 B page-aligned Kembali storm ring
	cbj_ice40_top_v1_1_0.v
	iCE40HX8K Verilog
	50 us dark-slot DC null, shared sqrt(3) + Q3.13 CORDIC, dual SPI
	cbj_ice40hx8k_ct256_v1_1_0.pcf
	Physical Pin Map
	42/42 verified 3.3 V LVCMOS balls on iCE40HX8K-CT256 Banks 0 & 3
	cbj_ice40_tb_v1_1_0.v
	RTL Testbench
	Self-checking iverilog simulation of W = +1 winding & VSWR trip
	Makefile
	Unified Build
	Deterministic targets: verify, netlist, fpga-bitstream, cad-stl, pdf
	.zenodo.json & CITATION.cff
	Archival Metadata
	DataCite reverse-citation graph & CFF 1.2.0 formal attribution
	audit_manifest_v1_1_0.json
	Pre-Flight Gate
	Moonshot 6-Gate & adversarial reproducibility audit record
	3. Deterministic Quick-Start (<= 5 s, Zero External Dependencies)
# 1. Run CAD clearance gate, PCF 42-ball parity check, C99 test suite, and RTL sim:
make verify SEED=42 CNC_TOL_MM=0.12


# 2. Generate the 13-channel ADL5380 + 32-channel AD7606C-16 KiCad 8.0 netlist:
make netlist


# 3. Synthesize & time the Lattice iCE40HX8K-CT256 bitstream (requires yosys/nextpnr):
make fpga-bitstream


# 4. Export watertight vessel STL & compile the APS revtex4-2 manuscript PDF:
make cad-stl
make pdf
4. Supply-Chain & Repository Hygiene
This bundle enforces zero lifecycle hook scripts, zero external network fetches during verification, and read-only CI execution (permissions: contents: read, persist-credentials: false). Keep terminal windows open or inspect the [PASS] summary lines emitted by make verify to confirm zero warnings and zero LSB threshold violations before flashing hardware.
