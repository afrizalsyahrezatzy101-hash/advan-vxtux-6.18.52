import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    # is net/core/netdev_rx_queue.c compiled?
    "ls -la net/core/netdev_rx_queue.c 2>&1",
    "grep -n 'netdev_rx_queue' net/core/Makefile | head -5",
    "ls -la net/core/built-in.a 2>&1; "
    "ar t net/core/built-in.a 2>/dev/null | grep -i rx_queue | head -5",
    # are the symbols in the built objects?
    "nm net/core/netdev_rx_queue.o 2>&1 | grep -E 'net_mp_(open|close)_rxq' | head -5",
    # usb phy
    "grep -n 'phy' drivers/usb/Makefile | head -10",
    "ls -la drivers/usb/phy/phy.o 2>&1",
    "nm drivers/usb/phy/phy.o 2>/dev/null | grep -E 'usb_(add|remove)_phy' | head -5",
    # CONFIG_USB_PHY
    "grep -E '^CONFIG_USB_PHY|^# CONFIG_USB_PHY' .config | head -3",
    # minidump
    "grep -rn 'minidump_save_extend_information' drivers/soc/sprd/ --include=*.c 2>/dev/null | head -5",
    "grep -n 'SPRD.*SYSDUMP\|sprd_sysdump' drivers/soc/sprd/Makefile 2>/dev/null | head -8",
    # LL_RESERVED_SPACE_EX in netdevice.h
    "grep -n 'LL_RESERVED_SPACE' include/linux/netdevice.h | head -8",
]


def run(cmd):
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:95]}")
    print(run(c))
    print("-" * 70)
