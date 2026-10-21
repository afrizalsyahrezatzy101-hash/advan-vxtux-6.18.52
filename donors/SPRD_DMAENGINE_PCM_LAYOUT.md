# Stock `sprd-dmaengine-pcm.ko` layout (kernel 5.4.210, aarch64)

Decoded from `donors/stock_vendor_modules/sprd-dmaengine-pcm.ko` with
`readelf -r` + `objdump -dr` (the `.text` CFI jump table at 0x4320..0x4370
holds the real function pointers; `.rela.data.sprd_pcm_driver` points at
those thunks).

## Section bases

| section | vaddr | file off | size |
|---|---|---|---|
| `.bss`  | 0x0 | 0x108 | 0x14 |
| `.data` | 0x0 | 0x108 | 0x264 |
| `.rodata` | 0x0 | 0x370 | 0x16c0 |
| `.text` | 0x0 | 0x2000 | 0x4374 |

## `sprd_pcm_ops` (data 0x1e8, size 0x78 = 15 pointers)

Legacy `struct snd_soc_pcm_ops` layout:

| off | symbol |
|---|---|
| +0x00 | `sprd_pcm_open` |
| +0x08 | `sprd_pcm_close` |
| +0x10 | `snd_pcm_lib_ioctl` |
| +0x18 | `sprd_pcm_hw_params` |
| +0x20 | `sprd_pcm_hw_free` |
| +0x28 | `sprd_pcm_prepare` |
| +0x30 | `sprd_pcm_trigger` |
| +0x38 | `sprd_pcm_pointer` |
| +0x68 | `sprd_pcm_mmap` |

## `sprd_soc_platform` (data 0xf0, size 0xf8)

Legacy `struct snd_soc_platform`:

| off | symbol |
|---|---|
| +0x00 | `.rodata` 0x1025 (name string) |
| +0x38 | `sprd_snd_platform_probe` |
| +0x40 | `sprd_snd_platform_remove` |
| +0x68 | `sprd_pcm_new` |
| +0x70 | `sprd_pcm_free_dma_buffers` |
| +0xb8 | `&sprd_pcm_ops` (data 0x1e8) |

## `sprd_pcm_driver` (data 0x0, size 0xf0)

`struct platform_driver`:

| off | symbol |
|---|---|
| +0x00 | `sprd_soc_platform_probe` |
| +0x08 | `sprd_soc_platform_remove` |
| +0x28 | `.rodata` 0x729 (driver name "sprd-pcm-audio") |
| +0x38 | `__this_module` |
| +0x50 | `sprd_pcm_of_match` (rodata 0x1238) |
| +0xf0 | `.rodata` 0x1025 |

## `sprd_pcm_of_match` (rodata 0x1238, size 0x1f8 = 31 ptrs = 5 entries + NULL)

DT compatibles (from `.rodata` strings):

1. `unisoc,sharkl5-pcm-platform`
2. `unisoc,roc1-pcm-platform`
3. `unisoc,qogirl6-pcm-platform`
4. `unisoc,qogirn6pro-pcm-platform`
5. (5th — see the 5th string in `.rela.data.sprd_pcm_driver`)

Stock DT node `/sprd-pcm-audio` carries
`unisoc,sharkl5-pcm-platform` + `unisoc,sharkl5pro-pcm-platform`.

## Debug knobs

| symbol | section | init |
|---|---|---|
| `test_ddr_buffer_size` | `.bss` 0x0c | 0 |
| `test_force_stop` | `.bss` 0x10 | 0 |
| `test_normal_cap_use_iram` | `.data` 0x260 | 1 |
| `debug_pointer_log` | `.bss` 0x08 | 0 |
| `pm_dma` | `.bss` | 0 (ptr) |

## Lockdep static keys

`init_pm_dma.__key` and `init_pm_dma.__key.3` (both `.bss`) — the two
`__mutex_init()` calls inside the blob's `init_pm_dma()` helper.

## 5.4 -> 6.18 API mapping used by the replacement

| 5.4 | 6.18 |
|---|---|
| `snd_soc_platform` | `snd_soc_component_driver` |
| `snd_soc_platform_driver` | `snd_soc_component_driver` + `platform_driver` |
| `.pcm_new` | `.pcm_construct` |
| `snd_soc_register_platform` | `devm_snd_soc_register_component` |
| ops take `(substream, ...)` | ops take `(component, substream, ...)` |
| `snd_soc_pcm_ops.ioctl` | `snd_soc_component_driver.ioctl` (component first) |
| `vma->vm_flags \|=` | `vm_flags_set(vma, ...)` |
| `remap_pfn_range(...4 args)` | `remap_pfn_range(..., vma->vm_page_prot)` |
| `entry->text.read` | `entry->c.text.read` |
| `seq_printf(buffer,...)` | `snd_iprintf(buffer,...)` |
| `runtime->hw.periods` | `runtime->periods` |
| `params_frame_bytes()` | `params_buffer_bytes()/params_channels()` |
