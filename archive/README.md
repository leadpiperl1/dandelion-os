# Archive and legacy artifacts

This directory is reserved for historical and generated artifacts that are not part of the active source tree.

The active branch target remains Dandelion OS v3.2.0. Older outputs such as v3.0.0 files are retained here only for reference and migration safety.

## Rules

- `build/` is for local generated binaries and intermediate outputs.
- `release/` is for staged release payloads.
- `archive/` is for historical bundles, legacy versioned outputs, and migration references.
- do not treat archived files as active runtime definitions.

The goal is to keep the live repository honest: active source stays in the canonical tree, while history stays archived.
