"""For each of the 31 stock vendor .ko, find in the VXTux tree:
  - the config symbol that builds its replacement
  - the .ko name the build currently emits for it
  - whether the replacement source is present at all

Output drives the next build change.
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

STOCK = []
with open("E:/vxtux_build_repo/donors/stock_modules.txt",
          encoding="utf-8", errors="replace") as f:
    for line in f:
        s = line.strip()
        if not s:
            continue
        s = re.split(r"[\t|,;]", s)[0].strip()
        if s:
            STOCK.append(s)

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
files = [f.replace("\\", "/") for f in r.stdout.splitlines()]

# every obj-$(CONFIG_X) += ... line in every Makefile
ko_of = {}          # CONFIG symbol -> set of .ko names
dir_of = {}         # CONFIG symbol -> set of source dirs
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
        sym, objs = m.group(1), m.group(2).replace("\\", "").strip()
        if not objs:
            continue
        for o in objs.split():
            if o.endswith(".o"):
                ko_of.setdefault(sym, set()).add(o[:-2])
            else:
                dir_of.setdefault(sym, set()).add(o.rstrip("/"))
                ko_of.setdefault(sym, set()).add(o.rstrip("/"))

cfg = open(CFG, encoding="utf-8", errors="replace").read()
vals = {}
for line in cfg.splitlines():
    line = line.strip()
    if line.startswith("CONFIG_") and "=" in line:
        k, v = line.split("=", 1)
        vals[k] = v
    elif line.startswith("# CONFIG_") and line.endswith(" is not set"):
        vals[line[2:line.index(" is not set")].strip()] = "n"

# which symbols are ON, and do they produce each stock .ko name?
on_syms = {s: v for s, v in vals.items() if v in ("y", "m")}
name_to_syms = {}
for s, kos in ko_of.items():
    for k in kos:
        name_to_syms.setdefault(k, set()).add(s)

print("=" * 96)
print(f"{'stock .ko':<30} {'state':<28} {'emitted .ko':<28} {'src'}")
print("=" * 96)

plan = []
for st in STOCK:
    syms = name_to_syms.get(st, set())
    if not syms:
        # maybe the source is there but under a different name
        plan.append((st, None, None, None))
        print(f"{st:<30} {'NO .ko PRODUCED':<28}")
        continue
    on = [s for s in syms if s in on_syms]
    off = [s for s in syms if s not in on_syms]
    if on:
        s0 = sorted(on)[0]
        state = f"{s0.replace('CONFIG_','')}={vals[s0]}"
        srcs = sorted(dir_of.get(s0, ()))
        src = srcs[0] if srcs else ""
        print(f"{st:<30} {state:<28} {st:<28} {src}")
        plan.append((st, s0, vals[s0], src))
    else:
        s0 = sorted(syms)[0]
        state = f"{s0.replace('CONFIG_','')}={vals.get(s0,'absent')}"
        srcs = sorted(dir_of.get(s0, ()))
        src = srcs[0] if srcs else ""
        print(f"{st:<30} {state + ' (OFF)':<28} {st:<28} {src}")
        plan.append((st, s0, vals.get(s0, "absent"), src))

print()
print("=" * 96)
need_m = [(st, s, src) for st, s, v, src in plan if v == "y"]
have_m = [(st, s, src) for st, s, v, src in plan if v == "m"]
none = [st for st, s, v, src in plan if s is None]
print(f"already =m (name match): {len(have_m)}")
for st, s, src in have_m:
    print(f"    {st:<30} {s}")
print(f"=y, would become =m    : {len(need_m)}")
for st, s, src in need_m:
    print(f"    {st:<30} {s:<44} {src}")
print(f"no .ko produced at all  : {len(none)}")
for st in none:
    print(f"    {st}")
