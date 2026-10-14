/* SPDX-License-Identifier: GPL-2.0 */
/*
 * ion.h — compat donor 4.14 -> VXTux 6.18 (dipakai vdsp_smem.c/h dan
 * xvp_main.c lewat #include "ion.h"; resolusi: direktori includer tak
 * punya ion.h, jatuh ke -I$(src) di Makefile).
 *
 * Era 4.14, staging ION Sprd menyediakan:
 *   struct dma_buf *ion_new_alloc(size, heap_id_mask, flags);
 * Pohon ini sudah tanpa ION core; include/linux/ion.h adalah shim yang
 * diambil dari marohinmark UMS512 5.4 (SoC sama) dan menyediakan
 * ion_alloc() dengan signature identik — inline stub bila CONFIG_ION=n.
 * ION_HEAP_ID_MASK_SYSTEM/VDSP datang dari uapi/linux/sprd_ion.h
 * (ikut tersertakan oleh <linux/sprd_ion.h>).
 */
#ifndef VXTUX_NPU_VDSP_ION_COMPAT_H
#define VXTUX_NPU_VDSP_ION_COMPAT_H

#include <linux/ion.h>
#include <linux/sprd_ion.h>
#include <linux/sprd_iommu.h>

static inline struct dma_buf *ion_new_alloc(size_t size,
					    unsigned int heap_id_mask,
					    unsigned int flags)
{
	return ion_alloc(size, heap_id_mask, flags);
}

/* Era 4.14, ion_free() milik ION Sprd menerima struct dma_buf * dan
 * melepas referensi buffer. Padanan era 6.18 = dma_buf_put(). Nama
 * dipisah (vdsp_ion_free) supaya tak bentrok dengan deklarasi
 * ion_free(struct ion_buffer *) di <linux/ion.h> shim. */
static inline int vdsp_ion_free(struct dma_buf *dmabuf)
{
	dma_buf_put(dmabuf);
	return 0;
}

#endif /* VXTUX_NPU_VDSP_ION_COMPAT_H */
