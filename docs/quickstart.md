# Dandelion OS Quickstart

Welcome. This repo is designed to be approachable for humans, contributors, researchers, and AI systems. The goal is simple: keep the project understandable, reproducible, and usable.

## Ethos

Die Gedanken sind frei.

We aim to make the system accessible without gatekeeping. Shared understanding matters more than hidden setup steps.

## Quick setup

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
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
make build
make test
make check
make check-strict
make verify
make usb
make clean
```

## What to expect

- `make build` compiles the project into `build/`
- `make test` runs the project self-test target
- `make check` runs the portable verification flow
- `make check-strict` runs the stricter validation path
- `make usb` stages the live USB deployment flow
- `make clean` removes generated build artifacts

## Documentation

- [README.md](../README.md)
- [docs/BUILDING.md](BUILDING.md)
- [docs/DEVELOPMENT.md](DEVELOPMENT.md)
- [CONTRIBUTING.md](../CONTRIBUTING.md)
- [SECURITY.md](../SECURITY.md)
