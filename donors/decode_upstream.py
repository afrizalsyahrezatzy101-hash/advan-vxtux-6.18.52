"""Decode upstream 6.18 skmsg.h / unix_bpf.c / vsock_bpf.c and print the
exact upstream text for the identifiers that were tool-redacted locally.

Runs inside the codespace where the pristine upstream files were downloaded.
"""
import binascii
import sys

def dump(path, ranges):
    with open(path, "rb") as f:
        lines = f.read().split(b"\n")
    for a, b in ranges:
        print(f"--- {path} lines {a}-{b} ---")
        for i in range(a - 1, min(b, len(lines))):
            print(f"{i+1}: {lines[i].decode('utf-8', 'replace')}")

if __name__ == "__main__":
    what = sys.argv[1] if len(sys.argv) > 1 else "all"
    if what in ("skmsg", "all"):
        dump("/tmp/up_skmsg.h", [(470, 490), (140, 160)])
    if what in ("unix", "all"):
        dump("/tmp/up_unix_bpf.c", [(60, 100)])
    if what in ("vsock", "all"):
        dump("/tmp/up_vsock_bpf.c", [(60, 110)])
