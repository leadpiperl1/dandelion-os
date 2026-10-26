# ==============================================================================
# DANDELION OS v2.8.0 (CODENAME: SOVEREIGN DESKTOP & EMBEDDED AI RELEASE)
# STANDARD OPEN-SOURCE BUILD & DETERMINISTIC VERIFICATION MAKEFILE
# ==============================================================================
# Version:     v2.8.0 (Strict 3-Digit Triadic SemVer: MAJOR=2, MINOR=8, PATCH=0)
# Author:      Justin D. Grimm (K-Lani 13 Actual, @leadpiperl1, ORCID: 0009-0000-5299-0243)
# License:     Dual GPL-2.0-or-later / MIT
# ==============================================================================

CC          ?= gcc
PYTHON      ?= python3
VERSION     := v2.8.0
TARGET_BIN  := dandelion_os_grimm_fbw_$(VERSION)
SRC_C       := dandelion_os_grimm_fbw_$(VERSION).c
HDR_H       := dandelion_os_v2.8.0.h
VERIFY_PY   := verify_dandelion_os_$(VERSION).py
USB_SH      := mk_switchblade_usb_$(VERSION).sh
PUSH_SH     := push_dandelion_day_github_$(VERSION).sh

CFLAGS      := -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror
LDFLAGS     := -lm -lpthread

# Auto-detect Wild 0.11 parallel linker if present on host
ifneq ($(shell command -v wild 2>/dev/null),)
    LDFLAGS += -fuse-ld=wild
endif

.PHONY: all build check shell json usb clean help

all: build

build: $(TARGET_BIN)

$(TARGET_BIN): $(SRC_C) $(HDR_H)
	@echo "[BUILD] Compiling $(SRC_C) -> $(TARGET_BIN) with strict ISO C99 flags..."
	$(CC) $(CFLAGS) -o $(TARGET_BIN) $(SRC_C) $(LDFLAGS)
	@mkdir -p bin
	@cp -f $(TARGET_BIN) bin/$(TARGET_BIN)
	@echo "[OK] Built $(TARGET_BIN) (Zero warnings, -Werror clean)."

check: build
	@echo "[CHECK] Running 60-second deterministic verification harness ($(VERIFY_PY) --seed 42)..."
	$(PYTHON) $(VERIFY_PY) --seed 42

shell: build
	./$(TARGET_BIN) --shell

json: build
	./$(TARGET_BIN) --json "div compute fbw pitch trim"

usb: build
	chmod +x $(USB_SH)
	./$(USB_SH)

clean:
	@echo "[CLEAN] Removing temporary build directories..."
	rm -rf build_whiteroom_$(VERSION) __pycache__ *.o

help:
	@echo "=============================================================================="
	@echo " Dandelion OS $(VERSION) ('Sovereign Desktop & Embedded AI Release') Targets:"
	@echo "   make          / make build : Compile C99 microkernel with -O3 & -Werror"
	@echo "   make check                 : Run <=60s verification harness & unit tests"
	@echo "   make shell                 : Launch interactive F-16 FBW ternary shell"
	@echo "   make json                  : Emit composable zero-TUI JSON telemetry line"
	@echo "   make usb                   : Stage Dual-Mode Switchblade Live USB RootFS"
	@echo "   make clean                 : Clean temporary build artifacts"
	@echo "=============================================================================="
