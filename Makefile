VERSION          := v3.0.0
SEMVER           := 3.0.0
CC               ?= gcc
STRICT_WARNINGS  ?= 0
USE_WILD_LINKER  ?= 0

BASE_CFLAGS      := -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2
STRICT_CFLAGS    := -Wconversion -Wundef -Werror
COMMON_CFLAGS    := $(BASE_CFLAGS)
ifeq ($(STRICT_WARNINGS),1)
COMMON_CFLAGS    += $(STRICT_CFLAGS)
endif

CFLAGS           ?= $(COMMON_CFLAGS)
SHFLAGS          := -std=c99 -O3 -fPIC -shared
LDFLAGS          := -lm

BUILD_DIR        := build
TARGET           := $(BUILD_DIR)/dandelion_os_grimm_fbw_$(VERSION)
SHLIB            := $(BUILD_DIR)/libdandelion_os_$(VERSION).so
SRC              := dandelion_os_grimm_fbw_$(VERSION).c
HDR              := dandelion_os_$(VERSION).h

.PHONY: all shared test verify clean build check check-strict usb help

all: build

help:
	@echo "Dandelion OS build targets:"
	@echo "  make build        Compile the C99 microkernel and shared library into ./build"
	@echo "  make test         Run the built-in self-test"
	@echo "  make verify      Run the Python verification harness"
	@echo "  make check        Alias for make verify"
	@echo "  make check-strict Run the strict verification flow"
	@echo "  make clean        Remove build artifacts"
	@echo "  make usb          Show the USB staging status"

build: $(TARGET) $(SHLIB)

$(TARGET): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(SHLIB): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SHFLAGS) $(SRC) -o $(SHLIB) $(LDFLAGS)

test: all
	./$(TARGET) --self-test --tnpt dandelion_b158_$(VERSION).tnpt --seed 42

verify: all
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode portable

check: verify

check-strict: all
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode strict

usb:
	@echo "[USB] USB staging is not implemented in this repository; no portable live-rootfs step is configured."

clean:
	rm -rf $(BUILD_DIR)
