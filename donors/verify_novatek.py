import os
import re

TREE = "E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
D = "drivers/input/touchscreen/novatek_nvt_ts_spi"

# The plan flagged several symbols in nt36xxx.h - that file is a HEADER,
# so a match there is a prototype, not a definition. Verify which file
# really defines each symbol.
SYMS = [
    "Boot_Update_Firmware", "CTP_SPI_READ", "CTP_SPI_WRITE", "ENG_RST_ADDR",
    "SWRST_N8_ADDR", "esd_check", "esd_retry", "fw_entry",
    "nvt_bld_crc_enable", "nvt_boot_ready", "nvt_bootloader_reset",
    "nvt_change_mode", "nvt_check_fw_reset_state", "nvt_check_fw_status",
    "nvt_clear_fw_status", "nvt_eng_reset", "nvt_esd_check_enable",
    "nvt_extra_proc_init", "nvt_fw_crc_enable", "nvt_fw_version_seq_ops",
    "nvt_get_fw_info", "nvt_get_fw_pipe", "nvt_get_mdata",
    "nvt_mp_parse_ain", "nvt_mp_parse_array", "nvt_mp_parse_dt",
    "nvt_mp_parse_u32", "nvt_mp_proc_deinit", "nvt_mp_proc_init",
    "nvt_read_mdata", "nvt_read_pid", "nvt_selftest_seq_ops",
    "nvt_seq_ops", "nvt_set_page", "nvt_sw_reset", "nvt_sw_reset_idle",
    "nvt_update_firmware", "nvt_write_addr", "print_selftest_result", "ts",
]


def read(p):
    return open(p, "r", encoding="utf-8", errors="replace").read()


print("=== header nt36xxx.h around the flagged lines ===")
h = read(os.path.join(TREE, D, "nt36xxx.h"))
hl = h.splitlines()
for i in (320, 325, 326, 327, 328, 332, 333, 334, 335, 336, 338):
    if i - 1 < len(hl):
        print(f"  {i:4}: {hl[i-1]}")

print()
print("=== real definitions in .c files ===")
cfiles = [f"{D}/{f}" for f in os.listdir(os.path.join(TREE, D))
          if f.endswith(".c")]
for s in SYMS:
    found = []
    for rel in cfiles:
        txt = read(os.path.join(TREE, rel))
        for m in re.finditer(r"^([A-Za-z_][\w \t\*@]*?)\b" + re.escape(s) +
                             r"\b\s*(\(|\[|=|;)", txt, re.M):
            head = m.group(1).strip()
            line = txt[:m.start()].count("\n") + 1
            found.append((rel.split("/")[-1], line, head, "static" in head))
    if found:
        for fn, line, head, st in found:
            print(f"  {s:<32} {fn}:{line:<5} {'STATIC' if st else 'global':<7} "
                  f"| {head[:60]}")
    else:
        print(f"  {s:<32} *** NOT DEFINED IN ANY .c ***")
