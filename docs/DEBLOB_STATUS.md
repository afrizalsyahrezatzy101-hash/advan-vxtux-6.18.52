# Deblob status — 2026-10-21

The stock ROM ships **31 vendor .ko** in `/lib/modules`. The VXTux build
now produces a module with the same file name for **24** of them, and
**18** of those carry the same API (the stock blob's own functions,
locals included).

The measurement is `donors/verify_deblob2.py`; it writes
`docs/DEBLOB_VERIFICATION.md`.

## What the build produces

- **202 modules** (was 201 before `sprd_apipe.ko` was fixed)
- **164,210 symbols** in vmlinux + all modules (`donors/all_syms.txt`)

## The three checks

1. **NAME** — the build emits a `.ko` with the stock file name.
2. **API** — that `.ko` has every symbol the stock `.ko` had, local
   symbols included. The stock blob's locals are the driver's own
   functions, so a replacement without them is not the same code.
3. **DEPS** — every symbol the replacement imports exists in the build
   (vmlinux **or** a module the build produces).

Check 3 was reporting false gaps until v34: it looked at the vmlinux
only, and a module's symbols live in the `.ko`:

```
$ nm built_kos/drivers/trusty/trusty-ipc.ko | grep tipc_chan_connect
000000000000206c T tipc_chan_connect
```

## The real gaps

### 5 modules with a missing API

| stock blob | missing | what it is |
|---|---|---|
| `aes-ce-ccm` | 0 | **closed** — see below |
| `apsys-dvfs` | 70 | the per-SoC DVFS op tables (`qogirl6_*`, `sharkl5*`, `roc1_*`, `n6pro_*`) — none of them are in the tree |
| `novatek_ts_spi` | 11 | the Novatek NT36xxx touch controller's pen/sequence API |
| `sdhci-sprd` | 9 | the SDHCI host controller's SW-CQ and write-protect API |
| `unisoc-mailbox` | 0 | **closed** — see below |

#### `aes-ce-ccm`: closed (build #115)

The stock blob has `ce_aes_ccm_final` as a **global** function, next
to a local label of the same name inside `ce_aes_ccm_crypt_tail`. The
two are different things: the local label is the "final round key
cancels out" step of the CCM mac, the global one is a standalone
two-block AES helper that the stock blob's `ce_aes_ccm_encrypt` and
`ce_aes_ccm_decrypt` call at `.text+0x6a4` and `.text+0x968`:

    out = AES(out, rk) ^ AES(in, rk)

It is now a global `ce_aes_ccm_final` in
`arch/arm64/crypto/aes-ce-ccm-core.S`, built on the `aes_encrypt`
macro that was already there (that macro takes two data registers for
exactly this reason). The upstream 6.18 local label was renamed to
`ce_aes_ccm_final_block` to free the name — the stock 5.4 blob has no
local label of that name, because its `ce_aes_ccm_encrypt` has no
`final_iv` handling at all.

The symbol surface is now identical to the stock blob.

#### `unisoc-mailbox`: closed (build #113)

The stock blob carries **two** layers in one file. Layer 0 is the Linux
`mbox_controller` framework driver, which was already in
`drivers/mailbox/sprd-mailbox.c`. Layer 1 is the OEM "revision 1" layer:
the `sprd_mbox_phy_ops_r1` vtable, the per-channel FIFO table, the
deliver thread, the PM notifier and the wakeup-flag API.

The contract was recovered with Ghidra 12.1.4
(`donors/ghidra/DumpFuncs.java`, output in `donors/ghidra_out/`) and
implemented in `drivers/mailbox/sprd-mailbox-r1.c` +
`drivers/mailbox/sprd-mailbox-r1.h`, wired into the composite module
as `unisoc-mailbox-y := sprd-mailbox.o sprd-mailbox-r1.o`.

The vtable layout was confirmed from the stock blob's relocation table
with `donors/ko_data.py`: `sprd_mbox_phy_ops_r1` points at
0x104368 startup, 0x104370 shutdown, 0x104378 flush, 0x104380 send,
0x10438c inbox_isr, 0x104390 outbox_isr, 0x104394 typeinfo,
0x104398 debug_show.

The symbol surface is now **identical** to the stock blob:

```
$ comm -23 stock_syms built_syms   # in stock, missing from built
$ comm -13 stock_syms built_syms   # in built, not in stock
                                    # both empty
```

**The `_r1` API has no consumer in the stock ROM.** The stock vmlinux
does not import `sprd_mbox_change_wakeup_flag_r1`,
`sprd_mbox_wakeup_flag_callback_register_r1` or
`sprd_mbox_wakeup_flag_callback_unregister_r1`, no stock `.ko` imports
`get_tx_fifo_mask` or `mbox_start_send_tx_fifo`, and a scan of the
whole stock filesystem finds no reference either. It is implemented
anyway, because the replacement module has to carry the same API
surface as the blob it replaces — the deblob check compares the local
symbols too.


### 7 stock blobs with no module at all

`audio_sipc`, `mmdvfs`, `sprd-dmaengine-pcm`, `sprd-sipc-virt-bus`,
`sprd-vsp-pw-domain`, `sprd_time_sync`, `usb_f_vser`

Six of the seven have **no source in the tree** — they are ports still
to be written, not build fixes. `sprd-dmaengine-pcm` has only a header
(`drivers/vxtux/sound/asoc/platform_include/sprd-dmaengine-pcm.h`).

## Kernel API drift is not a gap

The stock blobs were built for the 5.4 kernel; this tree is 6.18. The
kernel renamed a lot of its own API, so a replacement imports symbols
the stock one did not:

```
_printk  __kmalloc_cache_noprof  mutex_lock_nested
validate_usercopy_range  __list_add_valid_or_report  ...
```

All of them are present in the build. The report calls these out
separately as "kernel API drift".

## What the last few versions fixed

| version | what |
|---|---|
| v31 | `sprd_apipe.ko` — the apipe character device, the stock `sprd_apipe.ko` |
| v32 | **wrong** — created a `drivers/sound/` Makefile chain |
| v33 | undid v32: `drivers/sound/` is a dead duplicate of `drivers/vxtux/sound/asoc/`, deleted |
| v34 | the verification's two false-gap bugs |

## The silent-descent trap

Four times now a directory of sources sat in the tree, complete, with
its Kconfig, and nothing built it — because kbuild descends into a
directory only when its parent lists it:

- `drivers/soc/sprd/modem/`
- `drivers/vxtux/audio/audio_pipe/`
- `drivers/sound/soc/sprd/` (this one turned out to be a *duplicate*,
  not a missing chain)
- `drivers/sound/` (the same)

`donors/scan_dup_kconfig_syms.py` now reports **0** symbols defined
in two reachable Kconfig files.
