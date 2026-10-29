# Dandelion OS Changelog

All notable changes to this project will be documented in this file.

The format is based on Keep a Changelog and follows the repository's vX.Y.Z release discipline.

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

## [v3.0.0] - 2026-10-29

### Added
- Dandelion OS v3.0.0 sovereign build artifacts and verification flow
- portable C99 microkernel and verification harness
- embedded AI and F-16 fly-by-wire inspired control logic model
- public release and operational documentation set

### Changed
- established a reproducible user-space verification model
- clarified the build contract around `make build`, `make check`, and `make verify`

### Security
- published a private vulnerability reporting path
