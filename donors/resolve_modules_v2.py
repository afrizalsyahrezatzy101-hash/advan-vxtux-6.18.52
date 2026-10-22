"""Accurate stock-module resolution using the git index for enumeration.

The previous walk-based version missed sources because the stock module
names use '-' where the kernel uses '_' and vice versa. Here we
enumerate the tree with `git ls-files` (fast, exact) and match each
stock module against every .c stem, then resolve the config symbol
from the driver's own Makefile.

Usage:
    python3 resolve_modules_v2.py <tree> <vxtux.config> <vxtux_ko_list>
                                  <stock.names> [-o out.md]
"""
import os
import re
import subprocess
import sys

# The `git` on PATH in this environment is a Hermes shim that recurses
# ("BUG (fork bomb)"), so always call the real Git for Windows binary.
GIT = r"C:\Program Files\Git\cmd\git.exe"
if not os.path.exists(GIT):
    GIT = "git"


def git_files(tree):
    """Enumerate the kernel tree's own files.

    The kernel tree lives inside the parent repo, so `git -C tree
    ls-files` lists the parent repo's index relative to the repo root.
    Filter to the tree's own subdirectory and strip the prefix.
    """
    r = subprocess.run([GIT, "-C", tree, "ls-files"],
                       capture_output=True, text=True, timeout=300)
    if r.returncode != 0:
        return []
    # `git -C <dir> ls-files` prints paths relative to <dir> already.
    return [f.replace("\\", "/") for f in r.stdout.splitlines() if f]


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


def canon(s):
    return s.replace("-", "_").lower()


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
    ko_canon = {canon(k) for k in kos}

    files = git_files(tree)
    print(f"git ls-files: {len(files):,} files")

    # index: canon(stem) -> [paths]
    stem_index = {}
    for f in files:
        if not f.endswith(".c"):
            continue
        stem = os.path.basename(f)[:-2]
        stem_index.setdefault(canon(stem), []).append(f)

    # read every Makefile once
    mk_cache = {}

    def makefile_text(rel):
        if rel not in mk_cache:
            p = os.path.join(tree, rel)
            if os.path.exists(p):
                mk_cache[rel] = open(p, "r", encoding="utf-8",
                                     errors="replace").read()
            else:
                mk_cache[rel] = ""
        return mk_cache[rel]

    def config_syms_for(rel_src):
        """Collect CONFIG_X symbols that gate this source file."""
        syms = set()
        base = os.path.basename(rel_src)
        d = os.path.dirname(rel_src)
        for _ in range(5):
            txt = makefile_text(os.path.join(d, "Makefile"))
            if txt:
                for m in re.finditer(
                        r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*([^\n]*)",
                        txt):
                    sym, objs = m.group(1), m.group(2)
                    stem = base[:-2]
                    sub = os.path.basename(d)
                    if stem in objs or re.search(
                            r"(^|/)" + re.escape(stem) + r"(\s|$)", objs):
                        syms.add(sym)
                    if re.search(r"(^|/)" + re.escape(sub) + r"/", objs):
                        syms.add(sym)
            nd = os.path.dirname(d)
            if nd == d:
                break
            d = nd
        return syms

    stock = [l.strip() for l in open(stocknames, encoding="utf-8",
                                     errors="replace") if l.strip()]

    rows = []
    for s in stock:
        c = canon(s)
        srcs = stem_index.get(c, [])
        syms = set()
        for src in srcs[:3]:
            syms |= config_syms_for(src)
        vals = {k: cfg.get(k, "absent") for k in sorted(syms)}
        ko_hit = c in ko_canon

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

        rows.append({"name": s, "status": status, "cfg": vals,
                     "src": srcs[:3], "syms": sorted(syms)})

    counts = {}
    for r in rows:
        counts[r["status"]] = counts.get(r["status"], 0) + 1

    covered = counts.get("MODULE", 0) + counts.get("BUILTIN", 0)
    print()
    print("=" * 78)
    print("STOCK MODULE RESOLUTION (git-index based)")
    print(f"  vxtux .ko built : {len(kos)}")
    print(f"  stock .ko       : {len(stock)}")
    for st in ("MODULE", "BUILTIN", "MODULE_NOT_BUILT", "DISABLED",
               "NOT_PRESENT"):
        print(f"  {st:<18}: {counts.get(st, 0)}")
    print(f"  --> COVERED     : {covered}/{len(stock)} "
          f"({100.0 * covered / max(1, len(stock)):.0f}%)")
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
            print(f"  {r['name']:<26} {cs:<38} {sc[:64]}")
        print()

    L = ["# Stock Vendor Module Resolution — VXTux 6.18.52\n"]
    L.append(f"- VXTux .ko built: {len(kos)}")
    L.append(f"- Stock vendor .ko: {len(stock)}")
    for st in ("MODULE", "BUILTIN", "MODULE_NOT_BUILT", "DISABLED",
               "NOT_PRESENT"):
        L.append(f"- {st}: {counts.get(st, 0)}")
    L.append(f"- **COVERED (MODULE + BUILTIN): {covered}/{len(stock)} "
             f"({100.0 * covered / max(1, len(stock)):.0f}%)**")
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
