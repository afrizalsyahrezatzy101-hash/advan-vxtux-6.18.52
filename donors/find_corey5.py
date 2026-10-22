import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
with open(os.path.join(ROOT, "Makefile"), "rb") as f:
    d = f.read()
lines = d.split(b"\n")
for i in range(1205, 1240):
    if i < len(lines):
        print(f"{i+1}: {lines[i].decode('utf-8','replace')}")
print("=" * 60)
# search for 'init/' in Makefile
for i, l in enumerate(lines, 1):
    if b"init/" in l or b"kernel/" in l or b"fs/" in l or b"net/" in l:
        print(f"{i}: {l.decode('utf-8','replace')}")
