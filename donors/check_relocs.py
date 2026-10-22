import struct
d = open("init/main.o", "rb").read()
# .rela.text at 0x5a118 size 0x12d8 = 4824 bytes = 201 rela (4824/24)
rela_start = 0x5a118
rela_end = 0x5a118 + 0x12d8
print(f".rela.text: {rela_start:#x} - {rela_end:#x}")

# Print first 3 and last 3 entries
def rela_at(off):
    r_offset = struct.unpack("<Q", d[off:off+8])[0]
    r_info = struct.unpack("<Q", d[off+8:off+16])[0]
    r_addend = struct.pack("<i", struct.unpack("<i", d[off+16:off+20])[0])  # signed
    r_sym = r_info >> 32
    r_type = r_info & 0xFFFFFFFF
    return r_offset, r_sym, r_type, r_addend

for i in range(3):
    off = rela_start + i * 24
    r = rela_at(off)
    print(f"first{i}: off=0x{r[0]:x} r_sym={r[1]} r_type=0x{r[2]:x}")

for i in range(201-3, 201):
    off = rela_start + i * 24
    r = rela_at(off)
    print(f"last{i}: off=0x{r[0]:x} r_sym={r[1]} r_type=0x{r[2]:x}")

# Now scan past rela_end - do we find garbage that looks like big r_sym?
print("\n===scan past .rela.text end===")
for extra_off in [0x12d8, 0x12d8+8, 0x12d8+16, 0x12d8+24]:
    off = rela_start + 201*24 + extra_off
    if off + 24 > len(d):
        break
    r = rela_at(off)
    print(f"off+0x{extra_off:x} r_sym={r[1]} r_type=0x{r[2]:x} (looks like next rela data)")

# Find any r_sym > 485 in .rela.text (main.o)
print("\n===big r_sym in .rela.text===")
for i in range(201):
    off = rela_start + i * 24
    r = rela_at(off)
    if r[1] > 485:
        print(f"  index={i}: r_sym={r[1]} r_type=0x{r[2]:x}")