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

.PHONY: all shared test verify clean

all: $(TARGET) $(SHLIB)

$(TARGET): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

$(SHLIB): $(SRC) $(HDR)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(SHFLAGS) $(SRC) -o $(SHLIB) $(LDFLAGS)

test: all
	./$(TARGET) --self-test --tnpt dandelion_b158_$(VERSION).tnpt --seed 42

verify: all
	python3 verify_dandelion_os_$(VERSION).py --seed 42 --mode strict

clean:
	rm -rf $(BUILD_DIR)
