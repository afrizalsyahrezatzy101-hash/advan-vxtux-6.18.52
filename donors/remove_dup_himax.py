"""Remove the duplicated Himax hx83102e touchscreen driver.

There are two copies of the same driver in the tree:

  drivers/input/touchscreen/sprd/hx83102e/     (the older copy)
  drivers/vxtux/touch/hx83102e/                (the VXTux port)

They define the SAME Kconfig symbols (TOUCHSCREEN_HIMAX_CHIPSET,
TOUCHSCREEN_HIMAX_COMMON, TOUCHSCREEN_HIMAX_ONCELL,
TOUCHSCREEN_HIMAX_INCELL, ...) and BOTH are built:

  drivers/input/touchscreen/sprd/Makefile:
      obj-$(CONFIG_TOUCHSCREEN_HIMAX_COMMON) += hx83102e/
  drivers/vxtux/Makefile:
      obj-$(CONFIG_TOUCHSCREEN_HIMAX_CHIPSET) += touch/hx83102e/

Two definitions of one symbol is a Kconfig error, and it only shows up
once both Kconfigs are sourced:

  drivers/vxtux/touch/hx83102e/hxchipset/Kconfig:22: error: choice
  value has a prompt outside its choice group

(the two choice groups for the same symbols get merged, and the prompt
of one ends up in the menu of the other).

The VXTux copy is the one that is built for this board and the one the
driver tree descends into, so the OLD copy under drivers/input/ is
removed: its Kconfig, its Makefile and its sources.

The symbol names stay exactly as they are - they are what the stock
DT bindings and the stock blob hxchipset_83102e.ko use.

Usage:
    python3 remove_dup_himax.py [--apply]
"""
import os
import shutil
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

# the duplicate copy to remove
DUP_DIR = "drivers/input/touchscreen/sprd/hx83102e"
DUP_PARENT = "drivers/input/touchscreen/sprd/Makefile"

# the copy that stays
KEEP_DIR = "drivers/vxtux/touch/hx83102e"


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    print("== the two copies ==")
    for d in (KEEP_DIR, DUP_DIR):
        p = os.path.join(TREE, d)
        if not os.path.isdir(p):
            print(f"  {d}: absent")
            continue
        n = sum(len(f) for _, _, f in os.walk(p))
        print(f"  {d}: {n} files")

    print()
    print("== the symbols both define ==")
    syms = {}
    for d in (KEEP_DIR, DUP_DIR):
        k = os.path.join(TREE, d, "hxchipset", "Kconfig")
        if not os.path.exists(k):
            continue
        txt = read(k)
        names = set()
        for line in txt.split("\n"):
            s = line.strip()
            if s.startswith("config "):
                names.add(s[7:].strip())
        syms[d] = names
    if len(syms) == 2:
        a, b = list(syms.values())
        common = sorted(a & b)
        print(f"  {len(common)} symbols defined in BOTH:")
        for c in common:
            print(f"    {c}")

    print()
    print("== remove the duplicate ==")
    if not APPLY:
        print("  (dry run)")
    else:
        # 1) the parent's obj- line
        mp = os.path.join(TREE, DUP_PARENT)
        if os.path.exists(mp):
            txt = read(mp)
            lines = [l for l in txt.split("\n")
                     if "hx83102e" not in l]
            new = "\n".join(lines)
            if not new.endswith("\n"):
                new += "\n"
            write(mp, new)
            print(f"  {DUP_PARENT}: dropped the hx83102e/ entry")

        # 2) the parent Kconfig's source line
        kp = os.path.join(TREE, "drivers/input/touchscreen/sprd/Kconfig")
        if os.path.exists(kp):
            txt = read(kp)
            lines = [l for l in txt.split("\n")
                     if "hx83102e" not in l]
            new = "\n".join(lines)
            if not new.endswith("\n"):
                new += "\n"
            write(kp, new)
            print(f"  drivers/input/touchscreen/sprd/Kconfig: dropped "
                  f"the hx83102e source")

        # 3) the directory itself
        dp = os.path.join(TREE, DUP_DIR)
        if os.path.isdir(dp):
            shutil.rmtree(dp)
            print(f"  removed {DUP_DIR}/")


if __name__ == "__main__":
    main()
