<<<<<<< HEAD
# Dandelion OS & Shingari v4.3.0 Triadic Build & Verification Makefile

SHG_FILES := $(wildcard shg/*.shg)
RUST_GEN := build/shingari_generated.rs
JULIA_GEN := build/ShingariPilot.jl
BIN_OUT := harappan_servo.bin
LIB_SO := build/libdandelion_os_v3.1.0.so

all: triad

triad: $(LIB_SO) $(BIN_OUT) verify

$(RUST_GEN) $(JULIA_GEN) $(BIN_OUT): $(SHG_FILES) scripts/shingaric_v4_3_0.py
	@mkdir -p build
	python3 scripts/shingaric_v4_3_0.py $(SHG_FILES) \
		--emit-rust $(RUST_GEN) \
		--emit-julia $(JULIA_GEN) \
		--emit-bin $(BIN_OUT) \
		--lib-path $(LIB_SO)

$(LIB_SO): $(RUST_GEN)
	@mkdir -p build
	cargo build --release
	@if [ -f target/release/libdandelion_os_v3_1_0.so ]; then \
		cp target/release/libdandelion_os_v3_1_0.so $(LIB_SO); \
	elif [ -f target/release/libdandelion_os_shigari.so ]; then \
		cp target/release/libdandelion_os_shigari.so $(LIB_SO); \
	fi
	@echo "[0 GREEN] Linked $(LIB_SO) (64-byte C-ABI verified)"

verify: $(LIB_SO) $(BIN_OUT) $(JULIA_GEN)
	python3 export_servo_bin.py
	julia --startup-file=no -O3 verify_triad.jl

watch:
	@echo "[WATCH] Monitoring shg/*.shg and export scripts..."
	@while true; do \
		inotifywait -q -e close_write shg/ scripts/ export_servo_bin.py verify_triad.jl 2>/dev/null; \
		make triad || true; \
	done

clean:
	rm -rf build target $(BIN_OUT)

.PHONY: all triad verify watch clean
=======
# ==============================================================================
# DANDELION OS v4.1.2 (BRIGHT PORCH & SUNSTONE EDITION) - MASTER BUILD SYSTEM
# Target: Sovereign Bare-Metal ISO C99 & Linux 6.12+ PREEMPT_RT / sched_ext
# Strict Linus's Law Compliance: -std=c99 -O3 -Wall -Wextra -Wconversion -Werror
# ==============================================================================

CC       = gcc
CFLAGS   = -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror -Iinclude
LDFLAGS  = -lm
BUILD    = build

TARGETS  = $(BUILD)/dandelion_os_grimm_fbw \
           $(BUILD)/libdandelion_os.so \
           $(BUILD)/ternary_raft \
           $(BUILD)/fox_daemon_core \
           $(BUILD)/dandelion_mtp_assist

.PHONY: all clean dirs test

all: dirs $(TARGETS)

dirs:
	@mkdir -p $(BUILD)

$(BUILD)/dandelion_os_grimm_fbw: src/dandelion_os_grimm_fbw_v3.5.1.c include/dandelion_os_v3.5.1.h
	$(CC) $(CFLAGS) -DDANDELION_STANDALONE_MAIN $< -o $@ $(LDFLAGS)

$(BUILD)/libdandelion_os.so: src/dandelion_os_grimm_fbw_v3.5.1.c include/dandelion_os_v3.5.1.h
	$(CC) $(CFLAGS) -fPIC -shared $< -o $@ $(LDFLAGS)

$(BUILD)/ternary_raft: src/ternary_raft.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(BUILD)/fox_daemon_core: src/fox_daemon_core.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

$(BUILD)/dandelion_mtp_assist: src/dandelion_mtp_assist_v4_1_1.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

test: all
	@echo "=== [1/6] Running 16-Gate Shingari/Grimm/Fowler/DNHM C99 & Python FFI Suite ==="
	python3 verification/verify_dandelion_os_v4_1_1.py
	@echo ""
	@echo "=== [2/6] Running Ternary Raft Consensus & 3-Way Split-Brain Simulation ==="
	./$(BUILD)/ternary_raft
	@echo ""
	@echo "=== [3/6] Running Fox Daemon L1 Pruning & Thermal-Entropy Gate ==="
	./$(BUILD)/fox_daemon_core
	@echo ""
	@echo "=== [4/6] Running Dandelion OS v4.1.2 MTP & Kramhaus Tensegrity Kernel ==="
	./$(BUILD)/dandelion_mtp_assist
	@echo ""
	@echo "=== [5/6] Running Gemini Argon 'Dave's Not Here, Man' 612-Impasse Audit ==="
	python3 verification/verify_daves_not_here_argon_v1_0_0.py
	@echo ""
	@echo "=== [6/6] Running Epistemic Weight & Grand Negus Gold-Pressed Latinum Test ==="
	python3 verification/verify_epistemic_weight_boundary_v1_0_0.py

clean:
	@rm -rf $(BUILD)
>>>>>>> feature/shingari-v4.4.0
