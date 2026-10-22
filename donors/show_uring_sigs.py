import os

ROOT = "/workspaces/vxtux-build/6.18.52-VXTux-UMS512"

FILES = [
    ("include/linux/io_uring/cmd.h", None),
    ("include/linux/io_uring/net.h", None),
    ("include/linux/io_uring.h", None),
    ("kernel/Makefile", None),
]

for rel, _ in FILES:
    fp = os.path.join(ROOT, rel)
    print("=" * 70)
    print(rel, "exists:", os.path.exists(fp))
    if not os.path.exists(fp):
        continue
    with open(fp, "rb") as f:
        d = f.read()
    lines = d.split(b"\n")
    for i, l in enumerate(lines, 1):
        t = l.decode("utf-8", "replace")
        if any(k in t for k in ("io_uring_cmd_sock", "io_uring_cmd_mark_cancelable",
                                "io_uring_cmd_issue_blocking", "io_uring_cmd_do_in_task",
                                "io_uring_cmd_done", "__io_uring_cancel", "__io_uring_free",
                                "io_is_uring_fops", "uring_cmd", "obj-y", "obj-")):
            print(f"  {i}: {t}")
