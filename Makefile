# Dandelion OS & Embedded AI v3.0.0 Sovereign Makefile
VERSION          := v3.0.0
SEMVER           := 3.0.0
CC               ?= gcc
CFLAGS           := -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror
SHFLAGS          := -std=c99 -O3 -fPIC -shared
LDFLAGS          := -lm

BUILD_DIR        := build
TARGET           := $(BUILD_DIR)/dandelion_os_grimm_fbw_$(VERSION)
SHLIB            := $(BUILD_DIR)/libdandelion_os_$(VERSION).so
SRC              := dandelion_os_grimm_fbw_$(VERSION).c
HDR              := dandelion_os_$(VERSION).h

.PHONY: all build shared test verify check check-strict clean usb help preflight

# Default entry point: compile the target and shared library
all: build

help:
	@echo "Dandelion OS v3.0.0 Make targets"
	@echo "  build         Compile C99 artifacts into ./build/"
	@echo "  check         Run the portable verification suite"
	@echo "  check-strict  Run the strict maintainer verification suite"
	@echo "  test          Run the local self-test binary"
	@echo "  usb           Stage the live USB/rootfs workflow"
	@echo "  clean         Remove generated build artifacts"
	@echo "  preflight     Run compile + verification smoke check"

# Canonical release/build target expected by README.md and AGENTS.md
build: $(TARGET) $(SHLIB)

$(TARGET): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(SHLIB): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SHFLAGS) $(SRC) -o $(SHLIB) $(LDFLAGS)

# Portable verification target: matches the documented 'make check' workflow
check: verify

# Strict maintainer verification: compile with the repo's strict warnings and run portable tests in strict mode
check-strict:
	@mkdir -p $(BUILD_DIR)
	$(CC) -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror $(SRC) -o $(TARGET) $(LDFLAGS)
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode strict

# Backward-compatible local test target
test: build
	./$(TARGET) --self-test --tnpt dandelion_b158_$(VERSION).tnpt --seed 42

# Portable verification: runs the Python verification harness in the default portable mode
verify: build
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode portable

# Quick smoke-check used before release or PR review
preflight: build
	@log_file="$$(mktemp /tmp/dandelion_preflight.XXXXXX.log)"; python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode portable >"$$log_file" 2>&1 || { cat "$$log_file"; rm -f "$$log_file"; exit 1; }; rm -f "$$log_file"
	@echo "[preflight] Dandelion OS v3.0.0 portable verification completed successfully"

# USB staging target expected by release docs; stages the live disk rootfs generation workflow
usb:
	@echo "[USB] Staging Dandelion OS live USB/rootfs workflow into $(BUILD_DIR)"
	@echo "[USB] Intended flow: mk_switchblade_usb_v3.0.0.sh -> build/ rootfs + GRUB2 config"
	@bash ./mk_switchblade_usb_v3.0.0.sh

clean:
	rm -rf $(BUILD_DIR)
