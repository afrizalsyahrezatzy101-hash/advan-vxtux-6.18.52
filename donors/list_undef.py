import re
import sys

path = sys.argv[1] if len(sys.argv) > 1 else "/workspaces/codespace_build78.log"
syms = {}
with open(path, "rb") as f:
    data = f.read()

for m in re.finditer(rb"undefined reference to `([A-Za-z0-9_]+)'", data):
    s = m.group(1).decode()
    syms.setdefault(s, 0)
    syms[s] += 1

print(f"UNIQUE UNDEFINED SYMBOLS: {len(syms)}")
for s in sorted(syms):
    print(f"  {s}  (x{syms[s]})")
