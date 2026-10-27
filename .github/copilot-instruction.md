# Copilot instructions for Dandelion OS

## Project overview
This repository contains the Dandelion OS project with embedded Dandelion AI components.

## Key directories
- `src/` — core OS/runtime logic
- `scripts/` — maintenance and setup scripts
- `tools/` — build and utility tooling
- `docs/` — architecture and contributor guidance
- `tests/` — repo tests
- `third_party/` or `vendor/` — external code, if present

## Build
Use the canonical build flow from the repo:
- `make`
- or project-specific scripts if defined in `README.md`

## Test
Use the narrowest relevant validation:
- `make test`
- `pytest`
- `python -m unittest`
- repo-specific scripts

## Before making changes
- Read the relevant README and local docs first
- Prefer the smallest change that solves the task
- Preserve compatibility unless a breaking change is explicitly requested
- Do not modify build infrastructure or deployment behavior without updating docs and validation

## Coding conventions
- C: keep code portable and avoid undefined behavior
- Python: prefer readable, idiomatic code and repo patterns
- Shell: keep scripts idempotent and predictable
- JavaScript: match existing style and dependency patterns

## Safety
- Do not edit generated files or vendor code without clear reason
- Keep changes scoped to the task
- If a change affects architecture, update docs and validation

## Preferred output
When making changes:
- explain the reason briefly
- list the files changed
- mention what validation was run
