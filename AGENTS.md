# AGENTS.md

This repository is the working tree for Dandelion OS v3.2.0. Use this file as the canonical operational guide for coding agents and contributors in this branch.

## Core principles

1. Prefer explicit invariants over hidden complexity.
2. Keep safety gates fail-closed and auditable.
3. Avoid silent coercion of constrained ternary or Fowler-encoded values.
4. Keep runtime limits centralized in the canonical source and header files.
5. Prefer deterministic verification over ad hoc tuning.
6. Preserve human sovereignty and human review as the final authority.

## Repository intent

Dandelion OS is a sovereign, AI-assisted operating model with a compact C99 runtime and verification harness. The project combines:

- a symbolically structured language parser,
- bounded ternary logic,
- fail-closed safety and validation checks,
- Python-based reproducible verification,
- shell and release tooling around the runtime.

## Canonical version

The active branch state is v3.2.0. Historical v3.0.0 files are treated as archive material unless explicitly reintroduced as part of a deliberate migration.

## Required workflow

```bash
python3 -m pip install -r requirements.txt
make build
make check
make check-strict
```

## Build and verification commands

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

## Repo hygiene rules

- Do not mix active source with historical or generated artifacts without a clear reason.
- Keep build outputs in `build/` or `release/`.
- Treat root-level versioned bundles as historical artifacts unless intentionally promoted.
- Favor single-version naming conventions and avoid stale file names in the active source tree.
- Keep docs and implementation aligned to the same version.

## Safety expectations

- Failure paths should be explicit and deterministic.
- Dangerous or ambiguous values should fail closed.
- Validation logic should be explainable and testable.
- Reproducibility matters more than cleverness.

## Operational notes

The project is intentionally verify-first and auditable. If a change does not show a clear runtime or safety rationale, ask before merging it.
