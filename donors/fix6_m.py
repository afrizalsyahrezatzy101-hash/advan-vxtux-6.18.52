"""Fix the duplicate SPRD_FLASH_DRV definition.

What was wrong
--------------
The symbol is defined TWICE:

  drivers/Kconfig:271
      config SPRD_FLASH_DRV
              bool "Spreadtrum camera flash HAL"

  drivers/sprd/flash/Kconfig:1
      config SPRD_FLASH_DRV
              tristate "Spreadtrum camera flash HAL"
              depends on HAS_IOMEM

and only ONE of the two Kconfig files is ever sourced. The source
chain is:

  drivers/Kconfig:171   source "drivers/soc/sprd/Kconfig"
  drivers/Kconfig:268   source "drivers/vxtux/Kconfig"

but NOTHING sources drivers/sprd/Kconfig - it is not in the chain at
all. So the drivers/Kconfig definition is the live one, and that one
is a `bool`.

A bool can never be a module, so:

  $ ./scripts/config --file .config --module SPRD_FLASH_DRV
  CONFIG_SPRD_FLASH_DRV=m
  $ make ARCH=arm64 olddefconfig
  # CONFIG_SPRD_FLASH_DRV is not set

olddefconfig throws the value away. The stock ROM ships
sprd_flash_drv.ko, so the replacement has to be a loadable module.

The fix
-------
1. drivers/sprd/Kconfig is added to the source chain, so the
   drivers/sprd/flash/Kconfig definition (the tristate one) is the
   live one.
2. The duplicate in drivers/Kconfig is removed - two definitions of
   one symbol is a Kconfig error waiting to happen, and the scanner
   in donors/scan_dup_kconfig_syms.py exists precisely to catch this
   class of thing.

Why the drivers/Kconfig copy existed
-------------------------------------
It was written for the port where no source existed for the vendor
sprd_flash_drv.ko and the register/dispatch contract was recovered
from the blob with radare2. The source has since been recovered
(drivers/sprd/flash/flash_drv.c, ported 2026-10-04), so the
drivers/sprd/flash/ tree is the real one and the inline copy is
stale.

Usage:
    python3 fix6_m.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # ------------------------------------------------------------------
    # 1) drop the duplicate from drivers/Kconfig
    # ------------------------------------------------------------------
    print("== 1) drivers/Kconfig: drop the duplicate definition ==")
    rel = "drivers/Kconfig"
    p = os.path.join(TREE, rel)
    txt = read(p)
    block = """config SPRD_FLASH_DRV
	bool "Spreadtrum camera flash HAL"
	help
	  Registration HAL between the V4L2 flash subdevice and per-chip flash
	  drivers. Ports the vendor sprd_flash_drv.ko; the register/dispatch
	  contract was recovered from that module with radare2 (no source exists
	  in the donors). Chip drivers call sprd_flash_register() to publish
	  themselves; the subdevice calls sprd_flash_ctrl()/sprd_flash_cfg().
	  Registers a misc device "sprd_flash" and creates its own platform
	  device, so it needs no DT node.
"""
    if block not in txt:
        print("   !! the block was not found verbatim")
    else:
        repl = """# SPRD_FLASH_DRV is defined in drivers/sprd/flash/Kconfig, which is
# sourced through drivers/sprd/Kconfig. It used to be defined here as
# a bool, for the port where no source existed for the vendor
# sprd_flash_drv.ko and the contract was recovered from the blob with
# radare2. The source has since been recovered, and a bool can never
# be a module - the stock ROM ships sprd_flash_drv.ko, so the
# replacement has to be loadable.
"""
        if APPLY:
            write(p, txt.replace(block, repl))
            print(f"   WROTE {rel}")
        else:
            print("   (dry run)")

    # ------------------------------------------------------------------
    # 2) source drivers/sprd/Kconfig
    # ------------------------------------------------------------------
    print()
    print("== 2) drivers/Kconfig: source drivers/sprd/Kconfig ==")
    p = os.path.join(TREE, rel)
    txt = read(p)
    if 'source "drivers/sprd/Kconfig"' in txt:
        print("   already sourced")
    else:
        anchor = 'source "drivers/vxtux/Kconfig"'
        if anchor not in txt:
            print("   !! the anchor line was not found")
        else:
            add = (anchor + "\n\n"
                   "# The Unisoc camera flash HAL (drivers/sprd/flash/). It replaces the\n"
                   "# vendor sprd_flash_drv.ko, so it has to be a loadable module -\n"
                   "# drivers/sprd/flash/Kconfig declares it tristate for that reason.\n"
                   'source "drivers/sprd/Kconfig"')
            if APPLY:
                write(p, txt.replace(anchor, add))
                print(f"   WROTE {rel}")
            else:
                print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) verify
    # ------------------------------------------------------------------
    print()
    print("== verify ==")
    txt = read(os.path.join(TREE, rel))
    print("   drivers/Kconfig still defines it:",
          "config SPRD_FLASH_DRV" in txt)
    print("   drivers/sprd/Kconfig is sourced:",
          'source "drivers/sprd/Kconfig"' in txt)
    # the duplicate scanner
    print()
    print("== duplicate scan ==")
    r = os.system("cd /e/vxtux_build_repo && python3 donors/"
                  "scan_dup_kconfig_syms.py 2>&1 | tail -6")
    return 0


if __name__ == "__main__":
    sys.exit(main())
