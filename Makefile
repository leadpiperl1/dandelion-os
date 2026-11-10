CC      = gcc
CFLAGS  = -O3 -std=c11 -Wall -Wextra -Werror -Wpedantic -Iinclude

.PHONY: all c11_kernel verify rust_build julia_check clean

all: c11_kernel verify

c11_kernel:
	@mkdir -p build
	$(CC) $(CFLAGS) src/argon_methylation_engine.c -o build/argon_methylation_engine
	./build/argon_methylation_engine

verify: c11_kernel
	python3 tests/verify_argon_rc_methylation.py --seed 42 --out-dir telemetry

rust_build:
	cd rust && cargo build --release

julia_check:
	julia --project=julia -e 'include("julia/src/control_plane.jl"); println("[JULIA] DandelionRoute Control Plane verified.")'

clean:
	rm -rf build rust/target julia/Manifest.toml
