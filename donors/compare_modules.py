"""Compare the VXTux-built .ko module set against the stock vendor .ko set.

The stock ROM ships 129 vendor .ko modules (extracted from the stock
vendor ramdisk). The VXTux build produces its own set. This script
produces a three-way classification:

  MATCH       - same module name in both
  ONLY_VXTUX  - module only the VXTux build produces (mainline module)
  ONLY_STOCK  - vendor module not yet produced by the VXTux build

"ONLY_STOCK" is the actionable list: each entry is a vendor driver that
still needs to be either ported, enabled in the config, or confirmed
built-in to vmlinux.

Usage:
    python3 compare_modules.py <vxtux_ko_dir_or_list> <stock_ko_dir_or_list> [-o out.md]
"""
import os
import re
import sys

# Modules that are legitimately absent from the .ko list because they are
# built into vmlinux (=y) in the VXTux config. Populated from the config
# by the caller; these names are treated as "built-in" not "missing".
BUILTIN_HINTS = set()


def read_list(path):
    """Read a module list: either a directory of .ko or a text file list."""
    if os.path.isdir(path):
        out = set()
        for root, dirs, files in os.walk(path):
            for fn in files:
                if fn.endswith(".ko"):
                    out.add(fn[:-3])
        return out
    out = set()
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            m = re.search(r"([A-Za-z0-9_.-]+)\.ko\b", line)
            if m:
                out.add(m.group(1))
            else:
                out.add(os.path.basename(line))
    return out


def norm(name):
    """Normalise module names: the stock build uses '-' and '_'
    interchangeably."""
    return name.replace("-", "_")


def main():
    if len(sys.argv) < 3:
        print(__doc__)
        return
    vpath, spath = sys.argv[1], sys.argv[2]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    v = {norm(x) for x in read_list(vpath)}
    s = {norm(x) for x in read_list(spath)}

    match = v & s
    only_v = v - s
    only_s = s - v

    print("=" * 78)
    print("MODULE SET DIFF")
    print(f"  vxtux : {len(v)} modules")
    print(f"  stock : {len(s)} modules")
    print(f"  match : {len(match)}")
    print(f"  only vxtux: {len(only_v)}")
    print(f"  only stock: {len(only_s)}")

    print()
    print("--- only in stock (vendor drivers still to be ported/enabled) ---")
    for m in sorted(only_s):
        print(f"  {m}")

    lines = [
        "# Module Set Diff — VXTux 6.18.52 vs Stock vendor modules\n",
        f"- vxtux: `{vpath}` ({len(v)} modules)",
        f"- stock: `{spath}` ({len(s)} modules)",
        f"- match: {len(match)}",
        f"- only in vxtux: {len(only_v)}",
        f"- only in stock: {len(only_s)}\n",
        "## Only in stock (vendor drivers not yet in the VXTux build)\n",
    ]
    for m in sorted(only_s):
        lines.append(f"- `{m}`")
    lines.append("")
    lines.append("## Only in vxtux (mainline modules the stock build does not ship)\n")
    for m in sorted(only_v):
        lines.append(f"- `{m}`")
    lines.append("")
    lines.append("## Match\n")
    for m in sorted(match):
        lines.append(f"- `{m}`")
    lines.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(lines))
        print(f"\nwrote {outpath}")


if __name__ == "__main__":
    main()
