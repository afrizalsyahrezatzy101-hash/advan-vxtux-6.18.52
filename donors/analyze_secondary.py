import struct

STOCK = "E:/vxtux_build_repo/donors/stock_kernel/boot_a.bin"
d = open(STOCK, "rb").read()

SEC_OFF = 0x25bad8c
sec = d[SEC_OFF:]

# find all non-zero runs
runs = []
i = 0
n = len(sec)
while i < n:
    if sec[i]:
        j = i
        while j < n and (sec[j] or (j + 64 < n and any(sec[j:j + 64]))):
            j += 1
        runs.append((SEC_OFF + i, SEC_OFF + j, j - i))
        i = j
    else:
        i += 1

print(f"secondary region: 0x{SEC_OFF:x}..0x{len(d):x} ({len(sec):,} bytes)")
print(f"non-zero runs: {len(runs)}")
for a, b, l in runs[:20]:
    print(f"  0x{a:08x}..0x{b:08x}  {l:,} bytes")
    print(f"     head: {d[a:a+32].hex(' ')}")

# check for known magics in the secondary region
print()
for name, sig in (("ANDROID!", b"ANDROID!"), ("lz4leg", b"\x02\x21\x4c\x18"),
                  ("cpio", b"070701"), ("gzip", b"\x1f\x8b\x08"),
                  ("fdt", b"\xd0\x0d\xfe\xed"), ("ELF", b"\x7fELF"),
                  ("SPRD", b"SPRD"), ("UNISOC", b"UNISOC")):
    idx = SEC_OFF
    hits = []
    while len(hits) < 3:
        i = d.find(sig, idx)
        if i < 0:
            break
        hits.append(hex(i))
        idx = i + 1
    print(f"  {name}: {hits}")
