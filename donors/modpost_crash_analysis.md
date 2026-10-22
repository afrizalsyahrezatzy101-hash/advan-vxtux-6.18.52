# VXTux 6.18.52 — modpost SIGSEGV Analysis

**Build:** `/workspaces/vxtux-build/6.18.52-VXTux-UMS512` (codespace `solid-tribble-9rjjxvjqq5pcpq5g`)
**Binary:** `scripts/mod/modpost`
**Status:** Root cause identified. Not fixed, per task scope.

---

## TL;DR

The crash is **not** caused by any particular object file. It is a host-tool
(`modpost`) source defect: `scripts/mod/modpost.h` hardcodes the **x86-64**
relocation decoding (`ELF_R_SYM(i) = i >> 8`) while the kernel being scanned is
**AArch64**, which uses a different `r_info` layout. Every object file crashes
modpost identically, so a bisect over `.vmlinux.objs` cannot converge.

The offending code is an uncommitted local patch in `modpost.h`, added to make
modpost build against Bionic's `<elf.h>`. It solves a host-build problem by
hardcoding the wrong architecture's ABI.

---

## 1. Bisect — and why it does not converge

### Method note
The suggested command used `... | tail -5; echo exit=$?`, which reports
**`tail`'s** exit status, not modpost's. Any bisect built on it would silently
treat every run as success. I used the raw exit code of the pipeline head
instead:

```bash
# ~/work/mprun.sh <start> <count>  -> tests objects [start, start+count)
sed -n "$((S+1)),$((S+N))p" .vmlinux.objs | tr '\n' '\0' \
  | xargs -0 ./scripts/mod/modpost -o /tmp/s.symvers -e >/tmp/mp.log 2>&1
echo "RC=$rc"
```

Two exit codes appear and must be distinguished:
- `RC=139` — direct invocation, true segfault (`128 + SIGSEGV`).
- `RC=125` — under `xargs`, which reports a signal death of its child as 125.

### Results

| Slice | Objects | RC | Note |
|---|---|---|---|
| full list | 3002 | 125 | `terminated by signal 11` |
| `[0,1)` | 1 | 125 | crashes |
| `[0,2)` | 2 | 125 | crashes |
| `[0,5)` | 5 | 125 | crashes |
| `[0,50)` | 50 | 125 | crashes |
| `[0,500)` | 500 | 125 | crashes |
| *(truly empty)* | 0 | **0** | **clean exit** |

A single-object invocation, no xargs involved:

```
$ ./scripts/mod/modpost -o /tmp/m.symvers -e init/main.o
ERROR: modpost: missing MODULE_LICENSE() in init/main.o
WARNING: modpost: missing MODULE_DESCRIPTION() in init/main.o
Segmentation fault (core dumped)
RC=139
```

### The bisect is degenerate

**A one-object slice crashes; a zero-object slice exits 0.** The crash does not
depend on which objects are present, only on *any* object being present. There is
therefore no offending object to isolate — every object triggers it. This is a
property of modpost itself, not of the input set.

Spot-checked individually across the list (all RC=139):

```
init/main.o                                        RC=139
init/version.o                                     RC=139
arch/arm64/kernel/head.o                           RC=139
drivers/firmware/efi/libstub/tpm.stub.o            RC=139
lib/string.o                                       RC=139
fs/xattr.o                                         RC=139
```

One caveat on a false lead: an early "empty list" probe appeared to still process
`init/version.o`. That probe was malformed (`sed -n '2,1p'` plus a shell-quoting
artefact), not a sign that modpost reads `.vmlinux.objs` itself. A genuinely empty
invocation exits 0.

---

## 2. Fault location

A `-g -O0` rebuild of modpost from the tree's own source gives an exact frame
(this build was placed in `/tmp`; the tree was not modified):

```
Program received signal SIGSEGV, Segmentation fault.
#0  section_rela (mod=..., elf=..., fsecndx=1, fromsec=... ".text",
                  start=0x7ffff7d502e8, stop=0x7ffff7d515c0)
        at scripts/mod/modpost.c:1391
#1  check_sec_ref (mod=..., elf=...) at scripts/mod/modpost.c:1491
#2  read_symbols (modname=0x7fffffffe687 "init/main.o") at scripts/mod/modpost.c:1666
#3  main (argc=5, argv=...) at scripts/mod/modpost.c:2385

1391    taddr = tsym->st_value + TO_NATIVE(rela->r_addend);

taddr   = 10021698144
r_offset = 12
tsym    = 0x8010d7d4b990
r_type  = 19
r_sym   = 3019898881
rela    = 0x7ffff7d502e8
```

**`r_sym = 3019898881`** is the tell. `init/main.o` has only **485** symbols, so a
symbol index of 3.02 billion is structurally impossible. `tsym =
elf->symtab_start + r_sym` therefore points ~72 GB past the symbol table, and the
`tsym->st_value` dereference at line 1391 faults.

Note the call path is `main -> read_symbols -> check_sec_ref -> section_rela`,
confirming this is ordinary section-cross-reference processing, not a rare or
unusual object.

### Faulting instruction (matches the existing `modpost_gdb_backtrace.txt`)

Disassembling `read_symbols+3727`:

```asm
   <+3706>:  call   get_rel_type_and_sym.isra.0
   <+3711>:  mov    0x5c(%rsp),%edx
   <+3715>:  mov    0x10(%r15),%rax          ; elf->symtab_start
   <+3719>:  lea    (%rdx,%rdx,2),%rdx       ; i * 3
   <+3723>:  lea    (%r10,%rdx,8),%rbx       ; base + i*24   (Elf64_Sym = 24 bytes)
=> <+3727>:  mov    0x8(%rbx),%rcx           ; tsym->st_value   <-- FAULT
   <+3731>:  lea    (%rax,%rcx,1),%r14       ; taddr
```

The 24-byte stride is correct for `Elf64_Sym`. The fault is purely the out-of-range
`r_sym` index being used as an array offset. Register `rsi = 0xb400000113` and
`rdi = 0x13` in the pre-existing backtrace are exactly the raw `r_info` and the
mangled type described below — independent corroboration from a separate run.

---

## 3. ELF observations

`init/main.o` itself is a well-formed, unremarkable AArch64 relocatable object.
Nothing is wrong with the object.

```
$ aarch64-linux-gnu-readelf -SW init/main.o
  [21] .rela.ref.text   RELA    ...  18  68  20  8      # sh_entsize = 24, correct
  [68] .symtab          SYMTAB  ...  18  69  221  8     # sh_entsize = 24, correct

$ aarch64-linux-gnu-readelf -s init/main.o
Symbol table '.symtab' contains 485 entries
```

`sh_entsize` is 24 for both `.symtab` and `.rela.*` — the classic 6.x modpost
`sh_entsize` bug does **not** apply here.

The first relocation of `.rela.text`:

```
$ aarch64-linux-gnu-readelf -r init/main.o
  Offset   Info             Type                    Sym. Name
  000c     00b400000113     R_AARCH64_ADR_PREL_PG_HI21   __tracepoints
```

`r_info = 0x00b400000113`.

| Decode | Symbol index | Type |
|---|---|---|
| **AArch64 correct** (`sym = r_info >> 32`, `type = r_info & 0xffffffff`) | 275 | 180 (`0xb4`) `R_AARCH64_ADR_PREL_PG_HI21` |
| **What modpost computes** (`sym = r_info >> 8`, `type = r_info & 0xff`) | **3019898881** | **19** (`0x13`) `R_AARCH64_CALL26` |
| **What gdb observed** | 3019898881 | 19 |

The computed values match the debugger exactly. Both the symbol index and the
relocation type are wrong.

---

## 4. Root cause

`scripts/mod/modpost.h` defines, at two separate places (lines 46 and 107):

```c
#define ELF_R_SYM(i)     ((i) >> 8)
#define ELF_R_TYPE(i)    ((i) & 0xff)
```

and then hardwires them to both classes:

```c
#define ELF64_R_SYM(i)   ELF_R_SYM(i)
#define ELF64_R_TYPE(i)  ELF_R_TYPE(i)
```

That layout — symbol in bits [8:31], type in bits [0:7] — is the **x86-64 /
i386 psABI** encoding. It is inherited from glibc's `<elf.h>`, where
`ELF64_R_SYM(i)` is `((i) >> 32)` and `ELF64_R_TYPE(i)` is `((i) & 0xffffffff)`;
`ELF_R_SYM = i >> 8` is the 32-bit generic default. AArch64 ELF64 follows the
ELF64 gABI layout: **symbol index in the low 32 bits, type in the high 32 bits**.
The shift-by-8 decode is simply wrong for this target.

These definitions are **not upstream**. They appear in an uncommitted local
modification:

```
$ git diff --stat HEAD -- scripts/mod/
 scripts/mod/modpost.c | 41 +++++++++++++++++++
 scripts/mod/modpost.h | 65 ++++++++++++++++++++++++++++++
 2 files changed, 106 insertions(+)
```

The diff carries this rationale:

> Bionic's `<elf.h>` declares `ELF{32,64}_ST_BIND` as macros that call
> `ELF_ST_BIND(x)`, but never provides `ELF_ST_BIND` itself (glibc-only).
> Define the underlying bit extractors so the cascade resolves.

So the patch was introduced to let modpost compile against **Bionic's `<elf.h>`**
(Android) rather than glibc's. That is a legitimate host-build goal, but the
patch hardcodes one architecture's relocation ABI instead of deferring to
whatever `<elf.h>` actually declares. Because `modpost.h` `#undef`s the
ELF64/ELF32 macros *after* including `<elf.h>` (lines 30-40) and re-defines them
with the x86-64 shift, the correct glibc/Bionic definitions are deliberately
overridden. The correct fix is to stop overriding `ELF64_R_SYM`/`ELF64_R_TYPE` at
all and let the platform header supply them — the Bionic gap was in
`ELF_ST_BIND`/`ELF_ST_TYPE` only, and those are the ones that actually needed the
shim.

The build is also cross-targeted: the kernel under test is AArch64
(`CONFIG_64BIT`, `ARCH=arm64`, `.o` files read with
`aarch64-linux-gnu-readelf`), while modpost is compiled by **host x86-64 gcc**.
modpost must decode *target* objects, so it must use the target ABI's `r_info`
layout, not the host's. It currently uses neither — it uses an x86-64 hardcode.

---

## Hypothesis

The modpost SIGSEGV is a host-side toolchain defect, not a defect in any of the
3002 objects and not a stale-binary problem. `scripts/mod/modpost.h` contains an
uncommitted patch — originally written to work around Bionic's `<elf.h>` missing
`ELF_ST_BIND`/`ELF_ST_TYPE` — which incidentally `#undef`s and then redefines
`ELF64_R_SYM`/`ELF64_R_TYPE` to the x86-64 psABI layout (`sym = r_info >> 8`,
`type = r_info & 0xff`) for both ELF32 and ELF64. The kernel being scanned is
AArch64, whose ELF64 `r_info` puts the symbol index in the low 32 bits and the
relocation type in the high 32 bits, so for the very first relocation of
`init/main.o` (`r_info = 0x00b400000113`, genuinely symbol 275 /
`R_AARCH64_ADR_PREL_PG_HI21`) modpost computes a symbol index of 3019898881 and a
type of 19. `section_rela()` at `modpost.c:1391` then evaluates
`elf->symtab_start + r_sym` and dereferences `tsym->st_value`, which lands roughly
72 GB beyond a 485-entry symbol table and faults. Because the decode is wrong for
the *architecture* rather than for the *content*, every object in `.vmlinux.objs`
crashes identically and an empty list exits cleanly — which is precisely why the
halving bisect cannot converge, since the "crashing half" is the whole list
whenever it is non-empty. The object files themselves are well-formed (correct
`sh_entsize = 24` for `.symtab` and `.rela.*`, valid AArch64 relocations), and a
`-g` rebuild from the tree's own source reproduces the crash, ruling out both
object corruption and a stale prebuilt binary.