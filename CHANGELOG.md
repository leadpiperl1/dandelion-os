# Dandelion OS Changelog

This file separates two records that are easy to confuse:

- the project's canonical version lineage in [`VERSION_LOG.md`](VERSION_LOG.md)
- the release objects actually published on the GitHub Releases page

A version can be documented as the current milestone without yet having a GitHub Release object. The release page should only be treated as authoritative for published artifacts.

## [Unreleased]

### Added
- public-facing quickstart and contributor onboarding materials
- security disclosure policy and issue templates
- project-level docs for build, verification, and contribution flow
- `make help` target for discoverability

### Changed
- aligned the C/C++ workflow with the repository's real Makefile targets
- normalized the build contract around `make build` and `make check`
- improved project documentation for public access and trust

### Fixed
- removed reliance on a nonexistent `./configure` entrypoint in CI
- resolved the mismatch between documented commands and the actual build system

## Current canonical milestone

### [v3.5.0] - 2026-11-02

**Heimat-Agora — Sovereign Swarm, 1.25-Bit Sparse Kernel & Accessibility MTP Release**

- This is the current canonical version recorded in [`VERSION_LOG.md`](VERSION_LOG.md).
- It consolidates the post-`v3.0.0` project lineage into a single milestone.
- At the time of this cleanup, this version was not yet visible as a published GitHub Release entry.

## Published GitHub releases

### [v3.0.0] - 2026-10-26

**Sovereign Argon Frontier & Embedded AI Release**

- Major bedrock-lineage transition to the unified Argon Frontier evaluation core.
- Established the user-space verification flow and operational documentation.
- This version is represented in the GitHub Releases page.

### [v2.8.0] - 2026-10-26

**Sovereign Desktop & Embedded AI Release**

- Full C99 static-analysis hardening and modular automation milestone.
- This version is represented in the GitHub Releases page.

### [v2.7.2] - 2026-10-24

- Published the LEGO-style build manual, standardized Makefile, and verification flow.
- This version is represented in the GitHub Releases page.

## Historical project lineage

The following milestones remain part of the repository's recorded history and live in [`VERSION_LOG.md`](VERSION_LOG.md):

- `v3.4.0` — 1.25-bit sparse ternary kernel and bounded local autonomy
- `v3.3.1` — `dandelion-restd` C99 Iceberg REST catalog
- `v3.3.0` — distributed stigmergic mesh and hybrid ring node specification
- `v3.2.0` — Kramhaus torque `sched_ext` governor and red-team audit milestone
- `v3.1.0` — Universal Grimm's Law v3.4.0 and Shingari v3.10.0 integration
- `v3.0.1` — Bite 10 sensory accessibility and polyglot bridge milestone
- `v2.7.0` — 9-Bite Universal Grimm's Law milestone
- `v2.0.1` — foundational F-16-inspired fly-by-wire model

This file is intentionally explicit about the difference between project lineage and release publication. The project may have a stable internal history even when the release page is not yet fully synchronized.
