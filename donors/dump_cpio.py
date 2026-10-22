import sys

CPIO = "/workspaces/vxtux-build/stock_kernel/vb.out"
d = open(CPIO, "rb").read()
i = d.find(b"070701")
print("magic at", i)
seg = d[i:i + 600]
for j in range(0, 600, 120):
    chunk = seg[j:j + 120]
    try:
        txt = chunk.decode("ascii")
    except Exception:
        txt = repr(chunk)
    print(f"{i + j:08x}: {txt}")
