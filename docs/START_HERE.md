# Start here: Dandelion OS

Welcome. This guide is a map into the project, not a replacement for its ideas or documentation.

## The ethos comes first

[`ACCESSIBILITY.md`](../ACCESSIBILITY.md) is the statement of Dandelion OS's purpose and values. Accessibility is not a feature bolted onto the system: it is the reason the project is being built. Read it first, and treat its human-centered commitments as the lens for technical decisions, contributions, and future releases.

This guide does not rewrite, summarize away, or supersede that document. The project's existing terminology, architecture, and historical ideas remain in their original files.

## A gentle path through the repository

1. **Purpose and principles:** [`ACCESSIBILITY.md`](../ACCESSIBILITY.md)
2. **Project overview and build instructions:** [`README.md`](../README.md)
3. **Version milestones:** [`VERSION_LOG.md`](../VERSION_LOG.md)
4. **Change notes and published-release distinction:** [`CHANGELOG.md`](../CHANGELOG.md)
5. **How to contribute:** [`CONTRIBUTING.md`](../CONTRIBUTING.md)
6. **Security reporting:** [`SECURITY.md`](../SECURITY.md)

The version log describes project lineage. Git tags and GitHub Releases are separate records; a milestone in project documentation does not automatically mean a GitHub Release was published. Check the actual Releases page before describing a milestone as a published release.

## Try the portable build

From the repository root, with a C compiler and Python available:

```sh
make build
make check
```

See the README and Makefile for prerequisites and additional checks. Hardware- or kernel-specific behavior should be described as such; distinguish it from what the portable build and checks actually verify.

## Contributing with care

- Preserve the human-centered accessibility ethos in `ACCESSIBILITY.md`.
- Keep existing ideas and their historical context intact; explain changes rather than silently erasing lineage.
- Prefer clear language, small reviewable changes, and working links.
- Report what was tested and where results depend on particular hardware, kernels, or environments.
- Follow `CONTRIBUTING.md` and `SECURITY.md` for contribution and vulnerability-reporting guidance.
