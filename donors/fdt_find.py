#!/usr/bin/env python3
"""Find a node in a flattened device tree and print its properties.

The stock DTB came out of the vendor boot image, and there is no dtc
on the machine, so this walks the FDT structure directly.

    python3 fdt_find.py donors/stock_initramfs/stock.dtb syst-timer
"""
import struct
import sys

FDT_BEGIN_NODE = 1
FDT_END_NODE = 2
FDT_PROP = 3
FDT_NOP = 4
FDT_END = 9


def parse(dtb):
    magic, totalsize, off_dt_struct, off_dt_strings, off_mem, ver, last_comp, boot_cpuid, size_dt_strings, size_dt_struct = \
        struct.unpack(">10I", dtb[:40])
    assert magic == 0xD00DFEED, hex(magic)
    strings = dtb[off_dt_strings:off_dt_strings + size_dt_strings]
    p = off_dt_struct
    end = off_dt_struct + size_dt_struct
    stack = [""]
    out = []
    while p < end:
        (tok,) = struct.unpack(">I", dtb[p:p + 4])
        p += 4
        if tok == FDT_BEGIN_NODE:
            e = dtb.index(b"\x00", p)
            name = dtb[p:e].decode()
            p = (e + 4) & ~3
            stack.append(name)
        elif tok == FDT_END_NODE:
            stack.pop()
        elif tok == FDT_PROP:
            (plen, nameoff) = struct.unpack(">II", dtb[p:p + 8])
            p += 8
            e = strings.index(b"\x00", nameoff)
            pname = strings[nameoff:e].decode()
            val = dtb[p:p + plen]
            p = (p + plen + 3) & ~3
            out.append(("/".join(stack[1:]), pname, val))
        elif tok == FDT_NOP:
            continue
        elif tok == FDT_END:
            break
    return out


def main():
    dtb = open(sys.argv[1], "rb").read()
    want = sys.argv[2] if len(sys.argv) > 2 else None
    nodes = parse(dtb)
    if want is None:
        # No filter: list every node that has a "compatible" property.
        seen = set()
        for path, pname, val in nodes:
            if pname == "compatible" and path not in seen:
                seen.add(path)
                print("=" * 60)
                print("node:", path)
                for p2, n2, v2 in nodes:
                    if p2 == path:
                        if n2 == "compatible":
                            print(f"  {n2} = {v2.decode('utf-8','replace')}")
                        else:
                            print(f"  {n2} = {v2.hex()}  ({v2!r})")
        return
    for path, pname, val in nodes:
        if pname == "compatible" and want in val.decode("utf-8", "replace"):
            print("=" * 60)
            print("node:", path)
            for p2, n2, v2 in nodes:
                if p2 == path:
                    if n2 == "compatible":
                        print(f"  {n2} = {v2.decode('utf-8','replace')}")
                    else:
                        print(f"  {n2} = {v2.hex()}  ({v2!r})")


if __name__ == "__main__":
    main()
