import struct

STOCK = "E:/vxtux_build_repo/donors/stock_kernel/boot_a.bin"
d = open(STOCK, "rb").read()
print(f"boot_a.bin: {len(d):,} bytes")

MAGIC = b"\xd0\x0d\xfe\xed"
idx = 0
hits = []
while len(hits) < 10:
    i = d.find(MAGIC, idx)
    if i < 0:
        break
    hits.append(i)
    idx = i + 1
print(f"FDT magic at: {[hex(h) for h in hits]}")
for h in hits:
    tot = struct.unpack_from(">I", d, h + 4)[0]
    print(f"  @0x{h:x} totalsize={tot:,}")

# where does the stock kernel end?
KERNEL_OFF = 0x1000
import struct
ksize = struct.unpack_from("<I", d, 8)[0]
print(f"\nstock kernel: 0x{KERNEL_OFF:x} .. 0x{KERNEL_OFF+ksize:x} "
      f"({ksize:,} bytes)")
print(f"tail: 0x{KERNEL_OFF+ksize:x} .. 0x{len(d):x} "
      f"({len(d)-KERNEL_OFF-ksize:,} bytes)")

# what is in the tail? scan for a cpio / gzip / dtb
tail = d[KERNEL_OFF + ksize:]
print(f"\ntail first 64: {tail[:64].hex()}")
for name, sig in (("gzip", b"\x1f\x8b\x08"), ("cpio", b"070701"),
                  ("lz4", b"\x04\x22\x4d\x18"), ("lz4leg", b"\x02\x21\x4c\x18"),
                  ("fdt", MAGIC), ("ANDROID", b"ANDROID!")):
    i = tail.find(sig)
    print(f"  {name}: {'@tail+0x%x (abs 0x%x)' % (i, KERNEL_OFF+ksize+i) if i >= 0 else '-'}")
