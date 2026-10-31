# Dandelion OS contribution guide

Thanks for helping improve Dandelion OS. This project works best when contributions are small, clear, and reproducible.

## Contributor expectations

- Keep changes focused and reviewable.
- Prefer explicit, simple solutions over hidden complexity.
- Do not add undocumented environment requirements.
- Keep build and verification commands reproducible from a fresh checkout.
- Document behavior changes when they affect how contributors use the project.

## Local setup

```bash
git clone https://github.com/<your-user>/dandelion-os.git
cd dandelion-os
python3 -m venv .venv
. .venv/bin/activate
python -m pip install --upgrade pip
python -m pip install -r requirements.txt
```

## Build and validation workflow

Run the standard checks before opening a PR:

```bash
make clean
make build
make check
```

If your change is stricter or release-related, also run:

```bash
make check-strict
```

## Branch workflow

- Create a feature branch from `main`
- Keep PRs small and targeted
- Describe the problem and the fix clearly
- Include validation evidence in the PR description

## Pull request checklist

Before opening a PR, confirm:

- the branch is up to date with `main`
- the repo builds locally
- verification passes locally
- docs are updated if commands or workflows changed
- the change does not introduce hidden dependencies
- the PR description explains the motivation and the validation steps

## Coding expectations

- Prefer readable C99 code and clear Python verification logic
- Keep the developer workflow understandable to newcomers
- Avoid surprising changes in versioning or release behavior
- Preserve the repo’s maintainability as you work

## Reporting issues

Use the issue tracker for bugs, feature ideas, or cleanup items. If you discover a security issue, use the private path described in `SECURITY.md`.

## Be respectful

The repo values calm collaboration, reproducibility, and trust. Good engineering beats cleverness when the result is understandable and reliable.
