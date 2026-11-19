CC ?= gcc
PYTHON ?= python3
BUILD_DIR ?= $(HOME)/.cache/dandelion_os_v5_1_0
CFLAGS_C99 := -std=c99 -O3 -Wall -Wextra -Wconversion -Werror
CFLAGS_C11 := -std=c11 -O3 -Wall -Wextra -Wconversion -Werror

.PHONY: all verify clean

all: \
	$(BUILD_DIR)/dandelion-v5 \
	$(BUILD_DIR)/libdandelion_successor_v5_0_0.so \
	$(BUILD_DIR)/asymmetric_ratchet_dnd3_v1_0_1 \
	$(BUILD_DIR)/bounded_thermo_cell64_v1_0_1 \
	$(BUILD_DIR)/qpv_dandelion_hindsight_v1_0_0 \
	$(BUILD_DIR)/kembali_carbon_microkernel_v1_2_1

$(BUILD_DIR)/dandelion-v5: src/dandelion_successor_v5_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) $< -o $@

$(BUILD_DIR)/libdandelion_successor_v5_0_0.so: src/dandelion_successor_v5_0_0.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS_C99) -fPIC -shared $< -o $@

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

verify: all
	$(BUILD_DIR)/dandelion-v5
	$(BUILD_DIR)/asymmetric_ratchet_dnd3_v1_0_1
	$(BUILD_DIR)/bounded_thermo_cell64_v1_0_1
	$(BUILD_DIR)/qpv_dandelion_hindsight_v1_0_0
	$(BUILD_DIR)/kembali_carbon_microkernel_v1_2_1
	$(MAKE) --no-print-directory -C hardware/chiral_bell_jar_v1_1_0 BUILD_DIR="$(BUILD_DIR)/cbj" verify
	$(PYTHON) madras/dandelion_communicator_madras_v5_0_0.py --verify
	$(PYTHON) benchmarks/fidel_dene_shingari_256byte_codec_v1_0_0.py --seed 42
	$(PYTHON) benchmarks/nadene_bergson_channel_benchmark_v1_0_1.py --seed 42 --trials 200 --out "$(BUILD_DIR)/nadene_bergson_audit_v1_0_1.json"
	$(PYTHON) benchmarks/verify_asymmetric_ratchet_dnd3_v1_0_1.py --seed 42 > $(BUILD_DIR)/ratchet_receipt.json
	$(PYTHON) benchmarks/verify_bounded_thermo_cosmology_v1_0_1.py --seed 42 > $(BUILD_DIR)/cosmo_receipt.json
	$(PYTHON) benchmarks/verify_qpv_dandelion_loss_v1_0_0.py --seed 42
	$(PYTHON) benchmarks/verify_gemini_carbon_kembali_v1_2_1.py --seed 42 > $(BUILD_DIR)/kembali_receipt.json
	$(PYTHON) benchmarks/verify_ergospheric_healthcheck_v3_5_1.py --seed 42 > $(BUILD_DIR)/ergo_receipt.json
	$(PYTHON) benchmarks/verify_dandelion_noo_rubik_v4_2_1.py --seed 42 > $(BUILD_DIR)/rubik_receipt.json
	$(PYTHON) benchmarks/methylated_interior_search_benchmark_v1_0_1.py --seed 42 > $(BUILD_DIR)/epig_receipt.json
	env -u BASH_ENV bash "$(CURDIR)/scripts/audit_wfx_loot.sh" "$(CURDIR)"

clean:
	rm -rf "$(BUILD_DIR)"
