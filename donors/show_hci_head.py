import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "include/net/bluetooth/hci.h"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()
lines = d.split(b"\n")

print("=== head (1..30) ===")
for i in range(0, 30):
    print(f"{i+1}: {lines[i].decode('utf-8','replace')}")

print()
print("=== 'struct hci_dev' occurrences ===")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "hci_dev" in t and ("struct hci_dev;" in t or "struct hci_dev " in t):
        print(f"{i}: {t}")

print()
print("=== last 20 lines ===")
for i in range(len(lines) - 20, len(lines)):
    print(f"{i+1}: {lines[i].decode('utf-8','replace')}")
