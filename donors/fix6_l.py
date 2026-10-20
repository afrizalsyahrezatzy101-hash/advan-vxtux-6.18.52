"""Match the module layout to the stock ROM's own config.

The stock 5.4 config is the ground truth for what is a module and what
is built into vmlinux on this device:

  $ grep -E 'TWOFISH' donors/stock_kernel/stock_Image.config
  CONFIG_CRYPTO_TWOFISH=m
  CONFIG_CRYPTO_TWOFISH_COMMON=m

So twofish is a MODULE in the stock ROM - that is why
/lib/modules/twofish_common.ko exists. The tree has
CONFIG_CRYPTO_TWOFISH=y, and the select in crypto/Kconfig

  config CRYPTO_TWOFISH
          tristate "Twofish"
          select CRYPTO_ALGAPI
          select CRYPTO_TWOFISH_COMMON

then forces CRYPTO_TWOFISH_COMMON to =y as well, so the common part is
built into vmlinux and never becomes a .ko.

The stock blob agrees about the shape: twofish_common.ko imports
nothing at all and exports twofish_setkey + __twofish_setkey, so it is
a self-contained module, and no stock blob imports twofish_setkey.

Same for the mailbox: the stock ROM ships unisoc-mailbox.ko, so
SPRD_MBOX has to be a module. It is tristate in drivers/mailbox/
Kconfig already; the tree just had it =y.

And the flash driver: the stock ROM ships sprd_flash_drv.ko.

Usage:
    python3 fix6_l.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

# symbol -> value, taken from the stock 5.4 config where it has one
WANT = {
    # donors/stock_kernel/stock_Image.config: CONFIG_CRYPTO_TWOFISH=m
    "CONFIG_CRYPTO_TWOFISH": "m",
    # ... CONFIG_CRYPTO_TWOFISH_COMMON=m
    "CONFIG_CRYPTO_TWOFISH_COMMON": "m",
    # the stock ROM ships unisoc-mailbox.ko
    "CONFIG_SPRD_MBOX": "m",
    # the stock ROM ships sprd_flash_drv.ko
    "CONFIG_SPRD_FLASH_DRV": "m",
    # the stock ROM ships nvmem_sprd_cache_efuse.ko
    "CONFIG_NVMEM_SPRD_CACHE_EFUSE": "m",
    # the stock ROM ships sprd_apipe.ko
    "CONFIG_SPRD_AUDIO_PIPE": "m",
}


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    cp = os.path.join(TREE, ".config")
    d = read(cp)
    print("== the config, before ==")
    for sym in WANT:
        m = re.search(rf"^(# )?{sym}( is not set|=.*)$", d, re.M)
        print(f"   {sym:38s} {m.group(0) if m else 'ABSENT'}")

    print()
    print("== the stock 5.4 config, for reference ==")
    sc = read("E:/vxtux_build_repo/donors/stock_kernel/stock_Image.config")
    for sym in WANT:
        m = re.search(rf"^(# )?{sym}( is not set|=.*)$", sc, re.M)
        print(f"   {sym:38s} {m.group(0) if m else 'ABSENT'}")

    print()
    print("== apply ==")
    for sym, val in WANT.items():
        pat = rf"^(# )?{sym}( is not set|=.*)$"
        m = re.search(pat, d, re.M)
        new = f"{sym}={val}"
        if m and m.group(0) == new:
            print(f"   {sym}: already {val}")
            continue
        if m:
            d = re.sub(pat, new, d, count=1, flags=re.M)
        else:
            d = d.rstrip("\n") + f"\n{new}\n"
        print(f"   {sym} -> {val}")

    if APPLY:
        write(cp, d)
        print("   WROTE .config")
    else:
        print("   (dry run)")

    # the build also needs the same values in the codespace; print the
    # commands that do it there
    print()
    print("== the same on the codespace ==")
    args = " ".join(f"--{'module' if v == 'm' else 'enable'} {s[len('CONFIG_'):]}"
                    for s, v in WANT.items())
    print(f"   ./scripts/config --file .config {args}")
    print("   make ARCH=arm64 olddefconfig")


if __name__ == "__main__":
    main()
