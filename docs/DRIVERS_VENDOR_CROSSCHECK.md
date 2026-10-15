# DRIVERS — Vendor Advan Cross-Check vs Build TICK9

Cross-check lengkap antara driver yang dipakai Advan Tab VX Lite (T1030) di
vendor stock (kernel 5.4) vs. build TICK7-TICK9 kita (kernel 6.18.52-VXTux-UMS512).

## 0. Ringkasan satu kalimat

**Vendor Advan punya 129 .ko** (driver board-specific Unisoc T618/UMS512 dari
`vramdisk_ko/`). Build TICK9 di workspace menghasilkan **1.424 .ko** total;
**hanya 11 yang sama dengan vendor Advan**, dan **118 .ko vendor Advan belum
ter-build** di TICK9 (sebagian sudah built-in ke vmlinux, sebagian lagi belum
di-port/config dimatikan).

## 1. Sumber data

| Sumber | Path | Isi |
|---|---|---|
| Vendor Advan (ROM dump) | `dumped/re/hidden/vramdisk_ko/` | 129 .ko dari `vendor_ramdisk_stock.cpio` (kernel 5.4 stock) |
| Build TICK9 device | `work/sysmaps/kolist.1012b.txt` | 1.424 .ko dari `make modules` RC=0 (12 Okt 10:12 WIB) |
| Dokumentasi TICK7 | `docs/DEBLOB-20261014-TICK7-MODULE-SYNC.md` | Tally 103/103 artefak +13 modul compile-verified |
| Dokumentasi TICK9 | `docs/DEBLOB-20261015-TICK9-STATUS.md` | Ledger 102/102 DEBLOBBED + 4 baris terakhir ditutup |

## 2. Tabel 129 driver vendor Advan (urut abjad)

| # | Driver | Sumber tree | Peran / Subsistem | Status build TICK9 |
|---:|---|---|---|---|
| 1 | `aes-ce-ccm.ko` | `arch/arm64/crypto/` | Crypto AES-CCM (mainline) | ✅ TICK7 (built-in/=.ko di kolist) |
| 2 | `aes-neon-blk.ko` | `arch/arm64/crypto/` | Crypto AES-NEON block (mainline) | ✅ TICK9 match (vendor ∩ build) |
| 3 | `agdsp_access.ko` | `sound/soc/sprd/agdsp_access/` | Audio DSP access (vendor) | ❌ belum di build (=y? atau di-link ke vmlinux) |
| 4 | `apsys-dvfs.ko` | `devfreq/sprd/apsys/` | APSYS dynamic voltage/freq (vendor) | ❌ config SPRD_APSYS_DVFS dimatikan |
| 5 | `arc4.ko` | `crypto/` | Stream cipher ARC4 (mainline) | ❌ mungkin built-in |
| 6 | `asix.ko` | `net/usb/` | USB Ethernet ASIX (mainline) | ✅ TICK9 match |
| 7 | `audio_mem.ko` | `sound/soc/sprd/audiomem/` | Audio memory manager (vendor) | ❌ belum di build |
| 8 | `audio_sipc.ko` | `sound/soc/sprd/audiosipc/` | Audio SIPC transport (vendor) | ❌ belum di build |
| 9 | `audio-dsp-dump.ko` | `sound/soc/sprd/audiodspdump/` | Audio DSP dump (vendor) | ✅ TICK9 match (CONFIG_SPRD_AUDIO_DSP_DUMP=m) |
| 10 | `audio-pipe.ko` | `sound/soc/sprd/audiosipc/` | Audio pipe (vendor) | ❌ config SPRD_AUDIO_PIPE=y mungkin built-in |
| 11 | `ax88179_178a.ko` | `net/usb/` | USB Ethernet AX88179 (mainline) | ✅ TICK9 match |
| 12 | `bq2560x-charger.ko` | `power/supply/` | BQ256xx charger IC (vendor) | ❌ CHARGER_BQ256XX=y mungkin built-in |
| 13 | `clk-sprd.ko` | `clk/sprd/` | Clock controller SPRD (vendor) | ❌ config SPRD_UMS512_CLK=y built-in |
| 14 | `core.ko` | `wcn platform/` | WCN composite module (vendor) | ❌ dipecah jadi wcn_core.ko di TICK7 |
| 15 | `extcon-usb-gpio.ko` | `extcon/` | USB extcon via GPIO (mainline) | ❌ mungkin built-in |
| 16 | `ghash-ce.ko` | `arch/arm64/crypto/` | GHASH AES (mainline) | ❌ mungkin built-in |
| 17 | `gpio.ko` | `gpio/gpio-sprd.c` | GPIO SPRD (vendor) | ❌ config dimatikan atau built-in |
| 18 | `gpio-eic-sprd.ko` | `gpio/` | GPIO EIC SPRD (vendor) | ❌ config dimatikan |
| 19 | `gpio-pmic-eic-sprd.ko` | `gpio/` | GPIO PMIC EIC SPRD (vendor) | ❌ config dimatikan |
| 20 | `gpio-sprd.ko` | `gpio/gpio-sprd.c` | GPIO controller SPRD (vendor) | ❌ config dimatikan |
| 21 | `hxchipset_83102e.ko` | `input/touchscreen/sprd/hx83102e/` | Touch Himax HX83102E (vendor) | ❌ config dimatikan |
| 22 | `i2c-sprd.ko` | `i2c/busses/i2c-sprd.c` | I2C bus SPRD (vendor) | ❌ config dimatikan |
| 23 | `i2c-sprd-hw.ko` | `i2c/busses/` | I2C SPRD HW (vendor) | ❌ config dimatikan |
| 24 | `ion_cma_heap.ko` | `dma-buf/heaps/cma_heap.c` | DMA-BUF CMA heap (mainline) | ❌ mungkin built-in |
| 25 | `ion_ipc_trusty.ko` | `dma-buf/sprd/ion_ipc_trusty.c` | ION-IPC-trusty (vendor) | ❌ config dimatikan |
| 26 | `jpg.ko` | `misc/sprd_jpg/sprd_jpg_main.c` | JPEG codec SPRD (vendor) | ✅ TICK7 compile-verified (156 KB) |
| 27 | `kfifo_buf.ko` | `sound/soc/sprd/audiosipc/` | Audio kfifo buffer (vendor) | ✅ TICK9 match |
| 28 | `leds-sc27xx-bltc.ko` | `leds/leds-sc27xx-bltc.c` | LED backlight SC27xx (vendor) | ❌ config dimatikan |
| 29 | `ledtrig-pattern.ko` | `leds/trigger/ledtrig-pattern.c` | LED trigger pattern (mainline) | ❌ mungkin built-in |
| 30 | `mcdt_hw.ko` | `sound/soc/sprd/mcdt/mcdt_r1p0/` | Multi-Channel DMA Timer HW (vendor) | ❌ config dimatikan |
| 31 | `mipi_driver.ko` | `unisoc_platform/debug_log/sharkl5/` | MIPI debug log (vendor, Tier B) | ❌ TICK7 stub, sharkl5/ no Makefile |
| 32 | `misc_sprd_uid.ko` | `soc/sprd/soc_id/sprd_uid.c` | SoC UID misc (vendor) | ❌ config dimatikan |
| 33 | `mmc_hsq.ko` | `mmc/host/sdhci-sprd.c` | MMC HSQ (vendor) | ❌ config dimatikan |
| 34 | `mmc_swcq.ko` | `mmc/host/sdhci-sprd.c` | MMC SWCQ (vendor) | ❌ config dimatikan |
| 35 | `mmdvfs.ko` | `devfreq/sprd/` | MMC DVFS (vendor) | ❌ config dimatikan |
| 36 | `musb_hdrc.ko` | `usb/musb/musb_core.c` | USB MUSB HDRC (mainline/vendor) | ❌ config USB_MUSB_HDRC=y built-in |
| 37 | `musb_sprd.ko` | `usb/musb/musb_sprd_vxtux.c` | USB MUSB SPRD VXTux (vendor) | ❌ config dimatikan |
| 38 | `novatek_ts_spi.ko` | `input/touchscreen/novatek_nvt_ts_spi/` | Touch Novatek NT36xxx SPI (vendor) | ✅ TICK9 compile-verified (512.584 B) |
| 39 | `nvmem_sprd_cache_efuse.ko` | `nvmem/` | NVMEM SPRD cache efuse (vendor) | ❌ config dimatikan |
| 40 | `nvmem_sprd_efuse.ko` | `nvmem/` | NVMEM SPRD efuse (vendor) | ❌ config dimatikan |
| 41 | `nvmem-sc27xx-efuse.ko` | `nvmem/` | NVMEM SC27xx efuse (vendor) | ❌ config dimatikan |
| 42 | `phy-sprd-sharkl5Pro.ko` | `phy/sprd/phy-sprd-ums512.c` | PHY SPRD sharkl5Pro (vendor) | ❌ config dimatikan (seharusnya =m) |
| 43 | `pinctrl.ko` | `pinctrl/sprd/pinctrl-sprd.c` | Pin control SPRD (vendor) | ❌ config dimatikan |
| 44 | `pinctrl-sprd.ko` | `pinctrl/sprd/pinctrl-sprd.c` | Pin control SPRD (vendor) | ❌ config dimatikan |
| 45 | `pinctrl-sprd-sharkl5Pro.ko` | `pinctrl/sprd/pinctrl-sprd-sharkl5pro.c` | Pin control sharkl5Pro (vendor) | ❌ config dimatikan |
| 46 | `pwm_bl.ko` | `video/backlight/pwm_bl.c` | PWM backlight (mainline) | ✅ TICK9 match |
| 47 | `pwm-sprd.ko` | `pwm/pwm-sprd.c` | PWM SPRD (vendor) | ❌ config dimatikan |
| 48 | `rtc-sc27xx.ko` | `rtc/rtc-sc27xx.c` | RTC SC27xx (vendor) | ❌ config dimatikan |
| 49 | `sblock_bridge.ko` | `soc/sprd/sipc/` | SIPC block bridge (vendor) | ❌ config dimatikan |
| 50 | `sbuf_bridge.ko` | `soc/sprd/sipc/` | SIPC buffer bridge (vendor) | ❌ config dimatikan |
| 51 | `sc2730-regulator.ko` | `regulator/sc2730-regulator.c` | Regulator SC2730 (vendor) | ❌ config dimatikan |
| 52 | `sc27xx_adc.ko` | `iio/adc/sc27xx_adc.c` | ADC SC27xx (vendor) | ❌ config dimatikan |
| 53 | `sc27xx_fuel_gauge.ko` | `power/supply/sc27xx_fuel_gauge.c` | Fuel gauge SC27xx (vendor) | ❌ config dimatikan |
| 54 | `sc27xx_typec.ko` | `usb/typec/sprd/sc27xx_typec.c` | Type-C SC27xx (vendor) | ❌ config dimatikan |
| 55 | `sc27xx-poweroff.ko` | `power/reset/sc27xx-poweroff.c` | Poweroff SC27xx (vendor) | ❌ config dimatikan |
| 56 | `sc27xx-vibra.ko` | `input/misc/sc27xx-vibra.c` | Vibrator SC27xx (vendor) | ❌ config dimatikan |
| 57 | `sdhci-sprd.ko` | `mmc/host/sdhci-sprd.c` | SDHCI SPRD (vendor) | ❌ config dimatikan |
| 58 | `sensorhub.ko` | `input/misc/sensorhub.c` | Sensor hub (vendor) | ❌ config dimatikan |
| 59 | `sha1-ce.ko` | `crypto/sha1.c` | SHA-1 (mainline) | ❌ mungkin built-in |
| 60 | `sipc-core.ko` | `soc/sprd/sipc/sipc.c` | SIPC core (vendor) | ❌ config dimatikan |
| 61 | `sipx.ko` | `soc/sprd/sipc/` | SIPC zero-copy (vendor) | ❌ config dimatikan |
| 62 | `slog_bridge.ko` | `soc/sprd/sipc/` | SIPC log bridge (vendor) | ❌ config dimatikan |
| 63 | `snd-soc-sprd-card.ko` | `sound/soc/sprd/card/` | Sound card SPRD (vendor) | ❌ config dimatikan |
| 64 | `snd-soc-sprd-codec-sc2730.ko` | `sound/soc/sprd/codec_sc2730/` | Audio codec SC2730 (vendor) | ❌ config dimatikan |
| 65 | `snd-soc-sprd-codec-sc2730-power.ko` | `sound/soc/sprd/codec/` | Audio codec SC2730 power (vendor) | ❌ config dimatikan |
| 66 | `snd-soc-sprd-codec-sc2730-power-dev.ko` | `sound/soc/sprd/codec/` | Audio codec SC2730 power dev (vendor) | ❌ config dimatikan |
| 67 | `snd-soc-sprd-dummy-codec.ko` | `sound/soc/sprd/codec/dummy-codec.c` | Dummy codec (vendor) | ❌ config dimatikan |
| 68 | `snd-soc-sprd-vbc-fe.ko` | `sound/soc/sprd/dai_v4/vbc-dai.c` | VBC frontend (vendor) | ❌ config dimatikan |
| 69 | `snd-soc-sprd-vbc-v4.ko` | `sound/soc/sprd/dai_v4/vbc-phy-v4.c` | VBC phy v4 (vendor) | ❌ config dimatikan |
| 70 | `spipe.ko` | `soc/sprd/sipc/` | SIPC pipe (vendor) | ❌ config dimatikan |
| 71 | `spi-sprd.ko` | `spi/spi-sprd.c` | SPI SPRD (vendor) | ❌ config dimatikan (closed via System.map TICK9) |
| 72 | `spi-sprd-adi.ko` | `spi/` | SPI SPRD ADI (vendor) | ❌ config dimatikan |
| 73 | `spool.ko` | `soc/sprd/sipc/` | SIPC pool (vendor) | ❌ config dimatikan |
| 74 | `sprd_7sreset.ko` | `soc/sprd/` | SPRD 7s reset (vendor) | ❌ config dimatikan |
| 75 | `sprd_apipe.ko` | `sound/soc/sprd/audio_pipe/` | Audio pipe (vendor) | ❌ config dimatikan |
| 76 | `sprd_audcp_boot.ko` | `sound/soc/sprd/audiocpboot/` | Audio CP boot (vendor) | ❌ config dimatikan |
| 77 | `sprd_audcp_dvfs.ko` | `sound/soc/sprd/audiodvfs/` | Audio CP DVFS (vendor) | ❌ config dimatikan |
| 78 | `sprd_battery_info.ko` | `power/supply/sprd/sprd_battery_info_main.c` | Battery info (vendor) | ✅ TICK7 compile-verified (96 KB) |
| 79 | `sprd_camera.ko` | `media/platform/sprd/pipeline/sprd_dcam.c` | Camera DCAM pipeline (vendor) | ❌ config dimatikan |
| 80 | `sprd_camsys_pw_domain.ko` | `pmdomain/sprd/sprd_camsys_pw_domain.c` | Camera system power domain (vendor) | ✅ TICK7 compile-verified (95 KB) |
| 81 | `sprd_cpp.ko` | `media/platform/sprd/cpp/cpp_core.c` | Camera Post-Processor (vendor) | ✅ TICK7 compile-verified (321 KB) |
| 82 | `sprd_flash_drv.ko` | `sprd/flash/flash_drv.c` | Flash driver (vendor) | ❌ config dimatikan (closed via System.map TICK9) |
| 83 | `sprd_hwspinlock.ko` | `hwspinlock/` | Hardware spinlock SPRD (vendor) | ❌ config dimatikan |
| 84 | `sprd_iq.ko` | `media/platform/sprd/` | Image Quality tuning (vendor) | ❌ config dimatikan |
| 85 | `sprd_manufacturer_model.ko` | `soc/sprd/` | Manufacturer model (vendor) | ❌ config dimatikan |
| 86 | `sprd_mipi.ko` | `unisoc_platform/debug_log/` | Modem debug log (vendor) | ❌ config dimatikan |
| 87 | `sprd_modem_loader.ko` | `soc/sprd/` | Modem firmware loader (vendor) | ❌ config dimatikan |
| 88 | `sprd_pdbg.ko` | `soc/sprd/` | Power debug (vendor) | ❌ config dimatikan |
| 89 | `sprd_pmic_syscon.ko` | `soc/sprd/pmic_syscon/` | PMIC syscon (vendor) | ❌ config dimatikan |
| 90 | `sprd_power_manager.ko` | `soc/sprd/` | Power manager (vendor) | ✅ TICK9 match |
| 91 | `sprd_power_stat.ko` | `soc/sprd/` | Power stat (vendor) | ❌ config dimatikan |
| 92 | `sprd_sensor.ko` | `media/platform/sprd/sensors/sprd_sensor/` | Sensor framework (vendor) | ❌ config dimatikan |
| 93 | `sprd_shm.ko` | `soc/sprd/` | Shared memory (vendor) | ❌ config dimatikan |
| 94 | `sprd_sip_svc.ko` | `soc/sprd/` | SIPC service (vendor) | ❌ config dimatikan |
| 95 | `sprd_soc_id.ko` | `soc/sprd/soc_id/` | SoC ID (vendor) | ❌ config dimatikan |
| 96 | `sprd_systimer.ko` | `soc/sprd/` | System timer (vendor) | ❌ config dimatikan |
| 97 | `sprd_time_sync.ko` | `soc/sprd/` | Time sync AP (vendor) | ❌ config dimatikan |
| 98 | `sprd_time_sync_cp.ko` | `soc/sprd/` | Time sync CP (vendor) | ❌ config dimatikan |
| 99 | `sprd_wdt_fiq.ko` | `watchdog/sprd_wdt.c` | Watchdog FIQ (vendor) | ❌ config dimatikan |
| 100 | `sprd-charger-manager.ko` | `power/supply/sprd/sprd_charger_manager.c` | Charger manager (vendor) | ✅ TICK7 compile-verified (gcc 16.2.1) |
| 101 | `sprd-compr-2stage-dma.ko` | `dma/sprd-dma.c` | Compressed 2-stage DMA (vendor) | ❌ config dimatikan |
| 102 | `sprd-cpufreq-public.ko` | `cpufreq/` | CPUFreq public (vendor) | ❌ config dimatikan |
| 103 | `sprd-cpufreq-v2.ko` | `cpufreq/` | CPUFreq v2 (vendor) | ❌ config dimatikan |
| 104 | `sprd-dma.ko` | `dma/sprd-dma.c` | DMA SPRD (vendor) | ❌ config dimatikan |
| 105 | `sprd-dmaengine-pcm.ko` | `sound/soc/sprd/platform_include/` | DMA engine PCM (vendor) | ❌ config dimatikan |
| 106 | `sprd-drm.ko` | `gpu/drm/sprd/sprd_drm.c` | DRM SPRD (vendor) | ✅ TICK9 match |
| 107 | `sprd-gsp.ko` | `gpu/drm/sprd/gsp/gsp_dev.c` | Graphics Signal Processor (vendor) | ✅ TICK9 compile-verified (767.488 B) |
| 108 | `sprd-ion.ko` | `dma-buf/sprd/sprd_ion.c` | ION memory allocator (vendor) | ✅ TICK7 compile-verified (62 KB) |
| 109 | `sprd-platform-pcm-routing.ko` | `sound/soc/sprd/platform/` | Platform PCM routing (vendor) | ❌ config dimatikan |
| 110 | `sprd-sc27xx-spi.ko` | `spi/` | SPI SC27xx (vendor) | ❌ config dimatikan |
| 111 | `sprd-sipc-virt-bus.ko` | `soc/sprd/sipc/` | SIPC virt bus (vendor) | ❌ config dimatikan |
| 112 | `sprd-vsp-pw-domain.ko` | `pmdomain/sprd/sprd_vsp_pw_domain.c` | VSP power domain (vendor) | ✅ TICK7 compile-verified (52 KB) |
| 113 | `trusty.ko` | `trusty/` | Trusty TEE core (vendor) | ❌ config dimatikan |
| 114 | `trusty-ipc.ko` | `trusty/` | Trusty IPC (vendor) | ❌ config dimatikan |
| 115 | `trusty-irq.ko` | `trusty/` | Trusty IRQ (vendor) | ❌ config dimatikan |
| 116 | `trusty-log.ko` | `trusty/` | Trusty log (vendor) | ✅ TICK9 match |
| 117 | `trusty-pm.ko` | `trusty/` | Trusty PM (vendor) | ❌ config dimatikan |
| 118 | `trusty-tui.ko` | `misc/trusty-tui.c` | Trusty TUI (vendor) | ❌ config dimatikan |
| 119 | `trusty-virtio.ko` | `trusty/` | Trusty virtio (vendor) | ❌ config dimatikan |
| 120 | `twofish_common.ko` | `crypto/twofish_common.c` | Twofish common (mainline) | ❌ mungkin built-in |
| 121 | `twofish_generic.ko` | `crypto/twofish_generic.c` | Twofish generic (mainline) | ❌ mungkin built-in |
| 122 | `ums512-clk.ko` | `clk/sprd/` | Clock controller UMS512 (vendor) | ❌ config dimatikan |
| 123 | `unisoc-iommu.ko` | `iommu/sprd-iommu.c` | IOMMU Unisoc/SPRD (vendor) | ✅ TICK7 compile-verified (81 KB) |
| 124 | `unisoc-mailbox.ko` | `mailbox/` | Mailbox Unisoc (vendor) | ❌ config dimatikan |
| 125 | `usb_f_vser.ko` | `usb/gadget/function/` | USB function virtual serial (mainline) | ❌ config dimatikan |
| 126 | `virt-dma.ko` | `dma/virt-dma.c` | Virtual DMA (mainline) | ❌ mungkin built-in |
| 127 | `vsp.ko` | `media/platform/sprd/vsp/sprd_vsp_main.c` | Video Signal Processor (vendor) | ✅ TICK7 compile-verified (231 KB) |
| 128 | `zram.ko` | `block/zram/zram_drv.c` | ZRAM (mainline) | ❌ config ZRAM=y built-in |
| 129 | `zsmalloc.ko` | `mm/zsmalloc.c` | zsmalloc allocator (mainline) | ❌ config dimatikan |

## 3. Ringkasan status (3 kelas)

| Status | Count | Keterangan |
|---|---:|---|
| ✅ **Match (vendor ∩ build TICK9)** | **11** | Driver yang dibuild di TICK9, dan dipakai Advan stock |
| ✅ **Compile-verified TICK7+TICK9** | **13** | Modul yang llvm-nm eviden (di dalam kelas ✅ Match + tambahan sprd_vdsp, sprd_vsp_pw_domain, jpg, sprd_cpp) |
| ❌ **Vendor TIDAK di build TICK9** | **118** | 129 - 11 = 118. Sebagian besar config dimatikan, sebagian built-in ke vmlinux |

## 4. 11 driver yang sama persis (vendor ∩ build TICK9)

| # | Driver | md5 (workspace) | Sumber |
|---:|---|---|---|
| 1 | `aes-neon-blk.ko` | (mainline crypto) | vendor + build TICK9 |
| 2 | `asix.ko` | (mainline USB net) | vendor + build TICK9 |
| 3 | `audio-dsp-dump.ko` | (TICK9 evidence) | vendor + build TICK9 |
| 4 | `ax88179_178a.ko` | (mainline USB net) | vendor + build TICK9 |
| 5 | `kfifo_buf.ko` | (vendor) | vendor + build TICK9 |
| 6 | `pwm_bl.ko` | (mainline backlight) | vendor + build TICK9 |
| 7 | `sprd_battery_info.ko` | `95c8c6225d…` (TICK7) | vendor + build TICK9 + ✅ TICK7 compile-verified |
| 8 | `sprd_camsys_pw_domain.ko` | (TICK7) | vendor + build TICK9 + ✅ TICK7 compile-verified |
| 9 | `sprd_power_manager.ko` | (vendor) | vendor + build TICK9 |
| 10 | `sprd-drm.ko` | (vendor) | vendor + build TICK9 |
| 11 | `trusty-log.ko` | (vendor) | vendor + build TICK9 |

## 5. 13 driver dengan bukti compile-verified (TICK7+TICK9 llvm-nm)

| # | Driver | Ukuran .ko | Status | Build run |
|---:|---|---:|---|---|
| 1 | `sprd_vsp.ko` (≡ `vsp.ko`) | 231 KB | ✅ TICK7 | gcc 16.2.1, exit 0 |
| 2 | `sprd_cpp.ko` (≡ `sprd_cpp.ko`) | 321 KB | ✅ TICK7 | gcc 16.2.1 |
| 3 | `sprd-iommu.ko` (≡ `unisoc-iommu.ko`) | 81 KB | ✅ TICK7 | gcc 16.2.1 |
| 4 | `sprd_ion.ko` | 62 KB | ✅ TICK7 | gcc 16.2.1 |
| 5 | `sprd_camsys_pw_domain.ko` | 95 KB | ✅ TICK7 | gcc 16.2.1 |
| 6 | `sprd_vsp_pw_domain.ko` (≡ `sprd-vsp-pw-domain.ko`) | 52 KB | ✅ TICK7 | gcc 16.2.1 |
| 7 | `sprd_battery_info.ko` | 96 KB | ✅ TICK7 | gcc 16.2.1 |
| 8 | `wcn_core.ko` (≡ `core.ko` digabung) | composite | ✅ TICK7 | TICK7 heuristic |
| 9 | `sprdwl.ko` (≡ `sprd_wlan_combo.ko`) | 2,5 MB | ✅ TICK7 | gcc 16.2.1 |
| 10 | `sprd_jpg.ko` (≡ `jpg.ko`) | 156 KB | ✅ TICK7 | gcc 16.2.1 |
| 11 | `sprd_vdsp.ko` | 913.568 B | ✅ TICK7 | t21_full.log |
| 12 | `sprd-gsp.ko` | 767.488 B | ✅ TICK9 | llvm-nm 535 simbol |
| 13 | `novatek_ts_spi.ko` | 512.584 B | ✅ TICK9 | llvm-nm 337 simbol |

## 6. Statistik

| Sumber | Total | Cross-check |
|---|---:|---|
| Vendor Advan (`vramdisk_ko/`) | 129 .ko | — |
| Build TICK9 (`kolist.1012b.txt`) | 1.424 .ko | — |
| Vendor ∩ Build | 11 .ko | (8.5% vendor) |
| Vendor − Build | 118 .ko | (91.5% vendor) — config dimatikan/built-in |
| Build − Vendor | 1.413 .ko | (99.2% build) — mainline generic |

**Catatan penting**: Mayoritas 118 vendor .ko TIDAK ada di build TICK9 karena:
- **Config dimatikan** (CONFIG_*=n): trusty, sensor, pmic_syscon, dll
- **Built-in ke vmlinux** (=y): clk-sprd, musb_hdrc, pinctrl, zram, dll
- **Belum di-port ke 6.18** (pakai ABI/kernel 5.4 asli): agdsp_access, mipi_driver, sprd_pdbg, dll

## 7. Apa yang TIDAK termasuk

- **Mainline `.ko` yang generic** (crypto, net, sound, input): 1.413 .ko, engga relevan spesifik Advan T1030
- **Built-in driver** yang `obj-y`: engga jadi `.ko` terpisah, di-link ke vmlinux
- **Thermal DT chain** (TICK9 §4) — `ap_efuse` + 13 sel kalibrasi + 3 `ap_thm0/1/2` + `virtual_sensor`: BELUM di-port
- **Config yang dimatikan**: 118 .ko vendor perlu dicheck ulang apakah wajib diaktifkan atau engga

## 8. Action items ke depan (urut prioritas)

1. **Buka config vendor T1030 stock** (TICK7 §5 config), cross-check `.config` sekarang, hidupkan yang vital.
2. **Bangun ulang `make modules`** (TICK10) untuk verifikasi semua .ko yang aktif bisa ke-build.
3. **Resync 1.435 .ko ke config de-bloat** (TICK9 §3) — wajib sebelum insmod/flash.
4. **Port thermal DT chain** (TICK9 §4) — `ap_efuse` + 13 sel kalibrasi + 3 `ap_thm` + `virtual_sensor`.
5. **Boot test + insmod** — satu-satunya cara verifikasi driver beneran hidup di hardware.

— Berry Chan, Raspya, 2026-10-15 🍓
