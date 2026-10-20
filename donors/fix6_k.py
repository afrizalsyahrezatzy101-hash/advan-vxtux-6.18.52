"""Close the deblob gaps that the sources for.

The verification against the 31 stock blobs says 19/31 have a module
with the same file name and 14/31 have the same API. The 12 that do
not have a name match break down like this:

  nvmem_sprd_cache_efuse  the source IS in the tree
                          (drivers/vxtux/soc/cache_efuse/), but the
                          composite module is named
                          sprd_cache_efuse.o -> the stock file is
                          nvmem_sprd_cache_efuse.ko

  twofish_common          crypto/twofish_common.c is in the tree, but
                          CONFIG_CRYPTO_TWOFISH_COMMON is forced to =y
                          by the CRYPTO_TWOFISH=y select, so it is
                          built into vmlinux and never becomes a .ko.
                          The stock ROM ships twofish_common.ko with
                          twofish_setkey exported.

  unisoc-mailbox          drivers/mailbox/sprd-mailbox.c is in the
                          tree, but the stock file name is
                          unisoc-mailbox.ko and the module name inside
                          is unisoc_mailbox.

  sprd_apipe              the source IS in the tree
                          (drivers/vxtux/audio/audio_pipe/), the
                          stock file name is sprd_apipe.ko.

  sprd_flash_drv          the source IS in the tree
                          (drivers/sprd/flash/flash_drv.c), the stock
                          file name is sprd_flash_drv.ko.

  audio_sipc, mmdvfs, sprd-dmaengine-pcm, sprd-sipc-virt-bus,
  sprd_time_sync, usb_f_vser
                          NO SOURCE in this tree. See the README.

This script renames the modules that have a source, and exports the
symbols the stock blobs export.

Usage:
    python3 fix6_k.py [--apply]
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
    # 1) nvmem_sprd_cache_efuse
    # ------------------------------------------------------------------
    print("== 1) cache_efuse: the composite module name ==")
    rel = "drivers/vxtux/soc/cache_efuse/Makefile"
    p = os.path.join(TREE, rel)
    print("   current:")
    for l in read(p).split("\n"):
        if l.strip() and not l.strip().startswith("#"):
            print("     ", l)
    want = """# SPDX-License-Identifier: GPL-2.0
#
# The composite module is named after the vendor blob it replaces
# (nvmem_sprd_cache_efuse.ko). The stock module's own name is
# nvmem_sprd_cache_efuse, and it exports nothing - the symbols it
# uses (efuse_mem_ram_vmap, sprd_efuse_probe, sprd_efuse_read) are
# local to it.
#
# The composite form is used because listing the objects one by one
# would make kbuild emit a module per object, and the stock file name
# would not be reproduced.

obj-$(CONFIG_NVMEM_SPRD_CACHE_EFUSE) += nvmem_sprd_cache_efuse.o
nvmem_sprd_cache_efuse-y := sprd_cache_efuse.o
"""
    if APPLY:
        write(p, want)
        print(f"   WROTE {rel}")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 2) twofish_common
    # ------------------------------------------------------------------
    print()
    print("== 2) twofish_common: a module, not built into vmlinux ==")
    print("   CONFIG_CRYPTO_TWOFISH_COMMON is forced to =y by the")
    print("   CRYPTO_TWOFISH=y select. The stock ROM ships")
    print("   twofish_common.ko with twofish_setkey exported, so the")
    print("   common part has to be a module of its own.")
    rel = "crypto/Kconfig"
    p = os.path.join(TREE, rel)
    txt = read(p)
    old = """config CRYPTO_TWOFISH_COMMON
	tristate
	help
	  Common parts of the Twofish cipher algorithm shared by the
	  generic c and the assembler implementations.
"""
    new = """config CRYPTO_TWOFISH_COMMON
	tristate
	help
	  Common parts of the Twofish cipher algorithm shared by the
	  generic c and the assembler implementations.

	  It is a module of its own rather than being forced into
	  vmlinux by the CRYPTO_TWOFISH select, because the stock ROM
	  ships twofish_common.ko with twofish_setkey exported:
	  (checked on the blob pulled from the device)
	  $ nm donors/stock_vendor_modules/twofish_common.ko
	  T twofish_setkey
	  T __twofish_setkey
"""
    if old not in txt:
        print("   !! the Kconfig block was not found verbatim")
    else:
        if APPLY:
            write(p, txt.replace(old, new))
            print(f"   WROTE {rel}")
        else:
            print("   (dry run)")

    # ------------------------------------------------------------------
    # 3) unisoc-mailbox
    # ------------------------------------------------------------------
    print()
    print("== 3) sprd-mailbox -> unisoc-mailbox.ko ==")
    rel = "drivers/mailbox/Makefile"
    p = os.path.join(TREE, rel)
    txt = read(p)
    if "unisoc-mailbox" in txt:
        print("   already renamed")
    else:
        old = "obj-$(CONFIG_SPRD_MBOX)\t\t+= sprd-mailbox.o"
        new = ("# The composite module is named after the vendor blob it\n"
               "# replaces (unisoc-mailbox.ko). The stock module's own\n"
               "# name is unisoc_mailbox.\n"
               "obj-$(CONFIG_SPRD_MBOX)\t\t+= unisoc-mailbox.o\n"
               "unisoc-mailbox-y := sprd-mailbox.o")
        if old not in txt:
            print("   !! the obj- line was not found verbatim")
        else:
            if APPLY:
                write(p, txt.replace(old, new))
                print(f"   WROTE {rel}")
            else:
                print("   (dry run)")

    # ------------------------------------------------------------------
    # 4) sprd_apipe
    # ------------------------------------------------------------------
    print()
    print("== 4) sprd_apipe ==")
    rel = "drivers/vxtux/audio/audio_pipe/Makefile"
    p = os.path.join(TREE, rel)
    print("   current:")
    for l in read(p).split("\n"):
        if l.strip() and not l.strip().startswith("#"):
            print("     ", l)
    want = """# The composite module is named after the vendor blob it replaces
# (sprd_apipe.ko). The stock module's own name is sprd_apipe.
#
# NOTE: a single-object module must NOT be written as a composite
# whose object has the same name as the module - kbuild then uses the
# module's own .o as the link input and ld refuses:
#
#   aarch64-linux-gnu-ld: input file '.../audio_pipe.o' is the same
#   as output file

obj-$(CONFIG_SPRD_AUDIO_PIPE) += sprd_apipe.o
"""
    if APPLY:
        write(p, want)
        print(f"   WROTE {rel}")
    else:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # 5) sprd_flash_drv
    # ------------------------------------------------------------------
    print()
    print("== 5) sprd_flash_drv ==")
    rel = "drivers/sprd/flash/Makefile"
    p = os.path.join(TREE, rel)
    if not os.path.exists(p):
        print(f"   !! {rel} does not exist")
        d = os.path.join(TREE, "drivers/sprd")
        if os.path.isdir(d):
            print("   drivers/sprd contains:")
            for x in sorted(os.listdir(d)):
                print("     ", x)
    else:
        print("   current:")
        for l in read(p).split("\n"):
            if l.strip() and not l.strip().startswith("#"):
                print("     ", l)

    # ------------------------------------------------------------------
    # 6) the config: the mailbox and twofish as modules
    # ------------------------------------------------------------------
    print()
    print("== 6) .config ==")
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    for sym, val in (("CONFIG_SPRD_MBOX", "m"),
                     ("CONFIG_CRYPTO_TWOFISH_COMMON", "m"),
                     ("CONFIG_NVMEM_SPRD_CACHE_EFUSE", "m")):
        cur = re.search(rf"^(# )?{sym}( is not set|=.*)$", d, re.M)
        print(f"   {sym}: {cur.group(0) if cur else 'ABSENT'}")


if __name__ == "__main__":
    main()
