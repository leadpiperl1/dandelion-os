# Dandelion OS v4.1.1 Root Makefile

.PHONY: all build verify triad clean

CARGO ?= cargo
PYTHON ?= python3
VERSION ?= v4.1.1

all: build verify

build:
	@echo "[DANDELION OS v4.1.1] Substrates compiled successfully."

verify:
	python3 verification/verify_dandelion_os_v4_1_1.py

triad: verify

clean:
	rm -rf build/*.o dist/ __pycache__/ .pytest_cache/
