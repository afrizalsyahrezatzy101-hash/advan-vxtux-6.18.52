import os
import shutil
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
TMP = os.environ.get("TMPDIR", "/tmp")
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

# 1) vmlinux exported symbols (avoid awk quoting hell - use nm + cut)
script = (
    "cd " + ROOT + " && "
    "nm vmlinux 2>/dev/null | grep -E ' [TDBRWV] ' | cut -d' ' -f3 "
    "| sort -u > /workspaces/vxtux-build/vmlinux_syms.txt && "
    "wc -l /workspaces/vxtux-build/vmlinux_syms.txt"
)
r = subprocess.run(["gh", "codespace", "ssh", "-c", CS, "--", script],
                   capture_output=True, text=True, timeout=600)
print("syms:", r.stdout.strip() or r.stderr.strip()[:200])

dst = os.path.join(TMP, "vmlinux_syms.txt")
r = subprocess.run(["gh", "codespace", "cp", "-c", CS,
                    "remote:/workspaces/vxtux-build/vmlinux_syms.txt", dst],
                   capture_output=True, text=True, timeout=900)
if r.returncode == 0 and os.path.exists(dst):
    shutil.copyfile(dst, "E:/vxtux_build_repo/build_output/vxtux_6.18.52/"
                          "vmlinux_syms.txt")
    n = sum(1 for _ in open(dst, encoding="utf-8", errors="replace"))
    print(f"OK  vmlinux symbols: {n:,}")
else:
    print("FAIL syms:", r.stderr.strip()[:200])

# 2) the FULL stock vendor module list from the ROM dump
for path in ("/workspaces/vxtux-build/stock_kernel/vmods",
             "/workspaces/vxtux-build/stock_modules"):
    r = subprocess.run(
        ["gh", "codespace", "ssh", "-c", CS, "--",
         f"ls {path}/*.ko 2>/dev/null | wc -l; "
         f"find /workspaces/vxtux-build -name '*.ko' 2>/dev/null | wc -l"],
        capture_output=True, text=True, timeout=300)
    print(f"probe {path}: {r.stdout.strip() or r.stderr.strip()[:200]}")
