# Dandelion OS release checklist

Use this before tagging and publishing a new version.

## Required checks

- [ ] `make clean`
- [ ] `make build`
- [ ] `make check`
- [ ] `make check-strict` (if the change touches verification or release-critical behavior)
- [ ] `make verify` (if you want the strict alias step captured in the record)
- [ ] Confirm generated artifacts land in the intended versioned location
- [ ] Confirm no active work is being overwritten by old release files

## Release hygiene

- [ ] Version number is updated consistently across project references
- [ ] The release directory is created under `releases/<version>/`
- [ ] Historical versions remain preserved and immutable
- [ ] Root-level files are not being used as versioned outputs
- [ ] The build output path is version-scoped rather than reused across versions

## Documentation

- [ ] README reflects the current version status
- [ ] `docs/BUILDING.md` remains accurate
- [ ] `docs/DEVELOPMENT.md` remains accurate
- [ ] `docs/VERSIONING.md` matches the actual release policy
- [ ] `CONTRIBUTING.md` still matches the current workflow

## Final release step

- [ ] git tag `v<version>` is created
- [ ] the release bundle is staged under `releases/<version>/`
- [ ] the release notes are clear and reproducible
- [ ] the repo is ready for contributors to inspect or fork
