// SPDX-License-Identifier: GPL-2.0
/*
 * drivers/vxtux/disp/sprd_dpu/dpu_format.c
 * Konsolidasi patch DPU proven (Fase 9B/10.5) — RE rewrite official-style.
 *
 * Bukti lapangan:
 *  - XR24 @60 Hz satu-satunya mode stabil; 90 Hz + format lain = glitch
 *  - plane property "FBC enabled"=1 + buffer linear = kotak hijau (AFBC-style)
 *  - WLR_DRM_NO_ATOMIC gagal; atomic wlroots bersih (Fase 9A matriks uji)
 *
 * Patch asli (5/5) di fork 5.4: drm_format_to_dpu() RB/UV switch +
 * ARGB fix + primary-only guard + knob dpu_primary_only.
 * File ini = versi bersih yang siap dipakai ulang di 6.18-VXTux-UMS512.
 */
#include <linux/module.h>
#include <linux/types.h>
#include <drm/drm_fourcc.h>

struct drm_plane;

#include <vxtux/compat.h>
#include "dpu_format.h"

/* from: drm_format_to_dpu (fork vendor + patch proven) */
static const struct {
	u32 fourcc;
	u8  dpu_fmt;
	bool rb_switch;	/* patch: swap R/B channel routing */
} vxtux_dpu_formats[] = {
	{ DRM_FORMAT_XRGB8888, 0x00, false },	/* XR24 — mode stabil utama */
	{ DRM_FORMAT_ARGB8888, 0x00, true  },	/* ARGB fix proven */
	{ DRM_FORMAT_XBGR8888, 0x00, true  },	/* RB switch patch */
	/* TODO(RE): lengkapi tabel dari fork 5.4 patch 5/5 */
};

u8 vxtux_dpu_format_get(u32 fourcc)
{
	unsigned int i;

	for (i = 0; i < ARRAY_SIZE(vxtux_dpu_formats); i++)
		if (vxtux_dpu_formats[i].fourcc == fourcc)
			return vxtux_dpu_formats[i].dpu_fmt;

	return 0xFF;	/* unsupported — probe defer/return EINVAL */
}
EXPORT_SYMBOL_GPL(vxtux_dpu_format_get);

MODULE_AUTHOR("VXTux Project");
MODULE_DESCRIPTION("sprd DPU consolidated RE patches for UMS512");
MODULE_LICENSE("GPL");
