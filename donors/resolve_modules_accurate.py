"""Accurate stock-module resolution: derive the config symbol from the
driver's own Makefile instead of guessing names.

For each stock vendor .ko:
  1. find the matching source file in the VXTux tree
  2. walk up to its Makefile and read the obj-$(CONFIG_X) line
  3. read CONFIG_X out of the VXTux .config
     =m -> the driver builds as a module; check the .ko list
     =y -> built into vmlinux
     =n / absent -> not built
  4. if no source at all -> NOT_PRESENT (a genuine port gap)

Usage:
    python3 resolve_modules_accurate.py <tree> <vxtux.config>
                                        <vxtux_ko_list> <stock.names> [-o out.md]
"""
import os
import re
import sys


def load_config(path):
    cfg = {}
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.rstrip()
            if line.startswith("# CONFIG_") and line.endswith(" is not set"):
                cfg[line[2:line.index(" is not set")].strip()] = "n"
            elif line and not line.startswith("#") and "=" in line:
                k, v = line.split("=", 1)
                cfg[k.strip()] = v.strip()
    return cfg


def load_ko_names(path):
    out = set()
    with open(path, "r", encoding="utf-8", errors="replace") as f:
        for line in f:
            line = line.strip()
            if line.endswith(".ko"):
                out.add(os.path.basename(line)[:-3])
    return out


def candidate_stems(stock_name):
    """Filename stems that could produce this module name."""
    n = stock_name
    stems = {n, n.replace("_", "-"), n.replace("-", "_")}
    # kernel module names come from the source file stem
    stems.add(n.split("_")[0] + "-" + "_".join(n.split("_")[1:]))
    return {s for s in stems if s}


def find_sources(tree, stock_name):
    """Find .c files in the tree whose stem could build this module."""
    stems = candidate_stems(stock_name)
    hits = []
    skip = (".git", "io_uring.disabled", "Documentation", "tools", "scripts",
            "samples", "lib", "certs", "sound/soc/codecs")
    for root, dirs, files in os.walk(tree):
        rel = os.path.relpath(root, tree).replace("\\", "/")
        if any(rel == s or rel.startswith(s + "/") for s in skip):
            dirs[:] = []
            continue
        for fn in files:
            if not fn.endswith(".c"):
                continue
            stem = fn[:-2]
            if stem in stems or stem.replace("-", "_") == stock_name:
                hits.append(os.path.join(rel, fn))
    return hits


def makefile_config_symbols(tree, rel_src):
    """From a source path, find the obj-$(CONFIG_X) symbol for it."""
    d = os.path.dirname(rel_src)
    syms = set()
    # walk up until we find a Makefile that mentions the object
    for _ in range(4):
        mk = os.path.join(tree, d, "Makefile")
        if os.path.exists(mk):
            txt = open(mk, "r", encoding="utf-8", errors="replace").read()
            base = os.path.basename(rel_src)
            for m in re.finditer(
                    r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*([^\n]*)",
                    txt):
                sym, objs = m.group(1), m.group(2)
                if base[:-2] in objs or os.path.splitext(base)[0] in objs:
                    syms.add(sym)
            # the dir may be added as a whole: obj-$(CONFIG_X) += subdir/
            for m in re.finditer(
                    r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*([^\n]*)",
                    txt):
                sym, objs = m.group(1), m.group(2)
                sub = os.path.basename(d)
                if re.search(r"(^|/)" + re.escape(sub) + r"/", objs):
                    syms.add(sym)
        nd = os.path.dirname(d)
        if nd == d or not nd:
            break
        d = nd
    return syms


def main():
    if len(sys.argv) < 5:
        print(__doc__)
        return
    tree, cfgpath, kolist, stocknames = sys.argv[1:5]
    outpath = None
    if "-o" in sys.argv:
        outpath = sys.argv[sys.argv.index("-o") + 1]

    cfg = load_config(cfgpath)
    kos = load_ko_names(kolist)
    stock = [l.strip() for l in open(stocknames, encoding="utf-8",
                                     errors="replace") if l.strip()]

    rows = []
    for s in stock:
        srcs = find_sources(tree, s)
        syms = set()
        for src in srcs[:3]:
            syms |= makefile_config_symbols(tree, src)
        vals = {c: cfg.get(c, "absent") for c in sorted(syms)}
        ko_hit = s in kos or s.replace("-", "_") in kos or \
            s.replace("_", "-") in kos

        if ko_hit:
            status = "MODULE"
        elif not srcs:
            status = "NOT_PRESENT"
        elif any(v == "y" for v in vals.values()):
            status = "BUILTIN"
        elif any(v == "m" for v in vals.values()):
            status = "MODULE_NOT_BUILT"
        else:
            status = "DISABLED"

        rows.append({
            "name": s, "status": status, "cfg": vals,
            "src": srcs[:3], "syms": sorted(syms),
        })

    counts = {}
    for r in rows:
        counts[r["status"]] = counts.get(r["status"], 0) + 1

    print("=" * 78)
    print("ACCURATE STOCK MODULE RESOLUTION")
    print(f"  vxtux .ko built : {len(kos)}")
    print(f"  stock .ko       : {len(stock)}")
    for st in ("MODULE", "BUILTIN", "MODULE_NOT_BUILT", "DISABLED",
               "NOT_PRESENT"):
        print(f"  {st:<18}: {counts.get(st, 0)}")
    print()
    for st in ("NOT_PRESENT", "DISABLED", "MODULE_NOT_BUILT", "BUILTIN",
               "MODULE"):
        sel = [r for r in rows if r["status"] == st]
        if not sel:
            continue
        print(f"--- {st} ({len(sel)}) ---")
        for r in sel:
            cs = ", ".join(f"{k}={v}" for k, v in r["cfg"].items()) or "—"
            sc = ", ".join(r["src"][:2]) or "—"
            print(f"  {r['name']:<26} {cs:<40} {sc[:70]}")
        print()

    L = ["# Stock Vendor Module Resolution — VXTux 6.18.52 (Makefile-derived)\n"]
    L.append(f"- VXTux .ko built: {len(kos)}")
    L.append(f"- Stock vendor .ko: {len(stock)}")
    for st in ("MODULE", "BUILTIN", "MODULE_NOT_BUILT", "DISABLED",
               "NOT_PRESENT"):
        L.append(f"- {st}: {counts.get(st, 0)}")
    L.append("")
    for st in ("NOT_PRESENT", "DISABLED", "MODULE_NOT_BUILT", "BUILTIN",
               "MODULE"):
        sel = [r for r in rows if r["status"] == st]
        if not sel:
            continue
        L.append(f"## {st} ({len(sel)})\n")
        L.append("| stock module | config symbol | value | source |")
        L.append("|---|---|---|---|")
        for r in sel:
            if r["cfg"]:
                cs = ", ".join(f"`{k}`" for k in r["cfg"])
                vs = ", ".join(f"`{v}`" for v in r["cfg"].values())
            else:
                cs, vs = "—", "—"
            sc = ", ".join(f"`{x}`" for x in r["src"][:2]) or "—"
            L.append(f"| `{r['name']}` | {cs} | {vs} | {sc} |")
        L.append("")

    if outpath:
        with open(outpath, "w", encoding="utf-8") as f:
            f.write("\n".join(L))
        print(f"wrote {outpath}")


if __name__ == "__main__":
    main()
