import struct
d = open("init/version.o", "rb").read()
sym_off = 0x13018
# sym 19
off = sym_off + 19*24
data = d[off:off+24]
print(f"hex at 0x{off:x}: {data.hex()}")
st_name = struct.unpack("<I", d[off:off+4])[0]
st_info = d[off+4]
st_other = d[off+5]
st_shndx = struct.unpack("<H", d[off+6:off+8])[0]
st_value = struct.unpack("<Q", d[off+8:off+16])[0]
st_size = struct.unpack("<Q", d[off+16:off+24])[0]
print(f"sym19: st_name=0x{st_name:x}={st_name} info=0x{st_info:x} shndx={st_shndx} val=0x{st_value:x} sz=0x{st_size:x}")

# Now check strtab at 0x13498
print(f"\nstrtab starts at 0x13498: {d[0x13498:0x13498+64]}")
# sym19 name at offset st_name
print(f"name of sym19 at strtab+{st_name}: {d[0x13498+st_name:0x13498+st_name+40]}")

# Check for null ptr patterns. Where would strtab be NULL?
# If sym is past symtab_stop, modpost reads garbage.
nsym = (0x2d78) // 24
symtab_stop = sym_off + nsym * 24
print(f"\nsymtab_stop: 0x{symtab_stop:x} (file size ~0x24388)")

# Check sh_link and sh_offset for symtab section
sh_link = 36
sec_36_off = struct.unpack("<Q", d[0x239b8 + sh_link*64 + 24:0x239b8 + sh_link*64 + 32])[0]
print(f"section 36 (strtab) sh_offset: 0x{sec_36_off:x}")