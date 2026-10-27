# Copilot instructions for Dandelion OS

## Project overview
This repository contains the Dandelion OS project with embedded Dandelion AI components.
The project is a mixed C/Python/Shell/JavaScript codebase and includes build tooling, runtime scripts, and possibly embedded or host-side components.

## Before making changes
- Read the relevant README and any module-specific docs before changing behavior.
- Prefer the smallest possible change that addresses the issue.
- Preserve existing interfaces and compatibility unless the task explicitly requires a breaking change.
- Do not change build scripts, packaging, or deployment behavior without updating documentation or validation steps.

## Build and validation
Document the exact commands used in this repo. Examples:
- Build: `make`
- Tests: `pytest`, `python -m unittest`, or repo-specific test scripts
- Lint/format: `black`, `ruff`, `clang-format`, or project-defined wrappers
- Validate before finalizing: run the narrowest relevant test/build target

If the repo uses Makefiles or custom scripts, prefer those instead of inventing ad hoc commands.

## Language-specific guidance
- C: keep code portable, avoid undefined behavior, preserve ABI compatibility, and be careful around memory ownership.
- Python: prefer clear, readable logic; favor existing repo patterns over new abstractions.
- Shell: keep scripts idempotent and POSIX-safe unless the repo explicitly requires Bash features.
- JavaScript: prefer existing style and tooling conventions; avoid adding heavy dependencies without justification.

## Architecture and file placement
- Keep OS/platform code in the appropriate runtime or platform-specific directories.
- Keep support scripts in scripts/ or tooling directories.
- Keep build logic in Makefiles or build scripts rather than embedding logic in application code.
- If a feature spans layers (kernel/runtime/tooling), update all affected files consistently.

## Safety rules
- Do not delete generated or vendor files unless asked.
- Avoid editing build artifacts or lockfiles unless the change specifically requires it.
- Do not silently change environment assumptions, install scripts, or boot-time configuration.
- When uncertain, ask for clarification instead of making a risky change.

## Preferred output
When making code changes:
- explain the reason for the change briefly
- call out validation performed
- include the exact files changed
- keep scope narrow and aligned with the task
