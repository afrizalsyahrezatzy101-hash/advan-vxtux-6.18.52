# mipi_driver (sharkl5 variant) — 6.18 port notes

Vendor blob: `mipi_driver.ko` — modinfo name=mipi_driver,
description='Spreadtrum SoC Modem Debug Log Driver', depends=sprd_mipi.
Compatible from the blob's own `.rodata`: `sprd,dbg-log-sharkl5`,
`sprd,dbg-log-sharkl5pro`.

Donor (verbatim source):
`work/donors_20260930/bocchi_unisoc54/drivers/unisoc_platform/debug_log/sharkl5/{main.c,phy.c,phy.h,Makefile}`.
Ghidra 12.1.4 headless matched the blob's functions one-for-one against these
files (see the `mipi_driver` row in `work/blob_map/ledger.json`).

## What changed from the donor

1. **Includes.** `sprd,sharkl5-mask.h` + `sprd,sharkl5-regs.h` are not in this
   tree. Replaced with the tree's RE-recovered `sprd,sharkl5pro-regs.h`
   (the mask half was dropped: no `MASK_*` constant is used).
2. **Three register spellings** that the tree's RE header carries under its
   `RF_`-prefixed names were renamed to those names; offsets are identical
   (0x007C / 0x00B4 / 0x0030).
3. **Three registers the RE header lacks** are defined locally in `main.c`
   with their provenance in the comment: 0x0008 `REG_MM_AHB_GEN_CLK_CFG`,
   0x0078 `REG_..._CSI_2P2L_CTRL_DB`, 0x008C `REG_..._REG_SEL_CFG_0`. All
   three offsets are confirmed by the vendor's own auto-generated ASIC
   headers in the U-Boot fork
   (`chip_sharkl5pro/{mm_ahb.h,anlg_phy_g10.h}`), not derived from context.
4. `#include <linux/string.h>` added (6.18 no longer pulls it transitively
   through `of_gpio.h` in every path).

No other line differs from the donor. No API delta was needed: every call
(`gpio_request_one`, `of_get_named_gpio`, `syscon_regmap_lookup_by_phandle`,
`regmap_update_bits`, `devm_clk_get`, `pm_runtime_*`, `usleep_range`,
`module_platform_driver`) still exists in 6.18.

## Wiring

- `sharkl5/Makefile` — donor's exact rule:
  `obj-$(CONFIG_SPRD_MIPI_LOG_R5P0) += mipi_driver.o` /
  `mipi_driver-objs := main.o phy.o`.
- `../Makefile` — donor's exact descent line
  `obj-$(CONFIG_SPRD_MIPI_LOG_R5P0) += sharkl5/`.
- `../Kconfig` — donor's `SPRD_MIPI_LOG_R5P0` symbol.
- Config: `CONFIG_SPRD_MIPI_LOG_R5P0=y` in
  `arch/arm64/configs/vxtux_618_fragment.defconfig`.

## DT contract (stock DTB, node `modem-dbg-log@324f0000`, status okay)

compatible = "sprd,dbg-log-sharkl5", "sprd,dbg-log-sharkl5pro" — identical to
the driver's `dt_ids[]`. Every property the probe reads is present:
`sprd,syscon-mm-ahb` (<0x13> = mm_ahb_regs@62200000), `sprd,syscon-dsi-apb`
(<0x17> = anlg_phy_g10_regs@323f0000), `sprd,ch-name` (6), `sprd,ch-index`,
`sprd,dcfix`, `sprd,mm`, `sprd,mipi-l5pro`, `power-domains`, `vddio-supply`,
`mipi-switch-en-gpios`, `mipi-switch-mode-gpios`, and exactly the seven
clock-names the probe looks up (serdes_eb, mm_eb, ana_eb, dphy_cfg_eb,
dphy_ref_eb, dsi_csi_test_eb, cphy_cfg_eb). No `src0..src4` clocks exist in
the node, so `clk_src[]` stays NULL and the `sprd,div1_map` read fails with
rc != 0 — vendor behaviour, logged, non-fatal.

## Linkage proof symbols (non-static, defined by this port)

`dbg_phy_init`, `dbg_phy_exit` (phy.c). `dbg_log_probe` and the six
`inter_dbg_log_*` ops are `static` in the donor and do not appear in
System.map — per the project rule, the two global phy entry points plus the
`__initcall__kmod_mipi_driver__*` entry are what proves the object entered
the link.

## Replaced placeholders

Two 686/685-byte stub files (`mipi_driver.c`, `sprd_mipi.c`, dated
2026-10-08, "VXTux-chan noble substitute") sat in this directory with
invented compatibles (`sprd,mipi_driver`, `sprd,sprd_mipi` — neither exists
in the blob or the stock DT) and no Makefile, so they never compiled. Backed
up as `*.stub_20261010.bak` and replaced by this port.
