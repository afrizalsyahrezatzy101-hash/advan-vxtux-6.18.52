"""Pure-Python LZ4 legacy-block decompressor.

The SPRD vendor_boot image stores the vendor ramdisk as an LZ4 legacy
stream (magic 0x184C2102) starting at the first page after the header.

Legacy frame layout:
    u32 magic = 0x184C2102
    u32 blockSize          (bit31 set => the block is stored uncompressed,
                            real size = blockSize & 0x7FFFFFFF)
    u32 blockChecksum
    u8  data[blockSize]
Repeated until blockSize == 0.

Block format (LZ4 block):
    token u8 : high nibble = literal length, low nibble = match length
    if literal_len == 15: read varint-ish extension (255 chains)
    literals
    if not at end: u16 offset, then match length extension
    match length += 4 (minimum match)
"""
import mmap
import os
import struct
import sys

MAGIC = 0x184C2102


def lz4_block_decompress(src, out_len_hint=None):
    """Decompress one LZ4 block. Returns (bytes, consumed)."""
    dst = bytearray()
    i = 0
    n = len(src)
    while i < n:
        token = src[i]
        i += 1
        lit_len = token >> 4
        if lit_len == 15:
            while True:
                b = src[i]
                i += 1
                lit_len += b
                if b != 255:
                    break
        dst += src[i:i + lit_len]
        i += lit_len
        if i >= n:
            break
        # match
        offset = src[i] | (src[i + 1] << 8)
        i += 2
        match_len = token & 0x0F
        if match_len == 15:
            while True:
                b = src[i]
                i += 1
                match_len += b
                if b != 255:
                    break
        match_len += 4
        start = len(dst) - offset
        if start < 0:
            raise ValueError("bad offset")
        for k in range(match_len):
            dst.append(dst[start + k])
    return bytes(dst), i


def lz4_legacy_decompress(data):
    """Decompress a whole LZ4 legacy stream."""
    off = 0
    out = bytearray()
    blocks = 0
    while off + 12 <= len(data):
        magic, block_size = struct.unpack_from("<II", data, off)
        if magic != MAGIC:
            raise ValueError(
                f"bad magic at {off}: 0x{magic:08x} (want 0x{MAGIC:08x})")
        off += 8
        checksum = struct.unpack_from("<I", data, off)[0]
        off += 4
        if block_size == 0:
            break
        if block_size & 0x80000000:
            # uncompressed block
            real = block_size & 0x7FFFFFFF
            out += data[off:off + real]
            off += real
        else:
            chunk, used = lz4_block_decompress(data[off:off + block_size])
            out += chunk
            off += used
        blocks += 1
    return bytes(out), blocks


def main():
    img = sys.argv[1] if len(sys.argv) > 1 else \
        "G:/UMS512/twrp_zip/vendor_boot_stock.img"
    start = int(sys.argv[2], 0) if len(sys.argv) > 2 else 0x1000
    outp = sys.argv[3] if len(sys.argv) > 3 else \
        "E:/vxtux_build_repo/donors/stock_kernel/vendor_ramdisk.cpio"

    f = open(img, "rb")
    mm = mmap.mmap(f.fileno(), 0, access=mmap.ACCESS_READ)
    print(f"{img}: {len(mm)} bytes, start=0x{start:x}")
    magic = struct.unpack_from("<I", mm, start)[0]
    print(f"magic at start: 0x{magic:08x} (want 0x{MAGIC:08x})")
    if magic != MAGIC:
        print("not an lz4 legacy stream at that offset")
        return

    data = mm[start:]
    out, blocks = lz4_legacy_decompress(data)
    print(f"decompressed {blocks} blocks -> {len(out)} bytes")
    os.makedirs(os.path.dirname(outp), exist_ok=True)
    with open(outp, "wb") as g:
        g.write(out)
    print(f"saved {outp}")
    print(f"  '.ko' occurrences: {out.count(b'.ko')}")
    print(f"  head: {out[:16].hex()}")
    mm.close()


if __name__ == "__main__":
    main()
