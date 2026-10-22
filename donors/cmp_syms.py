#!/usr/bin/env python3
"""Compare the exported symbols of two .ko files.

    python3 cmp_syms.py stock.ko built.ko

Prints the symbols only the stock blob has, only the built module
has, and the ones both have. The stock side counts local symbols
too, because the deblob verifier compares the whole surface.
"""
import subprocess
import sys

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")

SKIP_PREFIX = ("__cfi_check", "__cfi_slowpath", "__typeid__", "__kstrtab",
                "__ksymtab", "__UNIQUE_ID", "____versions", "_note_",
                "__this_module", "__init", "__exit", "__func__",
                "__stack_chk", "__dso_handle", "__bss_start", "_etext",
                "_edata", "__end_rodata", "__param", "__mod_", "cleanup_module",
                "init_module", "__exit_funcs", "__init_funcs")


def syms(path):
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
    a = syms(sys.argv[1])
    b = syms(sys.argv[2])
    only_a = sorted(set(a) - set(b))
    only_b = sorted(set(b) - set(a))
    both = sorted(set(a) & set(b))
    print(f"stock: {len(a)} symbols, built: {len(b)} symbols, "
          f"common: {len(both)}")
    print(f"\nONLY IN STOCK ({len(only_a)}):")
    for n in only_a:
        print(f"  {a[n]} {n}")
    print(f"\nONLY IN BUILT ({len(only_b)}):")
    for n in only_b:
        print(f"  {b[n]} {n}")


if __name__ == "__main__":
    main()
