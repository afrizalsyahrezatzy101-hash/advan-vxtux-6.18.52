"""Can the remaining gap be closed by turning configs on?

For each DISABLED / MODULE_NOT_BUILT / NO_SYMBOL stock module:
  - find the Kconfig entry that defines the symbol
  - report its type (bool/tristate), its dependencies and its default
  - report whether the dependency chain is satisfiable
"""
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

SYMS = [
    "SPRD_AUDIO_DSP_DUMP", "SPRD_AUDIO_PIPE", "SPRD_AUDIO_SIPC",
    "SPRD_MMC_DVFS", "MMC_DVFS", "SPRD_DMAENGINE_PCM",
    "SND_SOC_SPRD_DMAENGINE_PCM", "SPRD_APIPE", "SPRD_FLASH",
    "USB_F_VSER", "SPRD_APSYS_DVFS", "SPRD_SIPC_VIRT_BUS",
    "SPRD_TIME_SYNC",
]


def main():
    r = subprocess.run([GIT, "-C", TREE, "ls-files", "*/Kconfig*"],
                       capture_output=True, text=True, timeout=300)
    kcfgs = [f.replace("\\", "/") for f in r.stdout.splitlines()
             if f.endswith("Kconfig") or f.endswith("Kconfig.arm64")]

    blob = {}
    for k in kcfgs:
        try:
            blob[k] = open(os.path.join(TREE, k), "r", encoding="utf-8",
                           errors="replace").read()
        except OSError:
            pass

    cfg = open(CFG, "r", encoding="utf-8", errors="replace").read()
    cfg_set = set()
    for line in cfg.splitlines():
        line = line.strip()
        if line.startswith("CONFIG_"):
            cfg_set.add(line[7:].split("=")[0])
        elif line.startswith("# CONFIG_") and line.endswith(" is not set"):
            cfg_set.add(line[2:line.index(" is not set")].strip())

    for sym in SYMS:
        print("=" * 72)
        print(f"### {sym}")
        found = False
        for k, txt in blob.items():
            m = re.search(r"^config " + sym + r"\s*$(.*?)^endconfig",
                          txt, re.S | re.M)
            if not m:
                continue
            found = True
            body = m.group(1)
            print(f"  {k}:")
            for line in body.splitlines():
                s = line.rstrip()
                if s.strip():
                    print(f"    {s}")
            deps = re.findall(r"depends on (.+)", body)
            for d in deps:
                for tok in re.findall(r"[A-Z][A-Z0-9_]{3,}", d):
                    if tok in cfg_set:
                        v = "y"
                        for line in cfg.splitlines():
                            if line.startswith(f"CONFIG_{tok}="):
                                v = line.split("=", 1)[1]
                                break
                        status = "OK" if v in ("y", "m") else "OFF"
                    else:
                        status = "not in .config"
                    print(f"    dep {tok:<32} {status}")
            break
        if not found:
            print("  (no Kconfig entry found)")
        # current value
        cur = "n/absent"
        for line in cfg.splitlines():
            if line.startswith(f"CONFIG_{sym}="):
                cur = line.split("=", 1)[1]
            elif line == f"# CONFIG_{sym} is not set":
                cur = "n"
        print(f"  CURRENT .config value: {cur}")
        print()


if __name__ == "__main__":
    main()
