import struct
d = open("init/version.o", "rb").read()
sym_off = 0x13018
for i in [0,1,2,5,10,15,19,25,30]:
    off = sym_off + i*24
    st_name = struct.unpack("<I", d[off:off+4])[0]
    st_info = d[off+4]
    st_other = d[off+5]
    st_shndx = struct.unpack("<H", d[off+6:off+8])[0]
    st_value = struct.unpack("<Q", d[off+8:off+16])[0]
    st_size = struct.unpack("<Q", d[off+16:off+24])[0]
    print(f"sym{i}: st_name={st_name} info=0x{st_info:x} shndx={st_shndx} val=0x{st_value:x} sz=0x{st_size:x}")