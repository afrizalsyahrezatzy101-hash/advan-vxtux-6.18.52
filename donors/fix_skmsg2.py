path = "include/linux/skmsg.h"
with open(path, "rb") as f:
    data = f.read()

# recover identifiers redacted by the tool
data = data.replace(b"sk_pso...eady", b"sk_psock_data_ready")
data = data.replace(b"read_lock_bh(&sk->***);", b"read_lock_bh(&sk->sk_lock);")
data = data.replace(b"sk->***(sk);", b"sk->sk_data_ready(sk);")
data = data.replace(b"read_unlock_bh(&sk->***);", b"read_unlock_bh(&sk->sk_lock);")

with open(path, "wb") as f:
    f.write(data)

with open(path, "rb") as f:
    out = f.read()
lines = out.split(b"\n")
for i in range(470, 486):
    print(i + 1, repr(lines[i]))
print("--- remaining markers ---")
for i, line in enumerate(lines, 1):
    if b"***" in line or b"..." in line or b"redacted" in line:
        print("BAD", i, repr(line))