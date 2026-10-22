"""Two fixes for the module build of the stock blobs.

1) drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx.c uses
   saved_command_line to look for 'oppo_ftm_mode=' in the kernel
   command line. saved_command_line is NOT exported by this kernel, so
   the module fails to link:

     ERROR: modpost: "saved_command_line"
     [drivers/input/touchscreen/novatek_nvt_ts_spi/novatek_ts_spi.ko]
     undefined!

   That Oppo factory-mode check is not part of the stock blob: the
   stock novatek_ts_spi.ko does not import saved_command_line at all
   (checked with nm -u on the blob pulled from the device). It is
   vendor cruft from a different board, and the same code path ends up
   enabling the IRQ in the normal case anyway:

       oppo_ftmmode_start = strstr(saved_command_line,"oppo_ftm_mode=");
       if ( oppo_ftmmode_start != NULL ) {
           ... factory2 -> boot_mode = 3
           else       -> nvt_irq_enable(1);
       } else {
           nvt_irq_enable(1);
       }

   So the whole block collapses to nvt_irq_enable(1), which is what
   the stock blob does on this board.

2) CONFIG_SPRD_AUDIO_AGDSP_ACCESS was off, so the provider of
   agdsp_can_access was never built, and audio-dsp-dump.ko failed:

     ERROR: modpost: "agdsp_can_access"
     [drivers/vxtux/audio/audiodspdump/audio-dsp-dump.ko] undefined!

   The provider is drivers/sound/soc/sprd/agdsp_access/, which is
   built-in (=y), and audio-dsp-dump is a module that needs it. Turn
   the symbol on.

Usage:
    python3 fix6_g.py [--apply]
"""
import os
import re
import sys

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
APPLY = "--apply" in sys.argv

NOVATEK = "drivers/input/touchscreen/novatek_nvt_ts_spi/nt36xxx.c"
CFG = ".config"


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def write(p, s):
    with open(p, "w", encoding="utf-8", newline="\n") as f:
        f.write(s)


def main():
    # 1) the oppo_ftm_mode block
    print("== 1) nt36xxx.c: the oppo_ftm_mode block ==")
    p = os.path.join(TREE, NOVATEK)
    txt = read(p)
    old = """	oppo_ftmmode_start = strstr(saved_command_line,"oppo_ftm_mode=");
	if ( oppo_ftmmode_start != NULL ) {
		oppo_ftmmode_start += strlen("oppo_ftm_mode=");
		if (strncmp(oppo_ftmmode_start, "factory2", strlen("factory2")) == 0) {
			ts->boot_mode = 3;
			NVT_LOG("boot_mode is FACTORY,RF and WLAN not need to enable irq\\n");
		} else {
			nvt_irq_enable(1);
		}
	} else {
		NVT_LOG("boot_mode is failed\\n");
		nvt_irq_enable(1);
	}
"""
    new = """	/*
	 * The stock novatek_ts_spi.ko does not import saved_command_line
	 * (verified with nm -u on the blob pulled from the device), so it
	 * has no Oppo factory-mode check either. The code that did look
	 * for "oppo_ftm_mode=" in the kernel command line needed
	 * saved_command_line, which this kernel does not export:
	 *
	 *   ERROR: modpost: "saved_command_line"
	 *   [drivers/input/touchscreen/novatek_nvt_ts_spi/novatek_ts_spi.ko]
	 *   undefined!
	 *
	 * On this board the check never fired anyway - its only effect
	 * was to enable the IRQ, which is what happens below.
	 */
	nvt_irq_enable(1);
"""
    if old not in txt:
        print("  !! the block was not found verbatim")
    else:
        txt2 = txt.replace(old, new)
        # the now unused variable
        txt2 = re.sub(r"^\s*char\s*\*\s*oppo_ftmmode_start\s*;\s*$\n",
                      "", txt2, flags=re.M)
        if APPLY:
            write(p, txt2)
            print(f"  {NOVATEK}: replaced with nvt_irq_enable(1)")
        else:
            print(f"  (dry run) {NOVATEK}")

    # 2) CONFIG_SPRD_AUDIO_AGDSP_ACCESS
    print()
    print("== 2) .config: CONFIG_SPRD_AUDIO_AGDSP_ACCESS ==")
    cp = os.path.join(TREE, CFG)
    d = read(cp)
    if re.search(r"^CONFIG_SPRD_AUDIO_AGDSP_ACCESS=y$", d, re.M):
        print("   already =y")
    elif re.search(r"^CONFIG_SPRD_AUDIO_AGDSP_ACCESS=m$", d, re.M):
        d = re.sub(r"^CONFIG_SPRD_AUDIO_AGDSP_ACCESS=m$",
                   "CONFIG_SPRD_AUDIO_AGDSP_ACCESS=y", d, flags=re.M)
        if APPLY:
            write(cp, d)
            print("   m -> y")
    else:
        d = re.sub(r"^# CONFIG_SPRD_AUDIO_AGDSP_ACCESS is not set$",
                   "CONFIG_SPRD_AUDIO_AGDSP_ACCESS=y", d, flags=re.M)
        if "CONFIG_SPRD_AUDIO_AGDSP_ACCESS=y" not in d:
            d = d + "\nCONFIG_SPRD_AUDIO_AGDSP_ACCESS=y\n"
        if APPLY:
            write(cp, d)
            print("   (was off) -> y")
    if not APPLY:
        print("   (dry run)")

    # 3) verify
    print()
    print("== verify ==")
    t = read(os.path.join(TREE, NOVATEK))
    print("   saved_command_line still referenced:",
          "saved_command_line" in t)
    print("   oppo_ftmmode_start still referenced:",
          "oppo_ftmmode_start" in t)
    d = read(cp)
    m = re.search(r"^(# )?CONFIG_SPRD_AUDIO_AGDSP_ACCESS.*$", d, re.M)
    print("  ", m.group(0) if m else "ABSENT")


if __name__ == "__main__":
    main()
