"""Add the remaining EXPORT_SYMBOLs to the file that actually DEFINES
the symbol.

The bulk pass inserted the exports into the file the plan pointed at,
but for nine symbols that file only carried an `extern` declaration;
the definition lives in a sibling .c file. This reuses the tested
C-aware scanner from apply_symbol_exports.py to find the real
definition (the one with a body or an initialiser, not a prototype)
and puts the export there.

Usage:
    python3 fix_remaining_exports.py [--apply]
"""
import importlib.util
import os
import re
import sys

BASE = os.path.dirname(os.path.abspath(__file__))
spec = importlib.util.spec_from_file_location(
    "ase", os.path.join(BASE, "apply_symbol_exports.py"))
ase = importlib.util.module_from_spec(spec)
spec.loader.exec_module(ase)

TREE = ase.TREE
APPLY = "--apply" in sys.argv

JOBS = {
    "ion_tipc": "drivers/dma-buf/sprd",
    "Boot_Update_Firmware":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "nvt_extra_proc_init":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "nvt_mp_proc_init":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "ts": "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "nvt_get_fw_pipe":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "nvt_get_mdata":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "nvt_mp_parse_dt":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
    "nvt_read_mdata":
        "drivers/input/touchscreen/novatek_nvt_ts_spi",
}

STATIC_RE = ase.STATIC_RE


def main():
    edited = 0
    for sym, d in sorted(JOBS.items()):
        full_dir = os.path.join(TREE, d)
        if not os.path.isdir(full_dir):
            print(f"  !! {sym}: dir {d} missing")
            continue
        done = False
        for fn in sorted(os.listdir(full_dir)):
            if not fn.endswith(".c"):
                continue
            p = os.path.join(full_dir, fn)
            txt = ase.read(p)
            if re.search(r"EXPORT_SYMBOL(_GPL)?\s*\(\s*" + re.escape(sym) +
                         r"\s*\)", txt):
                print(f"  {sym}: already exported in {fn}")
                done = True
                break
            lines = txt.split("\n")
            found = ase.find_defs(lines, {sym})
            if sym not in found:
                continue
            start, end, kind, is_static, is_extern = found[sym]
            if kind == "proto" or is_extern:
                # this file only has the declaration
                continue
            if is_static:
                lines[start] = STATIC_RE.sub("", lines[start], count=1)
            lines.insert(end + 1, f"EXPORT_SYMBOL_GPL({sym});")
            new_txt = "\n".join(lines)
            if ase.balance(new_txt) != ase.balance(txt):
                print(f"  !! {sym}: balance changed, skipped")
                continue
            if APPLY:
                ase.write(p, new_txt)
                print(f"  {sym}: {fn}:{start + 1} -> EXPORT_SYMBOL_GPL "
                      f"({'un-static' if is_static else 'as-is'})")
                edited += 1
            else:
                print(f"  (dry run) {sym}: {fn}:{start + 1}")
            done = True
            break
        if not done:
            print(f"  !! {sym}: definition not found in {d}")

    print(f"\nedited: {edited}")


if __name__ == "__main__":
    main()
