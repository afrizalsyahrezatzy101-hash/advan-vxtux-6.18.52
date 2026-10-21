# Honest Symbol Comparison — VXTux 6.18.52 vs stock vendor .ko

`init_module`/`cleanup_module` are excluded: they are module lifecycle artefacts, absent by design when a driver is built into vmlinux instead of loaded as a module.

- Stock .ko analysed: 31
- Symbols needing a match: 363
- Symbols matched: **52/363 (14.3%)**
- Modules at 100%: **7/19**
- In source but not exported (one-line fix): 40
- Absent from source (real port gap): 271

| stock .ko | config | value | matched | fixable | absent |
|---|---|---|---|---|---|
| `aes-ce-ccm` | `CONFIG_CRYPTO_AES_ARM64_CE_CCM` | `y` | 3/4 | 0 | 1 |
| `apsys-dvfs` | `-` | `absent` | 0/72 | 0 | 72 |
| `audio-dsp-dump` | `-` | `absent` | 0/14 | 0 | 14 |
| `audio-pipe` | `-` | `absent` | 0/0 | 0 | 0 |
| `audio_sipc` | `-` | `absent` | 0/30 | 0 | 30 |
| `gpio-pmic-eic-sprd` | `CONFIG_GPIO_PMIC_EIC_SPRD` | `y` | 0/0 | 0 | 0 |
| `ion_ipc_trusty` | `CONFIG_ION_IPC_TRUSTY` | `y` | 6/6 | 0 | 0 |
| `mmdvfs` | `-` | `absent` | 0/115 | 0 | 115 |
| `novatek_ts_spi` | `CONFIG_TOUCHSCREEN_NOVATEK_NVT_TS` | `y` | 0/51 | 40 | 11 |
| `nvmem-sc27xx-efuse` | `CONFIG_NVMEM_SC27XX_EFUSE` | `y` | 0/0 | 0 | 0 |
| `nvmem_sprd_cache_efuse` | `CONFIG_NVMEM_SPRD_CACHE_EFUSE` | `y` | 0/0 | 0 | 0 |
| `pwm_bl` | `CONFIG_BACKLIGHT_PWM` | `m` | 0/0 | 0 | 0 |
| `rtc-sc27xx` | `CONFIG_RTC_DRV_SC27XX` | `y` | 0/0 | 0 | 0 |
| `sc27xx_adc` | `CONFIG_SC27XX_ADC` | `y` | 0/0 | 0 | 0 |
| `sdhci-sprd` | `CONFIG_MMC_SDHCI_SPRD` | `y` | 1/10 | 0 | 9 |
| `sprd-dmaengine-pcm` | `-` | `absent` | 0/2 | 0 | 2 |
| `sprd-sipc-virt-bus` | `-` | `absent` | 0/0 | 0 | 0 |
| `sprd-vsp-pw-domain` | `CONFIG_SPRD_VSP_PW_DOMAIN` | `y` | 0/0 | 0 | 0 |
| `sprd_apipe` | `-` | `absent` | 0/0 | 0 | 0 |
| `sprd_flash_drv` | `-` | `absent` | 0/5 | 0 | 5 |
| `sprd_power_manager` | `CONFIG_SPRD_POWER_MANAGER` | `m` | 14/14 | 0 | 0 |
| `sprd_time_sync` | `-` | `absent` | 0/1 | 0 | 1 |
| `trusty` | `CONFIG_TRUSTY` | `y` | 13/13 | 0 | 0 |
| `trusty-ipc` | `CONFIG_TRUSTY_VIRTIO_IPC` | `y` | 10/10 | 0 | 0 |
| `trusty-irq` | `CONFIG_TRUSTY` | `y` | 0/0 | 0 | 0 |
| `trusty-log` | `CONFIG_TRUSTY_LOG` | `m` | 1/1 | 0 | 0 |
| `trusty-tui` | `CONFIG_TRUSTY_TUI` | `y` | 3/3 | 0 | 0 |
| `twofish_common` | `CONFIG_CRYPTO_TWOFISH_COMMON` | `y` | 1/1 | 0 | 0 |
| `ums512-clk` | `CONFIG_SPRD_UMS512_CLK` | `m` | 0/0 | 0 | 0 |
| `unisoc-mailbox` | `CONFIG_SPRD_MAILBOX` | `y` | 0/7 | 0 | 7 |
| `usb_f_vser` | `-` | `absent` | 0/4 | 0 | 4 |

## Fixable: present in source, missing EXPORT_SYMBOL

| stock .ko | symbol |
|---|---|
| `novatek_ts_spi` | `Boot_Update_Firmware` |
| `novatek_ts_spi` | `CTP_SPI_READ` |
| `novatek_ts_spi` | `CTP_SPI_WRITE` |
| `novatek_ts_spi` | `ENG_RST_ADDR` |
| `novatek_ts_spi` | `SWRST_N8_ADDR` |
| `novatek_ts_spi` | `esd_check` |
| `novatek_ts_spi` | `esd_retry` |
| `novatek_ts_spi` | `fw_entry` |
| `novatek_ts_spi` | `nvt_bld_crc_enable` |
| `novatek_ts_spi` | `nvt_boot_ready` |
| `novatek_ts_spi` | `nvt_bootloader_reset` |
| `novatek_ts_spi` | `nvt_change_mode` |
| `novatek_ts_spi` | `nvt_check_fw_reset_state` |
| `novatek_ts_spi` | `nvt_check_fw_status` |
| `novatek_ts_spi` | `nvt_clear_fw_status` |
| `novatek_ts_spi` | `nvt_eng_reset` |
| `novatek_ts_spi` | `nvt_esd_check_enable` |
| `novatek_ts_spi` | `nvt_extra_proc_init` |
| `novatek_ts_spi` | `nvt_fw_crc_enable` |
| `novatek_ts_spi` | `nvt_fw_version_seq_ops` |
| `novatek_ts_spi` | `nvt_get_fw_info` |
| `novatek_ts_spi` | `nvt_get_fw_pipe` |
| `novatek_ts_spi` | `nvt_get_mdata` |
| `novatek_ts_spi` | `nvt_mp_parse_ain` |
| `novatek_ts_spi` | `nvt_mp_parse_array` |
| `novatek_ts_spi` | `nvt_mp_parse_dt` |
| `novatek_ts_spi` | `nvt_mp_parse_u32` |
| `novatek_ts_spi` | `nvt_mp_proc_deinit` |
| `novatek_ts_spi` | `nvt_mp_proc_init` |
| `novatek_ts_spi` | `nvt_read_mdata` |
| `novatek_ts_spi` | `nvt_read_pid` |
| `novatek_ts_spi` | `nvt_selftest_seq_ops` |
| `novatek_ts_spi` | `nvt_seq_ops` |
| `novatek_ts_spi` | `nvt_set_page` |
| `novatek_ts_spi` | `nvt_sw_reset` |
| `novatek_ts_spi` | `nvt_sw_reset_idle` |
| `novatek_ts_spi` | `nvt_update_firmware` |
| `novatek_ts_spi` | `nvt_write_addr` |
| `novatek_ts_spi` | `print_selftest_result` |
| `novatek_ts_spi` | `ts` |

## Real port gap: symbol absent from the VXTux source

| stock .ko | symbol |
|---|---|
| `apsys-dvfs` | `apsys_dvfs_head` |
| `apsys-dvfs` | `dpu_devfreq_gov` |
| `apsys-dvfs` | `dpu_dvfs_chain` |
| `apsys-dvfs` | `dpu_dvfs_driver` |
| `apsys-dvfs` | `dpu_dvfs_head` |
| `apsys-dvfs` | `dpu_dvfs_notifier_call_chain` |
| `apsys-dvfs` | `dpu_vsp_dvfs_check_clkeb` |
| `apsys-dvfs` | `dvfs_class` |
| `apsys-dvfs` | `find_apsys_device_by_name` |
| `apsys-dvfs` | `get_display_power_status` |
| `apsys-dvfs` | `gsp_devfreq_gov` |
| `apsys-dvfs` | `gsp_dvfs_chain` |
| `apsys-dvfs` | `gsp_dvfs_driver` |
| `apsys-dvfs` | `gsp_dvfs_head` |
| `apsys-dvfs` | `gsp_dvfs_notifier_call_chain` |
| `apsys-dvfs` | `n6pro_AA_flag` |
| `apsys-dvfs` | `n6pro_soc_ver_id_check` |
| `apsys-dvfs` | `qogirl6_apsys_dvfs_ops` |
| `apsys-dvfs` | `qogirl6_apsys_val_to_volt` |
| `apsys-dvfs` | `qogirl6_dpu_dvfs_ops` |
| `apsys-dvfs` | `qogirl6_dpu_val_to_freq` |
| `apsys-dvfs` | `qogirl6_vdsp_val_to_freq` |
| `apsys-dvfs` | `qogirl6_vsp_dvfs_ops` |
| `apsys-dvfs` | `qogirl6_vsp_val_to_freq` |
| `apsys-dvfs` | `qogirn6pro_apsys_dvfs_ops` |
| `apsys-dvfs` | `qogirn6pro_apsys_val_to_volt` |
| `apsys-dvfs` | `qogirn6pro_dpu_dvfs_ops` |
| `apsys-dvfs` | `qogirn6pro_dpu_val_to_freq` |
| `apsys-dvfs` | `qogirn6pro_gsp_dvfs_ops` |
| `apsys-dvfs` | `qogirn6pro_gsp_val_to_freq` |
| `apsys-dvfs` | `qogirn6pro_gsp_val_to_volt` |
| `apsys-dvfs` | `qogirn6pro_vdsp_val_to_freq` |
| `apsys-dvfs` | `qogirn6pro_vpu_val_to_volt` |
| `apsys-dvfs` | `qogirn6pro_vpudec_val_to_freq` |
| `apsys-dvfs` | `qogirn6pro_vpudec_vsp_dvfs_ops` |
| `apsys-dvfs` | `qogirn6pro_vpuenc_val_to_freq` |
| `apsys-dvfs` | `qogirn6pro_vpuenc_vsp_dvfs_ops` |
| `apsys-dvfs` | `regmap_aon_base` |
| `apsys-dvfs` | `roc1_apsys_dvfs_ops` |
| `apsys-dvfs` | `roc1_apsys_val_to_volt` |
| `apsys-dvfs` | `roc1_dpu_dvfs_ops` |
| `apsys-dvfs` | `roc1_dpu_val_to_freq` |
| `apsys-dvfs` | `roc1_vdsp_dvfs_ops` |
| `apsys-dvfs` | `roc1_vdsp_val_to_freq` |
| `apsys-dvfs` | `roc1_vsp_dvfs_ops` |
| `apsys-dvfs` | `roc1_vsp_val_to_freq` |
| `apsys-dvfs` | `sharkl5_apsys_dvfs_ops` |
| `apsys-dvfs` | `sharkl5_apsys_val_to_volt` |
| `apsys-dvfs` | `sharkl5_dpu_dvfs_ops` |
| `apsys-dvfs` | `sharkl5_dpu_val_to_freq` |
| `apsys-dvfs` | `sharkl5_vsp_dvfs_ops` |
| `apsys-dvfs` | `sharkl5_vsp_val_to_freq` |
| `apsys-dvfs` | `sharkl5pro_apsys_dvfs_ops` |
| `apsys-dvfs` | `sharkl5pro_apsys_val_to_volt` |
| `apsys-dvfs` | `sharkl5pro_dpu_dvfs_ops` |
| `apsys-dvfs` | `sharkl5pro_dpu_val_to_freq` |
| `apsys-dvfs` | `sharkl5pro_vdsp_dvfs_ops` |
| `apsys-dvfs` | `sharkl5pro_vdsp_val_to_freq` |
| `apsys-dvfs` | `sharkl5pro_vsp_dvfs_ops` |
| `apsys-dvfs` | `sharkl5pro_vsp_val_to_freq` |
| `apsys-dvfs` | `vdsp_devfreq_gov` |
| `apsys-dvfs` | `vdsp_dvfs_chain` |
| `apsys-dvfs` | `vdsp_dvfs_driver` |
| `apsys-dvfs` | `vdsp_dvfs_head` |
| `apsys-dvfs` | `vdsp_dvfs_notifier_call_chain` |
| `apsys-dvfs` | `vpuenc_dvfs_chain` |
| `apsys-dvfs` | `vsp_devfreq_gov` |
| `apsys-dvfs` | `vsp_dvfs_chain` |
| `apsys-dvfs` | `vsp_dvfs_driver` |
| `apsys-dvfs` | `vsp_dvfs_get_dev_status` |
| `apsys-dvfs` | `vsp_dvfs_head` |
| `apsys-dvfs` | `vsp_dvfs_notifier_call_chain` |
| `audio-dsp-dump` | `audio_sblock_create` |
| `audio-dsp-dump` | `audio_sblock_destroy` |
| `audio-dsp-dump` | `audio_sblock_get` |
| `audio-dsp-dump` | `audio_sblock_get_arrived_count` |
| `audio-dsp-dump` | `audio_sblock_get_free_count` |
| `audio-dsp-dump` | `audio_sblock_init` |
| `audio-dsp-dump` | `audio_sblock_poll_wait` |
| `audio-dsp-dump` | `audio_sblock_put` |
| `audio-dsp-dump` | `audio_sblock_receive` |
| `audio-dsp-dump` | `audio_sblock_register_notifier` |
| `audio-dsp-dump` | `audio_sblock_release` |
| `audio-dsp-dump` | `audio_sblock_send` |
| `audio-dsp-dump` | `audio_sblock_send_finish` |
| `audio-dsp-dump` | `audio_sblock_send_prepare` |
| `audio_sipc` | `aud_get_aud_ipc_smsg_addr` |
| `audio_sipc` | `aud_get_aud_ipc_smsg_para_addr` |
| `audio_sipc` | `aud_ipc_ch_close` |
| `audio_sipc` | `aud_ipc_ch_open` |
| `audio_sipc` | `aud_ipc_dump` |
| `audio_sipc` | `aud_recv_block_param` |
| `audio_sipc` | `aud_recv_cmd` |
| `audio_sipc` | `aud_send_block_param` |
| `audio_sipc` | `aud_send_cmd` |
| `audio_sipc` | `aud_send_cmd_no_param` |
| `audio_sipc` | `aud_send_cmd_no_wait` |
| `audio_sipc` | `aud_send_cmd_no_wait_param` |
| `audio_sipc` | `aud_send_cmd_result` |
| `audio_sipc` | `aud_send_use_noreplychan` |
| `audio_sipc` | `aud_smsg_ch_close` |
| `audio_sipc` | `aud_smsg_ch_open` |
| `audio_sipc` | `aud_smsg_init_debugfs` |
| `audio_sipc` | `aud_smsg_ipc_create` |
| `audio_sipc` | `aud_smsg_ipc_destroy` |
| `audio_sipc` | `aud_smsg_irq_handler` |
| `audio_sipc` | `aud_smsg_recv` |
| `audio_sipc` | `aud_smsg_register_dump_func` |
| `audio_sipc` | `aud_smsg_rx_handler` |
| `audio_sipc` | `aud_smsg_send` |
| `audio_sipc` | `aud_smsg_set_mboxchan` |
| `audio_sipc` | `aud_smsg_suspend_init` |
| `audio_sipc` | `aud_smsg_wakeup_all_ch` |
| `audio_sipc` | `aud_smsg_wakeup_ch` |
| `audio_sipc` | `g_aud_ipc` |
| `audio_sipc` | `snd_vbc_sipc_deinit` |
| `mmdvfs` | `AddNode` |
| `mmdvfs` | `CreateNode` |
| `mmdvfs` | `DelNode` |
| `mmdvfs` | `Destroy` |
| `mmdvfs` | `GetFrontNodeFreq` |
| `mmdvfs` | `InitPQueue` |
| `mmdvfs` | `IsEmpty` |
| `mmdvfs` | `MatchPid_RemoveInvalidNode` |
| `mmdvfs` | `ShowQueue` |
| `mmdvfs` | `cpp_dvfs_config_table` |
| `mmdvfs` | `cpp_dvfs_exit` |
| `mmdvfs` | `cpp_dvfs_gov` |
| `mmdvfs` | `cpp_dvfs_init` |
| `mmdvfs` | `cpp_dvfs_ops` |
| `mmdvfs` | `dcam_axi_dvfs_exit` |
| `mmdvfs` | `dcam_axi_dvfs_gov` |
| `mmdvfs` | `dcam_axi_dvfs_init` |
| `mmdvfs` | `dcam_axi_dvfs_ops` |
| `mmdvfs` | `dcam_dvfs_config_table` |
| `mmdvfs` | `dcam_if_dvfs_exit` |
| `mmdvfs` | `dcam_if_dvfs_gov` |
| `mmdvfs` | `dcam_if_dvfs_init` |
| `mmdvfs` | `dcam_if_dvfs_ops` |
| `mmdvfs` | `dcamaxi_dvfs_config_table` |
| `mmdvfs` | `dvfs_get_cur_freq` |
| `mmdvfs` | `dvfs_get_dev_status` |
| `mmdvfs` | `dvfs_notify_callback` |
| `mmdvfs` | `dvfs_target` |
| `mmdvfs` | `fd_dvfs_config_table` |
| `mmdvfs` | `fd_dvfs_exit` |
| `mmdvfs` | `fd_dvfs_gov` |
| `mmdvfs` | `fd_dvfs_init` |
| `mmdvfs` | `fd_dvfs_ops` |
| `mmdvfs` | `g_mmreg_map` |
| `mmdvfs` | `get_auto_tune_en_show` |
| `mmdvfs` | `get_dvfs_coffe_show` |
| `mmdvfs` | `get_dvfs_enable_show` |
| `mmdvfs` | `get_dvfs_table_info_show` |
| `mmdvfs` | `get_fix_dvfs_show` |
| `mmdvfs` | `get_freq_upd_delay_en_show` |
| `mmdvfs` | `get_freq_upd_en_byp_show` |
| `mmdvfs` | `get_freq_upd_hdsk_en_show` |
| `mmdvfs` | `get_gfree_wait_delay_show` |
| `mmdvfs` | `get_idle_freq_show` |
| `mmdvfs` | `get_idle_index_show` |
| `mmdvfs` | `get_ip_dvfs_ops` |
| `mmdvfs` | `get_ip_status_show` |
| `mmdvfs` | `get_ip_status_store` |
| `mmdvfs` | `get_mmsys_power_show` |
| `mmdvfs` | `get_sw_trig_en_show` |
| `mmdvfs` | `get_sys_dvfs_clk_gate_ctrl_show` |
| `mmdvfs` | `get_sys_dvfs_hold_en_show` |
| `mmdvfs` | `get_sys_dvfs_min_volt_show` |
| `mmdvfs` | `get_sys_dvfs_wait_window_show` |
| `mmdvfs` | `get_sys_status_show` |
| `mmdvfs` | `get_sys_sw_dvfs_en_show` |
| `mmdvfs` | `get_work_freq_show` |
| `mmdvfs` | `get_work_index_show` |
| `mmdvfs` | `isp_dvfs_config_table` |
| `mmdvfs` | `isp_dvfs_exit` |
| `mmdvfs` | `isp_dvfs_gov` |
| `mmdvfs` | `isp_dvfs_init` |
| `mmdvfs` | `isp_dvfs_ops` |
| `mmdvfs` | `jpg_dvfs_config_table` |
| `mmdvfs` | `jpg_dvfs_exit` |
| `mmdvfs` | `jpg_dvfs_gov` |
| `mmdvfs` | `jpg_dvfs_init` |
| `mmdvfs` | `jpg_dvfs_ops` |
| `mmdvfs` | `mm_dvfs_hold_onoff` |
| `mmdvfs` | `mm_dvfs_read_current_volt` |
| `mmdvfs` | `mm_power_flag` |
| `mmdvfs` | `mm_sw_dvfs_onoff` |
| `mmdvfs` | `mmdcdc_sw_dvfs_onoff` |
| `mmdvfs` | `mmsys_adjust_target_freq` |
| `mmdvfs` | `mmsys_dvfs_exit` |
| `mmdvfs` | `mmsys_dvfs_gov` |
| `mmdvfs` | `mmsys_dvfs_init` |
| `mmdvfs` | `mmsys_dvfs_ops` |
| `mmdvfs` | `mmsys_glob_reg_lock` |
| `mmdvfs` | `mmsys_module_exit` |
| `mmdvfs` | `mmsys_module_init` |
| `mmdvfs` | `mmsys_notify_call_chain` |
| `mmdvfs` | `mmsys_register_notifier` |
| `mmdvfs` | `mmsys_set_fix_dvfs_value` |
| `mmdvfs` | `mmsys_sw_cgb_enable` |
| `mmdvfs` | `mmsys_unregister_notifier` |
| `mmdvfs` | `mtx_dvfs_config_table` |
| `mmdvfs` | `mtx_dvfs_exit` |
| `mmdvfs` | `mtx_dvfs_gov` |
| `mmdvfs` | `mtx_dvfs_init` |
| `mmdvfs` | `mtx_dvfs_ops` |
| `mmdvfs` | `register_ip_dvfs_ops` |
| `mmdvfs` | `set_auto_tune_en_store` |
| `mmdvfs` | `set_dvfs_coffe_store` |
| `mmdvfs` | `set_dvfs_enable_store` |
| `mmdvfs` | `set_dvfs_table_info_store` |
| `mmdvfs` | `set_fix_dvfs_store` |
| `mmdvfs` | `set_freq_upd_delay_en_store` |
| `mmdvfs` | `set_freq_upd_en_byp_store` |
| `mmdvfs` | `set_freq_upd_hdsk_en_store` |
| `mmdvfs` | `set_gfree_wait_delay_store` |
| `mmdvfs` | `set_idle_freq_store` |
| `mmdvfs` | `set_idle_index_store` |
| `mmdvfs` | `set_mmsys_power_store` |
| `mmdvfs` | `set_sw_trig_en_store` |
| `mmdvfs` | `set_sys_dvfs_clk_gate_ctrl_store` |
| `mmdvfs` | `set_sys_dvfs_hold_en_store` |
| `mmdvfs` | `set_sys_dvfs_min_volt_store` |
| `mmdvfs` | `set_sys_dvfs_wait_window_store` |
| `mmdvfs` | `set_sys_status_store` |
| `mmdvfs` | `set_sys_sw_dvfs_en_store` |
| `mmdvfs` | `set_work_freq_store` |
| `mmdvfs` | `set_work_index_store` |
| `mmdvfs` | `top_mm_dvfs_current_volt` |
| `mmdvfs` | `unregister_ip_dvfs_ops` |
| `novatek_ts_spi` | `SPI_RD_FAST_ADDR` |
| `novatek_ts_spi` | `nvt_check_spi_dma_tx_info` |
| `novatek_ts_spi` | `nvt_extra_proc_deinit` |
| `novatek_ts_spi` | `nvt_mp_parse_pen_array` |
| `novatek_ts_spi` | `nvt_pen_diff_seq_ops` |
| `novatek_ts_spi` | `nvt_read_fw_history` |
| `novatek_ts_spi` | `nvt_read_get_num_mdata` |
| `novatek_ts_spi` | `nvt_set_pen_inband_mode_1` |
| `novatek_ts_spi` | `nvt_set_pen_normal_mode` |
| `novatek_ts_spi` | `nvt_tx_auto_copy_mode` |
| `novatek_ts_spi` | `sprd_dpu_nt36xxx_touchscreen_callback` |
| `sdhci-sprd` | `_sdhci_request_atomic` |
| `sdhci-sprd` | `mmc_check_wp_fn` |
| `sdhci-sprd` | `mmc_hsq_swcq_init` |
| `sdhci-sprd` | `mmc_send_tuning_read` |
| `sdhci-sprd` | `mmc_set_powp` |
| `sdhci-sprd` | `mmc_wp_init` |
| `sdhci-sprd` | `mmc_wp_remove` |
| `sdhci-sprd` | `sdhci_sprd_irq_request_swcq` |
| `sdhci-sprd` | `sdhci_sprd_request_sync` |
| `sprd-dmaengine-pcm` | `pm_dma` |
| `sprd-dmaengine-pcm` | `test_force_stop` |
| `sprd_flash_drv` | `flash_get_flash_info` |
| `sprd_flash_drv` | `sprd_flash_cfg` |
| `sprd_flash_drv` | `sprd_flash_ctrl` |
| `sprd_flash_drv` | `sprd_flash_get_info` |
| `sprd_flash_drv` | `sprd_flash_register` |
| `sprd_time_sync` | `sprd_send_ap_time` |
| `unisoc-mailbox` | `get_tx_fifo_mask` |
| `unisoc-mailbox` | `mbox_phy_ops` |
| `unisoc-mailbox` | `mbox_start_send_tx_fifo` |
| `unisoc-mailbox` | `sprd_mbox_change_wakeup_flag_r1` |
| `unisoc-mailbox` | `sprd_mbox_phy_r1_init` |
| `unisoc-mailbox` | `sprd_mbox_wakeup_flag_callback_register_r1` |
| `unisoc-mailbox` | `sprd_mbox_wakeup_flag_callback_unregister_r1` |
| `usb_f_vser` | `kernel_vser_register_callback` |
| `usb_f_vser` | `kernel_vser_set_pass_mode` |
| `usb_f_vser` | `vser_iq_write` |
| `usb_f_vser` | `vser_pass_user_write` |
