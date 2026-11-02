# ==============================================================================
# DANDELION OS v3.5.0 ("HEIMAT-AGORA") — SOVEREIGN C99 & POLYGLOT MAKEFILE
# ==============================================================================
# Enforces the 6-Point CI & Security Contract Gate:
#   1. Universal targets (.PHONY: all build shared check check-strict test verify preflight release usb shell json clean help)
#   2. Separation of portable flags vs strict maintainer flags (STRICT_WARNINGS=1: -Werror -Wconversion)
#   3. Optional Wild linker support (USE_WILD_LINKER=1) with automatic host fallback
#   4. Zero-dependency ISO C99 microkernel & shared library ABI (.so) build
#   5. Deterministic <=60s 15-gate verification harness + Linus's Law Red-Team Audit
#   6. Secure mktemp preflight logging (remediating PR #9 symlink clobber vulnerability)
# ==============================================================================

VERSION := v3.5.0
SEMVER := 3.5.0
CC ?= gcc
PYTHON ?= python3
STRICT_WARNINGS ?= 0
USE_WILD_LINKER ?= 0

CFLAGS_PORTABLE := -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wundef
CFLAGS_STRICT := -Wconversion -Werror

ifeq ($(STRICT_WARNINGS),1)
    CFLAGS := $(CFLAGS_PORTABLE) $(CFLAGS_STRICT)
else
    CFLAGS := $(CFLAGS_PORTABLE)
endif

SHFLAGS := $(CFLAGS) -fPIC -shared
LDFLAGS := -lm

ifeq ($(USE_WILD_LINKER),1)
    ifneq ($(shell command -v wild 2>/dev/null),)
        LDFLAGS += -fuse-ld=wild
    endif
endif

BUILD_DIR := build
RELEASE_DIR := $(BUILD_DIR)/release
TARGET := $(BUILD_DIR)/dandelion_os_grimm_fbw_$(VERSION)
SHLIB := $(BUILD_DIR)/libdandelion_os_$(VERSION).so
SRC := dandelion_os_grimm_fbw_$(VERSION).c
HDR := dandelion_os_$(VERSION).h
VERIFY_PY := verify_dandelion_os_$(VERSION).py
USB_SH := mk_switchblade_usb_$(VERSION).sh
TNPT := dandelion_b158_$(VERSION).tnpt
INITRD := initramfs_dandelion_$(VERSION).cpio.gz
STATE_MAP := dandelion_os_grimm_whiteroom_state_map_$(VERSION).png
MANIFEST := audit_manifest_$(VERSION).json

.PHONY: all build shared check check-strict test verify preflight release usb shell json clean help

all: build shared

build: $(TARGET) $(SHLIB)

shared: $(SHLIB)

$(TARGET): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(SHLIB): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SHFLAGS) $(SRC) -o $(SHLIB) $(LDFLAGS)

check: build
	$(PYTHON) $(VERIFY_PY) --seed 42 --mode portable

check-strict:
	$(MAKE) clean
	$(MAKE) build STRICT_WARNINGS=1
	$(PYTHON) $(VERIFY_PY) --seed 42 --mode strict

test: build
	$(PYTHON) $(VERIFY_PY) --seed 42 --mode portable
	./$(TARGET) --verify --tnpt $(TNPT) --seed 42

verify: check-strict

preflight: build
	@log_file="$$(mktemp /tmp/dandelion_preflight.XXXXXX.log)"; \
	trap 'rm -f "$$log_file"' EXIT; \
	if $(PYTHON) $(VERIFY_PY) --seed 42 --mode strict >"$$log_file" 2>&1; then \
		cat "$$log_file"; \
		echo "[PREFLIGHT PASS] All 15 gates and Linus's Law fault-injection checks verified."; \
	else \
		cat "$$log_file" >&2; \
		exit 1; \
	fi

release: check-strict
	@mkdir -p $(RELEASE_DIR)
	@cp -f $(TARGET) $(SHLIB) $(TNPT) $(INITRD) $(STATE_MAP) $(MANIFEST) $(RELEASE_DIR)/
	@echo "[RELEASE] Dandelion OS $(VERSION) ('Heimat-Agora') staged in $(RELEASE_DIR)/"

usb: build
	@chmod +x $(USB_SH)
	@BUILD_DIR=/tmp/build_whiteroom_$(VERSION) OUT_DIR=$(BUILD_DIR) ./$(USB_SH)

shell: $(TARGET)
	./$(TARGET) --shell

json: $(TARGET)
	./$(TARGET) --json "div compute fbw pitch trim"

clean:
	rm -rf $(BUILD_DIR) /tmp/build_whiteroom_$(VERSION) __pycache__

help:
	@echo "Dandelion OS $(VERSION) ('Heimat-Agora') Build & Verification Targets:"
	@echo "  make all          - Build C99 microkernel binary and shared library (.so)"
	@echo "  make build        - Compile $(TARGET) and $(SHLIB) (STRICT_WARNINGS=$(STRICT_WARNINGS))"
	@echo "  make shared       - Compile zero-copy shared C-ABI library $(SHLIB)"
	@echo "  make check        - Run 15-gate verification harness in portable CI mode"
	@echo "  make check-strict - Compile with STRICT_WARNINGS=1 (-Werror -Wconversion) & run strict 15-gate audit"
	@echo "  make test         - Run portable 15-gate verification and C99 self-test suite"
	@echo "  make verify       - Alias for make check-strict"
	@echo "  make preflight    - Secure mktemp preflight verification gate (PR #9 hardened)"
	@echo "  make release      - Execute strict verification and stage release bundle in $(RELEASE_DIR)/"
	@echo "  make usb          - Stage Switchblade AI-Pen dual-mode Live USB RootFS"
	@echo "  make shell        - Launch interactive F-16 Fly-By-Wire HUD shell"
	@echo "  make json         - Emit sample zero-TUI JSON telemetry packet"
	@echo "  make clean        - Remove build/ directory and temporary artifacts"
