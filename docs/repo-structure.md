# Repository structure guidance

This repository is being normalized around a single canonical v3.2.0 state.

## Active layout principles

- `build/` holds generated binaries and intermediate outputs
- `release/` holds staged release artifacts
- `archive/` holds historical releases and legacy bundles
- `docs/` contains user-facing documentation and operating guidance
- the root remains reserved for active project metadata, source, and the canonical build entrypoints

## Why this matters

The earlier Drive-sync state introduced mixed-version files and generated artifacts into the repository root. That made it hard to tell which files were live source, which were historical references, and which were generated outputs.

The fix is not to remove project ambition; it is to separate active source from archive and generated state so the repository remains trustworthy and maintainable.

## Expected source-of-truth behavior

- source files should be clearly connected to the current version
- docs should describe the same version and workflow
- historical payloads should be archived instead of mixed into operational files
- generated outputs should never be mistaken for authored source
