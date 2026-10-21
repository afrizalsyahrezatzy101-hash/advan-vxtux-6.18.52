"""Symbol helpers for the stock vendor blobs.

In a Linux module every section has address 0, so an nm address is the
offset inside that symbol's own section. To read a table you must know
which section it lives in.
"""
import re
import subprocess

NM = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
      r"\15.2.rel1\bin\aarch64-none-linux-gnu-nm.exe")
READELF = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-readelf.exe")
OBJDUMP = (r"C:\Users\Administrator\scoop\apps\gcc-aarch64-none-linux-gnu"
           r"\15.2.rel1\bin\aarch64-none-linux-gnu-objdump.exe")

GLOBAL_TYPES = set("TDBRWA")


def symbols(path, defined_only=True):
    """-> [(addr, size, type, name)] sorted by address."""
    cmd = [NM, "-S"]
    if defined_only:
        cmd.append("--defined-only")
    cmd.append(path)
    out = subprocess.run(cmd, capture_output=True, text=True).stdout
    res = []
    for line in out.splitlines():
        p = line.split()
        if len(p) == 4 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.append((p[0], int(p[1], 16), p[2], p[3]))
        elif len(p) == 3 and re.fullmatch(r"[0-9a-f]{16}", p[0]):
            res.append((p[0], 0, p[1], p[2]))
    return res


def globals(path):
    return [s for s in symbols(path) if s[2] in GLOBAL_TYPES]


def locals_(path):
    return [s for s in symbols(path) if s[2] not in GLOBAL_TYPES]


def sections(path):
    """-> {name: (offset, size, addr, type, flags)}"""
    out = subprocess.run([READELF, "-SW", path], capture_output=True,
                         text=True).stdout
    secs = {}
    for line in out.splitlines():
        line = line.strip()
        if not line.startswith("["):
            continue
        if "]" not in line:
            continue
        p = line.split("]", 1)[1].split()
        if len(p) < 6:
            continue
        name = p[0]
        try:
            typ = p[1]
            addr = int(p[2], 16)
            off = int(p[3], 16)
            size = int(p[4], 16)
        except ValueError:
            continue
        secs[name] = (off, size, addr, typ, p[5] if len(p) > 5 else "")
    return secs


def relocs(path):
    """-> {rela_section: {offset: (type, symname, addend)}}"""
    out = subprocess.run([READELF, "-rW", path], capture_output=True,
                         text=True).stdout
    res = {}
    cur = None
    for line in out.splitlines():
        s = line.strip()
        if s.startswith("Relocation section"):
            cur = s.split("'")[1] if "'" in s else None
            continue
        if not s or not s[0].isdigit():
            continue
        p = s.split()
        if len(p) < 5:
            continue
        off = int(p[0], 16)
        typ = p[2]
        name = None
        addend = 0
        for f in p[3:]:
            if f == "+":
                continue
            if f.startswith("+"):
                try:
                    addend = int(f[1:], 16)
                except ValueError:
                    pass
                continue
            if f.startswith("0x"):
                continue
            if re.fullmatch(r"[0-9a-fA-F]{1,8}", f):
                if name is not None:
                    addend = int(f, 16)
                continue
            if name is None:
                name = f
        if name is None:
            continue
        res.setdefault(cur, {})[off] = (typ, name, addend)
    return res


def read_section(path, secs, secname, off, length):
    if secname not in secs:
        return None
    base, size = secs[secname][0], secs[secname][1]
    if off < 0 or off + length > size:
        return None
    with open(path, "rb") as f:
        f.seek(base + off)
        return f.read(length)


def cstring(path, secs, secname, off):
    raw = read_section(path, secs, secname, off, 4096)
    if raw is None:
        return None
    end = raw.find(b"\0")
    if end < 0:
        return None
    return raw[:end].decode("utf-8", "replace")
