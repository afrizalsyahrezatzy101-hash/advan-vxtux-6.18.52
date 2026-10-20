"""Restore tool-redacted identifiers in the skmsg / vsock-bpf / unix-bpf area.

Redaction shapes:
  b'sk_pso...eady'   ->  sk_psock_data_ready
  b'sk_pso...mpty'   ->  sk_psock_empty
  b'***' in expr pos ->  sk_psock_recvmsg
  b'***(sk, psock);' ->  sk_data_ready(sk)
  b'&sk->***'        ->  &sk->sk_lock
  b'sk->***(sk);'    ->  sk->sk_data_ready(sk);
"""
import os

def fix(path, rules):
    with open(path, "rb") as f:
        data = f.read()
    orig = data
    for old, new in rules:
        data = data.replace(old, new)
    if data != orig:
        with open(path, "wb") as f:
            f.write(data)
        print(f"FIXED {path}")
    else:
        print(f"nochange {path}")

fix("include/linux/skmsg.h", [
    (b"sk_pso...eady", b"sk_psock_data_ready"),
    (b"sk_pso...mpty", b"sk_psock_empty"),
    (b"read_lock_bh(&sk->***);", b"read_lock_bh(&sk->sk_lock);"),
    (b"sk->***(sk);", b"sk->sk_data_ready(sk);"),
    (b"read_unlock_bh(&sk->***);", b"read_unlock_bh(&sk->sk_lock);"),
])

fix("net/vmw_vsock/vsock_bpf.c", [
    (b"sk_pso...mpty", b"sk_psock_empty"),
    (b"sk_pso...eady", b"sk_psock_data_ready"),
    (b"copied = ***(sk, psock, msg, len, flags);",
     b"copied = sk_psock_recvmsg(sk, psock, msg, len, flags);"),
])

fix("net/unix/unix_bpf.c", [
    (b"sk_pso...mpty", b"sk_psock_empty"),
    (b"sk_pso...eady", b"sk_psock_data_ready"),
    (b"copied = ***(sk, psock, msg, len, flags);",
     b"copied = sk_psock_recvmsg(sk, psock, msg, len, flags);"),
])

print("--- residual check ---")
for p in ("include/linux/skmsg.h", "net/vmw_vsock/vsock_bpf.c", "net/unix/unix_bpf.c"):
    with open(p, "rb") as f:
        d = f.read()
    for i, l in enumerate(d.split(b"\n"), 1):
        if b"..." in l or b"***" in l or b"\xc2\xab" in l:
            print(f"RESIDUAL {p}:{i} {l.strip().hex()}")
print("ok")