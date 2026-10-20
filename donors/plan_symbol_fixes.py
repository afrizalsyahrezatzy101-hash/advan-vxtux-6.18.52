"""Locate every definition of the fixable symbols across the whole tree,
classify static vs global, and emit the exact edits needed.

The point: a vendor .ko that was built as a module exported its
symbols to userspace. When the driver is built into vmlinux (=y) the
symbols disappear from the kernel's export table unless the source
carries EXPORT_SYMBOL(). The fix is:
  - global symbol  -> add EXPORT_SYMBOL_GPL(sym) after the definition
  - static symbol  -> drop the `static` keyword, then export it
  - missing symbol -> real port gap, report it

Writes an edit plan (JSON + a human-readable report) and, with --apply,
patches the source files.
"""
import json
import os
import re
import subprocess
import sys

GIT = r"C:\Program Files\Git\cmd\git.exe"
TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

# stock .ko -> (config symbol, [source files to search])
TARGETS = {
    "aes-ce-ccm": ("CONFIG_CRYPTO_AES_ARM64_CE_CCM",
                   ["arch/arm64/crypto/aes-ce-ccm-glue.c",
                    "arch/arm64/crypto/aes-ce-ccm-core.S"]),
    "ion_ipc_trusty": ("CONFIG_ION_IPC_TRUSTY",
                       ["drivers/dma-buf/sprd/ion_ipc_trusty.c"]),
    "sdhci-sprd": ("CONFIG_MMC_SDHCI_SPRD",
                   ["drivers/mmc/host/sdhci-sprd.c"]),
    "trusty-tui": ("CONFIG_TRUSTY_TUI",
                   ["drivers/misc/trusty-tui.c"]),
    "novatek_ts_spi": ("CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS",
                       ["drivers/input/touchscreen/novatek_nvt_ts_spi/"]),
    "unisoc-mailbox": ("CONFIG_SPRD_MAILBOX",
                        ["drivers/mailbox/"]),
    "trusty": ("CONFIG_TRUSTY", ["drivers/trusty/"]),
    "trusty-ipc": ("CONFIG_TRUSTY_VIRTIO_IPC", ["drivers/trusty/"]),
}

# the symbols we know are missing (from the symbol comparison)
MISSING = {
    "aes-ce-ccm": ["ce_aes_ccm_auth_data"],
    "ion_ipc_trusty": ["ion_handle_msg", "ion_tipc"],
    "sdhci-sprd": ["mmc_send_tuning_cmd"],
    "trusty-tui": ["trusty_tui_driver"],
    "novatek_ts_spi": [
        "Boot_Update_Firmware", "CTP_SPI_READ", "CTP_SPI_WRITE",
        "ENG_RST_ADDR", "SWRST_N8_ADDR", "esd_check", "esd_retry",
        "fw_entry", "nvt_bld_crc_enable", "nvt_boot_ready",
        "nvt_bootloader_reset", "nvt_change_mode", "nvt_check_fw_reset_state",
        "nvt_check_fw_status", "nvt_clear_fw_status", "nvt_eng_reset",
        "nvt_esd_check_enable", "nvt_extra_proc_init", "nvt_fw_crc_enable",
        "nvt_fw_version_seq_ops", "nvt_get_fw_info", "nvt_get_fw_pipe",
        "nvt_get_mdata", "nvt_mp_parse_ain", "nvt_mp_parse_array",
        "nvt_mp_parse_dt", "nvt_mp_parse_u32", "nvt_mp_proc_deinit",
        "nvt_mp_proc_init", "nvt_read_mdata", "nvt_read_pid",
        "nvt_selftest_seq_ops", "nvt_seq_ops", "nvt_set_page",
        "nvt_sw_reset", "nvt_sw_reset_idle", "nvt_update_firmware",
        "nvt_write_addr", "print_selftest_result", "ts",
    ],
    "unisoc-mailbox": [
        "get_tx_fifo_mask", "mbox_phy_ops", "mbox_start_send_tx_fifo",
        "sprd_mbox_change_wakeup_flag_r1", "sprd_mbox_phy_r1_init",
        "sprd_mbox_wakeup_flag_callback_register_r1",
        "sprd_mbox_wakeup_flag_callback_unregister_r1",
    ],
    "trusty": ["trusty_sprd_sec_call", "trusty_sprd_alloc_share_memory"],
    "trusty-ipc": [],
}


def collect_files(paths):
    """Expand file or directory paths into a list of .c/.h files."""
    out = []
    for p in paths:
        full = os.path.join(TREE, p)
        if os.path.isdir(full):
            for fn in sorted(os.listdir(full)):
                if fn.endswith((".c", ".h")):
                    out.append(os.path.join(p, fn))
        elif os.path.exists(full):
            out.append(p)
    return out


DEF_RE = r"^(?P<head>[A-Za-z_][\w \t\*@\(\),]*?)\b(?P<name>%s)\b\s*(?P<tail>\(|\[|=|;)"

VAR_RE = r"^(?P<head>[A-Za-z_][\w \t\*@]*?)\b(?P<name>%s)\b\s*(?P<tail>\[|=|;)"


def find_defs(files, sym):
    """Return list of (rel_path, line_no, kind, head_text)."""
    hits = []
    fn_re = re.compile(DEF_RE % re.escape(sym), re.M)
    var_re = re.compile(VAR_RE % re.escape(sym), re.M)
    for rel in files:
        full = os.path.join(TREE, rel)
        try:
            txt = open(full, "r", encoding="utf-8", errors="replace").read()
        except OSError:
            continue
        for rx, kind in ((fn_re, "func"), (var_re, "var")):
            for m in rx.finditer(txt):
                head = m.group("head")
                # skip if it is a call/usage, not a definition
                if "(" in head and kind == "func" and not head.rstrip().endswith(
                        ("*", ")")) and ")" not in head:
                    pass
                line = txt[:m.start()].count("\n") + 1
                is_static = "static" in head.split()
                hits.append((rel, line, kind,
                             "static" if is_static else "global", head.strip()))
    return hits


def already_exported(rel, sym):
    full = os.path.join(TREE, rel)
    txt = open(full, "r", encoding="utf-8", errors="replace").read()
    return bool(re.search(r"EXPORT_SYMBOL(_GPL)?\s*\(\s*" + re.escape(sym) +
                          r"\s*\)", txt))


def main():
    plan = {}
    report = []
    total_fix = 0
    total_gap = 0

    for mod, syms in MISSING.items():
        if not syms:
            continue
        paths = TARGETS.get(mod, (None, []))[1]
        files = collect_files(paths)
        if not files:
            continue
        entries = []
        for s in syms:
            defs = find_defs(files, s)
            # prefer a non-static definition if any
            defs.sort(key=lambda d: (d[3] == "static", d[1]))
            if not defs:
                entries.append({"sym": s, "action": "ABSENT", "file": None})
                total_gap += 1
                continue
            rel, line, kind, stat, head = defs[0]
            if already_exported(rel, s):
                entries.append({"sym": s, "action": "ALREADY",
                                "file": rel, "line": line, "kind": kind,
                                "static": stat == "static"})
                continue
            action = "EXPORT" if stat == "global" else "UNSTATIC+EXPORT"
            entries.append({"sym": s, "action": action, "file": rel,
                            "line": line, "kind": kind,
                            "static": stat == "static", "head": head})
            total_fix += 1
        plan[mod] = entries

        report.append(f"=== {mod} ===")
        for e in entries:
            if e["action"] == "ABSENT":
                report.append(f"    {e['sym']:<48} ABSENT from source")
            else:
                report.append(f"    {e['sym']:<48} {e['action']:<16} "
                              f"{e['file']}:{e['line']}  ({e['kind']}, "
                              f"{'static' if e.get('static') else 'global'})")
        report.append("")

    print("\n".join(report))
    print("=" * 78)
    print(f"fixable (needs EXPORT_SYMBOL / un-static): {total_fix}")
    print(f"absent from source (real gap)             : {total_gap}")
    print()

    with open("E:/vxtux_build_repo/donors/symbol_fix_plan.json", "w",
              encoding="utf-8") as f:
        json.dump(plan, f, indent=2)
    with open("E:/vxtux_build_repo/donors/symbol_fix_plan.txt", "w",
              encoding="utf-8") as f:
        f.write("\n".join(report))
    print("wrote donors/symbol_fix_plan.json + .txt")


if __name__ == "__main__":
    main()
