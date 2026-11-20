# Makefile — Chiral Bell Jar Manuscript + CAD + PCB + C99 + iCE40HX8K RTL (v1.1.0)
PYTHON       ?= python3
CC           ?= cc
LATEXMK      ?= pdflatex
OPENSCAD     ?= openscad
YOSYS        ?= yosys
NEXTPNR      ?= nextpnr-ice40
ICETIME      ?= icetime
ICEPACK      ?= icepack
ICEPROG      ?= iceprog
IVERILOG     ?= iverilog
VVP          ?= vvp

SEED         ?= 42
CNC_TOL_MM   ?= 0.12
FPGA_FREQ    ?= 40
BUILD_DIR    ?= $(HOME)/.cache/dandelion_os_v5_1_1/cbj

CFLAGS       := -std=c11 -O2 -Wall -Wextra -Werror -I$(BUILD_DIR) -I.
LDFLAGS      := -lm

TEX_SRC      := chiral_bell_jar_manuscript_v1_1_0.tex
SCAD_SRC     := cbj_vessel_v1_1_0.scad
DAEMON_SRC   := cbj_esp32c5_daemon_v1_1_0.c
RUNNER_SRC   := cbj_c99_test_runner_v1_1_0.c
LUT_HDR      := $(BUILD_DIR)/cbj_cad_trim_lut_v1_1_0.h
NET_GEN      := cbj_rx13_netlist_gen_v1_1_0.py
SYN_GEN      := verify_cbj_synergy_v1_1_0.py

RTL_TOP_MOD  := cbj_ice40_top
RTL_SRC      := cbj_ice40_top_v1_1_0.v
RTL_TB       := cbj_ice40_tb_v1_1_0.v
PCF_FILE     := cbj_ice40hx8k_ct256_v1_1_0.pcf

TEST_BIN     := $(BUILD_DIR)/cbj_synergy_test_v1_1_0
RTL_SIM_VV   := $(BUILD_DIR)/cbj_ice40_tb_v1_1_0.vvp
FPGA_JSON    := $(BUILD_DIR)/$(RTL_TOP_MOD)_v1_1_0.json
FPGA_ASC     := $(BUILD_DIR)/$(RTL_TOP_MOD)_v1_1_0.asc
FPGA_BIN     := $(BUILD_DIR)/$(RTL_TOP_MOD)_v1_1_0.bin

.PHONY: all lut pcf-audit verify rtl-sim fpga-bitstream prog-sram netlist cad-stl pdf archive-zip clean

all: verify netlist

lut: $(LUT_HDR)

$(LUT_HDR): $(SYN_GEN) $(SCAD_SRC)
	@mkdir -p $(BUILD_DIR)
	$(PYTHON) $(SYN_GEN) \
		--scad $(SCAD_SRC) \
		--header-out $(LUT_HDR) \
		--seed $(SEED) \
		--cnc-tol-mm $(CNC_TOL_MM)

pcf-audit: $(RTL_SRC) $(PCF_FILE)
	@$(PYTHON) -c 'import re, pathlib; \
	v_txt = pathlib.Path("$(RTL_SRC)").read_text(); \
	top_m = re.search(r"module\s+$(RTL_TOP_MOD)\s*#\([^)]*\)\s*\((.*?)\);", v_txt, re.S); \
	assert top_m, "Could not parse $(RTL_TOP_MOD) port list"; \
	v_ports = set(); \
	[v_ports.update([f"{n}[{i}]" for i in range(int(hi)+1)] if hi else [n]) \
	 for hi, n in re.findall(r"(?:input|output)\s+(?:wire|reg)\s*(?:\[(\d+):0\])?\s*(\w+)", top_m.group(1))]; \
	pcf_pairs = re.findall(r"^\s*set_io\s+(\S+)\s+(\S+)", pathlib.Path("$(PCF_FILE)").read_text(), re.M); \
	pcf_ports = [p for p, _ in pcf_pairs]; pcf_balls = [b for _, b in pcf_pairs]; \
	assert len(pcf_balls) == len(set(pcf_balls)), "Duplicate BGA balls in PCF"; \
	assert set(pcf_ports) == v_ports, f"Port mismatch: sym_diff={set(pcf_ports) ^ v_ports}"; \
	print(f"[PASS] PCF Parity Audit: {len(v_ports)}/42 ports <-> {len(pcf_balls)} unique CT256 balls")'

$(TEST_BIN): $(LUT_HDR) $(DAEMON_SRC) $(RUNNER_SRC)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(DAEMON_SRC) $(RUNNER_SRC) -o $(TEST_BIN) $(LDFLAGS)

rtl-sim: $(RTL_SRC) $(RTL_TB)
	@mkdir -p $(BUILD_DIR)
	$(IVERILOG) -g2005 -Wall -o $(RTL_SIM_VV) $(RTL_SRC) $(RTL_TB)
	$(VVP) $(RTL_SIM_VV)

verify: pcf-audit $(TEST_BIN)
	$(TEST_BIN)
	@if command -v $(IVERILOG) >/dev/null 2>&1; then \
		$(MAKE) --no-print-directory rtl-sim; \
	else \
		echo "[SKIP] $(IVERILOG) not found in PATH; install iverilog for RTL simulation"; \
	fi

$(FPGA_JSON): $(RTL_SRC)
	@mkdir -p $(BUILD_DIR)
	$(YOSYS) -q -p "read_verilog $(RTL_SRC); synth_ice40 -top $(RTL_TOP_MOD) -json $(FPGA_JSON)"

$(FPGA_ASC): $(FPGA_JSON) $(PCF_FILE)
	$(NEXTPNR) --hx8k --package ct256 --freq $(FPGA_FREQ) \
		--json $(FPGA_JSON) --pcf $(PCF_FILE) --asc $(FPGA_ASC)
	$(ICETIME) -d hx8k -P ct256 -p $(PCF_FILE) -c $(FPGA_FREQ) $(FPGA_ASC)

$(FPGA_BIN): $(FPGA_ASC)
	$(ICEPACK) $(FPGA_ASC) $(FPGA_BIN)

fpga-bitstream: pcf-audit $(FPGA_BIN)
	@ls -lh $(FPGA_BIN)

prog-sram: $(FPGA_BIN)
	$(ICEPROG) -S $(FPGA_BIN)

netlist: $(NET_GEN)
	$(PYTHON) $(NET_GEN)

cad-stl: $(SCAD_SRC)
	@mkdir -p $(BUILD_DIR)
	$(OPENSCAD) -D 'SECTION_CUT=false' -o $(BUILD_DIR)/cbj_vessel_v1_1_0.stl $(SCAD_SRC)

pdf: $(TEX_SRC)
	@mkdir -p $(BUILD_DIR)
	$(LATEXMK) -interaction=nonstopmode -output-directory=$(BUILD_DIR) $(TEX_SRC)
	$(LATEXMK) -interaction=nonstopmode -output-directory=$(BUILD_DIR) $(TEX_SRC)

archive-zip: verify netlist
	@mkdir -p $(BUILD_DIR)
	zip -9 $(BUILD_DIR)/chiral_bell_jar_release_v1_1_0.zip \
		.zenodo.json CITATION.cff README.md audit_manifest_v1_1_0.json \
		$(TEX_SRC) $(SCAD_SRC) $(NET_GEN) $(SYN_GEN) \
		$(DAEMON_SRC) $(RUNNER_SRC) $(RTL_SRC) $(PCF_FILE) $(RTL_TB) Makefile

clean:
	rm -rf $(BUILD_DIR) $(LUT_HDR) cbj_rx13_carrier_v1_1_0.net
