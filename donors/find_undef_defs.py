import os
import re
import sys

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"
SYMS = """LL_RESERVED_SPACE_EX __io_uring_cancel __io_uring_cmd_do_in_task __io_uring_cmd_done
__io_uring_free __net_mp_close_rxq __net_mp_open_rxq __nf_ct_ext_find amba_bustype
io_is_uring_fops io_uring_cmd_issue_blocking io_uring_cmd_mark_cancelable io_uring_cmd_sock
ip_tunnel_init ip_tunnel_limit_headroom kzalloc_flex minidump_save_extend_information
nf_conncount_count past_receiver_capable past_sender_capable secure_tcpv6_seq
secure_tcpv6_ts_off tcp_chrono_start usb_add_phy usb_remove_phy""".split()

EXTS = (".c", ".h", ".S", ".lds")
SKIP = {".git", "Documentation", "tools/testing", "samples", "drivers/gpu/drm/panfrost"}

# a definition looks like:  <type> name(   or  #define name
DEF_RE = re.compile(rb"^\s*(?:(?:static\s+|extern\s+|const\s+|unsigned\s+|struct\s+|inline\s+)*[A-Za-z_][A-Za-z0-9_ \*]*\s+)([A-Za-z_][A-Za-z0-9_]*)\s*\(")
DEFINE_RE = re.compile(rb"^\s*#\s*define\s+([A-Za-z_][A-Za-z0-9_]*)")
EXPORT_RE = re.compile(rb"EXPORT_SYMBOL[A-Z_]*\s*\(\s*([A-Za-z_][A-Za-z0-9_]*)")

wanted = set(SYMS)
found = {s: [] for s in SYMS}

for dirpath, dirnames, filenames in os.walk(ROOT):
    dirnames[:] = [d for d in dirnames if d not in SKIP]
    for fn in filenames:
        if not fn.endswith(EXTS):
            continue
        p = os.path.join(dirpath, fn)
        try:
            with open(p, "rb") as f:
                lines = f.read().split(b"\n")
        except OSError:
            continue
        for i, l in enumerate(lines, 1):
            for rx in (DEF_RE, DEFINE_RE, EXPORT_RE):
                m = rx.search(l)
                if m:
                    name = m.group(1).decode()
                    if name in wanted:
                        found[name].append(f"{os.path.relpath(p, ROOT)}:{i}")
                    break

print("=== DEFINITION SEARCH ===")
for s in SYMS:
    hits = found[s]
    if hits:
        print(f"  {s}: {len(hits)} def(s) -> {hits[:3]}")
    else:
        print(f"  {s}: *** NO DEFINITION FOUND ***")
