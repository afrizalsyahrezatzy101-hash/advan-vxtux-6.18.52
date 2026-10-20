import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
TMP = os.environ.get("TMPDIR", "/tmp")

# 1) tar the stock vendor modules on the remote side, then pull
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", CS, "--",
     "cd /workspaces/vxtux-build/stock_kernel/vmods && "
     "tar czf /workspaces/vxtux-build/stock_vmods.tar.gz *.ko && "
     "ls -la /workspaces/vxtux-build/stock_vmods.tar.gz && "
     "ls *.ko | wc -l"],
    capture_output=True, text=True, timeout=300)
print(r.stdout.strip() or r.stderr.strip()[:200])

dst = os.path.join(TMP, "stock_vmods.tar.gz")
r = subprocess.run(
    ["gh", "codespace", "cp", "-c", CS,
     "remote:/workspaces/vxtux-build/stock_vmods.tar.gz", dst],
    capture_output=True, text=True, timeout=900)
if r.returncode == 0 and os.path.exists(dst):
    outdir = "E:/vxtux_build_repo/donors/stock_vendor_modules"
    os.makedirs(outdir, exist_ok=True)
    shutil.unpack_archive(dst, outdir, format="gztar")
    n = len([f for f in os.listdir(outdir) if f.endswith(".ko")])
    print(f"OK  extracted {n} stock .ko into {outdir}")
else:
    print(f"FAIL pull: {r.stderr.strip()[:200]}")

# 2) the vmlinux symbol table (for the =y substitutes)
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", CS, "--",
     "cd /workspaces/vxtux-build/6.18.52-VXTux-UMS512 && "
     "nm vmlinux 2>/dev/null | awk '\\$2 ~ /^[TDBRWV]$/ {print \\$3}' | sort -u "
     "> /workspaces/vxtux-build/vmlinux_syms.txt && "
     "wc -l /workspaces/vxtux-build/vmlinux_syms.txt"],
    capture_output=True, text=True, timeout=600)
print(r.stdout.strip() or r.stderr.strip()[:200])

dst2 = os.path.join(TMP, "vmlinux_syms.txt")
r = subprocess.run(
    ["gh", "codespace", "cp", "-c", CS,
     "remote:/workspaces/vxtux-build/vmlinux_syms.txt", dst2],
    capture_output=True, text=True, timeout=900)
if r.returncode == 0 and os.path.exists(dst2):
    shutil.copyfile(dst2, "E:/vxtux_build_repo/build_output/vxtux_6.18.52/"
                          "vmlinux_syms.txt")
    n = sum(1 for _ in open(dst2, encoding="utf-8", errors="replace"))
    print(f"OK  vmlinux symbols: {n:,}")
else:
    print(f"FAIL syms: {r.stderr.strip()[:200]}")
