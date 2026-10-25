# Dandelion OS local build guide

This project is easiest to build and experiment with from a normal local checkout. The main goal is to make the repo fork-friendly: contributors should be able to clone, install dependencies, build, and verify without chasing remote URLs or undocumented assumptions.

## Requirements

- gcc or clang
- make
- python3
- Optional: qemu-system-x86_64 for the virtual-boot target

## Fast path

```bash
git clone https://github.com/<your-user>/dandelion-os.git
cd dandelion-os
python3 -m venv .venv
. .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
make build
make check
```

## Common commands

```bash
make help
make build
make check
make shell
make json
make qemu
```

## Why this helps forks and contributors

- Local builds work from a clean checkout instead of expecting a special environment.
- Python dependencies are pinned in one place.
- The verification workflow is documented in a single place.
- Contributors can iterate on experiments without having to reverse-engineer the project layout.

## Notes

The verification harness (`verify_dandelion_os_v2.7.0.py`) depends on NumPy and Matplotlib for generating the state map and timeline artifacts, so installing `requirements.txt` is part of the supported workflow.
