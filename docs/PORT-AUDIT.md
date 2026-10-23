# PORT AUDIT — 129 .ko vendor Advan vs tree 6.18.52-VXTux-UMS512

Dibuat otomatis oleh `donors/port_audit.py` (jangan edit manual).

Status: **KO** = ter-build .ko · **Y** = sumber ada + `=y` (built-in) · **OBJ-Y** = selalu built-in · **OFF** = sumber + Kconfig ada, belum diaktifkan · **NOKCONFIG** = Makefile pakai CONFIG_ tapi entri Kconfig hilang · **OFF-DESIGN** = sengaja `# ... is not set` · **NOSRC** = sumber driver tidak ada di tree (butuh port kode baru)

## Ringkasan

| Status | Jumlah |
|---|---:|
| KO | 70 |
| Y | 47 |
| OFF | 2 |
| OFF-DESIGN | 1 |
| NOSRC | 33 |
| **total** | **153** |

## Tabel

| # | Driver | Status | Simbol | Sumber |
|---:|---|---|---|---|
| 1 | `aes-ce-ccm.ko` | KO | `CONFIG_CRYPTO_AES_ARM64_CE_CCM` | `./arch/arm64/crypto` |
| 2 | `aes-neon-blk.ko` | KO | `CONFIG_CRYPTO_AES_ARM64_NEON_BLK` | `./arch/arm64/crypto` |
| 3 | `aes-neon-blk.ko` | KO | `CONFIG_CRYPTO_AES_ARM64_NEON_BLK` | `./arch/arm64/crypto` |
| 4 | `agdsp_access.ko` | KO | `CONFIG_SPRD_AUDIO_AGDSP_ACCESS` | `./drivers/vxtux/audio/agdsp_access` |
| 5 | `apsys-dvfs.ko` | KO | `CONFIG_SPRD_APSYS_DVFS` | `./drivers/devfreq/sprd/apsys` |
| 6 | `asix.ko` | KO | `CONFIG_USB_NET_AX8817X` | `./drivers/net/usb` |
| 7 | `asix.ko` | KO | `CONFIG_USB_NET_AX8817X` | `./drivers/net/usb` |
| 8 | `audio-dsp-dump.ko` | KO | `CONFIG_SPRD_AUDIO_DSP_DUMP` | `./drivers/vxtux/audio/audiodspdump` |
| 9 | `audio-dsp-dump.ko` | KO | `CONFIG_SPRD_AUDIO_DSP_DUMP` | `./drivers/vxtux/audio/audiodspdump` |
| 10 | `audio-pipe.ko` | KO | `CONFIG_SPRD_AUDIO_PIPE` | `./drivers/vxtux/audio/audiosipc` |
| 11 | `audio_sipc.ko` | KO | `CONFIG_SPRD_AUDIO_SIPC` | `./drivers/vxtux/audio/audiosipc` |
| 12 | `ax88179_178a.ko` | KO | `CONFIG_USB_NET_AX88179_178A` | `./drivers/net/usb` |
| 13 | `ax88179_178a.ko` | KO | `CONFIG_USB_NET_AX88179_178A` | `./drivers/net/usb` |
| 14 | `clk-sprd.ko` | KO | `CONFIG_SPRD_COMMON_CLK` | `./drivers/clk/sprd` |
| 15 | `extcon-usb-gpio.ko` | KO | `CONFIG_EXTCON_USB_GPIO` | `./drivers/extcon` |
| 16 | `gpio-pmic-eic-sprd.ko` | KO | `CONFIG_GPIO_PMIC_EIC_SPRD` | `./drivers/gpio` |
| 17 | `ion_ipc_trusty.ko` | KO | `CONFIG_ION_IPC_TRUSTY` | `./drivers/dma-buf/sprd` |
| 18 | `mipi_driver.ko` | KO | `CONFIG_SPRD_MIPI_LOG_R5P0` | `./drivers/unisoc_platform/debug_log/sharkl5` |
| 19 | `mmc_hsq.ko` | KO | `CONFIG_MMC_HSQ` | `./drivers/mmc/host` |
| 20 | `mmdvfs.ko` | KO | `CONFIG_SPRD_MM_DVFS` | `./drivers/devfreq/sprd/mm` |
| 21 | `novatek_ts_spi.ko` | KO | `CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI` | `./drivers/input/touchscreen/novatek_nvt_ts_spi` |
| 22 | `novatek_ts_spi.ko` | KO | `CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS_SPI` | `./drivers/input/touchscreen/novatek_nvt_ts_spi` |
| 23 | `nvmem-sc27xx-efuse.ko` | KO | `CONFIG_NVMEM_SC27XX_EFUSE` | `./drivers/nvmem` |
| 24 | `nvmem_sprd_cache_efuse.ko` | KO | `CONFIG_NVMEM_SPRD_CACHE_EFUSE` | `./drivers/vxtux/soc/cache_efuse` |
| 25 | `nvmem_sprd_efuse.ko` | KO | `CONFIG_NVMEM_SPRD_EFUSE` | `./drivers/nvmem` |
| 26 | `phy-sprd-sharkl5Pro.ko` | KO | `` | `phy/sprd/phy-sprd-ums512.c` |
| 27 | `pwm_bl.ko` | KO | `CONFIG_BACKLIGHT_PWM` | `./drivers/video/backlight` |
| 28 | `pwm_bl.ko` | KO | `CONFIG_BACKLIGHT_PWM` | `./drivers/video/backlight` |
| 29 | `rtc-sc27xx.ko` | KO | `CONFIG_RTC_DRV_SC27XX` | `./drivers/rtc` |
| 30 | `sc2730-regulator.ko` | KO | `CONFIG_REGULATOR_SC2730` | `./drivers/regulator` |
| 31 | `sc27xx_adc.ko` | KO | `CONFIG_SC27XX_ADC` | `./drivers/iio/adc` |
| 32 | `sc27xx_typec.ko` | KO | `CONFIG_TYPEC_SPRD` | `./drivers/usb/typec/sprd` |
| 33 | `sdhci-sprd.ko` | KO | `CONFIG_MMC_SDHCI_SPRD` | `./drivers/mmc/host` |
| 34 | `sensorhub.ko` | KO | `CONFIG_IIO_SSP_SENSORHUB` | `./drivers/iio/common/ssp_sensors` |
| 35 | `slog_bridge.ko` | KO | `CONFIG_SPRD_SIPC_SLOG_BRIDGE` | `./drivers/vxtux/sipc` |
| 36 | `snd-soc-sprd-vbc-v4.ko` | KO | `CONFIG_VXTUX_SND_SPRD_VBC_V4` | `./drivers/vxtux/sound/asoc/dai_v4` |
| 37 | `spi-sprd.ko` | KO | `CONFIG_SPI_SPRD` | `./drivers/spi` |
| 38 | `sprd-dmaengine-pcm.ko` | KO | `CONFIG_VXTUX_SND_SPRD_DMAENGINE_PCM` | `./drivers/vxtux/sound/asoc/platform` |
| 39 | `sprd-drm.ko` | KO | `CONFIG_DRM_SPRD` | `./drivers/gpu/drm/sprd` |
| 40 | `sprd-drm.ko` | KO | `CONFIG_DRM_SPRD` | `./drivers/gpu/drm/sprd` |
| 41 | `sprd-gsp.ko` | KO | `CONFIG_DRM_SPRD_GSP` | `./drivers/gpu/drm/sprd/gsp` |
| 42 | `sprd-gsp.ko` | KO | `CONFIG_DRM_SPRD_GSP` | `./drivers/gpu/drm/sprd/gsp` |
| 43 | `sprd-sipc-virt-bus.ko` | KO | `CONFIG_SPRD_SIPC_VIRT_BUS` | `./drivers/vxtux/sipc` |
| 44 | `sprd_apipe.ko` | KO | `CONFIG_SPRD_APIPE` | `./drivers/vxtux/audio/audio_pipe` |
| 45 | `sprd_audcp_boot.ko` | KO | `CONFIG_SPRD_AUDIO_CPBOOT` | `./drivers/vxtux/audio/audiocpboot` |
| 46 | `sprd_audcp_dvfs.ko` | KO | `CONFIG_SPRD_HW_DEVICE_DVFS_AGCP` | `./drivers/vxtux/audio/audiodvfs` |
| 47 | `sprd_battery_info.ko` | KO | `CONFIG_SPRD_BATTERY_INFO` | `./drivers/power/supply/sprd` |
| 48 | `sprd_battery_info.ko` | KO | `CONFIG_SPRD_BATTERY_INFO` | `./drivers/power/supply/sprd` |
| 49 | `sprd_battery_info.ko` | KO | `CONFIG_SPRD_BATTERY_INFO` | `./drivers/power/supply/sprd` |
| 50 | `sprd_flash_drv.ko` | KO | `CONFIG_SPRD_FLASH_DRV` | `./drivers/sprd/flash` |
| 51 | `sprd_mipi.ko` | KO | `CONFIG_SPRD_MODEM_DEBUG` | `./drivers/unisoc_platform/debug_log` |
| 52 | `sprd_power_manager.ko` | KO | `CONFIG_SPRD_POWER_MANAGER` | `./drivers/soc/sprd/power_manager` |
| 53 | `sprd_power_manager.ko` | KO | `CONFIG_SPRD_POWER_MANAGER` | `./drivers/soc/sprd/power_manager` |
| 54 | `sprd_shm.ko` | KO | `CONFIG_SPRD_TSHM` | `./drivers/trusty` |
| 55 | `sprd_systimer.ko` | KO | `CONFIG_SPRD_SYSTIMER` | `./drivers/clocksource` |
| 56 | `sprd_time_sync.ko` | KO | `CONFIG_SPRD_TIME_SYNC` | `./drivers/soc/sprd/time_sync` |
| 57 | `sprd_time_sync_cp.ko` | KO | `CONFIG_SPRD_TIME_SYNC_CP` | `./drivers/soc/sprd/time_sync` |
| 58 | `sprdwl.ko` | KO | `CONFIG_WCN_WLAN` | `./drivers/net/wireless/sprd/sprdwcn/wlan` |
| 59 | `trusty-ipc.ko` | KO | `CONFIG_TRUSTY_VIRTIO_IPC` | `./drivers/trusty` |
| 60 | `trusty-irq.ko` | KO | `CONFIG_TRUSTY` | `./drivers/trusty` |
| 61 | `trusty-log.ko` | KO | `CONFIG_TRUSTY_LOG` | `./drivers/trusty` |
| 62 | `trusty-log.ko` | KO | `CONFIG_TRUSTY_LOG` | `./drivers/trusty` |
| 63 | `trusty-pm.ko` | KO | `CONFIG_TRUSTY` | `./drivers/trusty` |
| 64 | `trusty-tui.ko` | KO | `CONFIG_TRUSTY_TUI` | `./drivers/misc` |
| 65 | `trusty-virtio.ko` | KO | `CONFIG_TRUSTY_VIRTIO` | `./drivers/trusty` |
| 66 | `trusty.ko` | KO | `CONFIG_TRUSTY` | `./drivers/trusty` |
| 67 | `ums512-clk.ko` | KO | `CONFIG_SPRD_UMS512_CLK` | `./drivers/clk/sprd` |
| 68 | `unisoc-mailbox.ko` | KO | `CONFIG_SPRD_MBOX` | `./drivers/mailbox` |
| 69 | `usb_f_vser.ko` | KO | `CONFIG_USB_F_VSERIAL` | `./drivers/usb/gadget/function` |
| 70 | `wcn_core.ko` | KO | `CONFIG_SPRD_LEGACY_WCN_CORE` | `./drivers/net/wireless/sprd/sprdwcn/platform` |
| 71 | `audio_mem.ko` | Y | `CONFIG_SPRD_AUDIO_MEMORY_MANAGER` | `./drivers/vxtux/audio/audiomem` |
| 72 | `core.ko` | Y | `CONFIG_KPROBES` | `./arch/arm/probes/kprobes` |
| 73 | `ghash-ce.ko` | Y | `CONFIG_CRYPTO_GHASH_ARM64_CE` | `./arch/arm64/crypto` |
| 74 | `gpio-eic-sprd.ko` | Y | `CONFIG_GPIO_EIC_SPRD` | `./drivers/gpio` |
| 75 | `gpio-sprd.ko` | Y | `CONFIG_GPIO_SPRD` | `./drivers/gpio` |
| 76 | `i2c-sprd.ko` | Y | `CONFIG_I2C_SPRD` | `./drivers/i2c/busses` |
| 77 | `kfifo_buf.ko` | Y | `CONFIG_IIO_KFIFO_BUF` | `./drivers/iio/buffer` |
| 78 | `kfifo_buf.ko` | Y | `CONFIG_IIO_KFIFO_BUF` | `./drivers/iio/buffer` |
| 79 | `leds-sc27xx-bltc.ko` | Y | `CONFIG_LEDS_SC27XX_BLTC` | `./drivers/leds` |
| 80 | `ledtrig-pattern.ko` | Y | `CONFIG_LEDS_TRIGGER_PATTERN` | `./drivers/leds/trigger` |
| 81 | `mcdt_hw.ko` | Y | `CONFIG_SPRD_AUDIO_MCDT_R1P0` | `./drivers/vxtux/audio/mcdt/mcdt_r1p0` |
| 82 | `musb_hdrc.ko` | Y | `CONFIG_USB_MUSB_HDRC` | `./drivers/usb/musb` |
| 83 | `pinctrl-sprd.ko` | Y | `CONFIG_PINCTRL_SPRD` | `./drivers/pinctrl/sprd` |
| 84 | `pinctrl.ko` | Y | `CONFIG_PINCTRL` | `./drivers/base` |
| 85 | `pwm-sprd.ko` | Y | `CONFIG_PWM_SPRD` | `./drivers/pwm` |
| 86 | `sc27xx-poweroff.ko` | Y | `CONFIG_POWER_RESET_SC27XX` | `./drivers/power/reset` |
| 87 | `sc27xx-vibra.ko` | Y | `CONFIG_INPUT_SC27XX_VIBRA` | `./drivers/input/misc` |
| 88 | `sc27xx_fuel_gauge.ko` | Y | `CONFIG_FUEL_GAUGE_SC27XX` | `./drivers/power/supply` |
| 89 | `sipx.ko` | Y | `CONFIG_SPRD_SIPC_ZERO_COPY_SIPX` | `./drivers/soc/sprd/sipc` |
| 90 | `snd-soc-sprd-codec-sc2730.ko` | Y | `CONFIG_VXTUX_SND_SPRD_SC2730` | `./drivers/vxtux/sound/asoc/codec_sc2730` |
| 91 | `spi-sprd-adi.ko` | Y | `CONFIG_SPI_SPRD_ADI` | `./drivers/spi` |
| 92 | `spipe.ko` | Y | `CONFIG_SPRD_SIPC_SPIPE` | `./drivers/soc/sprd/sipc` |
| 93 | `spool.ko` | Y | `CONFIG_SPRD_SIPC_SPOOL` | `./drivers/soc/sprd/sipc` |
| 94 | `sprd-charger-manager.ko` | Y | `CONFIG_CHARGER_SPRD_MANAGER` | `./drivers/power/supply/sprd` |
| 95 | `sprd-dma.ko` | Y | `CONFIG_SPRD_DMA` | `./drivers/dma` |
| 96 | `sprd-iommu.ko` | Y | `CONFIG_SPRD_IOMMU` | `./drivers/iommu` |
| 97 | `sprd-ion.ko` | Y | `CONFIG_SPRD_ION` | `./drivers/dma-buf/sprd` |
| 98 | `sprd-platform-pcm-routing.ko` | Y | `CONFIG_VXTUX_SND_SPRD_PCM_ROUTING` | `./drivers/vxtux/sound/asoc/platform` |
| 99 | `sprd-sc27xx-spi.ko` | Y | `CONFIG_MFD_SC27XX_PMIC` | `./drivers/mfd` |
| 100 | `sprd-vsp-pw-domain.ko` | Y | `CONFIG_SPRD_VSP_PW_DOMAIN` | `./drivers/pmdomain/sprd` |
| 101 | `sprd_camsys_pw_domain.ko` | Y | `CONFIG_SPRD_CAMSYS_PW_DOMAIN` | `./drivers/pmdomain/sprd` |
| 102 | `sprd_camsys_pw_domain.ko` | Y | `CONFIG_SPRD_CAMSYS_PW_DOMAIN` | `./drivers/pmdomain/sprd` |
| 103 | `sprd_camsys_pw_domain.ko` | Y | `CONFIG_SPRD_CAMSYS_PW_DOMAIN` | `./drivers/pmdomain/sprd` |
| 104 | `sprd_hwspinlock.ko` | Y | `CONFIG_HWSPINLOCK_SPRD` | `./drivers/hwspinlock` |
| 105 | `sprd_ion.ko` | Y | `CONFIG_SPRD_ION` | `./drivers/dma-buf/sprd` |
| 106 | `sprd_jpg.ko` | Y | `CONFIG_SPRD_JPG` | `./drivers/misc/sprd_jpg` |
| 107 | `sprd_modem_loader.ko` | Y | `CONFIG_SPRD_MODEM_LOADER` | `./drivers/soc/sprd/modem/modem_loader` |
| 108 | `sprd_pmic_syscon.ko` | Y | `CONFIG_SPRD_PMIC_SYSCON` | `./drivers/soc/sprd/pmic_syscon` |
| 109 | `sprd_soc_id.ko` | Y | `CONFIG_SPRD_SOCID` | `./drivers/soc/sprd/soc_id` |
| 110 | `sprd_vsp.ko` | Y | `CONFIG_SPRD_VSP` | `./drivers/media/platform/sprd/vsp` |
| 111 | `sprd_vsp_pw_domain.ko` | Y | `CONFIG_SPRD_VSP_PW_DOMAIN` | `./drivers/pmdomain/sprd` |
| 112 | `sprd_wdt_fiq.ko` | Y | `CONFIG_SPRD_WATCHDOG_FIQ` | `./drivers/watchdog` |
| 113 | `twofish_common.ko` | Y | `CONFIG_CRYPTO_TWOFISH_COMMON` | `./crypto` |
| 114 | `twofish_generic.ko` | Y | `CONFIG_CRYPTO_TWOFISH` | `./crypto` |
| 115 | `virt-dma.ko` | Y | `CONFIG_DMA_VIRTUAL_CHANNELS` | `./drivers/dma` |
| 116 | `zram.ko` | Y | `CONFIG_ZRAM` | `./drivers/block/zram` |
| 117 | `zsmalloc.ko` | Y | `CONFIG_ZSMALLOC` | `./mm` |
| 118 | `arc4.ko` | OFF | `CONFIG_CRYPTO_ARC4` | `./crypto` |
| 119 | `gpio.ko` | OFF | `CONFIG_MTD_NAND_GPIO` | `./drivers/mtd/nand/raw` |
| 120 | `sprd_sensor.ko` | OFF-DESIGN | `CONFIG_SPRD_SENSOR` | `./drivers/media/platform/sprd/sensors/sprd_sensor` |
| 121 | `bq2560x-charger.ko` | NOSRC | `` | `power/supply/` |
| 122 | `hxchipset_83102e.ko` | NOSRC | `` | `input/touchscreen/sprd/hx83102e/` |
| 123 | `i2c-sprd-hw.ko` | NOSRC | `` | `i2c/busses/` |
| 124 | `ion_cma_heap.ko` | NOSRC | `` | `dma-buf/heaps/cma_heap.c` |
| 125 | `jpg.ko` | NOSRC | `` | `misc/sprd_jpg/sprd_jpg_main.c` |
| 126 | `misc_sprd_uid.ko` | NOSRC | `` | `soc/sprd/soc_id/sprd_uid.c` |
| 127 | `mmc_swcq.ko` | NOSRC | `` | `mmc/host/sdhci-sprd.c` |
| 128 | `musb_sprd.ko` | NOSRC | `` | `usb/musb/musb_sprd_vxtux.c` |
| 129 | `pinctrl-sprd-sharkl5Pro.ko` | NOSRC | `` | `pinctrl/sprd/pinctrl-sprd-sharkl5pro.c` |
| 130 | `sblock_bridge.ko` | NOSRC | `` | `soc/sprd/sipc/` |
| 131 | `sbuf_bridge.ko` | NOSRC | `` | `soc/sprd/sipc/` |
| 132 | `sha1-ce.ko` | NOSRC | `` | `crypto/sha1.c` |
| 133 | `sipc-core.ko` | NOSRC | `` | `soc/sprd/sipc/sipc.c` |
| 134 | `snd-soc-sprd-card.ko` | NOSRC | `` | `sound/soc/sprd/card/` |
| 135 | `snd-soc-sprd-codec-sc2730-power-dev.ko` | NOSRC | `` | `sound/soc/sprd/codec/` |
| 136 | `snd-soc-sprd-codec-sc2730-power.ko` | NOSRC | `` | `sound/soc/sprd/codec/` |
| 137 | `snd-soc-sprd-dummy-codec.ko` | NOSRC | `` | `sound/soc/sprd/codec/dummy-codec.c` |
| 138 | `snd-soc-sprd-vbc-fe.ko` | NOSRC | `` | `sound/soc/sprd/dai_v4/vbc-dai.c` |
| 139 | `sprd-compr-2stage-dma.ko` | NOSRC | `` | `dma/sprd-dma.c` |
| 140 | `sprd-cpufreq-public.ko` | NOSRC | `` | `cpufreq/` |
| 141 | `sprd-cpufreq-v2.ko` | NOSRC | `` | `cpufreq/` |
| 142 | `sprd_7sreset.ko` | NOSRC | `` | `soc/sprd/` |
| 143 | `sprd_camera.ko` | NOSRC | `` | `media/platform/sprd/pipeline/sprd_dcam.c` |
| 144 | `sprd_cpp.ko` | NOSRC | `` | `media/platform/sprd/cpp/cpp_core.c` |
| 145 | `sprd_cpp.ko` | NOSRC | `` | `321 KB` |
| 146 | `sprd_iq.ko` | NOSRC | `` | `media/platform/sprd/` |
| 147 | `sprd_manufacturer_model.ko` | NOSRC | `` | `soc/sprd/` |
| 148 | `sprd_pdbg.ko` | NOSRC | `` | `soc/sprd/` |
| 149 | `sprd_power_stat.ko` | NOSRC | `` | `soc/sprd/` |
| 150 | `sprd_sip_svc.ko` | NOSRC | `` | `soc/sprd/` |
| 151 | `sprd_vdsp.ko` | NOSRC | `` | `913.568 B` |
| 152 | `unisoc-iommu.ko` | NOSRC | `` | `iommu/sprd-iommu.c` |
| 153 | `vsp.ko` | NOSRC | `` | `media/platform/sprd/vsp/sprd_vsp_main.c` |
