#!/bin/bash
# Compile ONE file of the mm-dvfs driver to the scratch dir, using the
# kernel's own flag set.  Nothing is written into the source tree.
#
#   cc1.sh <file.c> [outname]
#
# The generated headers (autoconf.h, asm-offsets.h, ...) are staged in
# $KHDR; the source tree is the one at $KTREE.

KTREE="E:/vxtux_build_repo/6.18.52-VXTux-UMS512"
KHDR="C:/Users/Administrator/AppData/Local/hermes/cache/scratch/khdr"
OUT="C:/Users/Administrator/AppData/Local/hermes/cache/scratch/agent5"
GCC="C:/Users/Administrator/scoop/apps/gcc-aarch64-none-linux-gnu/15.2.rel1/bin/aarch64-none-linux-gnu-gcc"

SRC="$1"
NAME="${2:-$(basename "$SRC" .c)}"
mkdir -p "$OUT"

cd "$KTREE" || exit 1

"$GCC" -c -o "$OUT/$NAME.o" "$SRC" \
  -nostdinc \
  -I./arch/arm64/include \
  -I"$KHDR/arch/arm64/include/generated" \
  -I./include \
  -I./arch/arm64/include/uapi \
  -I./drivers/devfreq \
  -I./drivers/devfreq/sprd/mm \
  -I"$KHDR/arch/arm64/include/generated/uapi" \
  -I./include/uapi \
  -I"$KHDR/include/generated/uapi" \
  -I"$KHDR/include" \
  -I"$KHDR" \
  -include ./include/linux/compiler-version.h \
  -include ./include/linux/kconfig.h \
  -include ./include/linux/compiler_types.h \
  -D__KERNEL__ -mlittle-endian \
  -DKASAN_SHADOW_SCALE_SHIFT= \
  -std=gnu11 -fshort-wchar -funsigned-char -fno-common -fno-PIE \
  -fno-strict-aliasing -mgeneral-regs-only \
  -DCONFIG_CC_HAS_K_CONSTRAINT=1 -Wno-psabi -mabi=lp64 \
  -fno-asynchronous-unwind-tables -fno-unwind-tables \
  -Wa,-march=armv8.5-a -DARM64_ASM_ARCH='"armv8.5-a"' \
  -fno-delete-null-pointer-checks -O2 -fno-allow-store-data-races \
  -fstack-protector-strong -fno-omit-frame-pointer -fno-optimize-sibling-calls \
  -ftrivial-auto-var-init=zero -fno-stack-clash-protection -falign-functions=4 \
  -fstrict-flex-arrays=3 -fno-strict-overflow -fno-stack-check -fconserve-stack \
  -fno-builtin-wcslen \
  -Wall -Wextra -Wundef -Werror=implicit-function-declaration \
  -Werror=implicit-int -Werror=return-type -Werror=strict-prototypes \
  -Wno-format-security -Wno-trigraphs -Wno-frame-address \
  -Wno-address-of-packed-member -Wmissing-declarations -Wmissing-prototypes \
  -Wframe-larger-than=2048 -Wno-main -Wno-dangling-pointer \
  -Wvla-larger-than=1 -Wno-pointer-sign -Wcast-function-type \
  -Wno-array-bounds -Wno-stringop-overflow -Wno-alloc-size-larger-than \
  -Wimplicit-fallthrough=5 -Werror=date-time \
  -Wno-error=incompatible-pointer-types -Wno-error=implicit-function-declaration \
  -Wno-error=implicit-int -Wno-error=return-type -Wno-error=strict-prototypes \
  -Wno-error=designated-init -Wno-error=date-time -Werror=designated-init \
  -Wenum-conversion -Wunused -Wno-unused-but-set-variable \
  -Wno-unused-const-variable -Wno-packed-not-aligned \
  -Wno-format-overflow -Wno-format-truncation -Wno-stringop-truncation \
  -Wno-override-init -Wno-missing-field-initializers -Wno-type-limits \
  -Wno-shift-negative-value -Wno-maybe-uninitialized -Wno-sign-compare \
  -Wno-unused-parameter -g -gdwarf-5 \
  -mstack-protector-guard=sysreg -mstack-protector-guard-reg=sp_el0 \
  -mstack-protector-guard-offset=1616 \
  -DMODULE -DKBUILD_BASENAME="\"$NAME\"" -DKBUILD_MODNAME='"mmdvfs"' \
  -D__KBUILD_MODNAME=kmod_mmdvfs
RC=$?
echo "COMPILE_RC=$RC  ($SRC)"
exit $RC
