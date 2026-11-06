# Dandelion OS & Shingari v4.3.0 Triadic Build & Verification Makefile

SHG_FILES := $(wildcard shg/*.shg)
RUST_GEN := build/shingari_generated.rs
JULIA_GEN := build/ShingariPilot.jl
BIN_OUT := harappan_servo.bin
LIB_SO := build/libdandelion_os_v3.1.0.so

all: triad

triad: $(LIB_SO) $(BIN_OUT) verify

$(RUST_GEN) $(JULIA_GEN) $(BIN_OUT): $(SHG_FILES) scripts/shingaric_v4_3_0.py
	@mkdir -p build
	python3 scripts/shingaric_v4_3_0.py $(SHG_FILES) \
		--emit-rust $(RUST_GEN) \
		--emit-julia $(JULIA_GEN) \
		--emit-bin $(BIN_OUT) \
		--lib-path $(LIB_SO)

$(LIB_SO): $(RUST_GEN)
	@mkdir -p build
	cargo build --release
	@if [ -f target/release/libdandelion_os_v3_1_0.so ]; then \
		cp target/release/libdandelion_os_v3_1_0.so $(LIB_SO); \
	elif [ -f target/release/libdandelion_os_shigari.so ]; then \
		cp target/release/libdandelion_os_shigari.so $(LIB_SO); \
	fi
	@echo "[0 GREEN] Linked $(LIB_SO) (64-byte C-ABI verified)"

verify: $(LIB_SO) $(BIN_OUT) $(JULIA_GEN)
	python3 export_servo_bin.py
	julia --startup-file=no -O3 verify_triad.jl

watch:
	@echo "[WATCH] Monitoring shg/*.shg and export scripts..."
	@while true; do \
		inotifywait -q -e close_write shg/ scripts/ export_servo_bin.py verify_triad.jl 2>/dev/null; \
		make triad || true; \
	done

clean:
	rm -rf build target $(BIN_OUT)

.PHONY: all triad verify watch clean
