# Repository Hygiene & Lessons Learned

This project is a good example of what happens when raw creative material is moved into a disciplined engineering workflow: the repo becomes more trustworthy, easier to maintain, and easier for others to contribute to.

## The core idea

A repo looks its best when it is:

- honest
- organized
- reproducible
- reviewable
- versioned carefully
- protected from accidental destructive actions

The goal is not to flatten the personality of the project. The goal is to make the project easier to trust.

## What we fixed

We corrected a few common repo-quality issues:

- the root became too cluttered
- versions were colliding with one another
- build commands drifted from the real workflow
- docs weren’t always aligned with actual commands
- generated outputs and historical artifacts were mixed with active source
- release and sync decisions needed stronger human oversight

These are all normal growth problems when a project starts as a raw collection of files and then becomes a real repository.

## Best practices that matter most

### 1. Keep the root clean

The repository root should be reserved for essentials, not historical clutter.

Good root-level files include:

- `README.md`
- `Makefile`
- `requirements.txt`
- project metadata files
- a small set of top-level config files

Everything else should be moved into proper directories.

### 2. Separate source, build output, and history

Use a clear structure:

- `src/` for active source
- `include/` for headers
- `tests/` for validation
- `scripts/` for tooling
- `build/` for generated output
- `releases/<version>/` for frozen release bundles
- `archive/` for historical snapshots

This prevents old versions from overwriting or shadowing current work.

### 3. Make versions immutable

Never overwrite a previous release.

Each version should have its own folder and its own logical identity.

This makes a repo easier to review, easier to debug, and much more trustworthy over time.

### 4. Keep docs and code aligned

The README, build files, and docs must all agree on the project workflow.

The rule is simple:

- if a command is in the docs, it should exist
- if it exists, it should be documented
- if a workflow changes, docs must change with it

### 5. Protect documentation by default

Documentation files are not disposable.

Files such as:

- `README.md`
- `docs/`
- `SECURITY.md`
- `CONTRIBUTING.md`
- `AGENTS.md`
- `LICENSE`

should be protected against accidental pruning.

### 6. Use manifest-driven import and staging workflows

When moving raw export files into Git, do not delete unknown files blindly.

A safe workflow is:

- classify files as `keep`, `archive`, `delete`, `review`, or `protect`
- do a dry run before destructive actions
- require human review before deleting or rewriting important files
- preserve provenance of what was staged

### 7. Treat automation as stateful, not magical

Automated sync or release jobs should not run on a timer alone.

A safer pattern is:

- explicit state transitions
- approval tokens
- idempotent execution
- atomic write/rename operations
- audit logging of approval and completion

This prevents race conditions and partial execution.

### 8. Use GitHub protections and PR discipline

A healthy repo usually includes:

- protected `main` branch
- pull request requirement
- CI checks required before merge
- review requirements for important changes

This helps keep maintenance sane as the repo grows.

## Google Drive to GitHub best practices

Google Drive is a staging area, not the final home of a proper repo.

The best workflow is:

1. download the material into a temporary folder
2. remove duplicates and junk
3. sort files into appropriate categories
4. rename important files consistently
5. move old versions into archive or release folders
6. create the repo structure before committing
7. commit in meaningful chunks
8. push to GitHub
9. tag releases only after validation

Do not commit raw Drive exports as-is unless you are intentionally preserving a specific historical state.

## The real lesson

The best repos are not the ones with the most complexity.

They are the ones with:

- clean boundaries
- reproducible workflows
- stable versioning
- understandable structure
- explicit approvals
- auditable release behavior
- clear contributor guidance

That is the difference between a raw collection of files and a serious engineering project.

This project now reflects that principle: its story remains expressive, but its operational reality is clean, honest, and maintainable.

That is the real standard.
