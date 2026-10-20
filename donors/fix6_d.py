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
# FIX 3b: drivers/usb/phy/Kconfig
#   The entry was:
#       config USB_PHY
#           select EXTCON
#           def_bool n
#   `def_bool` on a symbol with no type makes it invisible and
#   impossible to enable (kconfig warns "uses select, but is not bool
#   or tristate"). The musb_sprd_vxtux.c driver calls usb_add_phy() and
#   usb_remove_phy(), which live in drivers/usb/phy/phy.o, and that
#   object is only built when CONFIG_USB_PHY is set - so the symbol
#   must become a real bool that is on.
# =====================================================================
print("== FIX 3b: drivers/usb/phy/Kconfig -> bool USB_PHY ==")
p = "drivers/usb/phy/Kconfig"
d = rd(p)

old = (b"config USB_PHY\n"
       b"\tselect EXTCON\n"
       b"\tdefault y\n")
new = (b"config USB_PHY\n"
       b"\tbool\n"
       b"\tselect EXTCON\n"
       b"\tdefault y\n"
       b"\thelp\n"
       b"\t  Support for USB physical layer (PHY) controllers. The generic\n"
       b"\t  USB PHY framework (drivers/usb/phy/phy.c) provides the\n"
       b"\t  usb_add_phy()/usb_remove_phy() API used by the Unisoc MUSB\n"
       b"\t  host and gadget drivers.\n")

if old in d:
    d = d.replace(old, new)
    wr(p, d)
    print("   patched")
else:
    print("   pattern not found, current state:")
    for i, l in enumerate(d.split(b"\n")[:16], 1):
        print(f"     {i}: {l.decode('utf-8','replace')}")

print()
print("== verify: USB_SUPPORT / USB_MUSB in .config ==")
d = rd(".config")
for key in (b"CONFIG_USB_SUPPORT", b"CONFIG_USB_MUSB", b"CONFIG_USB_PHY",
            b"CONFIG_EXTCON"):
    for l in d.split(b"\n"):
        if l.startswith(key) or l == b"# " + key:
            print("   ", l.decode("utf-8", "replace"))
            break
    else:
        print(f"    {key.decode()}: (not in .config)")
