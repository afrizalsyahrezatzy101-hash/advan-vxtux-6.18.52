import os
import re
import subprocess

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

# 1) dump berry_compat_link.c
with open(os.path.join(ROOT, "kernel/berry_compat_link.c"), "rb") as f:
    src = f.read()
print("=== kernel/berry_compat_link.c ===")
for i, l in enumerate(src.split(b"\n"), 1):
    print(f"{i}: {l.decode('utf-8','replace')}")

# 2) collect the symbols it defines
syms = set(re.findall(rb"^[A-Za-z_][A-Za-z0-9_ \t\*]*?\b([a-z_][a-z0-9_]{3,})\s*\(",
                       src, re.M))
syms = {s.decode() for s in syms}
print("\n=== candidate symbols defined ===")
for s in sorted(syms):
    print("  ", s)

# 3) check which of them ALREADY exist in the tree
print("\n=== which already exist elsewhere? ===")
for s in sorted(syms):
    r = subprocess.run(
        ["bash", "-lc",
         f"cd {ROOT} && grep -rn --include=*.c --include=*.h "
         f"-E '(^|[^a-zA-Z_]){s}\\s*\\(' . 2>/dev/null | "
         f"grep -v 'berry_compat_link.c' | head -3"],
        capture_output=True, text=True, timeout=300)
    out = r.stdout.strip()
    if out:
        print(f"  {s}: EXISTS")
        for line in out.splitlines()[:2]:
            print(f"      {line.strip()[:150]}")
    else:
        print(f"  {s}: not found (stub needed)")
