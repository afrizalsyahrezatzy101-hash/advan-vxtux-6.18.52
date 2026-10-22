# RE report: sprd-dmaengine-pcm.ko (stock blob) -> VXTux 6.18.52 source

Blob: `donors/stock_vendor_modules/sprd-dmaengine-pcm.ko` (61886 B, vermagic 5.4.210 SMP preempt
mod_unload modversions aarch64).
Original source path recovered from `.rodata`:
`/home/wufeng/SPRDROID13_MAIN_22C_W22.48.3_P4/bsp/modules/kernel5.4/audio_driver/sprd/platform/dmaengine_pcm/sprd-dmaengine-pcm.c`

## 1. Symbol surface to reproduce (nm, exact)

```
T cleanup_module            T init_module
B pm_dma           (data)   B test_force_stop      (data)
b debug_pointer_log (bss)   b init_pm_dma.__key    (bss)
b init_pm_dma.__key.3 (bss) b test_ddr_buffer_size (bss)
d sprd_i2s_pcm_hardware (rodata: snd_soc_hw_params)
d sprd_pcm_driver     (data: platform_driver)
d sprd_pcm_ops        (data: snd_soc_ops)
d sprd_soc_platform   (data)
d test_normal_cap_use_iram (data)
r sprd_pcm_hardware_v1 (rodata: snd_soc_hardware)
r sprd_pcm_of_match    (rodata: of_match)
t sprd_pcm_close            t sprd_pcm_config_dma
t sprd_pcm_dma_buf_done     t sprd_pcm_dma_step_config
t sprd_pcm_free_dma_buffers t sprd_pcm_hw_free
t sprd_pcm_hw_params        t sprd_pcm_mmap
t sprd_pcm_new              t sprd_pcm_open
t sprd_pcm_platform_proc_read  t sprd_pcm_platform_proc_write
t sprd_pcm_pm_notifier      t sprd_pcm_pointer
t sprd_pcm_prepare          t sprd_pcm_trigger
t sprd_snd_platform_probe   t sprd_snd_platform_remove
t sprd_soc_platform_probe   t sprd_soc_platform_remove
R __mod_of__sprd_pcm_of_match_device_table
```

## 2. DT compatible (recovered from .modinfo aliases + stock.dtb)

`alias=platform:sprd-audio` and:
```
unisoc,sharkl5-pcm-platform
unisoc,roc1-pcm-platform
unisoc,qogirl6-pcm-platform
unisoc,qogirn6pro-pcm-platform
```
`MODULE_DEVICE_TABLE(of, sprd_pcm_of_match)` -> `__mod_of__sprd_pcm_of_match_device_table`.

Stock DTB nodes (donors/fdt_find.py):
- `/sprd-pcm-audio`  compatible `unisoc,sharkl5-pcm-platform`,`unisoc,sharkl5pro-pcm-platform`
- `/sprd-pcm-iis`   compatible `unisoc,sharkl5-pcm-platform`

## 3. .modinfo (must match for modprobe/alias parity)

```
author=Jian chen <jian.chen@spreadtrum.com>
description=SPRD ASoC PCM DMA
license=GPL
name=sprd_dmaengine_pcm
depends=audio_mem,agdsp_access,snd-soc-sprd-card
```

## 4. Callback map (resolved from `.rela.data.sprd_pcm_driver` + `.text` PLT stubs)

`struct snd_pcm_ops sprd_pcm_ops` (size 0x78 = 6.18 snd_pcm_ops layout is different;
5.4 layout used by the blob):
```
+0x00 -> .text+0x4848  sprd_pcm_open
+0x08 -> .text+0xe9c   sprd_pcm_close
+0x10 -> .text+0x4340  snd_pcm_lib_ioctl
+0x18 -> .text+0x1168  sprd_pcm_hw_params
+0x20 -> .text+0x1cc4  sprd_pcm_hw_free
+0x28 -> .text+0x1ee4  sprd_pcm_prepare
+0x30 -> .text+0x1eec  sprd_pcm_trigger
+0x38 -> .text+0x22ac  sprd_pcm_pointer
+0x68 -> .text+0x2628  sprd_pcm_mmap
```
`struct snd_soc_platform sprd_soc_platform` (size 0xf8, 5.4 `snd_soc_platform_driver`):
```
+0x00 -> .rodata+0x1025 "sprd-pcm-dma"     (.name)
+0x38 -> .text+0x4330  sprd_snd_platform_probe
+0x40 -> .text+0x4334  sprd_snd_platform_remove
+0x68 -> .text+0x4338  sprd_pcm_new
+0x70 -> .text+0x433c  sprd_pcm_free_dma_buffers
+0xb8 -> .data+0x1e8   sprd_pcm_ops
```
`struct platform_driver sprd_pcm_driver` (size 0xf0):
```
+0x00 -> .text+0x4324  sprd_soc_platform_probe   (.probe)
+0x08 -> .text+0x4328  sprd_soc_platform_remove   (.remove)
+0x28 -> .rodata+0x729  "sprd-pcm-audio"           (.driver.name)
+0x38 -> __this_module
+0x50 -> .rodata+0x1238 sprd_pcm_of_match          (.driver.of_match_table)
+0xf0 -> .rodata+0x1025 "sprd-pcm-dma"
```

## 5. External symbol dependencies (from full relocation sweep)

Exported/consumed by the blob:
- `agdsp_access_enable/disable`  (tree: drivers/vxtux/audio/agdsp_access)
- `audio_mem_alloc/free/unmap/vmap`, `audio_addr_ap2dsp` (tree: drivers/vxtux/audio/audiomem)
- `get_sp_audio_debug_flag` (tree: drivers/vxtux/sound/asoc/card/sprd-asoc-common.c)
- `snd_soc_rtdcom_lookup(rtd, "sprd-pcm-dma")` — looks up the *pcm-routing* component by name.
- `snd_soc_set_runtime_hwparams`, `snd_pcm_hw_constraint_step`, `snd_pcm_hw_constraint_integer`
- `snd_pcm_lib_ioctl`, `snd_pcm_period_elapsed`
- `snd_soc_register_component`, `snd_soc_unregister_component`
- `snd_info_create_card_entry`, `snd_info_get_line`, `seq_printf`
- `dma_request_slave_channel`, `dma_release_channel`, `dma_set_mask`, `sg_init_table`
- `register_pm_notifier`, `unregister_pm_notifier`
- `remap_pfn_range`, `devm_kmalloc`, `devm_kfree`, `memset`, `sscanf`, `printk`

## 6. rodata constants recovered

`struct snd_pcm_hardware`:
- `sprd_i2s_pcm_hardware` @ .rodata+0x1620, 0x50 bytes:
  info=0x000c0103, formats=0x444 (S16_LE|S24_LE|S32_LE), rates=0,
  period_bytes_min=1, period_bytes_max=0x10000, periods_min=1, periods_max=0x80, buffer_bytes_max=0x10000
- `sprd_pcm_hardware_v1` @ .rodata+0x1670, 0x50 bytes:
  info=0x000c0303, formats=0x444, rates=0, period_bytes_min=1, period_bytes_max=0x40000,
  periods_min=1, periods_max=0x140, buffer_bytes_max=0x39000

## 7. 5.4 -> 6.18 port notes

- 5.4 `snd_soc_platform` / `snd_soc_platform_driver` no longer exist in 6.18.
  6.18 uses `struct snd_soc_component_driver` with `.pcm_construct` (was `.pcm_new`),
  `.open/.close/.hw_params/.hw_free/.prepare/.trigger/.pointer/.mmap` callbacks
  (all take `struct snd_soc_component *` first arg).
- 5.4 `snd_soc_register_platform(dev, drv)` -> 6.18 `devm_snd_soc_register_component(dev, drv, NULL, 0)`.
- `snd_soc_unregister_component(dev)` in 6.18 is a macro -> `snd_soc_unregister_component_by_driver(dev, NULL)`.
- The tree already carries `drivers/vxtux/sound/asoc/asoc-618-compat.h` shim for exactly this
  legacy->component mapping, and `drivers/vxtux/sound/asoc/platform/sprd-platform-pcm-routing.c`
  already registers a component named "sprd-pcm-dma" (the rtdcom_lookup target).
