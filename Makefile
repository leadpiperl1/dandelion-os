# Dandelion OS & Embedded AI v3.0.0 Sovereign Makefile
VERSION          := v3.0.0
SEMVER           := 3.0.0
CC               ?= gcc
STRICT_WARNINGS  ?= 0

CFLAGS_COMMON    := -std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef
CFLAGS           := $(CFLAGS_COMMON) $(if $(filter 1,$(STRICT_WARNINGS)),-Werror,)
SHFLAGS          := -std=c99 -O3 -fPIC -shared
LDFLAGS          := -lm

BUILD_DIR        := build
TARGET           := $(BUILD_DIR)/dandelion_os_grimm_fbw_$(VERSION)
SHLIB            := $(BUILD_DIR)/libdandelion_os_$(VERSION).so
SRC              := dandelion_os_grimm_fbw_$(VERSION).c
HDR              := dandelion_os_$(VERSION).h

.PHONY: all build shared test check check-strict verify usb clean

all: build shared

build: $(TARGET)

shared: $(SHLIB)

$(TARGET): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(SHLIB): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SHFLAGS) $(SRC) -o $(SHLIB) $(LDFLAGS)

test: build
	./$(TARGET) --self-test --tnpt dandelion_b158_$(VERSION).tnpt --seed 42

check: test
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode portable

check-strict: build
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode strict

verify: check-strict

usb:
	bash ./mk_switchblade_usb_$(VERSION).sh

clean:
	rm -rf $(BUILD_DIR)
