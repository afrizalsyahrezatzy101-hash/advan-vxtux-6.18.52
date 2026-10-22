# Debloat Verification — stock vendor blob vs VXTux substitute

- Stock vendor .ko examined: 31
- COMPILE+SYMBOL: 4
- CONFIG-BUILTIN: 16
- DISABLED: 10
- MODULE-DISABLED: 1
- **Replaced by a built VXTux artifact: 20/31 (65%)**

| stock .ko | substitute | config | value | tier | symbols covered | missing |
|---|---|---|---|---|---|---|
| `aes-ce-ccm` | — | `CONFIG_CRYPTO_AES_ARM64_CE_CCM` | `y` | CONFIG-BUILTIN | 2/6 | `ce_aes_ccm_auth_data`, `ce_aes_ccm_final`, `cleanup_module`, `init_module` |
| `apsys-dvfs` | — | `CONFIG_SPRD_APSYS_DVFS` | `m` | MODULE-DISABLED | 0/74 | `apsys_dvfs_head`, `cleanup_module`, `dpu_devfreq_gov`, `dpu_dvfs_chain`, `dpu_dvfs_driver`, `dpu_dvfs_head` |
| `audio-dsp-dump` | — | `CONFIG_SPRD_AUDIO_DSP_DUMP` | `absent` | DISABLED | 0/16 | `audio_sblock_create`, `audio_sblock_destroy`, `audio_sblock_get`, `audio_sblock_get_arrived_count`, `audio_sblock_get_free_count`, `audio_sblock_init` |
| `audio-pipe` | — | `CONFIG_SPRD_AUDIO_PIPE` | `absent` | DISABLED | 0/2 | `cleanup_module`, `init_module` |
| `audio_sipc` | — | `CONFIG_SPRD_AUDIO_SIPC` | `absent` | DISABLED | 0/32 | `aud_get_aud_ipc_smsg_addr`, `aud_get_aud_ipc_smsg_para_addr`, `aud_ipc_ch_close`, `aud_ipc_ch_open`, `aud_ipc_dump`, `aud_recv_block_param` |
| `gpio-pmic-eic-sprd` | — | `CONFIG_GPIO_PMIC_EIC_SPRD` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `ion_ipc_trusty` | — | `CONFIG_ION_IPC_TRUSTY` | `y` | CONFIG-BUILTIN | 4/6 | `ion_handle_msg`, `ion_tipc` |
| `mmdvfs` | — | `CONFIG_SPRD_MMC_DVFS` | `absent` | DISABLED | 0/117 | `AddNode`, `CreateNode`, `DelNode`, `Destroy`, `GetFrontNodeFreq`, `InitPQueue` |
| `novatek_ts_spi` | — | `CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS` | `y` | CONFIG-BUILTIN | 0/53 | `Boot_Update_Firmware`, `CTP_SPI_READ`, `CTP_SPI_WRITE`, `ENG_RST_ADDR`, `SPI_RD_FAST_ADDR`, `SWRST_N8_ADDR` |
| `nvmem-sc27xx-efuse` | — | `CONFIG_NVMEM_SC27XX_EFUSE` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `nvmem_sprd_cache_efuse` | — | `CONFIG_NVMEM_SPRD_CACHE_EFUSE` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `pwm_bl` | `pwm_bl.ko` | `CONFIG_BACKLIGHT_PWM` | `m` | COMPILE+SYMBOL | 2/2 | — |
| `rtc-sc27xx` | — | `CONFIG_RTC_DRV_SC27XX` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `sc27xx_adc` | — | `CONFIG_SC27XX_ADC` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `sdhci-sprd` | — | `CONFIG_MMC_SDHCI_SPRD` | `y` | CONFIG-BUILTIN | 0/12 | `_sdhci_request_atomic`, `cleanup_module`, `init_module`, `mmc_check_wp_fn`, `mmc_hsq_swcq_init`, `mmc_send_tuning_cmd` |
| `sprd-dmaengine-pcm` | — | `CONFIG_SPRD_DMAENGINE_PCM` | `absent` | DISABLED | 0/4 | `cleanup_module`, `init_module`, `pm_dma`, `test_force_stop` |
| `sprd-sipc-virt-bus` | — | `CONFIG_SPRD_SIPC_VIRT_BUS` | `absent` | DISABLED | 0/2 | `cleanup_module`, `init_module` |
| `sprd-vsp-pw-domain` | — | `CONFIG_SPRD_VSP_PW_DOMAIN` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `sprd_apipe` | — | `CONFIG_SPRD_APIPE` | `absent` | DISABLED | 0/2 | `cleanup_module`, `init_module` |
| `sprd_flash_drv` | — | `CONFIG_SPRD_FLASH` | `absent` | DISABLED | 0/7 | `cleanup_module`, `flash_get_flash_info`, `init_module`, `sprd_flash_cfg`, `sprd_flash_ctrl`, `sprd_flash_get_info` |
| `sprd_power_manager` | `sprd_power_manager.ko` | `CONFIG_SPRD_POWER_MANAGER` | `m` | COMPILE+SYMBOL | 16/16 | — |
| `sprd_time_sync` | — | `CONFIG_SPRD_TIME_SYNC` | `absent` | DISABLED | 0/3 | `cleanup_module`, `init_module`, `sprd_send_ap_time` |
| `trusty` | — | `CONFIG_TRUSTY` | `y` | CONFIG-BUILTIN | 13/15 | `cleanup_module`, `init_module` |
| `trusty-ipc` | — | `CONFIG_TRUSTY_VIRTIO_IPC` | `y` | CONFIG-BUILTIN | 10/12 | `cleanup_module`, `init_module` |
| `trusty-irq` | — | `CONFIG_TRUSTY` | `y` | CONFIG-BUILTIN | 0/2 | `cleanup_module`, `init_module` |
| `trusty-log` | `trusty-log.ko` | `CONFIG_TRUSTY_LOG` | `m` | COMPILE+SYMBOL | 3/3 | — |
| `trusty-tui` | — | `CONFIG_TRUSTY_TUI` | `y` | CONFIG-BUILTIN | 2/5 | `cleanup_module`, `init_module`, `trusty_tui_driver` |
| `twofish_common` | — | `CONFIG_CRYPTO_TWOFISH_COMMON` | `y` | CONFIG-BUILTIN | 1/1 | — |
| `ums512-clk` | `ums512-clk.ko` | `CONFIG_SPRD_UMS512_CLK` | `m` | COMPILE+SYMBOL | 2/2 | — |
| `unisoc-mailbox` | — | `CONFIG_SPRD_MAILBOX` | `y` | CONFIG-BUILTIN | 0/8 | `get_tx_fifo_mask`, `init_module`, `mbox_phy_ops`, `mbox_start_send_tx_fifo`, `sprd_mbox_change_wakeup_flag_r1`, `sprd_mbox_phy_r1_init` |
| `usb_f_vser` | — | `CONFIG_USB_F_VSER` | `absent` | DISABLED | 0/6 | `cleanup_module`, `init_module`, `kernel_vser_register_callback`, `kernel_vser_set_pass_mode`, `vser_iq_write`, `vser_pass_user_write` |
