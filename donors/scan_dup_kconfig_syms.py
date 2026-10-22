"""Find Kconfig symbols that are defined in MORE THAN ONE file.

Two definitions of the same symbol is a Kconfig error, and it only
shows up when both files are sourced:

  <file>:<line>: error: choice value has a prompt outside its choice
  group

(the two choice groups for the same symbols get merged, and the prompt
of one ends up in the menu of the other)

It bit twice already: the Himax hx83102e driver existed in both
drivers/input/touchscreen/sprd/ and drivers/vxtux/touch/.

This scans every Kconfig for duplicate 'config NAME' definitions, and
also reports which of them are actually reachable (i.e. sourced from
the top-level Kconfig) - a duplicate in a file nobody sources is
harmless.

Usage:
    python3 scan_dup_kconfig_syms.py
"""
import os
import re
import subprocess

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

r = subprocess.run([GIT, "-C", TREE, "ls-files"],
                   capture_output=True, text=True, timeout=300)
files = [f.replace("\\", "/") for f in r.stdout.splitlines()]
kconfigs = [f for f in files if os.path.basename(f).startswith("Kconfig")]

# which Kconfig files are actually sourced, and from where
sourced = {}
for rel in kconfigs:
    p = os.path.join(TREE, rel)
    try:
        txt = open(p, "r", encoding="utf-8", errors="replace").read()
    except OSError:
        continue
    for m in re.finditer(r'^\s*source\s+"([^"]+)"', txt, re.M):
        sourced.setdefault(m.group(1), []).append(rel)

# reachability from the top-level Kconfig
reach = set()
stack = ["Kconfig"]
seen = set()
while stack:
    cur = stack.pop()
    if cur in seen:
        continue
    seen.add(cur)
    reach.add(cur)
    for child in sourced.get(cur, []):
        if child not in seen:
            stack.append(child)

# collect the definitions
defs = {}
for rel in kconfigs:
    p = os.path.join(TREE, rel)
    try:
        lines = open(p, "r", encoding="utf-8", errors="replace").read().split("\n")
    except OSError:
        continue
    for i, line in enumerate(lines, 1):
        s = line.strip()
        if s.startswith("config ") or s.startswith("menuconfig "):
            name = s.split(None, 1)[1].strip()
            defs.setdefault(name, []).append((rel, i))

dups = {n: v for n, v in defs.items() if len(v) > 1}
print("=" * 92)
print(f"Kconfig symbols defined more than once: {len(dups)}")
print("=" * 92)
real = 0
for name, places in sorted(dups.items()):
    reach_list = [(f, l) for f, l in places if f in reach]
    tag = ""
    if len(reach_list) > 1:
        tag = "  <== BOTH REACHABLE, this is a build error"
        real += 1
    elif len(reach_list) == 1:
        tag = "  (only one is sourced - harmless)"
    else:
        tag = "  (none sourced - harmless)"
    print(f"\n{name}{tag}")
    for f, l in places:
        mark = "*" if f in reach else " "
        print(f"   {mark} {f}:{l}")
print()
print("=" * 92)
print(f"symbols defined in two REACHABLE files: {real}")
print("=" * 92)
