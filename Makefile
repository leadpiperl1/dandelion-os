# ==============================================================================
# DANDELION OS v3.0.0 ("SOVEREIGN ARGON FRONTIER & EMBEDDED AI RELEASE")
# PORTABLE ISO C99 & VERIFICATION MAKEFILE (COPILOT AUDIT COMPLIANT)
# ==============================================================================
# Supported Compilers : GCC >= 9.0, Clang >= 11.0, or any ISO C99 compiler
# Supported Platforms : Linux (Primary PREEMPT_RT target) & POSIX (macOS / BSD)
# Build Output Dir    : ./build/ (keeps source tree clean; override via BUILD_DIR)
# Opt-In Wild Linker  : USE_WILD_LINKER ?= 0 (set USE_WILD_LINKER=1 to enable)
# Strict Warning Gate : STRICT_WARNINGS ?= 0 (set STRICT_WARNINGS=1 for -Werror)
# ==============================================================================

VERSION          := v3.0.0
CC               ?= cc
PYTHON           ?= python3
BUILD_DIR        ?= build
USE_WILD_LINKER  ?= 0
STRICT_WARNINGS  ?= 0

TARGET_NAME      := dandelion_os_grimm_fbw_$(VERSION)
TARGET_BIN       := $(BUILD_DIR)/$(TARGET_NAME)
SRC_C            := dandelion_os_grimm_fbw_$(VERSION).c
HDR_H            := dandelion_os_$(VERSION).h
VERIFY_PY        := verify_dandelion_os_$(VERSION).py
USB_SH           := mk_switchblade_usb_$(VERSION).sh
PUSH_SH          := push_dandelion_day_github_$(VERSION).sh

BASE_CFLAGS      := -std=c99 -O3 -Wall -Wextra -Wpedantic
STRICT_CFLAGS    := -Wshadow -Wformat=2 -Wconversion -Wundef -Werror
CFLAGS           ?= $(BASE_CFLAGS)
ifeq ($(STRICT_WARNINGS),1)
    CFLAGS       += $(STRICT_CFLAGS)
endif

LDFLAGS          ?= -lm -lpthread

# Copilot Point 3: Gate nonstandard 'wild' linker behind explicit opt-in USE_WILD_LINKER=1
ifeq ($(USE_WILD_LINKER),1)
    ifeq ($(shell command -v wild 2>/dev/null),)
        $(error [ERROR] USE_WILD_LINKER=1 was requested, but 'wild' linker was not found in PATH. Install wild or build with USE_WILD_LINKER=0)
    endif
    LDFLAGS      += -fuse-ld=wild
endif

.PHONY: all build check check-strict shell json usb clean help

all: build

build: $(TARGET_BIN)

$(TARGET_BIN): $(SRC_C) $(HDR_H)
	@command -v $(CC) >/dev/null 2>&1 || { echo "[ERROR] C compiler '$(CC)' not found in PATH."; exit 1; }
	@mkdir -p $(BUILD_DIR)
	@echo "[BUILD] Compiling $(SRC_C) -> $(TARGET_BIN) (CC=$(CC), STRICT_WARNINGS=$(STRICT_WARNINGS), USE_WILD_LINKER=$(USE_WILD_LINKER))..."
	$(CC) $(CFLAGS) -o $(TARGET_BIN) $(SRC_C) $(LDFLAGS)
	@echo "[OK] Built $(TARGET_BIN)."

check: build
	@echo "[CHECK] Running portable verification harness ($(VERIFY_PY) --seed 42 --mode portable)..."
	$(PYTHON) $(VERIFY_PY) --seed 42 --mode portable

check-strict:
	@$(MAKE) build STRICT_WARNINGS=1
	@echo "[CHECK-STRICT] Running strict hardware/compiler verification ($(VERIFY_PY) --seed 42 --mode strict)..."
	$(PYTHON) $(VERIFY_PY) --seed 42 --mode strict

shell: build
	./$(TARGET_BIN) --shell

json: build
	./$(TARGET_BIN) --json "div compute fbw pitch trim"

usb: build
	chmod +x $(USB_SH)
	USE_WILD_LINKER=$(USE_WILD_LINKER) BUILD_DIR=$(BUILD_DIR)/whiteroom_$(VERSION) ./$(USB_SH)

clean:
	rm -rf $(BUILD_DIR) out bin build_whiteroom_* __pycache__ *.o ./$(TARGET_NAME)

help:
	@echo "=============================================================================="
	@echo " Dandelion OS $(VERSION) ('Sovereign Argon Frontier & Embedded AI Release') Targets:"
	@echo "   make build                   - Compile C99 microkernel into $(BUILD_DIR)/"
	@echo "   make build STRICT_WARNINGS=1 - Compile with strict -Werror -Wconversion"
	@echo "   make build USE_WILD_LINKER=1 - Opt-in build with 'wild' parallel linker"
	@echo "   make check                   - Run portable verification suite (CI default)"
	@echo "   make check-strict            - Run strict bare-metal RT jitter (<1.0 us) check"
	@echo "   make shell                   - Launch interactive F-16 FBW shell"
	@echo "   make json                    - Emit zero-TUI JSON telemetry packet"
	@echo "   make usb                     - Stage Switchblade Dual-Mode Live USB RootFS"
	@echo "   make clean                   - Remove $(BUILD_DIR)/ and temporary build outputs"
	@echo "=============================================================================="
