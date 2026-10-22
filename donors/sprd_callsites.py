"""Map every internal call site of the 9 missing sdhci-sprd symbols.

Reads the objdump -d -r output of the stock blob, finds the "bl <addr>"
instructions that target each of the 9 functions, and reports which
function each call site sits in.

Usage:
    python3 donors/sprd_callsites.py
"""
import bisect
import re
import subprocess
import sys

NM = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
      "bin/aarch64-none-linux-gnu-nm.exe")
OBJDUMP = ("C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/"
           "bin/aarch64-none-linux-gnu-objdump.exe")
KO = "E:/vxtux_build_repo/donors/stock_vendor_modules/sdhci-sprd.ko"

# the 9 symbols the port is missing, with their addresses in the stock blob
TARGETS = {
    0x0030: "sdhci_sprd_request_sync",
    0x4a2c: "_sdhci_request_atomic",
    0x4af0: "mmc_hsq_swcq_init",
    0x4b78: "sdhci_sprd_irq_request_swcq",
    0x58cc: "mmc_check_wp_fn",
    0x5978: "mmc_set_powp",
    0x60f0: "mmc_wp_init",
    0x68ac: "mmc_wp_remove",
    0x6990: "mmc_send_tuning_read",
}


def main():
    dis = subprocess.run([OBJDUMP, "-d", "-r", "--section=.text", KO],
                         capture_output=True, text=True).stdout
    nm = subprocess.run([NM, "-S", KO], capture_output=True, text=True).stdout

    funcs = []
    for line in nm.splitlines():
        p = line.split()
        if len(p) == 4 and p[2] in "Tt":
            funcs.append((int(p[0], 16), int(p[1], 16), p[3]))
    funcs.sort()
    starts = [f[0] for f in funcs]

    def owner(addr):
        i = bisect.bisect_right(starts, addr) - 1
        if i < 0:
            return "?"
        s, sz, n = funcs[i]
        return n if addr < s + sz else "?"

    found = {}
    for line in dis.splitlines():
        m = re.match(r"^\s*([0-9a-f]{4,8}):\s+\S+\s+bl\s+([0-9a-f]+)\s", line)
        if not m:
            continue
        addr = int(m.group(1), 16)
        tgt = int(m.group(2), 16)
        if tgt in TARGETS:
            found.setdefault(TARGETS[tgt], []).append((addr, owner(addr)))

    for name in sorted(TARGETS.values()):
        sites = found.get(name, [])
        if not sites:
            print(f"{name:32s} (no internal call site - it is an API entry point)")
        for a, o in sites:
            print(f"{name:32s} called at 0x{a:04x} inside {o}")


if __name__ == "__main__":
    main()
