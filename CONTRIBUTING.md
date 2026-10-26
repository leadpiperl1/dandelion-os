# Contributing to Dandelion OS (`v2.8.0`)

Thank you for building with **Dandelion OS** (*Die Gedanken sind frei · Mutualism Over Replacement*). We welcome contributions from systems engineers, kernel hackers, linguists, and independent researchers.

## 1. The Triadic Engineering Contract (`-1 Blue`, `0 Green`, `+1 Red`)

Every module in Dandelion OS maps to our three-tier F-16 Digital Fly-By-Wire architecture:

* **`-1 Blue` (Human Pilot / Subject):** Preserves human sovereignty, clear CLI/HUD observability, and zero dark-pattern telemetry.
* **`0 Green` (OS Mediator / Object):** Enforces `PREEMPT_RT` + `sched_ext` determinism (`< 1.0 µs` jitter), LayerFS content-addressed storage, and fail-closed boundary checks (`Code 253` drop, `Code 251` Kintsugi Gold defect-pinning).
* **`+1 Red` (AI Co-Pilot / Action):** Executes multiplication-free 1.58-bit balanced ternary (`VBST`) inference, TokenGate routing, and `larchanka/skills` automated verification.

## 2. Pre-Submission Verification Checklist (`larchanka/code-review` 5 Pillars)

Before opening a Pull Request against `main`:

1. **Zero Compiler Warnings (`-Werror`):** Ensure your C99 changes compile cleanly via `make build` (`-std=c99 -O3 -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wconversion -Wundef -Werror`).
2. **60-Second Verification Gate:** Run `make check` (`python3 verify_dandelion_os_v2.8.0.py --seed 42`) and confirm all 15 linguistic/trapdoor vectors, ternary logic gates, jitter bounds (`< 1.0 µs`), and SemVer checks pass in under 60 seconds.
3. **Strict 3-Digit SemVer (`vX.Y.Z`):** Update `VERSION_LOG.md` and never commit duplicate collision filenames (`(1)`, `(2)`).
