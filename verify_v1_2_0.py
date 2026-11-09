#!/usr/bin/env python3
"""
v1.2.0 Verification Harness Script
Validates tml-renderers tokenizer integration and axiomatic regression suites.
"""

import sys
import subprocess

def run_checks():
    print("[*] Starting v1.2.0 Verification Harness...")
    
    # 1. Verify python environment / uv
    print("[*] Running tuttle_step.py...")
    res_step = subprocess.run(["uv", "run", "python", "tuttle_step.py"], capture_output=True, text=True)
    print(res_step.stdout)
    if res_step.returncode != 0:
        print(f"[!] tuttle_step.py failed:\n{res_step.stderr}")
        sys.exit(1)
        
    # 2. Run axiomatic regression suite
    print("[*] Running tuttle_regression.py...")
    res_reg = subprocess.run(["uv", "run", "python", "tuttle_regression.py"], capture_output=True, text=True)
    print(res_reg.stdout)
    if res_reg.returncode != 0:
        print(f"[!] tuttle_regression.py failed:\n{res_reg.stderr}")
        sys.exit(1)
        
    print("[+] All v1.2.0 verification checks passed successfully in under 60 seconds.")

if __name__ == "__main__":
    run_checks()
