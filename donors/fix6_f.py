"""Enable the config symbols that build the 12 still-uncovered stock
vendor blobs, and make the ones that build a directory-module emit a
.ko named exactly like the stock blob.

Both changes are needed for the deblob to be a real drop-in: the
loader insmods 'novatek_ts_spi.ko', so the build must produce a file
with that name, and it must be a MODULE (not built-in) for insmod to
work at all.

A) CONFIG symbols to turn on (=m where the Kconfig is tristate):

     CONFIG_SPRD_APSYS_DVFS          already =m
     CONFIG_SPRD_AUDIO_DSP_DUMP      tristate, currently absent -> =m
     CONFIG_SPRD_AUDIO_SIPC          BOOL in the Kconfig -> has to
                                     become tristate, otherwise it can
                                     only be =y and can never be a .ko
     CONFIG_NVMEM_SPRD_CACHE_EFUSE   =y -> =m
     CONFIG_SPRD_VSP_PW_DOMAIN       =y -> =m

B) Composite .ko renames (the tree's established pattern):

     drivers/soc/sprd/cache_efuse/Makefile
         sprd_cache_efuse.o  ->  nvmem_sprd_cache_efuse.ko
     drivers/pmdomain/sprd/Makefile
         sprd_vsp_pw_domain.o + vsp_regs.o  ->  sprd-vsp-pw-domain.ko

   The pmdomain one is careful: sprd_camsys_pw_domain.o and
   sprd_gpu_pw_domain.o share that Makefile and must stay separate
   modules.

Usage:
    python3 fix6_f.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def set_cfg(d, sym, val):
    """Set CONFIG_x=val in a .config body."""
    pat = re.compile(r"^" + re.escape(sym) + r"=.*$", re.M)
    off = re.compile(r"^# " + re.escape(sym) + r" is not set$", re.M)
    if pat.search(d):
        return pat.sub(f"{sym}={val}", d), f"{sym} -> {val}"
    if off.search(d):
        return off.sub(f"{sym}={val}", d), f"{sym} (was off) -> {val}"
    # append: .config is order sensitive but kbuild re-sorts, so a
    # trailing line is still honoured by oldconfig
    return d + f"\n{sym}={val}\n", f"{sym} (new) -> {val}"


def main():
    print("== A) .config symbols ==")
    cfg_path = os.path.join(TREE, ".config")
    d = read(cfg_path)
    orig = d
    for sym, val in [("CONFIG_SPRD_APSYS_DVFS", "m"),
                     ("CONFIG_SPRD_AUDIO_DSP_DUMP", "m"),
                     ("CONFIG_SPRD_AUDIO_SIPC", "m"),
                     ("CONFIG_NVMEM_SPRD_CACHE_EFUSE", "m"),
                     ("CONFIG_SPRD_VSP_PW_DOMAIN", "m")]:
        d, msg = set_cfg(d, sym, val)
        print(f"   {msg}")

    if APPLY and d != orig:
        write(cfg_path, d)
        print(f"   WROTE .config ({len(d):,} bytes)")
    elif not APPLY:
        print("   (dry run)")

    # ------------------------------------------------------------------
    # CONFIG_SPRD_AUDIO_SIPC is 'bool' in the Kconfig, so it can never
    # be =m. Make it tristate so the audio_sipc / audio-pipe .ko can
    # exist as modules.
    # ------------------------------------------------------------------
    print()
    print("== B) Kconfig: CONFIG_SPRD_AUDIO_SIPC bool -> tristate ==")
    kp = os.path.join(TREE, "drivers/vxtux/audio/Kconfig")
    kt = read(kp)
    # this Kconfig is flat (no endconfig), so match the block up to the
    # next 'config ' line
    m = re.search(r"^config SPRD_AUDIO_SIPC\s*$\n(.*?)(?=^config |\Z)",
                  kt, re.S | re.M)
    if m:
        body = m.group(1)
        if "\ttristate" not in body:
            newbody = re.sub(r"^\tbool\b", "\ttristate", body,
                             count=1, flags=re.M)
            kt2 = kt[:m.start(1)] + newbody + kt[m.end(1):]
            if APPLY:
                write(kp, kt2)
                print("   SPRD_AUDIO_SIPC: bool -> tristate")
            else:
                print("   (dry run) SPRD_AUDIO_SIPC: bool -> tristate")
        else:
            print("   already tristate")
    else:
        print("   !! config block not found")

    # SPRD_AUDIO_SIPC depends on SPRD_AUDIO_MEMORY_MANAGER, which has
    # to be on for the symbol to be settable at all
    m2 = re.search(r"^config SPRD_AUDIO_MEMORY_MANAGER\s*$\n(.*?)"
                   r"(?=^config |\Z)", kt, re.S | re.M)
    if m2:
        b2 = m2.group(1)
        k2 = "bool" if re.search(r"^\tbool\b", b2, re.M) else "tristate"
        d2, msg2 = set_cfg(d, "CONFIG_SPRD_AUDIO_MEMORY_MANAGER", "y")
        print(f"   {msg2}  (type={k2})")

    # ------------------------------------------------------------------
    # C) composite .ko renames
    # ------------------------------------------------------------------
    print()
    print("== C) composite .ko renames ==")
    jobs = [
        # (Makefile, symbol, new .ko name, objects)
        ("drivers/soc/sprd/cache_efuse/Makefile",
         "CONFIG_NVMEM_SPRD_CACHE_EFUSE", "nvmem_sprd_cache_efuse",
         ["sprd_cache_efuse.o"]),
        ("drivers/pmdomain/sprd/Makefile",
         "CONFIG_SPRD_VSP_PW_DOMAIN", "sprd-vsp-pw-domain",
         ["sprd_vsp_pw_domain.o", "vsp_regs.o"]),
    ]
    for rel, sym, ko, objs in jobs:
        p = os.path.join(TREE, rel)
        txt = read(p)
        lines = txt.split("\n")
        out = []
        dropped = []
        for line in lines:
            m = re.match(r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*(\S+)"
                         r"\s*$", line)
            if m and m.group(1) in objs:
                dropped.append(m.group(1))
                continue
            out.append(line)
        if not dropped:
            print(f"   !! {rel}: no obj- line for {sym} among {objs}")
            continue
        head = [
            "# SPDX-License-Identifier: GPL-2.0",
            "#",
            f"# ONE composite module, named after the vendor blob it",
            f"# replaces ({ko}.ko). The objects used to be listed one by",
            "# one, which made kbuild emit a module per object; the",
            "# composite form keeps them in a single .ko with the stock",
            "# file name, so the loader can insmod the same file.",
            "",
        ]
        new = [f"obj-$(CONFIG_{sym.replace('CONFIG_', '')}) += {ko}.o",
               f"{ko}-y := " + " ".join(objs)]
        # put the new block where the first obj- line was
        idx = None
        for i, l in enumerate(out):
            if l.startswith("obj-$("):
                idx = i
                break
        if idx is None:
            out = new + out
        else:
            out = out[:idx] + new + out[idx:]
        nt = "\n".join(head + out)
        if not nt.endswith("\n"):
            nt += "\n"
        if APPLY:
            write(p, nt)
            print(f"   {rel} -> {ko}.ko ({', '.join(dropped)})")
        else:
            print(f"   (dry run) {rel} -> {ko}.ko")

    # ------------------------------------------------------------------
    # D) the parent Makefiles must stop listing the subdir as a
    #    directory module, else there are two modules
    # ------------------------------------------------------------------
    print()
    print("== D) parent Makefiles ==")
    parents = [
        ("drivers/soc/sprd/Makefile", "CONFIG_NVMEM_SPRD_CACHE_EFUSE",
         "cache_efuse"),
        ("drivers/pmdomain/Makefile", "CONFIG_SPRD_VSP_PW_DOMAIN", "sprd"),
    ]
    for rel, sym, sub in parents:
        p = os.path.join(TREE, rel)
        txt = read(p)
        pat = re.compile(r"^obj-\$\(" + re.escape(sym) + r"\)\s*\+=\s*" +
                         re.escape(sub) + r"/\s*$", re.M)
        if not pat.search(txt):
            print(f"   {rel}: no directory listing for {sub}/ (ok)")
            continue
        rep = (f"# {sub}/ is built as a composite module; it is not listed\n"
               f"# here as a directory module any more.")
        nt = pat.sub(rep, txt)
        if APPLY:
            write(p, nt)
            print(f"   {rel}: dropped the {sub}/ directory listing")
        else:
            print(f"   (dry run) {rel}: would drop the {sub}/ listing")

    # ------------------------------------------------------------------
    # E) verify
    # ------------------------------------------------------------------
    print()
    print("== verify ==")
    d = read(cfg_path)
    for sym in ("CONFIG_SPRD_APSYS_DVFS", "CONFIG_SPRD_AUDIO_DSP_DUMP",
                "CONFIG_SPRD_AUDIO_SIPC", "CONFIG_NVMEM_SPRD_CACHE_EFUSE",
                "CONFIG_SPRD_VSP_PW_DOMAIN"):
        m = re.search(r"^" + re.escape(sym) + r"=.*$", d, re.M)
        print(f"   {m.group(0) if m else sym + ' ABSENT'}")
    for rel, ko in (("drivers/soc/sprd/cache_efuse/Makefile",
                     "nvmem_sprd_cache_efuse"),
                    ("drivers/pmdomain/sprd/Makefile", "sprd-vsp-pw-domain")):
        t = read(os.path.join(TREE, rel))
        print(f"   {rel}: "
              f"{'has ' + ko if ko in t else 'MISSING ' + ko}")


if __name__ == "__main__":
    main()
