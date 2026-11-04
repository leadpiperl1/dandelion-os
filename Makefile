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
