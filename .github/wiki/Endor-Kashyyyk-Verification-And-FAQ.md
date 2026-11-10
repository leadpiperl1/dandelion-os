# Endor-Kashyyyk Verification & FAQ (Living Trunk)

Run the deterministic verification gate from `${HOME}/workspace/dandelion-os`:

```bash
make -f Makefile all
```

1. Compiles `src/argon_methylation_engine.c` under `gcc -O3 -std=c11 -Wall -Wextra -Werror -Wpedantic -Iinclude` with zero warnings.
2. Verifies all 13 telluric epochs, `[17, 97, 118]` VBST history unpacking, and 6/13 `Byte 251` Kintsugi Gold shear welds.
3. Runs `python3 tests/verify_argon_rc_methylation.py --seed 42 --out-dir telemetry` and writes `telemetry/argon_rc_methylation_manifest.json`.
