import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
p = "kernel/berry_compat_link.c"
fp = os.path.join(ROOT, p)

with open(fp, "rb") as f:
    d = f.read()

# The bluetooth stubs live in hci_core.h now; this file does not need
# <net/bluetooth/hci.h> (which needs the uapi bluetooth.h for bdaddr_t).
old = b"#include <net/bluetooth/hci.h>\n"
if old in d:
    d = d.replace(old, b"")
    with open(fp, "wb") as f:
        f.write(d)
    print("removed bluetooth include")
else:
    print("include not found")

with open(fp, "rb") as f:
    out = f.read()
print("bytes:", len(out))
for i, l in enumerate(out.split(b"\n"), 1):
    t = l.decode("utf-8", "replace")
    if t.startswith("#include"):
        print(f"  {i}: {t}")
