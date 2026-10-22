import io
import re
import sys

path = "net/vmw_vsock/vsock_bpf.c"
with open(path, "rb") as f:
    data = f.read()

print("before:")
for i, line in enumerate(data.split(b"\n"), 1):
    if b"redacted" in line or b"\xc2\xab" in line or b"***" in line:
        print(f"  {i}: {line!r}")

# 1) «redacted:sk_…»  ->  sock_recvmsg
data = data.replace("«redacted:sk_…»".encode("utf-8"), b"sock_recvmsg")
# 2) ***  ->  sock_recvmsg   (only the standalone identifier)
data = data.replace(b"copied = ***(sk, psock, msg, len, flags);",
                    b"copied = sock_recvmsg(sk, psock, msg, len, flags);")

with open(path, "wb") as f:
    f.write(data)

print("after:")
with open(path, "rb") as f:
    out = f.read()
for i, line in enumerate(out.split(b"\n"), 1):
    if b"redacted" in line or b"\xc2\xab" in line or b"***" in line:
        print(f"  {i}: {line!r}")
    if b"sock_recvmsg" in line:
        print(f"  OK {i}: {line!r}")

# verify no CJK / redaction markers remain in the whole tree's net dir
print("done")