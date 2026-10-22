import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

def rd(p):
    with open(os.path.join(ROOT, p), "rb") as f:
        return f.read()

def wr(p, d):
    with open(os.path.join(ROOT, p), "wb") as f:
        f.write(d)
    print(f"  WROTE {p} ({len(d)} bytes)")

# =============================================================
# 1) .config: CONFIG_IO_URING=n, CONFIG_FUSE_IO_URING=n
#    The io_uring implementation was moved to io_uring.disabled/,
#    but CONFIG_IO_URING stayed =y, so the headers take the
#    "real implementation" branch and reference symbols that do
#    not exist. The headers already carry #else static-inline
#    stubs, so turning the config off is the intended path.
# =============================================================
print("== 1) .config : CONFIG_IO_URING / CONFIG_FUSE_IO_URING ==")
p = ".config"
d = rd(p)
for opt in (b"CONFIG_IO_URING", b"CONFIG_FUSE_IO_URING"):
    if b"\n" + opt + b"=y" in d:
        d = d.replace(b"\n" + opt + b"=y", b"\n# " + opt + b" is not set")
        print(f"  disabled {opt.decode()}")
    elif b"\n" + opt + b"=m" in d:
        d = d.replace(b"\n" + opt + b"=m", b"\n# " + opt + b" is not set")
        print(f"  disabled {opt.decode()} (was =m)")
    else:
        print(f"  {opt.decode()} not =y/=m")
wr(p, d)

# =============================================================
# 2) kernel/berry_compat_link.c must be linked into vmlinux
# =============================================================
print("== 2) kernel/Makefile : add berry_compat_link.o ==")
p = "kernel/Makefile"
d = rd(p)
if b"berry_compat_link.o" not in d:
    # append to the last obj-y block
    lines = d.split(b"\n")
    last = None
    for i, l in enumerate(lines):
        if l.startswith(b"obj-y") and l.rstrip().endswith(b"\\"):
            last = i
    if last is not None:
        lines.insert(last + 1, b"\t\t\t   berry_compat_link.o \\")
        wr(p, b"\n".join(lines))
        print("  added to obj-y list")
    else:
        # fall back: append a standalone obj-y line before the end
        d2 = d + b"\nobj-y\t\t\t\t+= berry_compat_link.o\n"
        wr(p, d2)
        print("  appended standalone obj-y")
else:
    print("  already present")

print("DONE")
