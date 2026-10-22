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
# FIX 4: CONFIG_SPRD_SYSDUMP must be off.
#   The sprd sysdump subsystem (which owns
#   minidump_save_extend_information() and sprd_dump_stack_reg()) was
#   never ported into this tree - only the header exists. The tree's
#   own caller in drivers/soc/sprd/modem/cp_dump/sprd_cp_dump.c
#   already has an #else branch that records the region and warns
#   instead of registering it for crash persistence, so turning the
#   config off selects the supported path.
# =====================================================================
print("== FIX 4: .config -> disable CONFIG_SPRD_SYSDUMP ==")
p = ".config"
d = rd(p)
if b"\nCONFIG_SPRD_SYSDUMP=y" in d:
    d = d.replace(b"\nCONFIG_SPRD_SYSDUMP=y",
                  b"\n# CONFIG_SPRD_SYSDUMP is not set")
    wr(p, d)
else:
    print("   already off or missing")

# show the Kconfig entry for reference
print()
print("== drivers/soc/sprd/modem/Kconfig SPRD_SYSDUMP ==")
d = rd("drivers/soc/sprd/modem/Kconfig")
lines = d.split(b"\n")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "SPRD_SYSDUMP" in t:
        for j in range(max(0, i - 3), min(len(lines), i + 5)):
            print(f"   {j+1}: {lines[j].decode('utf-8','replace')}")
        break

print()
print("SUMMARY of this batch:")
print("  1. af_packet.c      : LL_RESERVED_SPACE_EX -> LL_RESERVED_SPACE (3 sites)")
print("  2. netdev_rx_queue.c: __net_mp_open_rxq/__net_mp_close_rxq restored")
print("  3. usb/phy/Kconfig  : CONFIG_USB_PHY default y")
print("  4. .config          : CONFIG_SPRD_SYSDUMP off")
