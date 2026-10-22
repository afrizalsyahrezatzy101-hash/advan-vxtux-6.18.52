"""Rename two built-in drivers to emit a .ko with the STOCK BLOB's name.

  CONFIG_NVMEM_SPRD_CACHE_EFUSE  -> cache_efuse.ko
      stock blob: nvmem_sprd_cache_efuse.ko

  CONFIG_SPRD_VSP_PW_DOMAIN      -> sprd.ko
      stock blob: sprd-vsp-pw-domain.ko

Both currently build as a directory module (obj-$(CONFIG_X) += dir/),
which kbuild turns into <dir>.ko. The tree already has a convention for
this: the composite form, where one .ko is built from several objects
and named after the blob it replaces (see drivers/gpu/drm/sprd/gsp and
drivers/media/platform/sprd/vsp).

    obj-$(CONFIG_X) += <name>.o
    <name>-y := obj1.o obj2.o ...

The stock name is used verbatim, including the dash/underscore mix the
vendor shipped, so the userspace loader can insmod the same file name.

Usage:
    python3 rename_ko.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

# (Makefile to rewrite, obj-$(SYMBOL) line, the .ko name to emit)
JOBS = [
    ("drivers/soc/sprd/cache_efuse/Makefile",
     "CONFIG_NVMEM_SPRD_CACHE_EFUSE", "nvmem_sprd_cache_efuse"),
    ("drivers/pmdomain/sprd/Makefile",
     "CONFIG_SPRD_VSP_PW_DOMAIN", "sprd-vsp-pw-domain"),
]


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


for rel, sym, ko in JOBS:
    path = os.path.join(TREE, rel)
    if not os.path.exists(path):
        print(f"  !! {rel}: missing")
        continue
    txt = read(path)
    lines = txt.split("\n")
    objs = []
    out = []
    for line in lines:
        m = re.match(r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*(\S+)\s*$",
                     line)
        if m:
            objs.append(m.group(1))
            continue
        out.append(line)

    if not objs:
        print(f"  !! {rel}: no obj- line for {sym}")
        continue

    # the composite form: one .ko built from every object
    new = [f"obj-$(CONFIG_{sym.replace('CONFIG_', '')}) += {ko}.o"]
    new.append(f"{ko}-y := " + " ".join(objs))
    header = [
        "# SPDX-License-Identifier: GPL-2.0",
        "#",
        f"# ONE composite module, named after the vendor blob it replaces",
        f"# ({ko}.ko). The objects below used to be listed individually,",
        "# which made kbuild emit one module per object; the composite",
        "# form keeps them in a single .ko with the stock file name.",
        "",
    ]
    newtxt = "\n".join(header + new + out)
    # keep the original trailing content
    if txt.endswith("\n"):
        newtxt += "\n"

    if APPLY:
        write(path, newtxt)
        print(f"  {rel}: -> {ko}.ko  ({', '.join(objs)})")
    else:
        print(f"  (dry run) {rel}: -> {ko}.ko  ({', '.join(objs)})")
        for l in header + new:
            print(f"      {l}")

print()
print("also update the parent Makefiles so the subdirectory is not")
print("listed twice (the composite module now replaces the dir module)")
for rel, sym, ko in JOBS:
    parent = os.path.join(os.path.dirname(os.path.dirname(rel)), "Makefile")
    p = os.path.join(TREE, parent)
    if not os.path.exists(p):
        continue
    txt = read(p)
    sub = os.path.basename(os.path.dirname(rel))
    m = re.search(r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*" +
                  re.escape(sub) + r"/\s*$", txt, re.M)
    if not m:
        continue
    newtxt = txt[:m.start()] + f"obj-$(CONFIG_{sym.replace('CONFIG_', '')}) += {sub}/\n" + txt[m.end():]
    # the subdir Makefile now emits the composite, so the parent must
    # not also list it as a module
    newtxt = re.sub(
        r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*" + re.escape(sub) +
        r"/\s*$",
        f"# {sub}/ is built as the composite module {ko}.ko; it is not\n"
        f"# listed here as a directory module any more.",
        newtxt, flags=re.M)
    if APPLY:
        write(p, newtxt)
        print(f"  {parent}: updated")
    else:
        print(f"  (dry run) {parent}: would be updated")
