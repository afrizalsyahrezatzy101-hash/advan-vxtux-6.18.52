// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2018 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

/*
 * The per-IP variants of the memory-management DVFS driver.
 *
 * Every camera IP (mtx, isp, fd, cpp, jpg, dcam_if, dcam_axi, plus the
 * mmsys block itself) owns a slice of the shared mm register window and
 * a frequency/voltage table.  The callbacks are the same in every
 * variant; only the register window, the field packing and the table
 * differ, so the variants are generated from one template below.
 *
 * The per-IP data (register offsets, bit masks, default indices, table)
 * was recovered from the stock mmdvfs.ko by disassembling the eight
 * ip_dvfs_init() clones and the eight <ip>_dvfs_ops tables.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/devfreq.h>
/* struct devfreq_governor and DEVFREQ_GOV_* live in the devfreq core,
 * not in <linux/devfreq.h>; "../../governor.h" is the 6.18 location.
 */
#include "../../governor.h"
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_opp.h>
#include <linux/slab.h>
#include <linux/stringify.h>

#include <linux/of.h>
#include <linux/pm_opp.h>

#include "sprd_mm_dvfs.h"

static u32 mm_read(u32 off)
{
	return readl(g_mmreg_map + off);
}

static void mm_write(u32 off, u32 val)
{
	writel(val, g_mmreg_map + off);
}

static void mm_update(u32 off, u32 clear_mask, u32 set_mask)
{
	mutex_lock(&mmsys_glob_reg_lock);
	if (clear_mask)
		mm_write(off, mm_read(off) & clear_mask);
	if (set_mask)
		mm_write(off, mm_read(off) | set_mask);
	mutex_unlock(&mmsys_glob_reg_lock);
}

/*
 * The frequency maps, one per IP, extracted from the stock blob.  The
 * field order is map_index, reserved, reg_add, fdiv_num, clk_level,
 * volt_level, axi_index, mtx_index, reserved2, volt_val.
 */

struct ip_dvfs_map_cfg fd_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 368U, 0, 76800000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 372U, 0, 192000000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 376U, 0, 307200000, 2, 0, 0, 0,
	  (char *)"0.7v" },
	{ 3, 0, 380U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 4, 0, 384U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 5, 0, 388U, 2, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 6, 0, 392U, 2, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 7, 0, 396U, 2, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
};

struct ip_dvfs_map_cfg isp_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 80U, 0, 256000000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 84U, 0, 307200000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 88U, 1, 384000000, 2, 0, 0, 0,
	  (char *)"0.75v" },
	{ 3, 0, 92U, 1, 468000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 4, 0, 96U, 2, 512000000, 4, 0, 0, 0,
	  (char *)"0.8v" },
	{ 5, 0, 100U, 2, 512000000, 4, 0, 0, 0,
	  (char *)"0.8v" },
	{ 6, 0, 104U, 2, 512000000, 4, 0, 0, 0,
	  (char *)"0.8v" },
	{ 7, 0, 108U, 2, 512000000, 4, 0, 0, 0,
	  (char *)"0.8v" },
};

struct ip_dvfs_map_cfg dcamaxi_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 304U, 0, 256000000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 308U, 0, 307200000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 312U, 1, 384000000, 2, 0, 0, 0,
	  (char *)"0.75v" },
	{ 3, 0, 316U, 1, 468000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 4, 0, 320U, 2, 468000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 5, 0, 324U, 2, 468000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 6, 0, 328U, 2, 468000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 7, 0, 332U, 2, 468000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
};

struct ip_dvfs_map_cfg dcam_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 272U, 0, 192000000, 0, 3, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 276U, 0, 192000000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 280U, 0, 256000000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 3, 0, 284U, 0, 256000000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 4, 0, 288U, 0, 307200000, 2, 0, 0, 0,
	  (char *)"0.7v" },
	{ 5, 0, 292U, 0, 307200000, 2, 0, 0, 0,
	  (char *)"0.7v" },
	{ 6, 0, 296U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 7, 0, 300U, 1, 468000000, 4, 0, 0, 0,
	  (char *)"0.75v" },
};

struct ip_dvfs_map_cfg jpg_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 112U, 0, 76800000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 116U, 0, 128000000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 120U, 0, 256000000, 2, 0, 0, 0,
	  (char *)"0.7v" },
	{ 3, 0, 124U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 4, 0, 128U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 5, 0, 132U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 6, 0, 136U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 7, 0, 140U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
};

struct ip_dvfs_map_cfg cpp_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 144U, 0, 76800000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 148U, 0, 128000000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 152U, 0, 256000000, 2, 0, 0, 0,
	  (char *)"0.7v" },
	{ 3, 0, 156U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 4, 0, 256U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 5, 0, 260U, 1, 384000000, 3, 0, 0, 0,
	  (char *)"0.75v" },
	{ 6, 0, 264U, 2, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
	{ 7, 0, 268U, 2, 384000000, 3, 0, 0, 0,
	  (char *)"0.8v" },
};

struct ip_dvfs_map_cfg mtx_dvfs_config_table[DVFS_FREQ_LEVEL_NUM] = {
	{ 0, 0, 336U, 0, 76800000, 0, 0, 0, 0,
	  (char *)"0.7v" },
	{ 1, 0, 340U, 0, 128000000, 1, 0, 0, 0,
	  (char *)"0.7v" },
	{ 2, 0, 344U, 0, 256000000, 2, 0, 0, 0,
	  (char *)"0.7v" },
	{ 3, 0, 348U, 0, 307200000, 3, 0, 0, 0,
	  (char *)"0.7v" },
	{ 4, 0, 352U, 1, 384000000, 4, 0, 0, 0,
	  (char *)"0.75v" },
	{ 5, 0, 356U, 1, 468000000, 5, 0, 0, 0,
	  (char *)"0.75v" },
	{ 6, 0, 360U, 2, 512000000, 6, 0, 0, 0,
	  (char *)"0.8v" },
	{ 7, 0, 364U, 2, 512000000, 6, 0, 0, 0,
	  (char *)"0.8v" },
};

/*
 * The per-IP parameters, read out of the stock blob.
 *
 * sw30_mask	bits to clear in 0x30 (the software DVFS gate)
 * upd220_clear	bits to clear in 0x220 (the update control)
 * upd220_set	bits to set in 0x220
 * delay		the value written to 0x214 (the update delay)
 * hold_clear	bits to clear in 0x0c (the voltage hold), 0 = leave alone
 * work_idx_reg	the work index register, and its default value
 * idle_idx_reg	the idle index register, and its default value
 * gate8		the bits set in 0x8 to clock-gate this IP's registers
 */
struct ip_dvfs_params {
	u32 sw30_mask;
	u32 upd220_clear;
	u32 upd220_set;
	u32 delay;
	u32 hold_clear;
	u32 work_idx_reg;
	u32 idle_idx_reg;
	u32 work_idx_def;
	u32 idle_idx_def;
	u32 gate8;
};

#define IP_PARAMS(ip)							\
	static const struct ip_dvfs_params ip##_params = {		\
		.sw30_mask	= ip##_SW30_MASK,			\
		.upd220_clear	= ip##_UPD220_CLEAR,		\
		.upd220_set	= ip##_UPD220_SET,			\
		.delay		= ip##_DELAY,			\
		.hold_clear	= ip##_HOLD_CLEAR,		\
		.work_idx_reg	= ip##_WORK_IDX_REG,		\
		.idle_idx_reg	= ip##_IDLE_IDX_REG,		\
		.work_idx_def	= ip##_WORK_IDX_DEF,		\
		.idle_idx_def	= ip##_IDLE_IDX_DEF,		\
		.gate8		= ip##_GATE8,			\
	}

#define fd_SW30_MASK		0xffffffbfU
#define fd_UPD220_CLEAR		0xffffdfffU
#define fd_UPD220_SET		0x00001000U
#define fd_DELAY		0x00000134U
#define fd_HOLD_CLEAR		0xffffffbfU
#define fd_WORK_IDX_REG		0x1f0U
#define fd_IDLE_IDX_REG		0x1f4U
#define fd_WORK_IDX_DEF		2U
#define fd_IDLE_IDX_DEF		0U
#define fd_GATE8		0x40U

#define isp_SW30_MASK		0xfffffffbU
#define isp_UPD220_CLEAR	0xfffff7ffU
#define isp_UPD220_SET		0x00000400U
#define isp_DELAY		0x13400000U
#define isp_HOLD_CLEAR		0xfffffffdU
#define isp_WORK_IDX_REG	0x1c0U
#define isp_IDLE_IDX_REG	0x1c4U
#define isp_WORK_IDX_DEF	4U
#define isp_IDLE_IDX_DEF	0U
#define isp_GATE8		0x02U

#define dcam_axi_SW30_MASK	0xfffffffeU
#define dcam_axi_UPD220_CLEAR	0xfffffff7U
#define dcam_axi_UPD220_SET	0x00000004U
#define dcam_axi_DELAY		0x0004d000U
#define dcam_axi_HOLD_CLEAR	0U
#define dcam_axi_WORK_IDX_REG	0x1e8U
#define dcam_axi_IDLE_IDX_REG	0x1ecU
#define dcam_axi_WORK_IDX_DEF	2U
#define dcam_axi_IDLE_IDX_DEF	0U
#define dcam_axi_GATE8		0x80U

#define dcam_if_SW30_MASK	0xffffffdfU
#define dcam_if_UPD220_CLEAR	0xfffffffdU
#define dcam_if_UPD220_SET	0x00000001U
#define dcam_if_DELAY		0x00000134U
#define dcam_if_HOLD_CLEAR	0xffffffdfU
#define dcam_if_WORK_IDX_REG	0x1e0U
#define dcam_if_IDLE_IDX_REG	0x1e4U
#define dcam_if_WORK_IDX_DEF	7U
#define dcam_if_IDLE_IDX_DEF	5U
#define dcam_if_GATE8		0x20U

#define jpg_SW30_MASK		0xffffffefU
#define jpg_UPD220_CLEAR	0xffffff7fU
#define jpg_UPD220_SET		0x00000040U
#define jpg_DELAY		0x00000134U
#define jpg_HOLD_CLEAR		0xfffffff7U
#define jpg_WORK_IDX_REG	0x1c8U
#define jpg_IDLE_IDX_REG	0x1ccU
#define jpg_WORK_IDX_DEF	5U
#define jpg_IDLE_IDX_DEF	0U
#define jpg_GATE8		0x08U

#define cpp_SW30_MASK		0xfffffff7U
#define cpp_UPD220_CLEAR	0xfffffdffU
#define cpp_UPD220_SET		0x00000100U
#define cpp_DELAY		0x0004d000U
#define cpp_HOLD_CLEAR		0xfffffffbU
#define cpp_WORK_IDX_REG	0x1d0U
#define cpp_IDLE_IDX_REG	0x1d4U
#define cpp_WORK_IDX_DEF	2U
#define cpp_IDLE_IDX_DEF	0U
#define cpp_GATE8		0x04U

#define mtx_SW30_MASK		0xfffffffdU
#define mtx_UPD220_CLEAR	0xffffffdfU
#define mtx_UPD220_SET		0x00000010U
#define mtx_DELAY		0x13400000U
#define mtx_HOLD_CLEAR		0U
#define mtx_WORK_IDX_REG	0x1d8U
#define mtx_IDLE_IDX_REG	0x1dcU
#define mtx_WORK_IDX_DEF	3U
#define mtx_IDLE_IDX_DEF	0U
#define mtx_GATE8		0x01U

IP_PARAMS(fd);
IP_PARAMS(isp);
IP_PARAMS(dcam_axi);
IP_PARAMS(dcam_if);
IP_PARAMS(jpg);
IP_PARAMS(cpp);
IP_PARAMS(mtx);

/*
 * The bit layout of one map register, per IP.  The stock driver packs
 * the three fields of a map entry differently for every IP.
 */
#define IP_PACK(ip, expr)						\
static void ip##_map_apply(struct ip_dvfs_map_cfg *cfg)		\
{								\
	mm_write(cfg->reg_add, expr);				\
}

/* fd, jpg and cpp: volt in 7:5, axi in 4:2, sub-level in 1:0 */
#define PACK_V5A2S2(c)	(((c)->volt_level & 0x7) << 5 |	\
			 ((c)->axi_index & 0x7) << 2 |	\
			 (c)->fdiv_num & 0x3)

/* isp: volt in 8:6, axi in 5:3, sub-level in 2:0 */
#define PACK_V6A3S3(c)	(((c)->volt_level & 0x7) << 6 |	\
			 ((c)->axi_index & 0x7) << 3 |	\
			 (c)->fdiv_num & 0x7)

/* dcam_axi: sub-level in 1:0, axi in 4:2 */
#define PACK_S2A2(c)	(((c)->fdiv_num & 0x3) |			\
			 ((c)->axi_index & 0x7) << 2)

/* mtx: sub-level in 2:0, volt in 5:3 */
#define PACK_S3V3(c)	(((c)->fdiv_num & 0x7) |			\
			 ((c)->volt_level & 0x7) << 3)

/* dcam_if: volt in 14:11, axi in 10:7, sub-level in 6:3, level in 2:0 */
#define PACK_DCAM_IF(c)	(((c)->volt_level & 0xf) << 11 |	\
				 ((c)->axi_index & 0xf) << 7 |	\
				 ((c)->fdiv_num & 0xf) << 3 |	\
				 (c)->map_index & 0x7)

IP_PACK(fd, PACK_V5A2S2(cfg));
IP_PACK(isp, PACK_V6A3S3(cfg));
IP_PACK(dcam_axi, PACK_S2A2(cfg));
IP_PACK(dcam_if, PACK_DCAM_IF(cfg));
IP_PACK(jpg, PACK_V5A2S2(cfg));
IP_PACK(cpp, PACK_V5A2S2(cfg));
IP_PACK(mtx, PACK_S3V3(cfg));


/*
 * The per-IP callback template.
 *
 * Every IP gets its own copies of these, differing only in the register
 * window they touch, which comes from <ip>_params.  The names are
 * generated with token pasting so the eight variants stay identical.
 */

#define IP_CALLBACKS(ip, tab)							\
static int ip##_ops_init(struct ip_dvfs_ops *ops)			\
{									\
	u32 i;								\
									\
	if (!ops)							\
		return -EINVAL;						\
									\
	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++)			\
		ip##_map_apply(&tab[i]);			\
									\
	mm_update(0x30, ip##_params.sw30_mask, 0);			\
	mm_update(0x220, ip##_params.upd220_clear,			\
		 ip##_params.upd220_set);				\
	mm_write(0x214, ip##_params.delay);				\
	mm_update(0x0c, ip##_params.hold_clear, 0);			\
	mm_write(ip##_params.work_idx_reg,				\
		 (mm_read(ip##_params.work_idx_reg) & ~0x7U) |	\
		 ip##_params.work_idx_def);				\
	mm_write(ip##_params.idle_idx_reg,				\
		 mm_read(ip##_params.idle_idx_reg) & ~0x7U);		\
	mm_update(0x08, 0, ip##_params.gate8);				\
									\
	return 0;							\
}									\
									\
static int ip##_get_dvfs_table(struct ip_dvfs_ops *ops,		\
			       struct ip_dvfs_map_cfg *table,		\
			       int count)				\
{									\
	int i;								\
									\
	if (!ops || !table)						\
		return -EINVAL;						\
									\
	for (i = 0; i < count && i < DVFS_FREQ_LEVEL_NUM; i++)		\
		table[i] = tab[i];			\
									\
	return 0;							\
}									\
									\
static int ip##_set_dvfs_table(struct ip_dvfs_ops *ops,		\
			       struct ip_dvfs_map_cfg *cfg)		\
{									\
	int i;								\
									\
	if (!ops || !cfg)						\
		return -EINVAL;						\
									\
	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++)			\
		tab[i].clk_level =			\
			cfg[i].clk_level;					\
									\
	return 0;							\
}									\
									\
static int ip##_get_index_from_table(struct ip_dvfs_ops *ops,	\
				      u64 freq, u32 *index)		\
{									\
	u32 i, best = 0;							\
									\
	if (!ops || !index)						\
		return -EINVAL;						\
									\
	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++) {			\
		if (freq > tab[i].clk_level)		\
			best = i;						\
	}								\
	*index = best;							\
	return 0;							\
}									\
									\
static int ip##_set_work_index(struct ip_dvfs_ops *ops, u32 index)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_write(ip##_params.work_idx_reg,				\
		 (mm_read(ip##_params.work_idx_reg) & ~0x7U) |	\
		 (index & 0x7U));					\
	return 0;							\
}									\
									\
static int ip##_get_work_index(struct ip_dvfs_ops *ops, u32 *index)	\
{									\
	if (!ops || !index)						\
		return -EINVAL;						\
									\
	*index = mm_read(ip##_params.work_idx_reg) & 0x7U;		\
	return 0;							\
}									\
									\
static int ip##_set_idle_index(struct ip_dvfs_ops *ops, u32 index)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_write(ip##_params.idle_idx_reg,				\
		 (mm_read(ip##_params.idle_idx_reg) & ~0x7U) |	\
		 (index & 0x7U));					\
	return 0;							\
}									\
									\
static int ip##_set_work_freq(struct ip_dvfs_ops *ops, u64 freq)	\
{									\
	u32 index;							\
	int ret;								\
									\
	if (!ops)							\
		return -EINVAL;						\
									\
	ret = ip##_get_index_from_table(ops, freq, &index);		\
	if (ret)							\
		return ret;						\
									\
	return ip##_set_work_index(ops, index);			\
}									\
									\
static int ip##_set_idle_freq(struct ip_dvfs_ops *ops, u64 freq)	\
{									\
	return ip##_set_work_freq(ops, freq);				\
}									\
									\
static int ip##_set_hw_dvfs_en(struct ip_dvfs_ops *ops, int en)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x30, en ? 0 : ip##_params.sw30_mask,			\
		 en ? ip##_params.sw30_mask : 0);				\
	return 0;							\
}									\
									\
static int ip##_set_auto_tune_en(struct ip_dvfs_ops *ops, u32 en)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x30, en ? 0 : ip##_params.sw30_mask,			\
		 en ? ip##_params.sw30_mask : 0);				\
	return 0;							\
}									\
									\
static int ip##_power_on(struct ip_dvfs_ops *ops)			\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x08, 0, ip##_params.gate8);				\
	return 0;							\
}									\
									\
static int ip##_power_off(struct ip_dvfs_ops *ops)			\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x08, ip##_params.gate8, 0);				\
	return 0;							\
}									\
									\
static int ip##_set_gfree_wait_delay(struct ip_dvfs_ops *ops, u32 d)\
{									\
	(void)ops;							\
	mm_write(0x214, d);						\
	return 0;							\
}									\
static int ip##_set_freq_upd_en_byp(struct ip_dvfs_ops *ops, int en)\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(12) : 0, en ? 0 : BIT(12));		\
	return 0;							\
}									\
static int ip##_set_freq_upd_delay_en(struct ip_dvfs_ops *ops, int en)\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(11) : 0, en ? 0 : BIT(11));		\
	return 0;							\
}									\
static int ip##_set_freq_upd_hdsk_en(struct ip_dvfs_ops *ops, int en)\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(7) : 0, en ? 0 : BIT(7));		\
	return 0;							\
}									\
static int ip##_set_sw_trig_en(struct ip_dvfs_ops *ops, int en)	\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(4) : 0, en ? 0 : BIT(4));		\
	return 0;							\
}									\
static int ip##_get_ip_status(struct ip_dvfs_ops *ops,			\
			       struct ip_dvfs_status *status)		\
{									\
	if (!ops || !status)						\
		return -EINVAL;						\
									\
	memset(status, 0, sizeof(*status));				\
	return 0;							\
}									\
static int ip##_update_target_freq(struct ip_dvfs_ops *ops,		\
				    u64 freq, u32 index, int is_fix)	\
{									\
	int ret;								\
									\
	(void)freq;							\
	(void)is_fix;							\
	if (!ops)							\
		return -EINVAL;						\
									\
	ret = ip##_get_index_from_table(ops, freq, &index);		\
	if (ret)							\
		return ret;						\
									\
	return ip##_set_work_index(ops, index);			\
}									\
static int ip##_set_fix_dvfs_value(struct ip_dvfs_ops *ops, u32 v)	\
{									\
	(void)ops;							\
	mmsys_set_fix_dvfs_value(v);					\
	return 0;							\
}									\
static int ip##_top_current_volt(struct ip_dvfs_ops *ops, u32 *volt)\
{									\
	if (!ops || !volt)						\
		return -EINVAL;						\
									\
	*volt = top_mm_dvfs_current_volt();				\
	return 0;							\
}									\
static int ip##_mm_current_volt(struct ip_dvfs_ops *ops, u32 *volt)	\
{									\
	if (!ops || !volt)						\
		return -EINVAL;						\
									\
	*volt = mm_dvfs_read_current_volt();				\
	return 0;							\
}

IP_CALLBACKS(fd, fd_dvfs_config_table);
IP_CALLBACKS(isp, isp_dvfs_config_table);
IP_CALLBACKS(dcam_axi, dcamaxi_dvfs_config_table);
IP_CALLBACKS(dcam_if, dcam_dvfs_config_table);
IP_CALLBACKS(jpg, jpg_dvfs_config_table);
IP_CALLBACKS(cpp, cpp_dvfs_config_table);
IP_CALLBACKS(mtx, mtx_dvfs_config_table);

/*
 * The mmsys-level callbacks.
 *
 * The mmsys block owns the global DVFS controls: the software DVFS
 * gate, the update delay and the update handshake bits.
 */
static int mmsys_ops_init(struct ip_dvfs_ops *ops)
{
	if (!ops)
		return -EINVAL;

	/* the global software DVFS gate starts enabled */
	mm_update(0x30, 0, 0x2);
	return 0;
}

static int mmsys_set_hw_dvfs_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mm_update(0x30, en ? 0 : 0x40, en ? 0x40 : 0);
	return 0;
}

static int mmsys_set_freq_upd_en_byp(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mm_update(0x220, en ? BIT(9) : 0, en ? 0 : BIT(9));
	return 0;
}

static int mmsys_set_freq_upd_delay_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mm_update(0x220, en ? BIT(8) : 0, en ? 0 : BIT(8));
	return 0;
}

static int mmsys_set_freq_upd_hdsk_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mm_update(0x220, en ? BIT(10) : 0, en ? 0 : BIT(10));
	return 0;
}

static int mmsys_set_sw_trig_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mm_update(0x220, en ? BIT(2) : 0, en ? 0 : BIT(2));
	return 0;
}

static int mmsys_power_on(struct ip_dvfs_ops *ops)
{
	(void)ops;
	mm_power_flag = 1;
	return 0;
}

static int mmsys_power_off(struct ip_dvfs_ops *ops)
{
	(void)ops;
	mm_power_flag = 0;
	return 0;
}

static int mmsys_get_ip_status(struct ip_dvfs_ops *ops,
			       struct ip_dvfs_status *status)
{
	if (!ops || !status)
		return -EINVAL;

	memset(status, 0, sizeof(*status));
	return 0;
}

static int mmsys_update_target_freq(struct ip_dvfs_ops *ops, u64 freq,
				     u32 index, int is_fix)
{
	(void)ops;
	(void)freq;
	(void)index;
	(void)is_fix;

	mmsys_adjust_target_freq();
	return 0;
}

static int mmsys_top_current_volt(struct ip_dvfs_ops *ops, u32 *volt)
{
	if (!ops || !volt)
		return -EINVAL;

	*volt = top_mm_dvfs_current_volt();
	return 0;
}

static int mmsys_mm_current_volt(struct ip_dvfs_ops *ops, u32 *volt)
{
	if (!ops || !volt)
		return -EINVAL;

	*volt = mm_dvfs_read_current_volt();
	return 0;
}

/*
 * The per-IP operations tables.
 */
#define IP_OPS(ip)							\
struct ip_dvfs_ops ip##_dvfs_ops = {				\
	.name			= #ip "_DVFS_OPS",			\
	.ver			= 1,				\
	.dvfs_init		= ip##_ops_init,		\
	.set_hw_dvfs_en		= ip##_set_hw_dvfs_en,		\
	.set_auto_tune_en	= ip##_set_auto_tune_en,	\
	.set_work_freq		= ip##_set_work_freq,		\
	.set_idle_freq		= ip##_set_idle_freq,		\
	.get_dvfs_table		= ip##_get_dvfs_table,		\
	.set_dvfs_table		= ip##_set_dvfs_table,		\
	.get_ip_status		= ip##_get_ip_status,		\
	.power_on		= ip##_power_on,		\
	.power_off		= ip##_power_off,		\
	.set_gfree_wait_delay	= ip##_set_gfree_wait_delay,	\
	.set_freq_upd_en_byp	= ip##_set_freq_upd_en_byp,	\
	.set_freq_upd_delay_en	= ip##_set_freq_upd_delay_en,	\
	.set_freq_upd_hdsk_en	= ip##_set_freq_upd_hdsk_en,	\
	.set_ip_dvfs_swtrig_en	= ip##_set_sw_trig_en,		\
	.set_ip_dvfs_work_index	= ip##_set_work_index,		\
	.get_ip_dvfs_work_index	= ip##_get_work_index,		\
	.set_ip_dvfs_idle_index	= ip##_set_idle_index,		\
	.get_ip_index_from_table	= ip##_get_index_from_table,	\
	.get_ip_index_from_table2	= ip##_get_index_from_table,	\
	.updata_target_freq	= ip##_update_target_freq,	\
	.set_fix_dvfs_value	= ip##_set_fix_dvfs_value,	\
	.get_top_current_volt	= ip##_top_current_volt,	\
	.get_mm_current_volt	= ip##_mm_current_volt,	\
}

IP_OPS(fd);
IP_OPS(isp);
IP_OPS(dcam_axi);
IP_OPS(dcam_if);
IP_OPS(jpg);
IP_OPS(cpp);
IP_OPS(mtx);

/*
 * The mmsys block is not a camera IP: it has no frequency map of its
 * own, only the global voltage and gate controls.
 */
struct ip_dvfs_ops mmsys_dvfs_ops = {
	.name			= "MMSYS_DVFS_OPS",
	.ver			= 1,
	.set_hw_dvfs_en		= mmsys_set_hw_dvfs_en,
	.set_freq_upd_delay_en	= mmsys_set_freq_upd_delay_en,
	.set_freq_upd_hdsk_en	= mmsys_set_freq_upd_hdsk_en,
	.set_freq_upd_en_byp	= mmsys_set_freq_upd_en_byp,
	.set_ip_dvfs_swtrig_en	= mmsys_set_sw_trig_en,
	.power_on		= mmsys_power_on,
	.power_off		= mmsys_power_off,
	.dvfs_init		= mmsys_ops_init,
	.get_ip_status		= mmsys_get_ip_status,
	.updata_target_freq	= mmsys_update_target_freq,
	.get_top_current_volt	= mmsys_top_current_volt,
	.get_mm_current_volt	= mmsys_mm_current_volt,
};

/*
 * The governor callbacks, shared by every IP.
 *
 * The governor asks the devfreq core for the target frequency and the
 * core hands the request to the IP's own update_target_freq callback.
 */
static int mm_dvfs_gov_get_target(struct devfreq *df, unsigned long *freq)
{
	struct ip_dvfs_dev *ip_dev = dev_get_drvdata(df->dev.parent);
	struct ip_dvfs_ops *ops;
	u32 index = 0;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	ops = ip_dev->ops;
	if (ops->get_ip_index_from_table)
		ops->get_ip_index_from_table(ops, *freq, &index);

	*freq = ip_dev->work_freq;
	return 0;
}

static int mm_dvfs_gov_event_handler(struct devfreq *df, unsigned int event,
				     void *data)
{
	struct ip_dvfs_dev *ip_dev = dev_get_drvdata(df->dev.parent);

	(void)data;
	if (!ip_dev)
		return -EINVAL;

	switch (event) {
	case DEVFREQ_GOV_START:
		ip_dev->dvfs_enable = 1;
		break;
	case DEVFREQ_GOV_STOP:
		ip_dev->dvfs_enable = 0;
		break;
	default:
		break;
	}

	return 0;
}

/*
 * The per-IP platform drivers and governors.
 *
 * Each IP registers a devfreq governor and a platform driver whose
 * of_match_table carries the IP's device-tree compatible string.
 */

/* the devfreq profile shared by every IP */
static struct devfreq_dev_profile dvfs_profile = {
	.target		= dvfs_target,
	.get_dev_status	= dvfs_get_dev_status,
	.get_cur_freq	= dvfs_get_cur_freq,
};

static int dvfs_probe(struct platform_device *pdev)
{
	struct ip_dvfs_dev *ip_dev;
	struct device *dev = &pdev->dev;
	int ret;

	ip_dev = devm_kmalloc(dev, sizeof(*ip_dev), GFP_KERNEL);
	if (!ip_dev)
		return -ENOMEM;

	mutex_init(&ip_dev->lock);
	ip_dev->ops = get_ip_dvfs_ops(dev_name(dev));
	if (!ip_dev->ops)
		return -EINVAL;

	platform_set_drvdata(pdev, ip_dev);
	dev_set_drvdata(dev, ip_dev);

	ret = dev_pm_opp_of_add_table(dev);
	if (ret) {
		dev_err(dev, "dvfs: Invalid operating-points in device tree.\n");
		return -EINVAL;
	}

	ip_dev->devfreq = devm_devfreq_add_device(dev, &dvfs_profile,
						  "mmdvfs", NULL);
	if (IS_ERR(ip_dev->devfreq)) {
		dev_err(dev, "failed to add devfreq dev with module-dvfs governor\n");
		dev_pm_opp_of_remove_table(dev);
		return -EINVAL;
	}

	return 0;
}

static int dvfs_remove(struct platform_device *pdev)
{
	struct ip_dvfs_dev *ip_dev = platform_get_drvdata(pdev);

	if (ip_dev) {
		if (ip_dev->devfreq)
			devfreq_remove_device(ip_dev->devfreq);
		dev_pm_opp_of_remove_table(&pdev->dev);
	}

	return 0;
}

/* The parameter must NOT be named "compatible": the preprocessor
 * substitutes it inside the ".compatible" field name too, which
 * would yield ".sprd,hwdvfs-mtx" and fail to compile.
 */
#define IP_DRIVER(ip, of_match)					\
const struct of_device_id ip##_dvfs_of_match[] = {		\
	{ .compatible = of_match },				\
	{ }								\
};									\
									\
struct platform_driver ip##_dvfs_driver = {		\
	.probe		= dvfs_probe,				\
	.remove		= dvfs_remove,				\
	.driver		= {					\
		.name	= #ip "-dvfs",				\
		.of_match_table = ip##_dvfs_of_match,		\
	},							\
};								\
									\
struct devfreq_governor ip##_dvfs_gov = {			\
	.name		= #ip "_governor",			\
	.get_target_freq = mm_dvfs_gov_get_target,		\
	.event_handler	= mm_dvfs_gov_event_handler,		\
};

IP_DRIVER(fd, "sprd,hwdvfs-mtx");
IP_DRIVER(isp, "sprd,hwdvfs-isp");
IP_DRIVER(dcam_axi, "sprd,hwdvfs-dcam-axi");
IP_DRIVER(dcam_if, "sprd,hwdvfs-dcam-if");
IP_DRIVER(jpg, "sprd,hwdvfs-jpg");
IP_DRIVER(cpp, "sprd,hwdvfs-cpp");
IP_DRIVER(mtx, "sprd,hwdvfs-mtx");
IP_DRIVER(mmsys, "sprd,hwdvfs-mmsys");

/*
 * The init and exit of one IP: register the ops, the governor and the
 * platform driver, and undo that on the way out.
 */
#define IP_INIT(ip)							\
int ip##_dvfs_init(void)						\
{									\
	int ret;								\
									\
	ret = register_ip_dvfs_ops(&ip##_dvfs_ops);			\
	if (ret) {							\
		pr_err("%s: failed to add ops: %d\n", #ip "_dvfs_init", ret); \
		return ret;						\
	}								\
									\
	ret = devfreq_add_governor(&ip##_dvfs_gov);			\
	if (ret) {							\
		pr_err("%s: failed to add governor: %d\n",		\
		       #ip "_dvfs_init", ret);				\
		unregister_ip_dvfs_ops(&ip##_dvfs_ops);			\
		return ret;						\
	}								\
									\
	ret = platform_driver_register(&ip##_dvfs_driver);		\
	if (ret) {							\
		pr_err("%s: failed to add driver: %d\n",			\
		       #ip "_dvfs_init", ret);				\
		devfreq_remove_governor(&ip##_dvfs_gov);			\
		unregister_ip_dvfs_ops(&ip##_dvfs_ops);			\
		return ret;						\
	}								\
									\
	return 0;							\
}									\
EXPORT_SYMBOL_GPL(ip##_dvfs_init);					\
									\
void ip##_dvfs_exit(void)						\
{									\
	platform_driver_unregister(&ip##_dvfs_driver);			\
	devfreq_remove_governor(&ip##_dvfs_gov);				\
	unregister_ip_dvfs_ops(&ip##_dvfs_ops);				\
}									\
EXPORT_SYMBOL_GPL(ip##_dvfs_exit);
/*
 * The stock mmdvfs.ko exports the per-IP governor, platform driver and
 * of_device_id table (nm on the stock blob: 'D'/'d' symbols named
 * <ip>_dvfs_gov, <ip>_dvfs_driver, <ip>_dvfs_of_match).  A consumer that
 * wants to drive one memory IP by hand looks them up by name, so they
 * have to be global, not static.
 */



/*
 * EXPORT_SYMBOL_GPL takes an identifier, not a string, so it cannot live
 * inside IP_INIT: the ## in the macro body would paste before the caller's
 * argument reaches it.  Invoking a separate macro at file scope after the
 * IP_INIT expansions does the paste in the right order.
 */
#define IP_EXPORT(ip)						EXPORT_SYMBOL_GPL(ip##_dvfs_of_match);			EXPORT_SYMBOL_GPL(ip##_dvfs_driver);			EXPORT_SYMBOL_GPL(ip##_dvfs_gov)

IP_INIT(fd);
IP_INIT(isp);
IP_INIT(dcam_axi);
IP_INIT(dcam_if);
IP_INIT(jpg);
IP_INIT(cpp);
IP_INIT(mtx);
IP_INIT(mmsys);
/*
 * Module entry/exit.  The vendor blob exposes exactly these two names as its
 * init_module/cleanup_module (confirmed with nm on stock mmdvfs.ko), so the
 * port keeps them rather than a plain module_init(): the per-IP init functions
 * are still exported individually for callers that bring the IPs up by hand.
 */
static int __init mmsys_module_init(void)
{
	int ret;

	ret = mmsys_dvfs_init();
	if (ret)
		return ret;

	ret = fd_dvfs_init();
	if (ret)
		goto err_fd;

	ret = isp_dvfs_init();
	if (ret)
		goto err_isp;

	ret = dcam_axi_dvfs_init();
	if (ret)
		goto err_dcam_axi;

	ret = dcam_if_dvfs_init();
	if (ret)
		goto err_dcam_if;

	ret = jpg_dvfs_init();
	if (ret)
		goto err_jpg;

	ret = cpp_dvfs_init();
	if (ret)
		goto err_cpp;

	ret = mtx_dvfs_init();
	if (ret)
		goto err_mtx;

	return 0;

err_mtx:	mtx_dvfs_exit();
err_cpp:	cpp_dvfs_exit();
err_jpg:	jpg_dvfs_exit();
err_dcam_if:	dcam_if_dvfs_exit();
err_dcam_axi:	dcam_axi_dvfs_exit();
err_isp:	isp_dvfs_exit();
err_fd:	fd_dvfs_exit();
	mmsys_dvfs_exit();
	return ret;
}

static void __exit mmsys_module_exit(void)
{
	mtx_dvfs_exit();
	cpp_dvfs_exit();
	jpg_dvfs_exit();
	dcam_if_dvfs_exit();
	dcam_axi_dvfs_exit();
	isp_dvfs_exit();
	fd_dvfs_exit();
	mmsys_dvfs_exit();
}

module_init(mmsys_module_init);
module_exit(mmsys_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nonamey <archanaberry>");
MODULE_DESCRIPTION("Unisoc memory hardware DVFS (replaces vendor mmdvfs.ko)");

IP_EXPORT(fd);
IP_EXPORT(isp);
IP_EXPORT(dcam_axi);
IP_EXPORT(dcam_if);
IP_EXPORT(jpg);
IP_EXPORT(cpp);
IP_EXPORT(mtx);
IP_EXPORT(mmsys);
