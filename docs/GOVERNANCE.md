# Dandelion OS repository governance

This repository is designed to remain transparent, reproducible, and safe to evolve. The operational principles below govern how files are imported, staged, versioned, and released.

## 1. Drive-to-Git import policy

Raw cloud exports, including Google Drive exports, must be treated as staging material, not as final repository content.

### Manifest-driven staging

Every import pass must go through a manifest-driven classification step.

- `keep`: approved source or documentation files
- `archive`: historical or legacy content that should remain but not be treated as active source
- `delete`: known junk, temp files, duplicates, and OS-generated artifacts
- `review`: ambiguous files that require human inspection
- `protect`: documentation or metadata that must not be pruned without approval

Unknown files must never be silently removed.

### Protected documentation rule

The following files are protected by default:

- `docs/`
- `*.md` files
- `LICENSE`
- `SECURITY.md`
- `CONTRIBUTING.md`
- `AGENTS.md`
- `CHANGELOG.md`
- release manifest files

These files cannot be pruned or rewritten without explicit human sign-off.

### Zero root pollution

The repository root is reserved for essentials only.

Allowed root-level files include:

- `README.md`
- `Makefile`
- `requirements.txt`
- `.gitignore`
- `.editorconfig`
- project metadata files

Active source belongs in:

- `src/`
- `include/`
- `tests/`
- `scripts/`

Generated and historical material belongs in:

- `build/`
- `releases/<version>/`
- `archive/`

## 2. Version immutability & release staging

Release artifacts are immutable. Once a version has been tagged, the data within that release directory is treated as frozen.

### Strict version isolation

Every version must live in its own directory.

```text
releases/3.0.0/
releases/3.2.0/
```

Never overwrite a previous release directory. Never reuse a root-level filename for multiple versions.

### Immutable tagging

Releases are anchored via Git tags.

```bash
git tag v3.0.0
```

A tagged release must remain reproducible. Source and build output are preserved so that anyone can inspect the exact version later.

### Release staging policy

Before a version is released:

1. Build the version into `build/<version>/`
2. Validate the output
3. Stage the release bundle into `releases/<version>/`
4. Tag the repository
5. Preserve older releases exactly as they were

This prevents “new version overwriting old version” problems.

## 3. Human approval gate state machine

Automated synchronization and release jobs are controlled by an explicit, idempotent state machine.

```text
[ STAGED_FOR_MORNING_BRIEF ]
        |
        v
   (Human Review)
        |
        v
[ APPROVED_FOR_RELEASE ]
        |
        v
  (Atomic Execution)
        |
        v
[ SYNC_COMPLETE ]
```

### State transition rules

- Time elapsed is necessary but not sufficient.
- Execution requires an explicit approval token such as `human_approved: true`.
- The job must check the current state before acting.
- Re-running a sync or release payload must not create duplicate side effects.

### Idempotent execution

Every automated sync or release action must be idempotent:

- safe to retry
- safe to re-run
- safe to replay
- no duplicate side effects

### Atomic execution

State transitions should happen atomically.

Recommended pattern:

- write a temporary state file
- validate the state transition
- rename the file into place
- record the approval and execution metadata

This prevents race conditions and partial execution.

## 4. Dry-run and review requirements

All destructive import or prune actions must support:

- dry-run mode
- log output of the planned changes
- a final human review before deletion
- rollback by rehydrating archived content if needed

A destructive workflow without a dry-run is not acceptable for imported Drive material.

## 5. Provenance and auditability

Every release or automation action should capture:

- who approved it
- when it was approved
- what was staged
- what was released
- what git tag was used
- what changed in the release directory

This keeps the project understandable and reviewable over time.

## 6. Practical rule set

- Never delete unknown files silently.
- Never overwrite a previous release.
- Never rely on elapsed time alone to authorize a release.
- Never treat the root as a dumping ground for generated files.
- Never let automation bypass human review on protected documentation.
- Preserve provenance, versioning, and operational transparency.

These rules keep Dandelion OS aligned with the ethos of clarity, immutability, and transparent engineering.
