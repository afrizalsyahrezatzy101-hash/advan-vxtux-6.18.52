import os

ROOT = "/workspaces/vxtux-build/6.18.52-VKTux-UMS512"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

BLOCK = (b"\n/* ====== HCI PAST (Power Aware Scanning) not ported to VXTux 6.18 ====== */\n"
         b"static inline bool past_receiver_capable(struct hci_dev *hdev)\n"
         b"{\n\t(void)hdev;\n\treturn false;\n}\n"
         b"static inline bool past_sender_capable(struct hci_dev *hdev)\n"
         b"{\n\t(void)hdev;\n\treturn false;\n}\n")


def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()


def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")


# 1) remove the block from the wrong place (include/net/bluetooth/hci.h)
p = "include/net/bluetooth/hci.h"
d = rd(p)
if BLOCK in d:
    d = d.replace(BLOCK, b"")
    wr(p, d)
    print("  removed block from hci.h")
else:
    print("  hci.h clean already")

# 2) insert into hci_core.h right after 'struct hci_dev { ... };'
p = "include/net/bluetooth/hci_core.h"
d = rd(p)
if b"past_receiver_capable" not in d:
    i = d.find(b"struct hci_dev {")
    if i < 0:
        print("  struct hci_dev NOT FOUND")
    else:
        # find the closing '};' of the struct
        j = d.find(b"\n};", i)
        if j < 0:
            print("  struct end NOT FOUND")
        else:
            k = j + 3
            d = d[:k] + BLOCK + d[k:]
            wr(p, d)
            print("  inserted stubs into hci_core.h")
else:
    print("  hci_core.h already has stubs")

# 3) check mgmt.c includes hci_core.h
p = "net/bluetooth/mgmt.c"
d = rd(p)
if b"hci_core.h" in d:
    print("  mgmt.c includes hci_core.h: YES")
else:
    print("  mgmt.c includes hci_core.h: NO -> adding")
    lines = d.split(b"\n")
    out = []
    inserted = False
    for l in lines:
        out.append(l)
        if not inserted and l.strip() == b"#include <net/bluetooth/bluetooth.h>":
            out.append(b"#include <net/bluetooth/hci_core.h>")
            inserted = True
    wr(p, b"\n".join(out))

# 4) hci_sync.c already includes hci_core.h - confirm
d = rd("net/bluetooth/hci_sync.c")
print("  hci_sync.c includes hci_core.h:", b"hci_core.h" in d)

print("DONE")
