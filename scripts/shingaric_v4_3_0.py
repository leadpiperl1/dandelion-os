#!/usr/bin/env python3
"""
shingaric_v4_3_0.py — The Shingari v4.3.0 Triadic Compiler & Build Bridge
"""
import argparse
import dataclasses
import pathlib
import re
import struct
import sys
import zlib
from typing import Dict, List, Optional, Tuple

VERSION = "4.3.0"

@dataclasses.dataclass(frozen=True)
class ShingariRootSpec:
    index: int
    code: str
    chord: Tuple[int, int, int]
    haptic: str
    human_intent: str
    os_syscall: str
    ai_kernel: str
    inode_p: int
    inode_q: int

SHINGARI_13_ROOTS: Dict[str, ShingariRootSpec] = {
    "01.ssf": ShingariRootSpec(1, "01.ssf", (-1, -1, -1), "...", "Scan storage & voxel roots", "sys_mount_squashfs", "Index .tnpt 577B table", 1505, 1282),
    "02.hrt": ShingariRootSpec(2, "02.hrt", (-1, -1, +1), "..-", "Check pilot pulse & thermal", "sys_read_temp_41c", "Clamp V=2/3 keyhole <=41C", 1505, 1781),
    "03.rtz": ShingariRootSpec(3, "03.rtz", (-1,  0, -1), ".0.", "Map 3D garden voxel lattice", "sys_mmap_l1_voxel", "Project 13x13x13 lattice", 1505, 1328),
    "04.sft": ShingariRootSpec(4, "04.sft", (-1, +1, -1), ".-.", "Sift rough signal & tremor", "sys_read_fifo_pipe", "Split Mori-Zwanzig P+Q=I", 1505, 967),
    "05.div": ShingariRootSpec(5, "05.div", ( 0, -1, +1), "0.-", "Sense pitch & muscle tremor", "sys_clock_monotonic", "Trim F-16 RSS deadband", 1505, 712),
    "06.orb": ShingariRootSpec(6, "06.orb", ( 0, +1, -1), "0-.", "Witness lived scar & shear", "sys_log_stress_raw", "Weld Kintsugi Gold (251)", 1603, 57),
    "07.tot": ShingariRootSpec(7, "07.tot", (-1,  0, +1), ".0-", "Hold 1/3 Open-Circle Sling", "treaty_greek_hub_0", "Unweight 2/3 Torso (353N)", 1505, 1410),
    "08.ohm": ShingariRootSpec(8, "08.ohm", (+1, -1,  0), "-.0", "Speak & compile 3-tap chord", "sys_emit_fowler_9b", "Eval 42-trit SWAR POPCNT", 1505, 1621),
    "09.unt": ShingariRootSpec(9, "09.unt", (+1,  0, +1), "-0-", "Read & feel bedrock scroll", "sys_zero_copy_dmabuf", "Mint tactile Pearl vX.Y.Z", 1505, 1945),
    "10.bec": ShingariRootSpec(10, "10.bec", (+1,  0, -1), "-0.", "Hail profplum & Nurse bus", "sys_sock_seqpacket", "Route FCC mesh p_c=0.1202", 1505, 2001),
    "11.int": ShingariRootSpec(11, "11.int", (+1, +1, -1), "--.", "Recall 243-slot memory pin", "sys_seek_243_vault", "Lock Avogadro N_A mod 3^13", 1505, 1729),
    "12.ves": ShingariRootSpec(12, "12.ves", (+1, -1, +1), "-.-", "Audit 18.50W & noise floor", "sys_cgroup_18_50w", "Screen Anti-Elias Byte 254", 1505, 1134),
    "13.wst": ShingariRootSpec(13, "13.wst", (+1, +1, +1), "---", "Seal Haiku seed to pillow", "sys_fsync_switchblade", "Step Chair44 Collar C_44", 1505, 2059),
}

@dataclasses.dataclass
class ShingariDeedIR:
    name: str
    root_code: str
    chord: Tuple[int, int, int]
    priority: str
    human_intent: str
    os_syscall: str
    os_params: Dict[str, float]
    ai_kernel: str
    ai_params: Dict[str, float]
    haptic_rx: Tuple[int, int, int]
    haptic_msg: str
    kintsugi_weld: int = 0

    def pack_64b(self) -> bytes:
        root_spec = SHINGARI_13_ROOTS[self.root_code]
        unload_mn = int(round(self.os_params.get("unload_n", 353.16) * 1000))
        temp_mc = int(round(self.os_params.get("temp_c", 41.0) * 1000))
        power_mw = int(round(self.os_params.get("power_w", 18.50) * 1000))
        mtp_ns = int(round(self.ai_params.get("mtp_us", 35.90) * 1000))
        euler_x100 = int(round(self.os_params.get("euler_safety", 4.81) * 100))

        fowler_9b = bytes([0x1B, 0x36, 0x6C, 0x00, 0xD8, 0xB1, 0x63, 0xC6, root_spec.index])
        prio_code = 98 if "98" in self.priority or "SOS" in self.priority.upper() else 1
        label_13b = self.name.encode("ascii", errors="ignore")[:13].ljust(13, b"\x00")

        header_60b = struct.pack(
            "<4sBbbbBHHIIIII9sbbbH13s",
            b"SHG4", root_spec.index,
            self.chord[0], self.chord[1], self.chord[2],
            self.kintsugi_weld, root_spec.inode_p, root_spec.inode_q,
            unload_mn, temp_mc, power_mw, mtp_ns, euler_x100,
            fowler_9b, self.haptic_rx[0], self.haptic_rx[1], self.haptic_rx[2],
            prio_code, label_13b,
        )
        crc = zlib.crc32(header_60b) & 0xFFFFFFFF
        return header_60b + struct.pack("<I", crc)

class ShingariCompilerV430:
    def __init__(self) -> None:
        self.ModuleName: str = "ShingariDefaultModule"
        self.Deeds: List[ShingariDeedIR] = []

    def _parse_chord(self, raw_chord_str: str, root_code: str) -> Tuple[Tuple[int, int, int], int]:
        expected = SHINGARI_13_ROOTS[root_code].chord
        if "!sos" in raw_chord_str.lower():
            return SHINGARI_13_ROOTS["10.bec"].chord, 251
        nums = re.findall(r"[+-]?\d+", raw_chord_str)
        if len(nums) != 3:
            return expected, 251
        clamped = [max(-1, min(1, int(n))) for n in nums]
        return (clamped[0], clamped[1], clamped[2]), 0

    def _parse_kv_params(self, text: str) -> Tuple[Dict[str, float], int]:
        params: Dict[str, float] = {}
        kintsugi = 0
        for key, val_str in re.findall(r"([a-zA-Z0-9_]+)\s*(?:=|<|<=)\s*([0-9]+(?:\.[0-9]+)?)", text):
            val = float(val_str)
            if key in ("temp_c", "thermal_max") and val > 41.0:
                val = 41.0; kintsugi = 251
            elif key in ("power_w", "power_max") and val > 18.50:
                val = 18.50; kintsugi = 251
            params[key] = val
        return params, kintsugi

    def compile(self, source: str) -> List[ShingariDeedIR]:
        lines = source.splitlines()
        current_deed: Optional[Dict] = None
        for raw_line in lines:
            line = raw_line.split("#", 1)[0].strip()
            if not line: continue
            if line.startswith("module "):
                parts = line.split()
                if len(parts) >= 2: self.ModuleName = parts[1]
                continue
            if line.startswith("deed "):
                name_match = re.match(r"deed\s+([a-zA-Z0-9_]+)", line)
                deed_name = name_match.group(1) if name_match else "deed"
                root_match = re.search(r"@root\(([^)]+)\)", line)
                raw_root = root_match.group(1).strip() if root_match else "07.tot"
                chord_match = re.search(r"@chord(\[[^\]]+\])", line)
                raw_chord = chord_match.group(1) if chord_match else str(list(SHINGARI_13_ROOTS[raw_root].chord))
                chord, chord_kintsugi = self._parse_chord(raw_chord, raw_root)
                prio_match = re.search(r"@priority\(([^)]+)\)", line)
                priority = prio_match.group(1) if prio_match else "SCHED_OTHER"
                current_deed = {
                    "name": deed_name, "root_code": raw_root, "chord": chord,
                    "priority": priority, "human_intent": SHINGARI_13_ROOTS[raw_root].human_intent,
                    "os_syscall": SHINGARI_13_ROOTS[raw_root].os_syscall, "os_params": {},
                    "ai_kernel": SHINGARI_13_ROOTS[raw_root].ai_kernel, "ai_params": {},
                    "haptic_rx": chord, "haptic_msg": "Triadic lock verified.", "kintsugi_weld": chord_kintsugi,
                }
                continue
            if current_deed is not None:
                if line.startswith("-1"):
                    msg = re.search(r'"([^"]*)"', line)
                    if msg: current_deed["human_intent"] = msg.group(1)
                elif line.startswith("0"):
                    fn = re.search(r":\s*([a-zA-Z0-9_]+)", line)
                    if fn: current_deed["os_syscall"] = fn.group(1)
                    p, k = self._parse_kv_params(line)
                    current_deed["os_params"].update(p)
                    if k: current_deed["kintsugi_weld"] = 251
                elif line.startswith("+1"):
                    fn = re.search(r":\s*([a-zA-Z0-9_]+)", line)
                    if fn: current_deed["ai_kernel"] = fn.group(1)
                    p, k = self._parse_kv_params(line)
                    current_deed["ai_params"].update(p)
                    if k: current_deed["kintsugi_weld"] = 251
                elif line.startswith("~<"):
                    msg = re.search(r'"([^"]*)"', line)
                    if msg: current_deed["haptic_msg"] = msg.group(1)
                elif line.startswith("seal") or line.startswith("end"):
                    self.Deeds.append(ShingariDeedIR(**current_deed))
                    current_deed = None
        return self.Deeds

    def emit_rust_no_std(self) -> str:
        lines = [
            "#[repr(C, align(64))]",
            "#[derive(Copy, Clone)]",
            "pub struct ShingariPacket64 {",
            "    pub data: [u8; 64],",
            "}",
            "const _: () = assert!(core::mem::size_of::<ShingariPacket64>() == 64);",
        ]
        for deed in self.Deeds:
            fn_name = f"shingari_deed_{deed.name}"
            lines.extend([
                "#[no_mangle]",
                f"pub extern \"C\" fn {fn_name}(pkt: *mut ShingariPacket64) -> i32 {{",
                "    if pkt.is_null() { return -1; }",
                "    let p = unsafe { &mut *pkt };",
                "    let mut temp_mc = u32::from_le_bytes([p.data[22], p.data[23], p.data[24], p.data[25]]);",
                "    let mut power_mw = u32::from_le_bytes([p.data[26], p.data[27], p.data[28], p.data[29]]);",
                "    let mut weld = p.data[8];",
                "    if temp_mc > 41_000 {",
                "        temp_mc = 41_000;",
                "        weld = 251;",
                "        p.data[22..26].copy_from_slice(&temp_mc.to_le_bytes());",
                "    }",
                "    if power_mw > 18_500 {",
                "        power_mw = 18_500;",
                "        weld = 251;",
                "        p.data[26..30].copy_from_slice(&power_mw.to_le_bytes());",
                "    }",
                "    p.data[8] = weld;",
                "    weld as i32",
                "}",
            ])
        return "\n".join(lines)

    def emit_julia_dispatch(self, lib_path: str = "build/libdandelion_os_v3.1.0.so") -> str:
        lines = [
            f"module {self.ModuleName}Pilot",
            f"const LIB_DANDELION_OS = abspath(\"{lib_path}\")",
            "abstract type AbstractAsyncIntent end",
            "abstract type AbstractOsSyscall end",
            "abstract type AbstractAiSwarKernel end",
            "mutable struct ShingariPacket64 bytes::NTuple{64, UInt8} end",
        ]
        for deed in self.Deeds:
            camel = "".join(w.capitalize() for w in deed.name.split("_"))
            pkt_hex = ", ".join(f"0x{b:02x}" for b in deed.pack_64b())
            fn_name = f"shingari_deed_{deed.name}"
            lines.extend([
                f"struct Intent_{camel} <: AbstractAsyncIntent end",
                f"struct Syscall_{deed.os_syscall} <: AbstractOsSyscall end",
                f"struct Kernel_{deed.ai_kernel} <: AbstractAiSwarKernel end",
                f"default_packet(::Intent_{camel}) = ShingariPacket64(({pkt_hex},))",
                f"@inline function dispatch_deed!(::Intent_{camel}, ::Syscall_{deed.os_syscall}, ::Kernel_{deed.ai_kernel}, pkt::Ref{{ShingariPacket64}})::Int32",
                f"    return @ccall LIB_DANDELION_OS.{fn_name}(pkt::Ptr{{ShingariPacket64}})::Int32",
                "end",
            ])
        lines.append("end")
        return "\n".join(lines)

def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("inputs", nargs="*")
    parser.add_argument("--emit-rust", type=pathlib.Path)
    parser.add_argument("--emit-julia", type=pathlib.Path)
    parser.add_argument("--emit-bin", type=pathlib.Path)
    parser.add_argument("--lib-path", default="build/libdandelion_os_v3.1.0.so")
    parser.add_argument("--cargo-warnings", action="store_true")
    args = parser.parse_args()

    source = "\n".join(pathlib.Path(p).read_text(encoding="utf-8") for p in args.inputs)
    compiler = ShingariCompilerV430()
    deeds = compiler.compile(source)

    if args.emit_rust:
        args.emit_rust.parent.mkdir(parents=True, exist_ok=True)
        args.emit_rust.write_text(compiler.emit_rust_no_std(), encoding="utf-8")
    if args.emit_julia:
        args.emit_julia.parent.mkdir(parents=True, exist_ok=True)
        args.emit_julia.write_text(compiler.emit_julia_dispatch(args.lib_path), encoding="utf-8")
    if args.emit_bin:
        args.emit_bin.parent.mkdir(parents=True, exist_ok=True)
        args.emit_bin.write_bytes(b"".join(d.pack_64b() for d in deeds))

if __name__ == "__main__":
    main()
