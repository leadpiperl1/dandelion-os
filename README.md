# Dandelion OS v3.0.0 ("Sovereign Argon Frontier & Embedded AI Release")

[![Version: v3.0.0](https://img.shields.io/badge/Version-v3.0.0-15803D?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/releases/tag/v3.0.0)
[![CI: GCC & Clang Matrix](https://img.shields.io/badge/GitHub_Actions_CI-GCC_%26_Clang-2563EB?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/.github/workflows/c-cpp.yml)
[![Kernel: Linux 6.12+ PREEMPT_RT + sched_ext](https://img.shields.io/badge/Linux_6.12+-PREEMPT__RT_%2B_sched__ext-1E40AF?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_rt_v3.0.0.config)
[![AI Engine: 1.58-Bit Ternary VBST + Dirac Sync](https://img.shields.io/badge/1.58--Bit_Ternary-VBST_%2B_Dirac_Sync-B91C1C?style=for-the-badge)](https://github.com/leadpiperl1/dandelion-os/blob/main/dandelion_os_grimm_fbw_v3.0.0.c)

![Dandelion OS v3.0.0 Blueprint & Telemetry State Map](https://raw.githubusercontent.com/leadpiperl1/dandelion-os/main/dandelion_os_grimm_whiteroom_state_map_v3.0.0.png)

## Quick Start

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
python3 -m pip install -r requirements.txt
make build
make check
```

This repository is designed for a portable Linux-like environment. The default build path is `make build`, and the project verification suite is `make check`.

## Project Purpose

Dandelion OS is a sovereign embedded AI operating system prototype combining a portable C99 core, deterministic verification tools, and a Linux-first real-time platform narrative.

## Contributing

We welcome contributions from builders, researchers, and maintainers. Please read [CONTRIBUTING.md](CONTRIBUTING.md) before opening a pull request.

## Security

Please report vulnerabilities privately. See [SECURITY.md](SECURITY.md).

## Documentation

- [README.md](README.md)
- [CONTRIBUTING.md](CONTRIBUTING.md)
- [SECURITY.md](SECURITY.md)
- [VERSION_LOG.md](VERSION_LOG.md)

## License

This project is released under the GNU General Public License v2.0 or later, with additional repository-level terms as noted in the project files.

---

The rest of this repository continues the full v3.0.0 design, verification, and release notes for the Dandelion OS project.
