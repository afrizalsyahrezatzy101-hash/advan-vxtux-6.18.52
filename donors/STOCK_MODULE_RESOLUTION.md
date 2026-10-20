# Stock Vendor Module Resolution — VXTux 6.18.52 (final)

- VXTux .ko built: 177
- Stock vendor .ko: 31
- MODULE: 4
- BUILTIN: 18
- MODULE_NOT_BUILT: 1
- DISABLED: 7
- NO_SYMBOL: 1
- **COVERED (MODULE + BUILTIN): 22/31 (71%)**

## NO_SYMBOL (1)

| stock module | config symbol | value | VXTux .ko |
|---|---|---|---|
| `audio_sipc` | — | — | — |

## DISABLED (7)

| stock module | config symbol | value | VXTux .ko |
|---|---|---|---|
| `audio-dsp-dump` | `CONFIG_SPRD_AUDIO_DSP_DUMP` | `absent` | — |
| `audio-pipe` | `CONFIG_SPRD_AUDIO_PIPE` | `absent` | — |
| `mmdvfs` | `CONFIG_SPRD_MMC_DVFS`, `CONFIG_MMC_DVFS` | `absent`, `absent` | — |
| `sprd-dmaengine-pcm` | `CONFIG_SPRD_DMAENGINE_PCM`, `CONFIG_SND_SOC_SPRD_DMAENGINE_PCM` | `absent`, `absent` | — |
| `sprd_apipe` | `CONFIG_SPRD_APIPE`, `CONFIG_SPRD_AUDIO_PIPE` | `absent`, `absent` | — |
| `sprd_flash_drv` | `CONFIG_SPRD_FLASH` | `absent` | — |
| `usb_f_vser` | `CONFIG_USB_F_VSER` | `absent` | — |

## MODULE_NOT_BUILT (1)

| stock module | config symbol | value | VXTux .ko |
|---|---|---|---|
| `apsys-dvfs` | `CONFIG_SPRD_APSYS_DVFS` | `m` | — |

## BUILTIN (18)

| stock module | config symbol | value | VXTux .ko |
|---|---|---|---|
| `aes-ce-ccm` | `CONFIG_CRYPTO_AES_ARM64_CE_CCM` | `y` | — |
| `gpio-pmic-eic-sprd` | `CONFIG_GPIO_PMIC_EIC_SPRD` | `y` | — |
| `ion_ipc_trusty` | `CONFIG_ION_IPC_TRUSTY` | `y` | — |
| `novatek_ts_spi` | `CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS` | `y` | — |
| `nvmem-sc27xx-efuse` | `CONFIG_NVMEM_SC27XX_EFUSE` | `y` | — |
| `nvmem_sprd_cache_efuse` | `CONFIG_NVMEM_SPRD_CACHE_EFUSE` | `y` | — |
| `rtc-sc27xx` | `CONFIG_RTC_DRV_SC27XX` | `y` | — |
| `sc27xx_adc` | `CONFIG_SC27XX_ADC` | `y` | — |
| `sdhci-sprd` | `CONFIG_MMC_SDHCI_SPRD` | `y` | — |
| `sprd-sipc-virt-bus` | `CONFIG_SPRD_SIPC_VIRT_BUS`, `CONFIG_SPRD_SIPC` | `absent`, `y` | — |
| `sprd-vsp-pw-domain` | `CONFIG_SPRD_VSP_PW_DOMAIN` | `y` | — |
| `sprd_time_sync` | `CONFIG_SPRD_TIME_SYNC`, `CONFIG_SPRD_TSHM` | `absent`, `y` | — |
| `trusty-ipc` | `CONFIG_TRUSTY_VIRTIO_IPC`, `CONFIG_TRUSTY` | `y`, `y` | — |
| `trusty-irq` | `CONFIG_TRUSTY` | `y` | — |
| `trusty-tui` | `CONFIG_TRUSTY_TUI` | `y` | — |
| `trusty` | `CONFIG_TRUSTY` | `y` | — |
| `twofish_common` | `CONFIG_CRYPTO_TWOFISH_COMMON` | `y` | — |
| `unisoc-mailbox` | `CONFIG_SPRD_MAILBOX`, `CONFIG_MAILBOX` | `y`, `y` | — |

## MODULE (4)

| stock module | config symbol | value | VXTux .ko |
|---|---|---|---|
| `pwm_bl` | `CONFIG_BACKLIGHT_PWM` | `m` | `pwm_bl` |
| `sprd_power_manager` | `CONFIG_SPRD_POWER_MANAGER` | `m` | `sprd_power_manager` |
| `trusty-log` | `CONFIG_TRUSTY_LOG` | `m` | `trusty-log` |
| `ums512-clk` | `CONFIG_SPRD_UMS512_CLK` | `m` | `ums512-clk` |
