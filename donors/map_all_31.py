"""Map each of the 31 stock vendor .ko to the config symbol and source
that would build its replacement, and report what is still missing.

This is the checklist for "every stock blob has a replacement .ko with
the same name".

Usage:
    python3 map_all_31.py
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")
STOCK_LIST = "E:/vxtux_build_repo/donors/stock_modules.txt"

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
files = [f.replace("\\", "/") for f in r.stdout.splitlines()]

# ---- every obj-$(CONFIG_X) line, and every composite -y := line
ko_of = {}
objs_of = {}
dir_of = {}
for f in files:
    if os.path.basename(f) != "Makefile":
        continue
    try:
        txt = open(os.path.join(TREE, f), "r", encoding="utf-8",
                   errors="replace").read()
    except OSError:
        continue
    for m in re.finditer(r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*"
                         r"([^\n]*)", txt):
        sym, rest = m.group(1), m.group(2).replace("\\", "").strip()
        if not rest:
            continue
        for o in rest.split():
            if o.endswith(".o"):
                ko_of.setdefault(sym, set()).add(o[:-2])
                objs_of.setdefault(sym, set()).add(o)
            else:
                d = o.rstrip("/")
                dir_of.setdefault(sym, set()).add(d)
                ko_of.setdefault(sym, set()).add(d)
    # composite anchors: <name>-y := obj1.o obj2.o
    for m in re.finditer(r"^([A-Za-z0-9_.\-]+)-y\s*:?=\s*([^\n]*)",
                         txt, re.M):
        ko_of.setdefault("@composite", set()).add(m.group(1))

# name -> the symbol(s) that emit it
name_syms = {}
for sym, kos in ko_of.items():
    if sym == "@composite":
        continue
    for k in kos:
        name_syms.setdefault(k, set()).add(sym)

# the composite anchors: find which CONFIG_ line emits each anchor
anchor_sym = {}
for f in files:
    if os.path.basename(f) != "Makefile":
        continue
    try:
        txt = open(os.path.join(TREE, f), "r", encoding="utf-8",
                   errors="replace").read()
    except OSError:
        continue
    for m in re.finditer(r"obj-\$\((CONFIG_[A-Za-z0-9_]+)\)\s*\+=\s*"
                         r"([A-Za-z0-9_.\-]+)\.o", txt):
        anchor_sym[m.group(2)] = m.group(1)
    for m in re.finditer(r"^([A-Za-z0-9_.\-]+)-y\s*:?=\s*([^\n]*)",
                         txt, re.M):
        anchor_sym.setdefault(m.group(1), None)

cfg = open(CFG, encoding="utf-8", errors="replace").read()
vals = {}
for line in cfg.splitlines():
    line = line.strip()
    if line.startswith("CONFIG_") and "=" in line:
        k, v = line.split("=", 1)
        vals[k] = v
    elif line.startswith("# CONFIG_") and line.endswith(" is not set"):
        vals[line[2:line.index(" is not set")].strip()] = "n"

stock = []
with open(STOCK_LIST, encoding="utf-8", errors="replace") as f:
    for line in f:
        s = re.split(r"[\t|,;]", line.strip())[0].strip()
        if s:
            stock.append(s)

print("=" * 104)
print(f"{'stock .ko':<28} {'state':<10} {'emits .ko':<28} {'source dir'}")
print("=" * 104)

covered, gaps = [], []
for s in stock:
    syms = name_syms.get(s, set())
    if not syms:
        # maybe the composite anchor is named differently
        print(f"{s:<28} {'NO CONFIG EMITS THIS NAME':<10}")
        gaps.append((s, None, None))
        continue
    best = None
    for sym in sorted(syms):
        v = vals.get(sym, "absent")
        if v in ("y", "m"):
            best = (sym, v)
            break
    if best is None:
        sym = sorted(syms)[0]
        print(f"{s:<28} {vals.get(sym,'absent'):<10} {s:<28} "
              f"(off: {sym})")
        gaps.append((s, sym, vals.get(sym, "absent")))
        continue
    sym, v = best
    srcs = sorted(dir_of.get(sym, ()))
    src = srcs[0] if srcs else "(inline in Makefile)"
    print(f"{s:<28} {sym.replace('CONFIG_','')+'='+v:<10} {s:<28} {src}")
    covered.append((s, sym, v))

print()
print("=" * 104)
print(f"config emits the stock name : {len(covered)}/{len(stock)}")
for s, sym, v in covered:
    print(f"   OK  {s:<28} {sym}={v}")
print(f"still not emitted            : {len(gaps)}/{len(stock)}")
for s, sym, v in gaps:
    print(f"   --  {s:<28} {sym or 'no symbol'}"
          f"{'=' + v if v else ''}")
print("=" * 104)
