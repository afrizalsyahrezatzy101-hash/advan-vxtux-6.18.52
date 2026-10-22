import os
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

cmds = [
    # which file failed to compile?
    "grep -B 8 \"unknown type name .bdaddr_t\" /workspaces/codespace_build80.log 2>/dev/null | head -20",
    # where is bdaddr_t defined?
    "grep -rn 'bdaddr_t' include/uapi/linux/bluetooth/bluetooth.h 2>/dev/null | head -5",
    "grep -rn 'struct bdaddr' include/uapi/linux/bluetooth/bluetooth.h 2>/dev/null | head -5",
    # does include/net/bluetooth/hci.h include the uapi header?
    "grep -n '#include' include/net/bluetooth/hci.h 2>/dev/null | head -20",
    # does include/net/bluetooth/bluetooth.h?
    "grep -n '#include' include/net/bluetooth/bluetooth.h 2>/dev/null | head -20",
]
for cmd in cmds:
    r = subprocess.run(["bash", "-lc", f"cd {ROOT} && {cmd}"],
                       capture_output=True, text=True, timeout=300)
    print(f"$ {cmd}")
    print((r.stdout.strip() or "(none)")[:2500])
    print("-" * 60)
