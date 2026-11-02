# Dandelion OS Changelog

All notable changes to this project are documented here. The project lineage is summarized from [`VERSION_LOG.md`](VERSION_LOG.md); this file is not a substitute for Git tags or GitHub Release records.

## Release records vs. project lineage

GitHub's **Releases** page shows release objects that have actually been published. It does not automatically include every version or development epoch recorded in the repository. As checked on 2026-11-02, I could verify published entries for `v2.7.2`, `v2.8.0`, and `v3.0.0`; the version log identifies `v3.5.0` as current, but I did not find a matching `v3.5.0` release entry. Keep this distinction visible rather than making up release records or rewriting history. Before publishing a missing release, verify its tag, target commit, notes, and artifacts.

The entries below describe the lineage documented by the project. They do **not** by themselves assert that each version has a corresponding GitHub Release object.

## [Unreleased]

### Added
- Public-facing quickstart and contributor onboarding materials.
- Security disclosure policy and issue templates.
- Project documentation for building, verification, and contribution flow.
- A `make help` target for discoverability.

### Changed
- Aligned the C/C++ workflow with the repository's Makefile targets.
- Clarified the build contract around `make build` and `make check`.
- Improved project documentation for public access and trust.

### Fixed
- Removed reliance on a nonexistent `./configure` entrypoint in CI.
- Resolved mismatches between documented commands and the build system.

## Current lineage

### [v3.5.0] - 2026-11-02

**Heimat-Agora — Sovereign Swarm, 1.25-Bit Sparse Kernel & Accessibility MTP Release**

- Consolidated the post-`v3.0.0` work into the current project version.
- Added the accessibility Motion-to-Photon latency architecture and the sparse ternary kernel milestone.
- Recorded as current in [`VERSION_LOG.md`](VERSION_LOG.md). A corresponding GitHub Release was not found during this cleanup.

### [v3.4.0] - 2026-11-01

- Introduced the 1.25-bit sparse ternary kernel and bounded local autonomy milestone.

### [v3.3.1] - 2026-10-31

- Added the `dandelion-restd` C99 Iceberg REST catalog and change-step jitter milestone.

### [v3.3.0] - 2026-10-30

- Added the distributed stigmergic mesh and hybrid ring node specification, including the 32-byte `DND3` frame milestone.

### [v3.2.0] - 2026-10-30

- Added the Kramhaus torque `sched_ext` governor and Byzantine-clue pruning milestones.

### [v3.1.0] - 2026-10-28

- Added the Universal Grimm's Law v3.4.0 and Shingari v3.10.0 integration milestone.

### [v3.0.1] - 2026-10-27

- Added the Bite 10 sensory accessibility and polyglot Shingari bridge milestone.

### [v3.0.0] - 2026-10-26

**Sovereign Argon Frontier & Embedded AI Release**

- Marked the major bedrock-lineage transition to the unified Argon Frontier evaluation core and White-Room C99 microkernel.
- Established the user-space verification flow and public operational documentation.
- A GitHub Release record is published for this version.

## Earlier lineage

### [v2.8.0] - 2026-10-26

**Sovereign Desktop & Embedded AI Release**

- Recorded C99 static-analysis hardening, modular automation, and the Topological Dirac operator milestone.
- A GitHub Release record is published for this version.

### [v2.7.2] - 2026-10-24

- Published the LEGO-style building manual, GNU Makefile, dual GPL-2.0-or-later / MIT license, verification harness, and Drive-to-GitHub daemon.
- A GitHub Release record is published for this version.

### [v2.7.0] - 2026-10-24

- Recorded the 9-Bite Universal Grimm's Law, fail-closed Fowler byte unpacker, and Linux `sched_ext` / `io_uring` integration milestones.

### [v2.0.1] - 2026-10-15

- Recorded the foundational F-16-inspired relaxed-static-stability control model across human, OS, and AI roles.

[Unreleased]: https://github.com/leadpiperl1/dandelion-os/compare/v3.5.0...HEAD
