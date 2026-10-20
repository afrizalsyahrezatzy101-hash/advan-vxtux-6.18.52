import struct

STOCK = "E:/vxtux_build_repo/donors/stock_kernel/boot_a.bin"
d = open(STOCK, "rb").read()

KERNEL_OFF = 0x1000
ksize = struct.unpack_from("<I", d, 8)[0]
kend = KERNEL_OFF + ksize

print(f"kernel : 0x{KERNEL_OFF:x} .. 0x{kend:x}")
print()
print(f"=== bytes at 0x{kend:x} .. 0x{kend+0x100:x} (gap) ===")
for i in range(kend, kend + 0x100, 16):
    row = d[i:i + 16]
    if not any(row):
        continue
    print(f"{i:08x}: {row.hex(' ')}")

# find the lz4 legacy stream extent
LZ4 = 0x184C2102
off = 0x2454000
print(f"\n=== lz4 legacy stream at 0x{off:x} ===")
n_blocks = 0
total_comp = 0
while off + 12 <= len(d):
    magic, bsize = struct.unpack_from("<II", d, off)
    if magic != LZ4:
        print(f"  stream ends at 0x{off:x} (magic 0x{magic:08x})")
        break
    off += 8
    struct.unpack_from("<I", d, off)[0]  # checksum
    off += 4
    if bsize == 0:
        print(f"  end marker at 0x{off - 12:x}")
        break
    if bsize & 0x80000000:
        real = bsize & 0x7FFFFFFF
        off += real
    else:
        off += bsize
    total_comp += bsize
    n_blocks += 1
    if n_blocks > 2000:
        break
print(f"  blocks: {n_blocks}, compressed bytes: {total_comp:,}")
print(f"  stream spans 0x2454000 .. 0x{off:x}  ({off - 0x2454000:,} bytes)")

# so the real ramdisk (compressed) is 0x2454000..0x{off:x}
print()
print(f"=== tail after the ramdisk ===")
print(f"  0x{off:x} .. 0x{len(d):x} = {len(d) - off:,} bytes")
tail = d[off:off + 64]
print(f"  first 64: {tail.hex(' ')}")
nz = 0
for i in range(off, len(d)):
    if d[i]:
        nz += 1
print(f"  non-zero bytes in tail: {nz:,}")
