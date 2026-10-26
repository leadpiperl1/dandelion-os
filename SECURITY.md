# Security Policy (`SECURITY.md`)

## Supported Versions

Only the latest 3-digit semantic version (`v3.0.0`) on the `main` branch is actively supported with security and real-time stability patches:

| Version | Supported | Kernel / Architecture |
| :--- | :---: | :--- |
| **`v3.0.0`** | ✅ Yes | Sovereign Argon Frontier & Embedded AI (10-Bite Grimm + LayerFS + Stigmergic Mesh) |
| `v2.8.0` | ⚠️ Legacy | 9-Bite Universal Grimm's Law & F-16 FBW Kernel |
| `< v2.8.0` | ❌ No | Superseded historical milestones |

## Core Security Invariants

Dandelion OS v3.0.0 enforces strict first-principles security boundaries at the C99 kernel and IPC levels:

1. **Zero Dynamic Heap Allocation (`malloc` = 0)**: The core C99 fly-by-wire kernel (`dandelion_os_v3.0.0.c`) uses deterministic static arenas (`8192` bytes, 64-byte cache-line aligned) to eliminate heap corruption, use-after-free (UAF), and memory exhaustion vulnerabilities.
2. **RFC 2104 SipHash-2-4 Keyed Message Authentication**: Every 32-byte Stigmergic Mesh frame (`dandelion_mesh_protocol_v3.0.0.c`) is authenticated with a 64-bit keyed SipHash-2-4 MAC (`mac_tag`) to prevent spoofing and replay across BLE and ESP-SDR air-gap relays.
3. **Fail-Closed `Byte 253` Hardware Latch**: Any unauthenticated frame, spectral parity violation, or invariant breach immediately latches `CODE_FAIL_CLOSED_253` (`253U`) and returns authority to the `-1` Blue Human Pilot channel.
4. **Hardened Unix Domain Socket (UDS) Permissions**: Local IPC sockets (`connect_dandyland.sh`) enforce directory mode `0700`, `umask 0077`, and explicit symlink rejection (`-L` guard) to prevent local privilege escalation or CWE-377 insecure temporary file attacks.
5. **Least-Privilege CI/CD Workflows**: All GitHub Actions workflows (`.github/workflows/ci.yml`) declare explicit `permissions: contents: read` at the top level and job level, monitored continuously by GitHub CodeQL and Dependabot (`.github/dependabot.yml`).

## Reporting a Vulnerability

If you discover a security vulnerability, memory safety bug, or cryptographic flaw in Dandelion OS:

1. **Use GitHub Private Vulnerability Reporting**: Please report suspected vulnerabilities via [GitHub Security Advisories](https://github.com/leadpiperl1/dandelion-os/security/advisories/new) rather than opening a public issue.
2. **Include a Reproducible Proof-of-Concept**: Provide the exact command, input frame, or C99 test case so our `< 60s` verification harness (`verify_dandelion_os_v3.0.0.py`) can reproduce and lock a regression test against the flaw.
3. **Response SLA**: We acknowledge security reports within **48 hours** and ship verified patches with a 3-digit semantic version increment (`vX.Y.Z`).
