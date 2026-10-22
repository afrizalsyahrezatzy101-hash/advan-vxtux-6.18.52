import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"


def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()


def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")


# =====================================================================
# FIX: the Novatek touchscreen config points at the WRONG driver.
#
#   CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS      = y
#       -> drivers/input/touchscreen/novatek-nvt-ts.c
#          the generic upstream i2c driver for the Acer Iconia One 7
#          B1-750 (NT11205). Not the Unisoc board's touch at all.
#
#   CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI  = n
#       -> drivers/input/touchscreen/novatek_nvt_ts_spi/
#          the VXTux port of the NT36xxx SPI family, which builds one
#          composite module named novatek_ts_spi.ko - the exact name
#          of the stock vendor blob it replaces.
#
# The .config had the two mixed up, so the stock blob's replacement was
# never compiled and an unrelated upstream driver was built in its
# place. Turn the real one on and the unrelated one off.
# =====================================================================
print("== FIX: Novatek touchscreen config ==")
p = ".config"
d = rd(p)
before = d

# turn the SPI driver on
if b"\n# CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI is not set" in d:
    d = d.replace(b"\n# CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI is not set",
                  b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI=m")
    print("   TOUCHSCREEN_NOVATEK_NVT_TS_SPI: n -> m")
elif b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI=m" in d:
    print("   TOUCHSCREEN_NOVATEK_NVT_TS_SPI already =m")
elif b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI=y" in d:
    print("   TOUCHSCREEN_NOVATEK_NVT_TS_SPI already =y")
else:
    print("   !! TOUCHSCREEN_NOVATEK_NVT_TS_SPI not found in .config")

# turn the unrelated i2c driver off
if b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS=y" in d:
    d = d.replace(b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS=y",
                  b"\n# CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS is not set")
    print("   TOUCHSCREEN_NOVATEK_NVT_TS: y -> n")
elif b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS=m" in d:
    d = d.replace(b"\nCONFIG_TOUCHSCREEN_NOVATEK_NVT_TS=m",
                  b"\n# CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS is not set")
    print("   TOUCHSCREEN_NOVATEK_NVT_TS: m -> n")
else:
    print("   TOUCHSCREEN_NOVATEK_NVT_TS already off")

if d != before:
    wr(p, d)
else:
    print("   .config unchanged")

# verify
print()
print("== verify ==")
d = rd(".config")
for key in (b"CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI",
            b"# CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI is not set",
            b"CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS=y",
            b"# CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS is not set"):
    hit = [l for l in d.split(b"\n") if l == key]
    if hit:
        print("  ", hit[0].decode())
