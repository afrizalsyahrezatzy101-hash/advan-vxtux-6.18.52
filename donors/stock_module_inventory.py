"""Build the definitive stock module inventory from every source we have.

Sources, in priority order:
  1. G:/UMS512/tools/unisocbypass/oc/kernel_rev/modules/*.ko  (ROM dump)
  2. G:/UMS512/twrp_zip/vendor/lib/modules/*.ko
  3. G:/UMS512/unlocked2/twrp/device-extracted/*.ko
  4. G:/UMS512/twrp_intel/*.ko

Output: donors/stock_modules.txt  (one module name per line, with the
source path appended after a tab) and a summary by size.
"""
import hashlib
import os
import re

ROOTS = [
    "G:/UMS512/tools/unisocbypass/oc/kernel_rev/modules",
    "G:/UMS512/twrp_zip/vendor/lib/modules",
    "G:/UMS512/unlocked2/twrp/device-extracted",
    "G:/UMS512/twrp_intel",
]
OUT = "E:/vxtux_build_repo/donors/stock_modules.txt"


def main():
    seen = {}
    for root in ROOTS:
        if not os.path.isdir(root):
            print(f"skip (missing): {root}")
            continue
        n = 0
        for dirpath, dirs, files in os.walk(root):
            for fn in files:
                if not fn.endswith(".ko"):
                    continue
                p = os.path.join(dirpath, fn)
                mod = fn[:-3]
                if mod in seen:
                    continue
                sz = os.path.getsize(p)
                seen[mod] = (p, sz)
                n += 1
        print(f"{root}: {n} new modules")

    print(f"\ntotal unique modules: {len(seen)}")
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        for mod in sorted(seen):
            p, sz = seen[mod]
            f.write(f"{mod}\t{sz}\t{p}\n")
    print(f"wrote {OUT}")

    print()
    print("--- by size (top 20) ---")
    for mod, (p, sz) in sorted(seen.items(), key=lambda kv: -kv[1][1])[:20]:
        print(f"  {mod:<40} {sz:>10,} bytes")


if __name__ == "__main__":
    main()
