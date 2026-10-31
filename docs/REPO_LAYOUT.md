# Dandelion OS repository layout

This repository should eventually follow a clean, predictable structure so that active code, build output, and historical releases do not collide.

## Target structure

```text
dandelion-os/
├── README.md
├── Makefile
├── requirements.txt
├── .github/
├── docs/
│   ├── BUILDING.md
│   ├── DEVELOPMENT.md
│   ├── quickstart.md
│   ├── VERSIONING.md
│   └── roadmap.md
├── src/
│   └── README.md
├── include/
│   └── README.md
├── tests/
│   └── README.md
├── scripts/
│   └── README.md
├── build/
│   ├── README.md
│   ├── current/
│   └── 3.0.0/
├── releases/
│   ├── README.md
│   └── 3.0.0/
├── archive/
│   └── README.md
├── .gitignore
├── .editorconfig
└── LICENSE
```

## Why this matters

The current structure keeps too much versioned or generated material in the root, which makes preserving old versions more difficult. The new layout separates:

- source materials
- generated build data
- immutable releases
- historical archives

## Rules

- `src/` and `include/` hold current source logic
- `tests/` hold verification scripts and test helpers
- `scripts/` hold release and packaging tooling
- `build/` is disposable build output
- `releases/` is the frozen artifact area
- `archive/` preserves prior snapshots or historical bundles

## Migration note

This directory layout is the recommended future state. The current root remains backward-compatible while the project transitions.
