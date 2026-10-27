# Security Policy (`SECURITY.md`)

## Supported Versions

| Version | Supported | Kernel / Architecture |
| :--- | :---: | :--- |
| **`v3.0.0`** | Yes | Sovereign Argon Frontier & Embedded AI (10-Bite Grimm + LayerFS + Stigmergic Mesh + Nested Chromatic Chromatography) |
| `< v3.0.0` | Legacy | Superseded historical milestones |

## Core Security Invariants

1. **Zero Dynamic Heap Allocation (`malloc` = 0)**: Deterministic static structures and `577-byte` `.tnpt` `mmap` tables eliminate heap corruption and use-after-free (UAF) bugs.
2. **Pre-Auth SipHash-2-4 & CRC-16/CCITT-FALSE**: Authenticates utterances and 32-byte Stigmergic BLE gossip frames.
3. **Fail-Closed `Byte 253` Hardware Latch**: Out-of-bounds Fowler bytes (`243..255`) never silently coerce to zero; `253` triggers an immediate fail-closed drop.
4. **Hardened Unix Domain Socket (UDS) Namespaces**: `validate_uds_socket_path()` enforces `/var/run/dandelion_*` and `/tmp/dandelion_*` prefixes and blocks `..` traversal.
5. **Least-Privilege CI/CD & Dependabot v2**: `.github/workflows/ci.yml` enforces `permissions: contents: read` (CWE-275), monitored by `.github/dependabot.yml`.

## Reporting a Vulnerability

Please report suspected vulnerabilities via [GitHub Security Advisories](https://github.com/leadpiperl1/dandelion-os/security/advisories/new).
