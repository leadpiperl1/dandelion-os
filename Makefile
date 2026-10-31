# ==============================================================================
# DANDELION OS v3.2.0 ("HEIMAT-AGORA") — SOVEREIGN C99 & POLYGLOT MAKEFILE
# ==============================================================================
VERSION := v3.2.0
SEMVER := 3.2.0
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
HDR := dandelion_os_grimm_fbw_$(VERSION).h
VERIFY_PY := verify_dandelion_os_$(VERSION).py
USB_SH := mk_switchblade_usb_$(VERSION).sh
TNPT := dandelion_b158_$(VERSION).tnpt
INITRD := initramfs_dandelion_$(VERSION).cpio.gz
STATE_MAP := dandelion_os_grimm_whiteroom_state_map_$(VERSION).png
MANIFEST := audit_manifest_$(VERSION).json

.PHONY: all build shared check check-strict test verify release usb shell json clean help

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
	./$(TARGET) --verify

verify: check-strict

release: check-strict
	@mkdir -p $(RELEASE_DIR)
	@cp -f $(TARGET) $(SHLIB) $(TNPT) $(INITRD) $(STATE_MAP) $(MANIFEST) $(RELEASE_DIR)/

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
	@echo "Dandelion OS $(VERSION) Build & Verification Targets"
