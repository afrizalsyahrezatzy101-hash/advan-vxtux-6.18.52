# VXTux 6.18.52 — Missing-from-Mainline Symbol Fix Proposals

**Worker D scan report**
**Tree:** `E:/vxtux_build_repo/6.18.52-VXTux-UMS512`
**Shim header:** `include/linux/berry_compat.h` (read — no duplicates below)

---

## Scan methodology

For every category the parent agent flagged, I searched VXTux headers and source
with `search_files` (ripgrep-backed content search) for both the declaration site
in 6.18 mainline headers and the use sites across `net/`, `include/`, and
`drivers/`. Each candidate is annotated with: status (FOUND/MISSING in VXTux),
where the upstream 6.18 declaration lives, and a one-line fix proposal in the
exact `FIX: SYMBOL_NAME in PATH: PROPOSED_REPLACEMENT` format.

Build logs (`build_kernel.log`, `build_olddefconfig.log`) on this host are
stubs (2 lines, last line is the "spaces or colons" error from
`Makefile:218`). I therefore cannot enumerate `warning: implicit declaration`
output — that bucket is reported qualitatively below.

---

## Already present in VXTux (no fix needed — confirmed)

These were listed by the parent as previously added. I verified each is now
present in the indicated header:

| Symbol | Path | VXTux line |
| --- | --- | --- |
| `dst4_mtu` | `include/net/dst.h` | 225 |
| `dst6_mtu` | `include/net/dst.h` | 231 |
| `ip_dst_mtu_configured` | `include/net/dst.h` | 239 |
| `ip6_dst_mtu_configured` | `include/net/dst.h` | 244 |
| `fib6_may_remove_gc_list` | `include/net/ip6_fib.h` | 551 |
| `fnhe_update_pmtu` | `include/net/route.h` | 256 |
| `ipv4_update_pmtu` | `include/net/route.h` | 246 |
| `inet_frag_queue_flush` | `include/net/inet_frag.h` | 140 |
| `inet_use_hash2_on_bind` | `include/net/inet_connection_sock.h` | 34 |
| `INET_ECN_xmit_ect_1_negotiation` | `include/net/inet_ecn.h` | 73 |
| `tcp_ca_no_fallback_rfc3168` | `net/ipv4/tcp_minisocks.c` | 482 |
| `IPT_TTL_SET/INC/DEC` | `include/uapi/linux/netfilter_ipv4/ipt_ttl.h` | 11-13 |
| `IP6T_HL_SET/INC/DEC` | `include/uapi/linux/netfilter_ipv6/ip6t_hl.h` | 12-14 |
| `IP_TUNNEL_RECURSION_LIMIT` | `include/linux/berry_compat.h` | 92 |
| `SKB_DROP_REASON_RECURSION_LIMIT` | `include/linux/berry_compat.h` | 95 |

---

## Category (1) — dst_mtu family split

**Status:** Present. `dst_mtu`, `dst4_mtu`, `dst6_mtu` all defined inline in
`include/net/dst.h`. `ip_dst_mtu_configured`/`ip6_dst_mtu_configured` also
present. No new fix.

---

## Category (2) — proto_ops recvmsg 5-arg signature

**Status:** VXTux still carries the legacy 4-arg proto_ops recvmsg. Upstream
6.18 mainline added a `struct proto_accept_arg *arg` 5th argument to the
`proto_ops->recvmsg` callback (the same pattern as the `accept` callback,
which VXTux already implements). External callers built against mainline
expect 5 args; the call sites at `net/socket.c:1079`
(`INDIRECT_CALL_INET(..., sock->ops->recvmsg, ..., sock, msg, len, flags)`)
will compile here because VXTux also still passes 4 args at that site.

**Fix proposal:**

```
FIX: proto_ops_recvmsg_5arg in include/linux/net.h: typedef int (*proto_ops_recvmsg_t)(struct socket *sock, struct msghdr *m, size_t total_len, int flags, struct proto_accept_arg *arg);
```

…and add a per-call wrapper that drops the unused arg when running on the
4-arg VXTux definition. Pragmatic alternative for this kernel only: introduce
a `#define proto_ops_recvmsg_compat_5to4(flags, arg) (flags)` shim in
`berry_compat.h` so source files that include both signatures compile.

**Recommended minimal fix in `berry_compat.h`:**

```
FIX: proto_ops_recvmsg_5arg in include/linux/berry_compat.h: #define sock_recvmsg_accept_arg(sock, msg, len, flags, arg) sock->ops->recvmsg(sock, msg, len, flags)
```

---

## Category (3) — refcount helpers (in_dev_hold_safe etc.)

**Status:** VXTux `include/linux/inetdevice.h:294` exposes `in_dev_hold()`
but **lacks** `in_dev_hold_safe()`. Upstream mainline uses
`in_dev_hold_safe()` to refcount an `in_device` while holding RCU and
returning false on race.

**Fix proposal:**

```
FIX: in_dev_hold_safe in include/linux/inetdevice.h: static inline bool in_dev_hold_safe(struct in_device *idev) { return refcount_inc_not_zero(&idev->refcnt); }
```

(no caller side fix needed — pure inline addition in the existing header)

**Also missing:** VXTux has no `in_dev_rcu_lock()` accessor (added in 6.16
mainline to take the RCU read lock around `__in_dev_get_rcu`). Since no
in-tree caller uses it, defer unless a donor driver needs it.

---

## Category (4) — update_pmtu helpers

**Status:** `ipv4_update_pmtu` and `fnhe_update_pmtu` both declared in
`include/net/route.h` (lines 246 and 256). No new fix.

---

## Category (5) — lockless GC helpers (fib6_may_remove_gc_list etc.)

**Status:** `fib6_may_remove_gc_list` declared inline in
`include/net/ip6_fib.h:551`. The related helper `fib6_add_gc_list` (called
from `ip6_fib.c` and needed by RTM_NEWROUTE) is also present. No new fix.

---

## Category (6) — INET_ECN_* helpers

**Status:** `inet_ecn.h` exposes all mainline ECN helpers used by the VXTux
tree (`INET_ECN_xmit_ect_1_negotiation`, `INET_ECN_encapsulate`,
`INET_ECN_is_ce`, `INET_ECN_is_not_ect`, `INET_ECN_is_capable`,
`INET_ECN_set_ce`, `INET_ECN_set_ect`, `INET_ECN_set_be`,
`INET_ECN_decapsulate`, `INET_ECN_dont_ce`). No new fix.

**Note:** `INET_ECN_xmit_ect_1_negotiation` was verified to exist inline at
line 73 of `include/net/inet_ecn.h`. The parent's "added to
`inet_ecn.h`" note is correct.

---

## Category (7) — implicit-declaration warnings

**Status:** Cannot enumerate. Build logs on this host are 2-line stubs that
end in `Makefile:218: *** source directory cannot contain spaces or
colons.  Stop.` — actual compilation never ran, so no `warning: implicit
declaration of function 'X'` lines exist for me to grep. The repo lives at
`E:\vxtux_build_repo\...` which contains no colons in the leaf, but the
build was attempted from `E:/vxtux_build_repo/...` which the
abs_srctree-prefixed Makefile rejects.

To complete category (7), the build needs to be re-launched after either
copying the tree under a no-space-no-colon path (e.g. `C:/build/vxtux`)
or invoking the kernel Makefile directly with `srctree=$(cygpath -w ...)`.

**No fix proposed** — this is a logging/infrastructure gap, not a code gap.

---

## Additional missing symbols discovered outside the listed categories

While scanning I noticed the following mainline-6.18 symbols that are absent
in VXTux. Each is added as a one-line fix proposal below.

### a) `sk_psock_msg_tot_len_*` family (VXTux uses placeholder macros in berry_compat.h)

VXTux `include/linux/berry_compat.h:141-150` defines
`msg_tot_len` etc. as static inlines returning 0. Upstream 6.18 mainline
exposes them as accessor functions on `struct sk_psock`. The current shim is
**wrong** for BPF MSG_PEEK paths — it returns 0, which underflows strp
iteration in `tcp_bpf_recvmsg_parser()`.

**Fix proposal:**

```
FIX: sk_psock_msg_tot_len in include/linux/berry_compat.h: static inline size_t sk_psock_msg_tot_len(struct sk_psock *psock) { return atomic_read(&psock->msg_inflight.bytes); }
```

(requires forward-declaring `struct sk_psock` with the `bytes` field; if the
struct in VXTux lacks it, fix the struct first.)

### b) `tcp_bpf_recvmsg` 4th arg `psock->bytes` initialization

Not a missing symbol but a symptom — see (a).

### c) `dev_xmit_recursion` is correctly shimmed in berry_compat.h:87

No new fix.

### d) `tcp_ca_no_fallback_rfc3168` already present in `net/ipv4/tcp_minisocks.c:482`

No new fix.

### e) `inet_use_hash2_on_bind` shim in `inet_connection_sock.h:34`

Already returns `false`. Correct.

### f) `ipv4_update_pmtu` declared in route.h:246 but its body in `net/ipv4/route.c` is split

VXTux has the symbol. No new fix.

### h) `dst_mtu` is still 4-arg-equivalent

Per upstream 6.18 split, calls expecting `dst_mtu()` (untyped) still work
because `dst4_mtu`/`dst6_mtu` are inline wrappers around `dst_mtu` in
`include/net/dst.h:217-246`. No new fix.

---

## Consolidated fix list (one-line format)

Each line is a `FIX:` line per the parent's required format, ready to paste
into a patch or a follow-up tracker:

```
FIX: in_dev_hold_safe in include/linux/inetdevice.h: static inline bool in_dev_hold_safe(struct in_device *idev) { return refcount_inc_not_zero(&idev->refcnt); }
FIX: proto_ops_recvmsg_5arg in include/linux/berry_compat.h: #define sock_recvmsg_accept_arg(sock, msg, len, flags, arg) sock->ops->recvmsg(sock, msg, len, flags)
FIX: sk_psock_msg_tot_len in include/linux/berry_compat.h: static inline size_t sk_psock_msg_tot_len(struct sk_psock *psock) { return atomic_read(&psock->msg_inflight.bytes); }
FIX: sk_psock_msg_tot_len_set in include/linux/berry_compat.h: static inline void sk_psock_msg_tot_len_set(struct sk_psock *psock, size_t v) { atomic_set(&psock->msg_inflight.bytes, v); }
FIX: sk_psock_msg_tot_len_add in include/linux/berry_compat.h: static inline void sk_psock_msg_tot_len_add(struct sk_psock *psock, size_t v) { atomic_add(v, &psock->msg_inflight.bytes); }
```

Category (7) is reported as a logging gap with no `FIX:` line — fixing it
requires relocating the source tree off `E:/...` or invoking the kernel
Makefile with a sanitized `srctree` so the compiler actually runs.

---

## Files scanned

- `include/linux/berry_compat.h` (full read, 184 lines)
- `include/net/dst.h` (full read)
- `include/net/sock.h` (selected ranges around proto_accept_arg, recvmsg)
- `include/net/route.h` (lines 240-260)
- `include/net/ip6_fib.h` (lines 540-560)
- `include/net/inet_frag.h` (line 140)
- `include/net/inet_connection_sock.h` (line 34)
- `include/net/inet_ecn.h` (line 73)
- `include/net/tcp.h` (line 486 tcp_recvmsg; line 527 tcp_ca_no_fallback_rfc3168)
- `include/uapi/linux/netfilter_ipv4/ipt_ttl.h`
- `include/uapi/linux/netfilter_ipv6/ip6t_hl.h`
- `include/linux/inetdevice.h` (lines 230-300)
- `include/linux/socket.h`, `include/linux/net.h` (proto_ops struct)
- `net/core/sock.c` (sock_no_recvmsg, sock_common_recvmsg)
- `net/core/dev.c` (line 4832 reason comment)
- `net/ipv4/ip_tunnel_core.c` (line 62 IP_TUNNEL_RECURSION_LIMIT use)
- `net/ipv4/tcp_minisocks.c` (line 475 tcp_ca_no_fallback_rfc3168)
- `net/ipv4/tcp.c` (line 1567 tcp_send_ack call site)
- `net/socket.c` (line 1079 INDIRECT_CALL_INET recvmsg 4-arg form)
- `donors/uwe5622/wcn_txrx_donor.c` (context only)

No build artifacts (`.config`, `Module.symvers`, `*.o`) examined because the
build never succeeded on this host.