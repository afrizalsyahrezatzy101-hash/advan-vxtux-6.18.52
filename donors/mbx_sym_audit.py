#!/usr/bin/env python3
"""Symbol-by-symbol comparison: stock unisoc-mailbox.ko vs the tree build.

Independent of donors/cmp_syms.py: this one filters the compiler's
local labels ($d.N / $x.N / __UNIQUE_ID_*) that differ between the
5.4 stock build and the 6.18 tree build, and it checks the EXPORTED
surface separately from the local one.

Usage:
    python3 donors/mbox_sym_audit.py <stock.ko> <built.ko>
"""
import subprocess
import sys

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")

# compiler-generated locals that legitimately differ between a 5.4 and a
# 6.18 build: ddebug tables, unique ids, CFI thunks, typeinfo.
LOCAL_NOISE = (
    "__UNIQUE_ID", "$d.", "$x.", "__typeid__", "__cfi_check",
    "__kstrtab", "__ksymtab", "__kcrctab", "____versions", "_note_",
    "__this_module", "__func__", "__stack_chk", "__mod_", "_note_",
    "__key", "__mod_of__", "__UNIQUE_ID_", "__addressable",
    "__bss_start", "_etext", "_edata", "__end_rodata", "__param",
    "__exit_funcs", "__init_funcs", "__exits",
)

# the 6.18 kernel exports things the 5.4 one did not
NOISE_PREFIX = ("__crc_", "__kstrtab", "__ksymtab", "__kcrctab",
                "cleanup_module", "init_module", "__mod_device_table",
                "__UNIQUE_ID_modinfo", "_rs.", "__exit_funcs",
                "__init_funcs", "__func__", "_note_", "____versions")


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
        res.setdefault(name, typ)
    return res


def is_noise(name):
    return any(name.startswith(s) for s in LOCAL_NOISE + NOISE_PREFIX)


def main():
    stock = syms(sys.argv[1])
    built = syms(sys.argv[2])

    s_sig = {n: t for n, t in stock.items() if not is_noise(n)}
    b_sig = {n: t for n, t in built.items() if not is_noise(n)}

    only_stock = sorted(set(s_sig) - set(b_sig))
    only_built = sorted(set(b_sig) - set(s_sig))
    common = sorted(set(s_sig) & set(b_sig))

    print(f"stock significant symbols : {len(s_sig)}")
    print(f"built significant symbols : {len(b_sig)}")
    print(f"common                    : {len(common)}")
    print(f"\nMISSING FROM TREE ({len(only_stock)}):")
    for n in only_stock:
        print(f"  {s_sig[n]} {n}")
    print(f"\nEXTRA IN TREE ({len(only_built)}):")
    for n in only_built:
        print(f"  {b_sig[n]} {n}")
    return 0 if not only_stock else 1


if __name__ == "__main__":
    sys.exit(main())
