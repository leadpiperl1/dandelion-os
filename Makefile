# Dandelion OS v4.1.1 Root Makefile
# Substrates: C99, Rust (#![no_std]), Julia 1.12 FFI, Python Verification, Zenodo Packaging

.PHONY: all build rust julia verify zenodo-bundle clean

CARGO ?= cargo
JULIA ?= julia
PYTHON ?= python3
VERSION ?= v4.1.1

all: build verify

build: rust julia
	@echo "[DANDELION OS v4.1.1] All substrates compiled successfully."

rust:
	@echo "[RUST] Building no_std Rust substrate..."
	$(CARGO) build --release

julia:
	@echo "[JULIA] Compiling FFI and dispatch bindings..."
	$(JULIA) --project=. -e 'using Pkg; Pkg.instantiate(); println("Julia FFI environment ready.")'

verify:
	@echo "[VERIFY] Running Dandelion OS $(VERSION) verification suite..."
	$(PYTHON) verification/verify_dandelion_os_v4_1_1.py --seed 42

zenodo-bundle: verify
	@echo "[ZENODIFY] Packaging Lazy Predator Zenodo release bundle ($(VERSION))..."
	@mkdir -p dist/zenodo_payload
	@cp -r verification/ dist/zenodo_payload/
	@cp target/release/*.a target/release/*.rlib dist/zenodo_payload/ 2>/dev/null || true
	@if [ -f .zenodo.json ]; then cp .zenodo.json dist/zenodo_payload/; else echo '{"title": "Dandelion OS $(VERSION)", "upload_type": "software"}' > dist/zenodo_payload/.zenodo.json; fi
	@if [ -f CITATION.cff ]; then cp CITATION.cff dist/zenodo_payload/; else echo 'cff-version: 1.2.0' > dist/zenodo_payload/CITATION.cff; fi
	@cd dist && zip -r dandelion_os_$(VERSION)_lazy_predator_zenodo.zip zenodo_payload/
	@echo "[ZENODIFY] Release archive successfully generated at dist/dandelion_os_$(VERSION)_lazy_predator_zenodo.zip"

clean:
	@echo "[CLEAN] Purging build artifacts..."
	$(CARGO) clean
	rm -rf build/*.o dist/ __pycache__/ .pytest_cache/
