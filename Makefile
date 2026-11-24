CC ?= gcc
NM ?= nm
PYTHON ?= python3
BUILD_DIR ?= $(HOME)/.cache/dandelion_os_v5_2_0
CFLAGS_C99 := -std=c99 -O3 -Wall -Wextra -Wconversion -Werror
CFLAGS_C99_STRICT := -std=c99 -O2 -ffp-contract=off -Wall -Wextra -Werror -pedantic
CFLAGS_C11 := -std=c11 -O3 -Wall -Wextra -Wconversion -Werror

C_BINARIES := \
	$(BUILD_DIR)/dandelion-v5 \
	$(BUILD_DIR)/libdandelion_successor_v5_0_0.so \
	$(BUILD_DIR)/libcognitive_carriage_gate_v1_1_0.so \
	$(BUILD_DIR)/test_cognitive_carriage_v1_1_0 \
	$(BUILD_DIR)/asymmetric_ratchet_dnd3_v1_0_1 \
	$(BUILD_DIR)/bounded_thermo_cell64_v1_0_1 \
	$(BUILD_DIR)/qpv_dandelion_hindsight_v1_0_0 \
	$(BUILD_DIR)/kembali_carbon_microkernel_v1_2_1 \
	$(BUILD_DIR)/rans243_static_v1_0_0 \
	$(BUILD_DIR)/adaptive_rans243_online_v1_0_0 \
	$(BUILD_DIR)/c42_patch_cascade_verifier_v1_0_0 \
	$(BUILD_DIR)/dandelion_quorum_governor_64b_v1_0_0 \
	$(BUILD_DIR)/triadic_falsification_gate_v1_1_0 \
	$(BUILD_DIR)/rotonium_oam_vs_tfln_oatc_audit_v1_0_0 \
	$(BUILD_DIR)/libcuprate_pump_probe_quench_32b_v3_15_1.so \
	$(BUILD_DIR)/libvp_vnp_bounded_3d_metrology_64b_v1_0_0.so \
	$(BUILD_DIR)/walk_forward_forecast_gate_v1_0_0 \
	$(BUILD_DIR)/quantum_energy_grid_governor_64b_v1_0_0

.PHONY: all verify verify-malloc clean

all: $(C_BINARIES)

$(BUILD_DIR)/dandelion-v5: src/dandelion_successor_v5_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) $< -o $@

$(BUILD_DIR)/libdandelion_successor_v5_0_0.so: src/dandelion_successor_v5_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) -fPIC -shared $< -o $@

$(BUILD_DIR)/libcognitive_carriage_gate_v1_1_0.so: src/cognitive_carriage_gate_v1_1_0.c include/cognitive_carriage_gate_v1_1_0.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) -pedantic -Iinclude -fPIC -shared src/cognitive_carriage_gate_v1_1_0.c -o $@

$(BUILD_DIR)/test_cognitive_carriage_v1_1_0: src/cognitive_carriage_gate_v1_1_0.c tests/test_cognitive_carriage_v1_1_0.c include/cognitive_carriage_gate_v1_1_0.h
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) -pedantic -Iinclude src/cognitive_carriage_gate_v1_1_0.c tests/test_cognitive_carriage_v1_1_0.c -o $@

$(BUILD_DIR)/asymmetric_ratchet_dnd3_v1_0_1: src/asymmetric_ratchet_dnd3_v1_0_1.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) -Wpedantic $< -o $@

$(BUILD_DIR)/bounded_thermo_cell64_v1_0_1: src/bounded_thermo_cell64_v1_0_1.c
	@mkdir -p $(BUILD_DIR)
	$(CC) -std=c99 -O3 -Wall -Wextra -Werror $< -lm -o $@

$(BUILD_DIR)/qpv_dandelion_hindsight_v1_0_0: src/qpv_dandelion_hindsight_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C11) $< -o $@

$(BUILD_DIR)/kembali_carbon_microkernel_v1_2_1: src/kembali_carbon_microkernel_v1_2_1.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C11) $< -o $@

$(BUILD_DIR)/rans243_static_v1_0_0: src/rans243_static_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/adaptive_rans243_online_v1_0_0: src/adaptive_rans243_online_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/c42_patch_cascade_verifier_v1_0_0: src/c42_patch_cascade_verifier_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/dandelion_quorum_governor_64b_v1_0_0: src/dandelion_quorum_governor_64b_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/triadic_falsification_gate_v1_1_0: src/triadic_falsification_gate_v1_1_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/rotonium_oam_vs_tfln_oatc_audit_v1_0_0: src/rotonium_oam_vs_tfln_oatc_audit_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/libcuprate_pump_probe_quench_32b_v3_15_1.so: src/cuprate_pump_probe_quench_32b_v3_15_1.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) -fPIC -shared $< -lm -o $@

$(BUILD_DIR)/libvp_vnp_bounded_3d_metrology_64b_v1_0_0.so: src/vp_vnp_bounded_3d_metrology_64b_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) -fPIC -shared $< -lm -o $@

$(BUILD_DIR)/walk_forward_forecast_gate_v1_0_0: src/walk_forward_forecast_gate_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

$(BUILD_DIR)/quantum_energy_grid_governor_64b_v1_0_0: src/quantum_energy_grid_governor_64b_v1_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99_STRICT) $< -lm -o $@

verify-malloc: all
	@for bin in $(C_BINARIES); do \
		if $(NM) -u "$$bin" | grep -Eq '\b(malloc|calloc|realloc|free)\b'; then \
			echo "[FAIL] Dynamic heap symbol found in $$bin" >&2; \
			exit 1; \
		fi; \
	done
	@echo "[OK] Zero dynamic heap allocation verified (malloc = 0 across all 18 C99/C11 targets)."

verify: verify-malloc
	$(BUILD_DIR)/dandelion-v5
	$(BUILD_DIR)/test_cognitive_carriage_v1_1_0 > $(BUILD_DIR)/cognitive_carriage_c99_receipt.json
	$(BUILD_DIR)/asymmetric_ratchet_dnd3_v1_0_1
	$(BUILD_DIR)/bounded_thermo_cell64_v1_0_1
	$(BUILD_DIR)/qpv_dandelion_hindsight_v1_0_0
	$(BUILD_DIR)/kembali_carbon_microkernel_v1_2_1
	$(BUILD_DIR)/rans243_static_v1_0_0 > $(BUILD_DIR)/rans243_static_receipt.txt
	$(BUILD_DIR)/adaptive_rans243_online_v1_0_0 > $(BUILD_DIR)/adaptive_rans243_receipt.txt
	$(BUILD_DIR)/c42_patch_cascade_verifier_v1_0_0 > $(BUILD_DIR)/c42_patch_cascade_receipt.txt
	$(BUILD_DIR)/dandelion_quorum_governor_64b_v1_0_0 > $(BUILD_DIR)/dandelion_quorum_c99_receipt.json
	$(BUILD_DIR)/triadic_falsification_gate_v1_1_0 > $(BUILD_DIR)/triadic_falsification_c99_receipt.json
	$(BUILD_DIR)/rotonium_oam_vs_tfln_oatc_audit_v1_0_0 > $(BUILD_DIR)/rotonium_oatc_c99_receipt.txt
	$(BUILD_DIR)/walk_forward_forecast_gate_v1_0_0 > $(BUILD_DIR)/walk_forward_c99_receipt.txt
	$(BUILD_DIR)/quantum_energy_grid_governor_64b_v1_0_0 > $(BUILD_DIR)/quantum_grid_c99_receipt.json
	$(MAKE) --no-print-directory -C hardware/chiral_bell_jar_v1_1_0 BUILD_DIR="$(BUILD_DIR)/cbj" verify
	$(PYTHON) madras/dandelion_communicator_madras_v5_0_0.py --verify
	$(PYTHON) benchmarks/verify_agency_liability_countermodel_v1_1_0.py --seed 42 > $(BUILD_DIR)/cognitive_carriage_py_receipt.json
	$(PYTHON) benchmarks/fidel_dene_shingari_256byte_codec_v1_0_0.py --seed 42
	$(PYTHON) benchmarks/nadene_bergson_channel_benchmark_v1_0_1.py --seed 42 --trials 200 --out "$(BUILD_DIR)/nadene_bergson_audit_v1_0_1.json"
	$(PYTHON) benchmarks/verify_asymmetric_ratchet_dnd3_v1_0_1.py --seed 42 > $(BUILD_DIR)/ratchet_receipt.json
	$(PYTHON) benchmarks/verify_bounded_thermo_cosmology_v1_0_1.py --seed 42 > $(BUILD_DIR)/cosmo_receipt.json
	$(PYTHON) benchmarks/verify_qpv_dandelion_loss_v1_0_0.py --seed 42
	$(PYTHON) benchmarks/verify_gemini_carbon_kembali_v1_2_1.py --seed 42 > $(BUILD_DIR)/kembali_receipt.json
	$(PYTHON) benchmarks/verify_ergospheric_healthcheck_v3_5_1.py --seed 42 > $(BUILD_DIR)/ergo_receipt.json
	$(PYTHON) benchmarks/verify_dandelion_noo_rubik_v4_2_1.py --seed 42 > $(BUILD_DIR)/rubik_receipt.json
	$(PYTHON) benchmarks/methylated_interior_search_benchmark_v1_0_1.py --seed 42 > $(BUILD_DIR)/epig_receipt.json
	$(PYTHON) benchmarks/verify_dandelion_quorum_parity_v1_0_0.py --seed 42 > $(BUILD_DIR)/dandelion_quorum_py_receipt.txt
	$(PYTHON) benchmarks/verify_triadic_falsification_contract_v1_1_0.py --seed 42 > $(BUILD_DIR)/triadic_falsification_py_receipt.json
	$(PYTHON) benchmarks/verify_rotonium_oam_vs_tfln_oatc_v1_0_0.py --seed 42 > $(BUILD_DIR)/rotonium_oatc_py_receipt.txt
	$(PYTHON) benchmarks/verify_cuprate_pump_probe_quench_v3_15_1.py --seed 42 --c-source src/cuprate_pump_probe_quench_32b_v3_15_1.c --json-out "$(BUILD_DIR)/cuprate_quench_receipt.json" > /dev/null
	$(PYTHON) benchmarks/verify_vp_vnp_bounded_3d_metrology_v1_0_0.py --seed 42 --c-source src/vp_vnp_bounded_3d_metrology_64b_v1_0_0.c --lean-witness verification/StatementWitness_VPVNP_v1_0_0.lean --json-out "$(BUILD_DIR)/vp_vnp_receipt.json" > /dev/null
	$(PYTHON) benchmarks/verify_walk_forward_forecast_gate_v1_0_0.py --seed 42 > $(BUILD_DIR)/walk_forward_py_receipt.txt
	$(PYTHON) benchmarks/verify_quantum_energy_dandelion_parity_v1_0_0.py --seed 42 > $(BUILD_DIR)/quantum_grid_py_receipt.json
	env -u BASH_ENV bash "$(CURDIR)/scripts/audit_wfx_loot.sh" "$(CURDIR)"
	@echo "[ === DANDELION OS v5.2.0 VERIFICATION SUITE PASSED (malloc = 0, --seed 42, CODE_SHOKUNIN_UNLATCHED = 255) === ]"

clean:
	rm -rf "$(BUILD_DIR)"
