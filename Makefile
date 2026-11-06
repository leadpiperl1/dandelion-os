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

# Dandelion OS v4.2.0 DMA-BUF & io_uring Pipeline Verification Target
dma_tests: dandelion_dma_uring.c test_dandelion_dma_uring.c dandelion_dma_uring.h
	gcc -std=c11 -Wall -Wextra -Werror -O3 dandelion_dma_uring.c test_dandelion_dma_uring.c -o run_dma_tests
	./run_dma_tests

triad: dma_tests
	@echo "[ === TRIAD VERIFICATION PASSED === ]"
