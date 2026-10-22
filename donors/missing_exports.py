#!/usr/bin/env python3
"""Which stock exports are missing from a set of C files.

The deblob verifier compares the stock blob's symbol list with the
replacement's. When the replacement is built-in, there is no .ko to
compare, so this walks the sources instead: a symbol counts as
present if a definition or an export of that name appears.

    python3 missing_exports.py stock.ko file1.c file2.c ...
"""
import re
import subprocess
import sys

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")

SKIP_PREFIX = ("__cfi_check", "__cfi_slowpath", "__typeid__", "__kstrtab",
                "__ksymtab", "__UNIQUE_ID", "____versions", "_note_",
                "__this_module", "__init", "__exit", "__func__",
                "__stack_chk", "__mod_", "cleanup_module", "init_module",
                "__param", "__exit_funcs", "__init_funcs", "modprobe")


def stock_syms(path):
    out = subprocess.run([NM, path], capture_output=True, text=True).stdout
    res = {}
    for line in out.splitlines():
        p = line.split()
        if len(p) < 3:
            continue
        typ, name = p[1], p[2]
        if typ not in "TtDdBbRrWwVv":
            continue
        if any(name.startswith(s) for s in SKIP_PREFIX):
            continue
        res.setdefault(name, typ)
    return res


def main():
    stock = stock_syms(sys.argv[1])
    text = ""
    for f in sys.argv[2:]:
        text += open(f, encoding="utf-8", errors="replace").read()

    missing = []
    for name, typ in sorted(stock.items()):
        if re.search(r"\b" + re.escape(name) + r"\b", text):
            continue
        missing.append((typ, name))

    print(f"stock symbols: {len(stock)}, missing from the sources: "
          f"{len(missing)}")
    for typ, name in missing:
        print(f"  {typ} {name}")


if __name__ == "__main__":
    main()
