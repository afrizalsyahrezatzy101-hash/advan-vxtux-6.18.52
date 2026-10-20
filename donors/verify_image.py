import hashlib
import os
import struct
import subprocess

CS = "solid-tribble-9rjjxvjqq5pcpq5g"
ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

CMDS = [
    "ls -la arch/arm64/boot/Image vmlinux",
    "file arch/arm64/boot/Image 2>/dev/null || head -c 64 arch/arm64/boot/Image | xxd | head -4",
    "python3 -c \"import struct;d=open('arch/arm64/boot/Image','rb').read(64);"
    "print('code0=0x%08x code1=0x%08x' % struct.unpack_from('<II',d,0));"
    "print('text_off=0x%x image_size=0x%x' % struct.unpack_from('<QQ',d,8));"
    "print('magic=%r' % d[56:60])\"",
    "sha256sum arch/arm64/boot/Image vmlinux",
    # module count
    "find . -name '*.ko' -newer .config 2>/dev/null | wc -l",
    "find . -name '*.ko' 2>/dev/null | wc -l",
    # kernel banner
    "strings vmlinux 2>/dev/null | grep -m 2 'Linux version'",
    # symbol count
    "nm vmlinux 2>/dev/null | wc -l",
    # module list
    "cat .modules.order 2>/dev/null | wc -l",
    "ls *.ko 2>/dev/null | wc -l",
    # any modules_install output
    "ls -la modules_install* 2>/dev/null | head -3",
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
