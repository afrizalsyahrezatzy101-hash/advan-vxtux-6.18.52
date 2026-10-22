import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
OUT = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"
TMP = os.environ.get("TMPDIR", "/tmp")

# 1) pull the new Image + vmlinux
for rel, dest in (("arch/arm64/boot/Image", "Image"), ("vmlinux", "vmlinux")):
    flat = os.path.join(TMP, dest)
    r = subprocess.run(["gh", "codespace", "cp", "-c", CS,
                        f"remote:{ROOT}/{rel}", flat],
                       capture_output=True, text=True, timeout=900)
    if r.returncode == 0 and os.path.exists(flat):
        shutil.copyfile(flat, os.path.join(OUT, dest))
        print(f"OK  {dest}: {os.path.getsize(os.path.join(OUT, dest)):,} bytes")
    else:
        print(f"FAIL {dest}: {r.stderr.strip()[:120]}")

# 2) the vmlinux symbol table (for the symbol comparison)
script = (f"cd {ROOT} && nm vmlinux 2>/dev/null "
          "| grep -E ' [TDBRWV] ' | cut -d' ' -f3 | sort -u "
          "> /workspaces/vxtux-build/vmlinux_syms86.txt && "
          "wc -l /workspaces/vxtux-build/vmlinux_syms86.txt")
r = subprocess.run(["gh", "codespace", "ssh", "-c", CS, "--", script],
                   capture_output=True, text=True, timeout=600)
print("syms:", r.stdout.strip() or r.stderr.strip()[:150])

flat = os.path.join(TMP, "vmlinux_syms86.txt")
r = subprocess.run(["gh", "codespace", "cp", "-c", CS,
                    "remote:/workspaces/vxtux-build/vmlinux_syms86.txt", flat],
                   capture_output=True, text=True, timeout=900)
if r.returncode == 0 and os.path.exists(flat):
    shutil.copyfile(flat, os.path.join(OUT, "vmlinux_syms.txt"))
    n = sum(1 for _ in open(flat, encoding="utf-8", errors="replace"))
    print(f"OK  vmlinux symbols: {n:,}")
else:
    print("FAIL syms:", r.stderr.strip()[:150])
