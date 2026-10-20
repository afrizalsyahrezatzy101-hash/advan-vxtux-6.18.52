import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # where is 'struct hci_dev' defined?
    "grep -rn 'struct hci_dev {' include/net/bluetooth/ net/bluetooth/*.h 2>/dev/null | head -5",
    # what do the callers include?
    "sed -n '1,25p' net/bluetooth/hci_sync.c 2>/dev/null | grep include",
    "sed -n '1,30p' net/bluetooth/mgmt.c 2>/dev/null | grep include",
    # does upstream 6.18 have past_receiver_capable anywhere?
    "grep -rn 'past_receiver_capable\\|past_sender_capable' net/bluetooth/ include/net/bluetooth/ 2>/dev/null | head -10",
    # hci_core.h location
    "ls include/net/bluetooth/ 2>/dev/null",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:2000])
    print("-" * 60)
