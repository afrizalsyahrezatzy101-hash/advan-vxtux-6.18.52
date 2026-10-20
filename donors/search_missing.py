import os
import re
import subprocess

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
CFG = os.path.join(TREE, ".config")

# For each NOT_PRESENT stock module, search the whole VXTux tree by its
# distinctive tokens (not just the guessed config symbol).
TARGETS = {
    "audio_dsp_dump": ["audio-dsp-dump", "audiodspdump", "audio_dsp_dump",
                       "SPRD_AUDIO_DSP_DUMP"],
    "audio_pipe": ["audio-pipe", "audiosipc", "SPRD_AUDIO_PIPE"],
    "audio_sipc": ["audiosipc", "audio_sipc", "audio-sipc"],
    "ion_ipc_trusty": ["ion_ipc_trusty", "ion-ipc-trusty", "ION_IPC_TRUSTY"],
    "novatek_ts_spi": ["novatek", "NVT_TS_SPI", "nvt_ts"],
    "rtc_sc27xx": ["rtc-sc27xx", "rtc_sc27xx", "RTC_SC27XX"],
    "sc27xx_adc": ["sc27xx_adc", "sc27xx-adc", "SARADC", "sc27xx-adc"],
    "sprd_apipe": ["apipe", "APIPE"],
    "sprd_dmaengine_pcm": ["dmaengine", "sprd-dmaengine", "sprd_pcm"],
    "sprd_sipc_virt_bus": ["sipc", "virt_bus", "virt-bus"],
    "sprd_time_sync": ["time_sync", "time-sync", "sprd_ts"],
    "trusty": ["trusty"],
    "trusty_ipc": ["trusty"],
    "trusty_irq": ["trusty"],
    "trusty_tui": ["trusty"],
}

cfg = open(CFG, "r", encoding="utf-8", errors="replace").read()

print("=== searching VXTux tree + config for each NOT_PRESENT driver ===\n")
for mod, toks in TARGETS.items():
    found_cfg = []
    for t in toks:
        for line in cfg.splitlines():
            if t.upper() in line.upper() and line.startswith("CONFIG_"):
                found_cfg.append(line)
    # find directories
    dirs = []
    for root, ds, fs in os.walk(TREE):
        rel = os.path.relpath(root, TREE)
        if any(p in rel for p in (".git", "io_uring.disabled",
                                  "Documentation", "tools", "scripts")):
            ds[:] = []
            continue
        for d in ds:
            if any(t.lower() in d.lower() for t in toks):
                dirs.append(os.path.join(rel, d).replace("\\", "/"))
        for f in fs:
            if f.endswith(".c") and any(t.lower() in f.lower() for t in toks):
                dirs.append(os.path.join(rel, f).replace("\\", "/"))
        if len(dirs) > 6:
            break
    print(f"--- {mod} ---")
    for c in sorted(set(found_cfg))[:6]:
        print(f"   cfg: {c}")
    for d in sorted(set(dirs))[:6]:
        print(f"   src: {d}")
    if not found_cfg and not dirs:
        print("   (nothing found)")
    print()
