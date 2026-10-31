# Dandelion OS development guide

This guide is the canonical contributor workflow for Dandelion OS.

## Project purpose

Dandelion OS is a compact, reproducible operating-system and embedded-AI prototype. The repository combines:

- C99 runtime and kernel logic
- Python verification and plotting
- shell automation and release tooling
- generated assets for validation and packaging

The project is strongest when the build flow is simple, explicit, and reproducible.

## Standard workflow

### 1. Clone and prepare the environment

```bash
git clone https://github.com/<your-user>/dandelion-os.git
cd dandelion-os
python3 -m venv .venv
. .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
```

### 2. Build the project

```bash
make build
```

This compiles the project into `build/`.

### 3. Run the portable verification suite

```bash
make check
```

This runs the project self-test and portable validation logic.

### 4. Run stricter verification

```bash
make check-strict
```

This is the stricter validation pass intended for maintainers and release-sensitive work.

### 5. Run the full verification alias

```bash
make verify
```

This is the canonical verification alias used for stricter project validation.

### 6. Generate the USB/live flow

```bash
make usb
```

This stages the Switchblade-style live deployment packaging flow.

### 7. Clean generated output

```bash
make clean
```

## Command map

```bash
make build         # compile the project into build/
make test          # run the built self-test target
make check         # portable verification flow
make check-strict  # stricter verification flow
make verify        # verification alias
make usb           # live USB / packaging flow
make clean         # remove build outputs
```

## Contributor expectations

- Keep commands reproducible and easy to run from a clean checkout.
- Prefer explicit build/test phases over hidden assumptions.
- Make generated artifacts disposable and build-only.
- Treat docs as part of the codebase: when the command surface changes, documentation must change with it.
- Favor clear, explicit failures rather than magical setup.

## Review checklist before a PR

Before a patch is merged or a release is cut, confirm:

- `make build` succeeds
- `make check` succeeds
- `make check-strict` passes when the work affects strict verification
- docs reflect the real command surface
- generated files are not being accidentally committed unless intentionally versioned

## Repository hygiene

This repo carries a strong narrative identity, but the operating reality should remain straightforward and transparent. If a phrase, symbol, or workflow is poetic, it must still map to a concrete file, command, or output in the repo.
