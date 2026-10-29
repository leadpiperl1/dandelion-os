# Dandelion OS v3.0.0 ("Sovereign Argon Frontier & Embedded AI Release")

[![Version: v3.0.0](https://img.shields.io/badge/Version-v3.0.0-15803D?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/releases/tag/v3.0.0)
[![CI: GCC & Clang Matrix](https://img.shields.io/badge/GitHub_Actions_CI-GCC_%26_Clang-2563EB?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/.github/workflows/c-cpp.yml)
[![Kernel: Linux 6.12+ PREEMPT_RT + sched_ext](https://img.shields.io/badge/Linux_6.12+-PREEMPT__RT_%2B_sched__ext-1E40AF?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_rt_v3.0.0.config)
[![AI Engine: 1.58-Bit Ternary VBST + Dirac Sync](https://img.shields.io/badge/1.58--Bit_Ternary-VBST_%2B_Dirac_Sync-B91C1C?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_os_grimm_fbw_v3.0.0.c)

![Dandelion OS v3.0.0 Blueprint & Telemetry State Map](https://raw.githubusercontent.com/leadpiperl1/dandelion-os/main/dandelion_os_grimm_whiteroom_state_map_v3.0.0.png)

## Why this project exists

Die Gedanken sind frei.

Dandelion OS is a reproducible, sovereign, open engineering project designed to be understandable by humans, AI systems, and other interfaces that need clear, deterministic behavior. The goal is not lock-in; the goal is shared understanding and a usable public platform.

## Quick start

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
python3 -m pip install -r requirements.txt
make build
make check
```

## Project purpose

This repository contains a portable C99 microkernel, verification harness, and Linux-first real-time system model. It is intended to support experimentation, reproducibility, and public collaboration across human and machine interfaces.

## Documentation

- [docs/quickstart.md](docs/quickstart.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)
- [SECURITY.md](SECURITY.md)
- [CHANGELOG.md](CHANGELOG.md)
- [VERSION_LOG.md](VERSION_LOG.md)

## Contributing

We welcome contributors who care about reproducibility, clarity, and public trust. Please read [CONTRIBUTING.md](CONTRIBUTING.md) before opening a pull request.

## Security

If you discover a security issue, please report it privately via [SECURITY.md](SECURITY.md).

## License

This project is released under the GNU General Public License v2.0 or later, with project-specific caveats noted in the repository.

---

The rest of this repository contains the full v3.0.0 system design, verification logic, and operational notes.
