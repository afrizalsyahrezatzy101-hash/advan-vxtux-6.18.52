import os
import re

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"

# (file, [symbols]) that need EXPORT_SYMBOL
JOBS = [
    ("arch/arm64/crypto/aes-ce-ccm-glue.c", ["ce_aes_ccm_auth_data"]),
    ("drivers/dma-buf/sprd/ion_ipc_trusty.c", ["ion_handle_msg", "ion_tipc"]),
    ("drivers/mmc/host/sdhci-sprd.c", ["mmc_send_tuning_cmd"]),
    ("drivers/misc/trusty-tui.c", ["trusty_tui_driver"]),
]

# the novatek touch driver: 40 symbols
NOV = "drivers/input/touchscreen/novatek_nvt_ts_spi"
NOV_SYMS = [
    "Boot_Update_Firmware", "CTP_SPI_READ", "CTP_SPI_WRITE", "ENG_RST_ADDR",
    "SWRST_N8_ADDR", "esd_check", "esd_retry", "fw_entry",
    "nvt_bld_crc_enable", "nvt_boot_ready", "nvt_bootloader_reset",
    "nvt_change_mode", "nvt_check_fw_reset_state", "nvt_check_fw_status",
    "nvt_clear_fw_status", "nvt_eng_reset", "nvt_esd_check_enable",
    "nvt_extra_proc_init", "nvt_fw_crc_enable", "nvt_fw_version_seq_ops",
    "nvt_get_fw_info", "nvt_get_fw_pipe", "nvt_get_mdata", "nvt_mp_parse_ain",
    "nvt_mp_parse_array", "nvt_mp_parse_dt", "nvt_mp_parse_u32",
    "nvt_mp_proc_deinit", "nvt_mp_proc_init", "nvt_read_mdata", "nvt_read_pid",
    "nvt_selftest_seq_ops", "nvt_seq_ops", "nvt_set_page", "nvt_sw_reset",
    "nvt_sw_reset_idle", "nvt_update_firmware", "nvt_write_addr",
    "print_selftest_result", "ts",
]


def read(p):
    with open(p, "r", encoding="utf-8", errors="replace") as f:
        return f.read()


def analyse(path, syms):
    full = os.path.join(TREE, path)
    if not os.path.exists(full):
        return f"MISSING {path}"
    txt = read(full)
    out = []
    for s in syms:
        # find the definition
        m = re.search(r"^([A-Za-z_][\w \t\*]*?)\b" + re.escape(s) +
                      r"\s*\(", txt, re.M)
        kind = "?"
        if m:
            head = m.group(1).strip()
            if "static" in head:
                kind = "STATIC"
            else:
                kind = "global"
        else:
            # maybe a variable
            m2 = re.search(r"^([A-Za-z_][\w \t\*]*?)\b" + re.escape(s) +
                           r"\s*(\[[^\]]*\])?\s*=", txt, re.M)
            if m2:
                head = m2.group(1).strip()
                kind = "STATIC-var" if "static" in head else "global-var"
            else:
                kind = "NOT-FOUND"
        already = bool(re.search(
            r"EXPORT_SYMBOL(_GPL)?\s*\(\s*" + re.escape(s) + r"\s*\)", txt))
        out.append(f"    {s:<34} {kind:<12} "
                   f"{'already exported' if already else 'NOT exported'}")
    return "\n".join(out)


print("=== aes-ce-ccm-glue.c ===")
print(analyse("arch/arm64/crypto/aes-ce-ccm-glue.c", JOBS[0][1]))
print()
print("=== ion_ipc_trusty.c ===")
print(analyse("drivers/dma-buf/sprd/ion_ipc_trusty.c", JOBS[1][1]))
print()
print("=== sdhci-sprd.c ===")
print(analyse("drivers/mmc/host/sdhci-sprd.c", JOBS[2][1]))
print()
print("=== trusty-tui.c ===")
print(analyse("drivers/misc/trusty-tui.c", JOBS[3][1]))
print()
print("=== novatek dir listing ===")
d = os.path.join(TREE, NOV)
for fn in sorted(os.listdir(d)) if os.path.isdir(d) else []:
    print("   ", fn)
print()
print("=== novatek symbols ===")
for fn in (["novatek_nvt_ts_spi.c"] if os.path.exists(
        os.path.join(d, "novatek_nvt_ts_spi.c")) else
        [f for f in os.listdir(d) if f.endswith(".c")]):
    print(f"  --- {fn} ---")
    print(analyse(f"{NOV}/{fn}", NOV_SYMS))
