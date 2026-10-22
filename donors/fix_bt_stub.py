import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "include/net/bluetooth/hci.h"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

BLOCK = (b"\n/* ====== HCI PAST (Power Aware Scanning) not ported to VXTux 6.18 ====== */\n"
         b"static inline bool past_receiver_capable(struct hci_dev *hdev)\n"
         b"{\n\t(void)hdev;\n\treturn false;\n}\n"
         b"static inline bool past_sender_capable(struct hci_dev *hdev)\n"
         b"{\n\t(void)hdev;\n\treturn false;\n}\n")

print("block present:", BLOCK in d)
if BLOCK in d:
    d = d.replace(BLOCK, b"")
    print("removed misplaced block")

# find where bdaddr_t is defined, and place the stubs AFTER it
i = d.find(b"typedef struct bdaddr_t")
if i < 0:
    i = d.find(b"} bdaddr_t;")
if i < 0:
    # try the modern form
    i = d.find(b"bdaddr_t;")
print("bdaddr_t anchor at:", hex(i) if i >= 0 else "NOT FOUND")

if i >= 0:
    # insert after the end of that definition
    j = d.find(b"\n", i)
    # skip to the end of the struct/typedef block
    depth = 0
    k = i
    while k < len(d):
        if d[k:k+1] == b"{":
            depth += 1
        elif d[k:k+1] == b"}":
            depth -= 1
            if depth == 0:
                k += 1
                break
        k += 1
    # skip to end of that line
    k = d.find(b"\n", k) + 1
    if BLOCK not in d:
        d = d[:k] + BLOCK + d[k:]
        print("inserted stubs at:", hex(k))

with open(fp, "wb") as f:
    f.write(d)

# verify
with open(fp, "rb") as f:
    out = f.read()
lines = out.split(b"\n")
for i, l in enumerate(lines, 1):
    t = l.decode("utf-8", "replace")
    if "bdaddr_t" in t and ("typedef" in t or "} bdaddr" in t):
        print(f"  {i}: {t}")
    if "past_receiver_capable" in t or "past_sender_capable" in t:
        print(f"  {i}: {t}")
print("bytes:", len(out))
