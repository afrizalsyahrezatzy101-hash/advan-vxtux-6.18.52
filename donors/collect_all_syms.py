"""Collect every symbol the build provides: vmlinux plus all modules.

Why
---
donors/verify_deblob2.py checked each replacement module's imports
against the built vmlinux only, and reported everything a MODULE
provides as "not in the built vmlinux":

  - `ums512-clk`: missing sprd_clk_probe, sprd_pll_ops, ...
  - `trusty-ipc`: missing tipc_chan_connect, ...
  - `trusty-irq`: missing trusty_fast_call32, ...

All of those are exported by modules, not by vmlinux:

  $ nm built_kos/drivers/trusty/trusty-ipc.ko | grep tipc_chan_connect
  000000000000206c T tipc_chan_connect

A module's symbols live in the .ko, not in vmlinux, so the check has
to look at both. The stock blob's import is satisfied if the symbol is
in the vmlinux OR in any module the build produces.

What it writes
--------------
  donors/vmlinux_syms.txt     the vmlinux symbols (unchanged)
  donors/all_syms.txt         vmlinux + every module's symbols
"""
import os
import re
import subprocess
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
BUILT = "E:/vxtux_build_repo/built_kos"
OUT = "E:/vxtux_build_repo/donors/all_syms.txt"
VML = "E:/vxtux_build_repo/donors/vmlinux_syms.txt"

NM = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
      "bin/aarch64-none-linux-gnu-nm.exe")


def nm_syms(path):
    """the global symbols of an object: T, D, B, R, W, V, G"""
    try:
        out = subprocess.run([NM, "-g", "--defined-only", path],
                             capture_output=True, text=True, timeout=60)
    except Exception as e:
        print("  nm failed:", e)
        return set()
    syms = set()
    for line in out.stdout.splitlines():
        p = line.split()
        if len(p) >= 2 and p[-2] and p[-2][0].isupper():
            syms.add(p[-1])
    return syms


def main():
    vml = set()
    if os.path.exists(VML):
        with open(VML, encoding="utf-8", errors="replace") as f:
            for line in f:
                s = line.strip()
                if s:
                    vml.add(s.split()[-1] if " " in s else s)
    print(f"vmlinux symbols: {len(vml):,}")

    mods = set()
    n = 0
    for root, _, fs in os.walk(BUILT):
        for fn in fs:
            if fn.endswith(".ko"):
                n += 1
                mods |= nm_syms(os.path.join(root, fn))
    print(f"modules: {n}, their global symbols: {len(mods):,}")

    allsyms = vml | mods
    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, "w", encoding="utf-8", newline="\n") as f:
        for s in sorted(allsyms):
            f.write(s + "\n")
    print(f"total: {len(allsyms):,} -> {OUT}")

    # the symbols the strict report was complaining about
    print()
    print("the ones the report called missing:")
    for s in ["sprd_clk_probe", "sprd_pll_ops", "tipc_chan_connect",
              "trusty_fast_call32", "trusty_call_notifier_register",
              "trusty_std_call32", "trusty_version_str_get",
              "trusty_enqueue_nop", "sprd_clk_regmap_init",
              "sprd_mux_ops", "sprd_div_ops", "sprd_gate_ops",
              "sprd_comp_ops", "sprd_sc_gate_ops", "sprd_pll_sc_gate_ops",
              "tipc_create_channel", "tipc_chan_destroy",
              "tipc_chan_get_txbuf_timeout", "tipc_chan_put_rxbuf",
              "tipc_chan_put_txbuf", "tipc_chan_queue_msg",
              "tipc_chan_shutdown"]:
        where = "vmlinux" if s in vml else ("module" if s in mods else "NOWHERE")
        print(f"  {s:34s} {where}")


if __name__ == "__main__":
    sys.exit(main())
