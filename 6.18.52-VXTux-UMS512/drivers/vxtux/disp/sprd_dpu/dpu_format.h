/* SPDX-License-Identifier: GPL-2.0 */
/*
 * drivers/vxtux/disp/sprd_dpu/dpu_format.h
 * Pemetaan fourcc -> format DPU vendor (konsolidasi patch proven Fase 9B).
 *
 * Kenapa header ini ada: vxtux_dpu_format_get() sudah EXPORT_SYMBOL_GPL tetapi
 * tanpa deklarasi publik => clang -Wmissing-prototypes. Warning itu baru muncul
 * setelah wiring disp/sprd_dpu/ diperbaiki (2026-09-18); sebelumnya berkasnya
 * tidak pernah dikompilasi sehingga cacatnya tersembunyi.
 */
#ifndef _VXTUX_SPRD_DPU_FORMAT_H
#define _VXTUX_SPRD_DPU_FORMAT_H

#include <linux/types.h>

/* 0xFF = fourcc tidak didukung oleh tabel DPU (pemanggil wajib cek). */
#define VXTUX_DPU_FMT_UNSUPPORTED	0xFF

u8 vxtux_dpu_format_get(u32 fourcc);

#endif /* _VXTUX_SPRD_DPU_FORMAT_H */
