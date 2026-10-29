# Dandelion OS Quickstart

Welcome. Dandelion OS is designed to be approachable for people, AI systems, researchers, and resilient communities. The goal is simple: make the system understandable, reproducible, and usable.

## The ethos

Die Gedanken sind frei.

We aim to make the system accessible across human, AI, and machine interfaces. The design goal is not gatekeeping; it is shared understanding.

## Quick setup

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
python3 -m pip install -r requirements.txt
make build
make check
```

## Common commands

```bash
make help
make clean
make build
make test
make verify
make check
```

## What to expect

- `make build` compiles the project into `build/`
- `make check` runs the portable project verification suite
- `make verify` runs the project verification script directly
- `make clean` removes generated build artifacts

## Documentation

- [README.md](../README.md)
- [CONTRIBUTING.md](../CONTRIBUTING.md)
- [SECURITY.md](../SECURITY.md)
- [CHANGELOG.md](../CHANGELOG.md)
