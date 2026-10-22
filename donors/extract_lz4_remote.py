import os
import subprocess

REMOTE = "/workspaces/vxtux-build/stock_kernel"
LOCAL = "E:/vxtux_build_repo/donors/stock_kernel"
os.makedirs(LOCAL, exist_ok=True)

CODESPACE = "solid-tribble-9rjjxvjqq5pcpq5g"

# 1) decompress the lz4 region on the codespace
cmds = [
    f"mkdir -p {REMOTE}",
    f"dd if=/workspaces/vxtux-build/boot_a.bin of={REMOTE}/lz4.bin bs=1 skip=$((0x2454000)) count=$((40*1024*1024)) 2>/dev/null",
    f"cd {REMOTE} && lz4 -d -f lz4.bin lz4.out 2>&1 | tail -2; ls -la {REMOTE}/lz4.out 2>&1",
    f"python3 -c \"import hashlib;d=open('{REMOTE}/lz4.out','rb').read();print('size',len(d));print('sha256',hashlib.sha256(d).hexdigest());print('first32',d[:32].hex());print('arm_magic',d[56:60])\" 2>&1",
]
for c in cmds:
    r = subprocess.run(
        ["gh", "codespace", "ssh", "-c", CODESPACE, "--", c],
        capture_output=True, text=True, timeout=600)
    print(f"$ {c}")
    print((r.stdout.strip() or r.stderr.strip())[:1500])
    print("-" * 60)
