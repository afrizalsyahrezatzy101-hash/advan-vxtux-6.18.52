import os
import re

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "include/net/bluetooth/hci.h"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

print("=== lines mentioning bdaddr ===")
for i, l in enumerate(d.split(b"\n"), 1):
    t = l.decode("utf-8", "replace")
    if "bdaddr" in t.lower():
        print(f"{i}: {t}")
