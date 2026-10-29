# Contributing to Dandelion OS

Thank you for helping build and improve Dandelion OS. We value clear engineering, reproducible builds, and respectful collaboration.

## How to contribute

1. Fork the repository.
2. Create a feature branch.
3. Make focused changes.
4. Run the appropriate verification commands.
5. Open a pull request with a clear summary and test evidence.

## Local setup

```bash
git clone https://github.com/leadpiperl1/dandelion-os.git
cd dandelion-os
python3 -m pip install -r requirements.txt
make build
make check
```

## Project standards

- Prefer small, reviewable changes.
- Keep build and verification commands reproducible.
- Do not add hidden runtime requirements or undocumented system dependencies.
- Preserve the repo’s versioning and build conventions.
- If you modify tooling or CI, validate the same workflow locally first.

## Quality gates

Before opening a pull request, make sure the following pass locally:

```bash
make clean
make build
make check
```

For compiler matrix checks, use the GitHub Actions workflow as guidance and validate both `gcc` and `clang` when feasible.

## Pull requests

Please include:

- a clear summary of the problem
- what changed
- why it was needed
- any validation or testing evidence
- notes about platform or dependency impacts

## Code review expectations

- Keep the scope narrow and explain the rationale.
- Respect the work already in the repo.
- Preserve project conventions and readability.
- Update documentation when behavior or usage changes.

## Reporting problems

Use the issue tracker for bug reports or feature ideas. For security issues, follow the private reporting instructions in [SECURITY.md](SECURITY.md).

## Community norms

Be respectful, concise, and constructive. This repo is a public engineering project with a strong emphasis on reproducibility, trust, and calm collaboration.
