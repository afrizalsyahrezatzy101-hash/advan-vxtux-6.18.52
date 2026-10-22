# Vendor vs VXTux Symbol Analysis — Ghidra/radare2 Report

**Task:** reverse-engineer vendor UMS512 kernel symbols vs VXTux 6.18.52 `.ko`
files; explain the `mod-symbature` (modpost symbol-parse) bug.
**Date:** 2026-10-20
**Host:** Windows 10, bash (git-bash/MSYS)

---

## 0. TL;DR — read this first

**The premise of the task could not be executed as written, for two hard reasons:**

1. **No Ghidra and no `radare2` on `PATH`.** Ghidra is not installed at all.
   radare2 6.2.2 exists on disk at `G:/tools/radare2-6.2.2-w64/bin` but is not
   on `PATH`; I invoked it by absolute/`PATH`-prepend (it works — see §2).
2. **The VXTux kernel has no build artifacts to analyze.** `E:/vxtux_build_repo/6.18.52-VXTux-UMS512`
   contains **zero** `.o`, **zero** `.a` (no `drivers/net/wireless/sprd/wcn/built-in.a` — the
   path named in the task does not exist), **zero** `.ko`, and no `vmlinux`. The build
   never ran: `build_kernel.log` is 2 lines —
   `ABS_SRCTREE=E:/vxtux_build_repo/...` / `Makefile:218: *** source directory cannot contain spaces or colons. Stop.`
   So `ar x .../built-in.a` is impossible — there is nothing to extract.

**Therefore I could not do the requested symbol-by-symbol comparison of VXTux 6.18
built-ins against a stock 5.4 tree.** `G:/UMS512` has **no kernel sources and no
`file2alias.c`** at all — it is a firmware-flashing / boot-image / TWRP workbench
(`boot_a.bin`, `fdl/`, `tools/spd_dump*`, `twrp_intel/`, `*.pac`), not a kernel tree.
No `drivers/gpu/drm/sprd/` or `drivers/soc/sprd/` source exists there to diff against.

**What I did instead** (the useful part, all grounded in real tool output):
I analyzed the **one genuine vendor AArch64 `.ko` that exists on this machine**
(`G:/UMS512/twrp_intel/hxchipset_83102e.ko`, `vermagic=5.4.210 ... aarch64` — i.e. a
real stock-UMS512-kernel module) with radare2 + GNU binutils, and used it to
**empirically confirm, on vendor data, the exact relocation-decode defect** that the
existing `donors/modpost_crash_analysis.md` inferred from `init/main.o`. I also
read the *current* `scripts/mod/modpost.h` in the VXTux tree and found it is
**already fixed** relative to that donor doc.

---

## 1. Tool availability (as requested)

| Tool | Status | Detail |
|---|---|---|
| `ghidra` / `ghidraRun` | **NOT INSTALLED** | `where ghidra` → not found; no `C:/Users/Administrator/tools`; nothing under `C:/Program Files` or `AppData/Local/Programs`. Requires JDK+download; not attempted. |
| `radare2` | **INSTALLED, NOT ON PATH** | `G:/tools/radare2-6.2.2-w64/bin/radare2.exe`, `radare2 6.2.2 +1 abi:142 @ windows-x86_64`. Works when `PATH` is prepended. |
| `aarch64-none-linux-gnu-*` binutils | **INSTALLED** | scoop `gcc-aarch64-none-linux-gnu/15.2.rel1/bin` — `readelf/objdump/ar/nm` 2.45.1. Used for the ELF ground truth. |
| `ar x` on a VXTux `.a` | **IMPOSSIBLE** | No `.a`/`built-in.a` exists anywhere in `E:/vxtux_build_repo` (0 hits). |

Ghidra was not used. Everything below is radare2 + binutils.

---

## 2. radare2 analysis of the vendor `.ko` (real output)

```
$ radare2 -A -q -e bin.relocs.apply=true -c 'iI' hxchipset_83102e.ko
arch     arm
bintype  elf
bits     64
class    ELF64
endian   little
machine  ARM aarch64
os       linux
```

* `r2 -A` (full `aa`/`aac`/`aar`/vtable/prelude analysis) recovered **271 functions**.
* Ground-truth cross-check with `readelf`: **40 sections, 1443 symtab entries,
  6345 AArch64 relocations, 97 undefined (import) symbols.**

Identified the module as the Himax touchscreen driver built against the **stock
UMS512 5.4.210 kernel** (this is the vendor baseline the task wanted):

```
.modinfo:
  description=Himax_common driver
  license=GPL
  depends=
  name=hxchipset_83102e
  vermagic=5.4.210 SMP preempt mod_unload modversions aarch64
```

Symbol classes of interest for the modpost bug:

* **`__versions` section present** (offset `01cd48`, size `0x1840`) → stock kernel
  had **`CONFIG_MODVERSIONS=y`**; the `.ko` carries a modversions CRC payload.
* **`__kcrctab`** section + a dedicated RELA section
  `.rela___kcrctab+himax_debug_init`, and **40 `__crc_*` CRC symbols**.
* 97 undefined imports the kernel must export — e.g. `__arch_copy_from_user`,
  `__kmalloc`, `_raw_spin_lock_irqsave`, `__cfi_slowpath`, `__check_object_size`,
  `printk`, `gpiod_*`, `of_property_*`, `input_*`, `i2c_register_driver`,
  `kmalloc_caches`, `devm_kmalloc`, `cpu_hwcaps`… (full list captured).

Relocation-type mix in this object (all AArch64, none x86):

```
1708 R_AARCH64_ADR_PREL_PG_HI21     719 R_AARCH64_ADD_ABS_LO12_NC   277 R_AARCH64_ABS64
1524 R_AARCH64_LDST64_ABS_LO12_NC   172 R_AARCH64_JUMP26            158 R_AARCH64_PREL32
1457 R_AARCH64_CALL26               132 R_AARCH64_LDST32_ABS_LO12_NC 104 R_AARCH64_LDST8_ABS_LO12_NC
  48 R_AARCH64_PREL64                40 R_AARCH64_ABS32                6 R_AARCH64_LDST16_ABS_LO12_NC
```

---

## 3. Symbols that MATCH / MISMATCH

### 3a. Relocation decode — MISMATCH reproduced on vendor data

The `donors/modpost_crash_analysis.md` conclusion is a **host-tool defect in
`scripts/mod/modpost.h`**: it hardcodes the **x86-64 psABI** `r_info` decode
(`ELF_R_SYM(i)=i>>8`, `ELF_R_TYPE(i)=i&0xff`) while scanning **AArch64** objects, whose
ELF64 layout is `sym=r_info>>32`, `type=r_info&0xffffffff`. I confirmed this on the
*vendor* `.ko` (not just `init/main.o`) with a decode-comparison script
(`decode_demo.py`, run against the real file):

```
[symtab] entries=1443 entsize=24
[relas]  sections=11 total_rela_entries=6345
[decode] relocs whose r_type != (r_info & 0xff)      : 6345  (100.0%)
[decode] relocs where modpost r_sym >= nsym (1443)   : 6345  (100.0%)

.rela___kcrctab+himax_debug_init #0  r_info=0x0000055600000102
    CORRECT_AARCH64 : sym=1366  type=0x000102  (__crc_himax_debug_init)
    MODPOST_PATCH  : sym=22917677057  type=0x02   <-- OOB deref / wrong type
```

**Every one of the 6345 vendor relocations is mis-decoded** (100%), each producing a
symbol index ~22.9 billion against a 1443-entry table (≈ 550 TB of stride → guaranteed
`SIGSEGV` in `section_rela()` at `modpost.c:1391`). So the crash is a property of the
decode, not of any particular `.o` — which is why the donor doc's bisect over
`.vmlinux.objs` cannot converge (a 1-object slice crashes, a 0-object slice exits 0).

### 3b. Symbol *content* — cannot compare (no stock tree)

A true symbol-contract diff (VXTux 6.18 sprd family vs stock 5.4) is **not possible
here**: `G:/UMS512` has no kernel sources and `E:` has no build products. For the
record, the VXTux 6.18 tree *does* carry the full sprd driver family in source
(`drivers/gpu/drm/sprd` 38 `.c`, `drivers/soc/sprd` 26 `.c`,
`drivers/net/wireless/sprd` 88 `.c`, plus `sprdwcn`), and `CONFIG_DRM_SPRD=m`,
`CONFIG_MODVERSIONS=y`, `CONFIG_CFI_CLANG=y`, `CONFIG_LOCALVERSION="-VXTux-UMS512"` —
so the 6.18 side is textually present but not compiled, hence not analyzable.

### 3c. IMPORTANT: the VXTux tree's `modpost.h` is already FIXED

`donors/modpost_crash_analysis.md` describes `#define ELF64_R_SYM(i) ELF_R_SYM(i)` at
two places. The **current** `E:/.../scripts/mod/modpost.h` does **not** do that. Its
final override (lines 107-137) correctly gives ELF64 the gABI layout and gates on
`KERNEL_ELFCLASS`:

```c
/* lines 122-137 (current tree) */
#define ELF64_R_SYM(i)   ((unsigned long long)(i) >> 32)      /* correct */
#define ELF64_R_TYPE(i)  ((unsigned long long)(i) & 0xffffffffULL)
#define ELF32_R_SYM(i)   ((i) >> 8)
#define ELF32_R_TYPE(i)  ((i) & 0xff)
#if KERNEL_ELFCLASS == ELFCLASS32
  #define ELF_R_SYM(i) ELF32_R_SYM(i)   ...
#else
  #define ELF_R_SYM(i) ELF64_R_SYM(i)   ...   /* correct for arm64 */
#endif
```

with comments explicitly describing the SIGSEGV it prevents. So the source-side bug
the donor doc describes has already been repaired in this tree. The stale `donors/`
Python scripts (`check_relocs.py`, `sym19.py`, `symcheck.py`) and
`modpost_gdb_backtrace.txt` still encode the *broken* build's state.

---

## 4. Hypothesis — what symbols modpost was failing to parse

**One-paragraph hypothesis:** modpost was failing to parse *vendor AArch64 symbol and
relocation references — in particular the `__crc_*` (modversions CRC) and undefined
export symbols such as `__arch_copy_from_user`, `__kmalloc`, `_raw_spin_lock_irqsave`,
`gpiod_*`, `of_property_*` that a stock UMS512 5.4 `.ko` imports* — because the host
tool `scripts/mod/modpost.h` was patched to hardcode the **x86-64 psABI** `r_info`
decode (`sym = r_info>>8`, `type = r_info&0xff`) while the objects it scans are
**AArch64** ELF64 relocatables whose layout is `sym = r_info>>32`,
`type = r_info&0xffffffff`. I reproduced this on a real vendor module: **100% of its
6345 relocations decode to an out-of-bounds symbol index** (e.g. `__crc_himax_debug_init`
at `r_info=0x0000055600000102` becomes symbol 22917677057 against a 1443-entry
symtab), so `section_rela()` dereferences `elf->symtab_start + r_sym` roughly 550 TB
past the table and `SIGSEGV`s at `modpost.c:1391` before a single symbol name is ever
resolved. Because the error is architectural rather than object-specific, every entry
in `.vmlinux.objs` crashes identically — which is why no bisect converges (any
non-empty slice fails, an empty slice exits 0) — and because the CRC/`__versions`
relocations are ordinary section-cross-reference entries (`main → read_symbols →
check_sec_ref → section_rela`), the modversions symbol class is hit on the very first
object, before any GPL-license or `EXPORT_SYMBOL` matching occurs. Notably the defect
is *not* present in the current VXTux source (`modpost.h` now correctly uses
`ELF64_R_SYM(i)=(i)>>32` gated on `KERNEL_ELFCLASS`), so the crash documented in
`donors/modpost_crash_analysis.md` corresponds to an **earlier or different build** of
that header than the one currently in `E:/vxtux_build_repo/6.18.52-VXTux-UMS512` — a
rebuild from the current source should clear the symbol-parse fault, and the remaining
real blocker on `E:` is unrelated: the kernel never compiles there because the source
path contains a colon (`Makefile:218`).

---

## 5. Files created / modified

* `C:/Users/Administrator/AppData/Local/hermes/cache/scratch/ghidra_dump/decode_demo.py`
  — ELF parser + 3-way `r_info` decode comparison; run against the real vendor `.ko`
  (produces the §3a numbers).
* Scratch artifacts (same dir): `vendor_sample.ko` (copy), `r2_funcs.txt` (271 r2 funcs),
  `undef_syms.txt` (97 imports), `relocs_sample.txt`, `reloc_types.txt`.
* **This report:** `E:/vxtux_build_repo/donors/ghidra_report.md`.
* No files in the VXTux kernel tree were modified (read-only analysis).

---

## 6. Issues encountered (honest status)

* **Ghidra unavailable** — not installed; would need JDK + download. radare2 used instead.
* **No VXTux build artifacts** — the exact file the task named
  (`drivers/net/wireless/sprd/wcn/built-in.a`) does not exist; kernel build never ran
  (colon-in-path failure, `build_kernel.log`). So no VXTux-vs-stock binary symbol diff
  could be produced.
* **No stock kernel source on `G:`** — `G:/UMS512` is a firmware/flash workbench, not a
  5.4 tree; `file2alias.c` and sprd driver sources absent. The stock comparison half of
  the task is blocked on data that isn't on this machine.
* `r2 -A` emitted some `ERROR: invalid memory` lines (harmless — vtable/prelude probing
  into relocatable sections) but still produced a clean 271-function list; I cross-validated
  every count against `readelf` rather than trusting r2 alone.