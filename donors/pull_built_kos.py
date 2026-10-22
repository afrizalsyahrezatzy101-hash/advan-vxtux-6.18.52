"""Pull the built .ko files from the codespace so their symbols can be
compared against the stock vendor blobs.

The whole module tree is tarred on the remote side and fetched in one
go, because gh codespace cp per file is slow.

Usage:
    python3 pull_built_kos.py [outdir]
"""
import os
import shutil
import subprocess
import sys

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
TMP = os.environ.get("TMPDIR", "/tmp")
OUT = sys.argv[1] if len(sys.argv) > 1 else \
    "E:/vxtux_build_repo/build_output/vxtux_6.18.52/ko"

# 1) tar the .ko tree on the remote side
r = subprocess.run(
    ["gh", "codespace", "ssh", "-c", CS, "--",
     f"cd {ROOT} && find . -name '*.ko' | tar -czf "
     "/workspaces/vxtux-build/ko87.tar.gz -T - && "
     "ls -l /workspaces/vxtux-build/ko87.tar.gz | awk '{print $5}'"],
    capture_output=True, text=True, timeout=900)
size = r.stdout.strip()
print("remote tar:", size or r.stderr.strip()[:200])
if not size.isdigit():
    raise SystemExit(1)

# 2) fetch it
flat = os.path.join(TMP, "ko87.tar.gz")
r = subprocess.run(["gh", "codespace", "cp", "-c", CS,
                    "remote:/workspaces/vxtux-build/ko87.tar.gz", flat],
                   capture_output=True, text=True, timeout=1800)
if r.returncode != 0 or not os.path.exists(flat):
    print("FAIL cp:", r.stderr.strip()[:200])
    raise SystemExit(1)
got = os.path.getsize(flat)
print(f"fetched {got:,} bytes (remote said {int(size):,})")
if got != int(size):
    print("!! size mismatch - the tarball is truncated")
    raise SystemExit(1)

# 3) unpack locally
if os.path.isdir(OUT):
    shutil.rmtree(OUT)
os.makedirs(OUT, exist_ok=True)
r = subprocess.run(["tar", "xzf", flat, "-C", OUT],
                   capture_output=True, text=True, timeout=900)
if r.returncode != 0:
    print("FAIL untar:", r.stderr.strip()[:300])
    raise SystemExit(1)

n = 0
for root, _, fs in os.walk(OUT):
    n += sum(1 for f in fs if f.endswith(".ko"))
print(f"OK  {n} .ko files -> {OUT}")
