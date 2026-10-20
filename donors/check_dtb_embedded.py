import os
import sys

IMG = "E:/vxtux_build_repo/build_output/vxtux_6.18.52/Image"
DTBS = [
    "E:/vxtux_build_repo/build_output/vxtux_6.18.52/ums512-1h10-vxtux.dtb",
    "E:/vxtux_build_repo/build_output/vxtux_6.18.52/ums512-1h10.dtb",
]

d = open(IMG, "rb").read()
print(f"Image: {len(d):,} bytes")

for dtb in DTBS:
    if not os.path.exists(dtb):
        print(f"{os.path.basename(dtb)}: MISSING")
        continue
    b = open(dtb, "rb").read()
    i = d.find(b)
    print(f"{os.path.basename(dtb)} ({len(b):,} bytes): "
          f"{'FOUND at 0x%x' % i if i >= 0 else 'not embedded'}")

# FDT magic = 0xd00dfeed, big-endian on the wire
MAGIC = b"\xd0\x0d\xfe\xed"
idx = 0
hits = []
while len(hits) < 6:
    i = d.find(MAGIC, idx)
    if i < 0:
        break
    hits.append(i)
    idx = i + 1
print(f"\nFDT magic occurrences in Image: {[hex(h) for h in hits]}")
for h in hits:
    import struct
    totalsize = struct.unpack_from(">I", d, h + 4)[0]
    print(f"  @0x{h:x} totalsize={totalsize}")
