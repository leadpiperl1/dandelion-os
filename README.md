# Dandelion OS v3.2.0

Dandelion OS is a sovereign, AI-assisted operating model built around deterministic verification, fail-closed safety gates, and a compact C99 runtime. The active branch state is v3.2.0, and the repository is being normalized to keep this version as the single source of truth.

## What this repo is

- C99 microkernel-style runtime and verification harness
- deterministic Python-based validation and reporting
- release and packaging tooling for build artifacts and USB/live-boot workflows
- documentation and governance for trustworthy contributor behavior

## Canonical version and state

This branch is intentionally aligned around Dandelion OS v3.2.0. Historical v3.0.0 files and generated payloads are no longer treated as the active operating source of truth and should be considered archive or migration references rather than current system definitions.

## Quick start

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
python3 -m pip install -r requirements.txt
make build
make check
```

## Useful commands

```bash
make help
make build
make check
make check-strict
make test
make verify
make shell
make json
make clean
```

## Repository hygiene

The repository is cleaned up around a stable structure:

- active source and docs remain in the canonical working tree
- generated outputs go into `build/` and `release/`
- historical artifacts and legacy outputs live under `archive/`
- root-level versioned bundles are not mistaken for the active source definition

## Build and verification notes

- `make build` compiles the runtime and shared object artifacts into `build/`
- `make check` runs the portable verification suite
- `make check-strict` enforces the stricter warning and verification path
- `make clean` removes generated working artifacts

## Documentation

- `AGENTS.md`
- `docs/quickstart.md`
- `docs/BUILDING.md`
- `docs/operating-principles.md`
- `docs/roadmap.md`
- `CONTRIBUTING.md`
- `SECURITY.md`

## Notes

The repo has a strong philosophy and clear safety intent, but it needed a cleanup pass after mixed-version artifacts and generated payloads were introduced into the same working tree. This cleanup normalizes the project around a single version and a clearer file lifecycle.
