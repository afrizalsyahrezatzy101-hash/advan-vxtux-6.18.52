import os
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
OUT = "E:/vxtux_build_repo/build_output/vxtux_6.18.52"

CMDS = [
    # what boot tools does the tree have?
    "ls tools/ 2>/dev/null | head -20",
    "find . -name 'mkbootimg*' -o -name 'unpack_bootimg*' 2>/dev/null | head -10",
    # is there a dtb?
    "find . -name '*.dtb' 2>/dev/null | head -10",
    "ls arch/arm64/boot/dts/ 2>/dev/null | head -10",
    # is there an existing boot image in the repo?
    "ls /workspaces/vxtux-build/*.img /workspaces/vxtux-build/boot* 2>/dev/null | head -10",
    # what does the stock boot image header look like (for repacking)?
    "python3 -c \"import struct;d=open('/workspaces/vxtux-build/boot_a.bin','rb').read(64);"
    "print(d[:64].hex())\"",
]


def run(cmd):
    r = subprocess.run(
        ["gh", "codespace", "ssh", "-c", CS, "--", f"cd {ROOT} && {cmd}"],
        capture_output=True, text=True, timeout=300)
    return (r.stdout.strip() or r.stderr.strip() or "(none)")


for c in CMDS:
    print(f"$ {c[:90]}")
    print(run(c))
    print("-" * 70)
