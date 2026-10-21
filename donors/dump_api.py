#!/usr/bin/env python3
"""Dump the API surface of the stock blobs that have no source in the tree.

The verifier compares the stock blob's symbols - locals included -
against the symbols of the replacement .ko, so the target for each
port is the full list, not just the exported ones.
"""
import subprocess
import sys

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")

MODULES = [
    "sprd-sipc-virt-bus",
    "sprd-vsp-pw-domain",
    "sprd-dmaengine-pcm",
    "usb_f_vser",
    "audio_sipc",
    "mmdvfs",
]

SKIP = ("__cfi_check", "__cfi_slowpath", "__typeid__", "__kstrtab",
        "__ksymtab", "__UNIQUE_ID", "____versions", "_note_",
        "__this_module", "__init", "__exit", "__func__", "__stack_chk")


def syms(path, kinds):
    out = subprocess.run([NM, path], capture_output=True, text=True).stdout
    res = []
    for line in out.splitlines():
        p = line.split()
        if len(p) < 3:
            continue
        addr, typ, name = p[0], p[1], p[2]
        if typ not in kinds:
            continue
        if any(s in name for s in SKIP):
            continue
        res.append((typ, name, addr))
    return res


def main():
    base = "E:/vxtux_build_repo/donors/stock_vendor_modules"
    for m in MODULES:
        path = f"{base}/{m}.ko"
        try:
            allsyms = syms(path, "TtDdBbRrWwVv")
        except Exception as e:
            print(f"{m}: {e}")
            continue
        glob = [s for s in allsyms if s[1] in
                [x[1] for x in syms(path, "TDBR")]]
        print(f"=== {m}.ko : {len(allsyms)} symbols "
              f"({len(glob)} global) ===")
        for typ, name, addr in sorted(allsyms, key=lambda s: s[1]):
            tag = ""
            if typ in "Tt":
                tag = "code"
            elif typ in "DdBb":
                tag = "data"
            elif typ in "Rr":
                tag = "rodata"
            print(f"  {typ} {name:45s} {tag}")
        print()


if __name__ == "__main__":
    main()
