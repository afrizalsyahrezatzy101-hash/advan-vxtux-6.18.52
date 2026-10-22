import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    # does local struct net_device have ->cfg ?
    "grep -n 'struct netdev_queue_mgmt_ops \\*queue_mgmt_ops\\|struct netdev_config \\*cfg\\|hds_config' include/linux/netdevice.h | head -8",
    # __netif_get_rx_queue
    "grep -rn '__netif_get_rx_queue' include/net/netdev_rx_queue.h include/linux/netdevice.h 2>/dev/null | head -5",
    # netdev_need_ops_lock
    "grep -rn 'netdev_need_ops_lock' include/ 2>/dev/null | head -3",
    # dev_xdp_prog_count
    "grep -rn 'dev_xdp_prog_count' include/ 2>/dev/null | head -3",
    # CONFIG_USB_PHI in Kconfig
    "grep -n -B 3 -A 3 'config USB_PHY' drivers/usb/phy/Kconfig 2>/dev/null | head -15",
    "cat drivers/usb/phy/Makefile 2>/dev/null | head -12",
    # minidump header decl
    "cat include/linux/soc/sprd/sprd_sysdump.h 2>/dev/null | head -40",
    # is there a minidump impl dir?
    "find . -path ./io_uring.disabled -prune -o -name '*minidump*' -print 2>/dev/null | head -10",
    "ls drivers/soc/sprd/ 2>/dev/null | head -30",
]


def run(cmd):
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:95]}")
    print(run(c))
    print("-" * 70)
