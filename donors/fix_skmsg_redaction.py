import re

path = "include/linux/skmsg.h"
with open(path, "rb") as f:
    data = f.read()

print("=== BEFORE (redacted lines) ===")
for i, line in enumerate(data.split(b"\n"), 1):
    if b"redacted" in line or b"\xc2\xab" in line or b"***" in line or b"..." in line:
        print(f"  {i}: {line!r}")

# The redaction pattern: identifier containing "sk_" was replaced by
#   - «redacted:sk_…»  (full identifier redacted, prefix kept)
#   - ***              (whole identifier redacted)
#   - sk_pso...eady    (middle of identifier elided)
#
# Recovered semantics from context:
#   sk_psock_data_ready(sk, psock)  - checks whether psock has data
#   sk_data_ready(sk)               - the sock's data_ready callback
#
data = data.replace("sk_pso...eady".encode("utf-8"), b"sk_psock_data_ready")
data = data.replace(b"***(sk, psock);", b"sk_data_ready(sk);")
data = data.replace(b"read_lock_bh(&sk->***);", b"read_lock_bh(&sk->sk_lock);")
data = data.replace(b"sk->***(sk);", b"sk->sk_data_ready(sk);")
data = data.replace(b"read_unlock_bh(&sk->***);", b"read_unlock_bh(&sk->sk_lock);")

with open(path, "wb") as f:
    f.write(data)

print("=== AFTER ===")
with open(path, "rb") as f:
    out = f.read()
for i, line in enumerate(out.split(b"\n"), 1):
    if b"redacted" in line or b"\xc2\xab" in line or b"***" in line or b"..." in line:
        print(f"  STILL BAD {i}: {line!r}")
    if b"sk_psock_data_ready" in line or b"sk_data_ready" in line or b"sk_lock" in line:
        print(f"  OK {i}: {line!r}")