/* SPDX-License-Identifier: GPL-2.0 */
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
 * The per-SoC register descriptors and the 19 op tables.
 *
 * Every offset, bit mask and shift below was recovered from the stock
 * apsys-dvfs.ko: the decompiled callback clones differ only in these
 * constants, so one descriptor per (SoC, block) pair drives the whole
 * table.
 *
 * UMS512 is sharkl5pro.  That is the variant that has to be functionally
 * correct; the rest exist so that the exported symbol surface of the stock
 * blob is preserved.
 */

#include "sprd_dvfs_soc.h"
#include "sprd_dvfs_ops.h"

/* ------------------------------------------------------------------ */
/* apsys								*/
/* ------------------------------------------------------------------ */

/*
 * The apsys block.  The "hold" flag lives in the block's own first
 * register, the wait-window at +4 and the min-volt at +0x10; the two
 * enable bits share the ctrl register.
 *
 * The qogirn6pro apsys block is different: its ctrl register is at +0x50
 * and its min-volt at +0x24.  It is also the only SoC whose DVFS engine
 * is gated on the aon silicon revision.
 */
const struct apsys_dvfs_regs apsys_regs = {
	.hold		= 0x00,
	.wait_window	= 0x04,
	.min_volt	= 0x10,
	.ctrl		= 0x3c,
	.ctrl_force_bit	= 0x2,
	.ctrl_gate_bit	= 0x1,
};
EXPORT_SYMBOL(apsys_regs);

const struct apsys_dvfs_regs apsys_regs_qogirn6pro = {
	.hold		= 0x00,
	.wait_window	= 0x04,
	.min_volt	= 0x24,
	.ctrl		= APSYS_CTRL_Q6PRO,
	.ctrl_force_bit	= 0x2,
	.ctrl_gate_bit	= 0x1,
};
EXPORT_SYMBOL(apsys_regs_qogirn6pro);

/*
 * The TOP dcdc block.  Every SoC but qogirn6pro reads the modem voltage
 * at +0x44; qogirn6pro reads +0x3f8 and roc1 +0x50.
 */
const struct top_dvfs_regs top_regs_sharkl5pro = {
	.base		= 0x322a0000,
	.size		= 0x1cc,
	.volt		= 0x44,
	.volt_shift	= 0x14,
};
EXPORT_SYMBOL(top_regs_sharkl5pro);

const struct top_dvfs_regs top_regs_qogirl6 = {
	.base		= 0x64014000,
	.size		= 0x1cc,
	.volt		= 0x44,
	.volt_shift	= 0x14,
};
EXPORT_SYMBOL(top_regs_qogirl6);

const struct top_dvfs_regs top_regs_qogirn6pro = {
	.base		= 0x64940000,
	.size		= 0x600,
	.volt		= 0x3f8,
	.volt_shift	= 0x14,
};
EXPORT_SYMBOL(top_regs_qogirn6pro);

const struct top_dvfs_regs top_regs_sharkl5 = {
	.base		= 0x322a0000,
	.size		= 0x150,
	.volt		= 0x44,
	.volt_shift	= 0x14,
};
EXPORT_SYMBOL(top_regs_sharkl5);

const struct top_dvfs_regs top_regs_roc1 = {
	.base		= 0x322a0000,
	.size		= 0x150,
	.volt		= 0x50,
	.volt_shift	= 0x14,
};
EXPORT_SYMBOL(top_regs_roc1);

/* ------------------------------------------------------------------ */
/* The IP (dpu / vsp / vdsp / gsp) register windows.			*/
/* ------------------------------------------------------------------ */

/*
 * The dpu block.  sharkl5pro, sharkl5 and qogirl6 share one layout; roc1
 * adds a sixth divider register and has bit 2 of the sw-trig register
 * reserved; qogirn6pro is entirely different.
 */
const struct ip_dvfs_hw_regs dpu_regs_sharkl5pro = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x1,
	.work_index	= 0x124,
	.idle_index	= 0x128,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 10,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x1,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x20,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x10,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
	.div0		= 0x94,
	.div1		= 0x98,
	.div2		= 0x9c,
	.div3		= 0x100,
	.div4		= 0x104,
};
EXPORT_SYMBOL(dpu_regs_sharkl5pro);

const struct ip_dvfs_hw_regs dpu_regs_qogirl6 = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x1,
	.work_index	= 0x124,
	.idle_index	= 0x128,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 10,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x1,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x20,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x10,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
	.div0		= 0x94,
	.div1		= 0x98,
	.div2		= 0x9c,
	.div3		= 0x100,
	.div4		= 0x104,
};
EXPORT_SYMBOL(dpu_regs_qogirl6);

const struct ip_dvfs_hw_regs dpu_regs_sharkl5 = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x1,
	.work_index	= 0x124,
	.idle_index	= 0x128,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 10,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x1,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x20,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x10,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
	.div0		= 0x94,
	.div1		= 0x98,
	.div2		= 0x9c,
	.div3		= 0x100,
	.div4		= 0x104,
};
EXPORT_SYMBOL(dpu_regs_sharkl5);

const struct ip_dvfs_hw_regs dpu_regs_roc1 = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x1,
	.work_index	= 0x124,
	.idle_index	= 0x128,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 10,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x1,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x20,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x10,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x4,
	.sw_trig_shift	= 0,
	.div0		= 0x94,
	.div1		= 0x98,
	.div2		= 0x9c,
	.div3		= 0x100,
	.div4		= 0x104,
	.div5		= 0x108,
};
EXPORT_SYMBOL(dpu_regs_roc1);

const struct ip_dvfs_hw_regs dpu_regs_qogirn6pro = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x1,
	.work_index	= 0x7bc,
	.idle_index	= 0x7c0,
	.gfree_wait	= 0x944,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 10,
	.upd_en_byp	= 0x48,
	.upd_en_byp_mask = 0x1,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x9ac,
	.upd_delay_mask = 0x20,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x9ac,
	.upd_hdsk_mask	= 0x10,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0x1c,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
	.div0		= 0x1f8,
	.div1		= 0x1fc,
	.div2		= 0x200,
	.div3		= 0x204,
	.div4		= 0x208,
};
EXPORT_SYMBOL(dpu_regs_qogirn6pro);

/*
 * The vsp block.  Its gfree-wait field is OR-ed in rather than masked
 * out, and its enable and sw-trig bits live at bit 1.
 */
const struct ip_dvfs_hw_regs vsp_regs_sharkl5pro = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x2,
	.work_index	= 0x11c,
	.idle_index	= 0x120,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 0,
	.gfree_wait_or	= 1,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x2,
	.upd_en_byp_shift = 1,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x8,
	.upd_delay_shift = 3,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x4,
	.upd_hdsk_shift	= 2,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x2,
	.sw_trig_shift	= 1,
};
EXPORT_SYMBOL(vsp_regs_sharkl5pro);

const struct ip_dvfs_hw_regs vsp_regs_qogirl6 = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x2,
	.work_index	= 0x11c,
	.idle_index	= 0x120,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 0,
	.gfree_wait_or	= 1,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x2,
	.upd_en_byp_shift = 1,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x8,
	.upd_delay_shift = 3,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x4,
	.upd_hdsk_shift	= 2,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x2,
	.sw_trig_shift	= 1,
};
EXPORT_SYMBOL(vsp_regs_qogirl6);

const struct ip_dvfs_hw_regs vsp_regs_sharkl5 = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x2,
	.work_index	= 0x11c,
	.idle_index	= 0x120,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 0,
	.gfree_wait_or	= 1,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x2,
	.upd_en_byp_shift = 1,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x8,
	.upd_delay_shift = 3,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x4,
	.upd_hdsk_shift	= 2,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x2,
	.sw_trig_shift	= 1,
};
EXPORT_SYMBOL(vsp_regs_sharkl5);

const struct ip_dvfs_hw_regs vsp_regs_roc1 = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x2,
	.work_index	= 0x11c,
	.idle_index	= 0x120,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 0,
	.gfree_wait_or	= 1,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x2,
	.upd_en_byp_shift = 1,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x8,
	.upd_delay_shift = 3,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x4,
	.upd_hdsk_shift	= 2,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x2,
	.sw_trig_shift	= 1,
};
EXPORT_SYMBOL(vsp_regs_roc1);

const struct ip_dvfs_hw_regs vsp_regs_qogirn6pro_vpudec = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x2,
	.work_index	= 0x7a4,
	.idle_index	= 0x7a8,
	.gfree_wait	= 0x948,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 0,
	.gfree_wait_or	= 1,
	.upd_en_byp	= 0x48,
	.upd_en_byp_mask = 0x4,
	.upd_en_byp_shift = 2,
	.upd_delay	= 0x9ac,
	.upd_delay_mask = 0x20,
	.upd_delay_shift = 5,
	.upd_hdsk	= 0x9ac,
	.upd_hdsk_mask	= 0x10,
	.upd_hdsk_shift	= 4,
	.sw_trig	= 0x1c,
	.sw_trig_mask	= 0x2,
	.sw_trig_shift	= 1,
};
EXPORT_SYMBOL(vsp_regs_qogirn6pro_vpudec);

const struct ip_dvfs_hw_regs vsp_regs_qogirn6pro_vpuenc = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x2,
	.work_index	= 0x79c,
	.idle_index	= 0x7a0,
	.gfree_wait	= 0x948,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 10,
	.upd_en_byp	= 0x48,
	.upd_en_byp_mask = 0x2,
	.upd_en_byp_shift = 1,
	.upd_delay	= 0x9ac,
	.upd_delay_mask = 0x8,
	.upd_delay_shift = 3,
	.upd_hdsk	= 0x9ac,
	.upd_hdsk_mask	= 0x4,
	.upd_hdsk_shift	= 2,
	.sw_trig	= 0x1c,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
};
EXPORT_SYMBOL(vsp_regs_qogirn6pro_vpuenc);

/*
 * The vdsp block.  Its gfree-wait field is 20 bits wide and its enable
 * bit lives in the control register at +0x8.
 */
const struct ip_dvfs_hw_regs vdsp_regs_sharkl5pro = {
	.ctrl		= 0x8,
	.ctrl_en_mask	= 0x4,
	.work_index	= 0x114,
	.idle_index	= 0x118,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0xfffff,
	.gfree_wait_shift = 0,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x4,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x2,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x1,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
};
EXPORT_SYMBOL(vdsp_regs_sharkl5pro);

const struct ip_dvfs_hw_regs vdsp_regs_roc1 = {
	.ctrl		= 0x8,
	.ctrl_en_mask	= 0x4,
	.work_index	= 0x114,
	.idle_index	= 0x118,
	.gfree_wait	= 0x130,
	.gfree_wait_mask = 0xfffff,
	.gfree_wait_shift = 0,
	.upd_en_byp	= 0x38,
	.upd_en_byp_mask = 0x4,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x134,
	.upd_delay_mask = 0x2,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x134,
	.upd_hdsk_mask	= 0x1,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0xc,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
};
EXPORT_SYMBOL(vdsp_regs_roc1);

/* the gsp block; its enable bit is bit 2 of the control register */
const struct ip_dvfs_hw_regs gsp_regs_qogirn6pro = {
	.ctrl		= 0x14,
	.ctrl_en_mask	= 0x4,
	.work_index	= 0x7ac,
	.idle_index	= 0x7b0,
	.gfree_wait	= 0x944,
	.gfree_wait_mask = 0x3ff,
	.gfree_wait_shift = 0,
	.upd_en_byp	= 0x48,
	.upd_en_byp_mask = 0x8,
	.upd_en_byp_shift = 0,
	.upd_delay	= 0x9ac,
	.upd_delay_mask = 0x80,
	.upd_delay_shift = 0,
	.upd_hdsk	= 0x9ac,
	.upd_hdsk_mask	= 0x40,
	.upd_hdsk_shift	= 0,
	.sw_trig	= 0x1c,
	.sw_trig_mask	= 0x1,
	.sw_trig_shift	= 0,
};
EXPORT_SYMBOL(gsp_regs_qogirn6pro);

/* ------------------------------------------------------------------ */
/* The per-SoC descriptors.						*/
/* ------------------------------------------------------------------ */

const struct soc_dvfs_desc dpu_desc_sharkl5pro = {
	.name = "sharkl5pro",
	.regs = &dpu_regs_sharkl5pro,
};
EXPORT_SYMBOL(dpu_desc_sharkl5pro);

const struct soc_dvfs_desc dpu_desc_qogirl6 = {
	.name = "qogirl6",
	.regs = &dpu_regs_qogirl6,
};
EXPORT_SYMBOL(dpu_desc_qogirl6);

const struct soc_dvfs_desc dpu_desc_sharkl5 = {
	.name = "sharkl5",
	.regs = &dpu_regs_sharkl5,
};
EXPORT_SYMBOL(dpu_desc_sharkl5);

const struct soc_dvfs_desc dpu_desc_roc1 = {
	.name = "roc1",
	.regs = &dpu_regs_roc1,
};
EXPORT_SYMBOL(dpu_desc_roc1);

const struct soc_dvfs_desc dpu_desc_qogirn6pro = {
	.name = "qogirn6pro",
	.regs = &dpu_regs_qogirn6pro,
};
EXPORT_SYMBOL(dpu_desc_qogirn6pro);

const struct soc_dvfs_desc vsp_desc_sharkl5pro = {
	.name = "sharkl5pro",
	.regs = &vsp_regs_sharkl5pro,
};
EXPORT_SYMBOL(vsp_desc_sharkl5pro);

const struct soc_dvfs_desc vsp_desc_qogirl6 = {
	.name = "qogirl6",
	.regs = &vsp_regs_qogirl6,
};
EXPORT_SYMBOL(vsp_desc_qogirl6);

const struct soc_dvfs_desc vsp_desc_sharkl5 = {
	.name = "sharkl5",
	.regs = &vsp_regs_sharkl5,
};
EXPORT_SYMBOL(vsp_desc_sharkl5);

const struct soc_dvfs_desc vsp_desc_roc1 = {
	.name = "roc1",
	.regs = &vsp_regs_roc1,
};
EXPORT_SYMBOL(vsp_desc_roc1);

const struct soc_dvfs_desc vsp_desc_qogirn6pro_vpudec = {
	.name = "qogirn6pro_vpudec",
	.regs = &vsp_regs_qogirn6pro_vpudec,
};
EXPORT_SYMBOL(vsp_desc_qogirn6pro_vpudec);

const struct soc_dvfs_desc vsp_desc_qogirn6pro_vpuenc = {
	.name = "qogirn6pro_vpuenc",
	.regs = &vsp_regs_qogirn6pro_vpuenc,
};
EXPORT_SYMBOL(vsp_desc_qogirn6pro_vpuenc);

const struct soc_dvfs_desc vdsp_desc_sharkl5pro = {
	.name = "sharkl5pro",
	.regs = &vdsp_regs_sharkl5pro,
};
EXPORT_SYMBOL(vdsp_desc_sharkl5pro);

const struct soc_dvfs_desc vdsp_desc_roc1 = {
	.name = "roc1",
	.regs = &vdsp_regs_roc1,
};
EXPORT_SYMBOL(vdsp_desc_roc1);

const struct soc_dvfs_desc gsp_desc_qogirn6pro = {
	.name = "qogirn6pro",
	.regs = &gsp_regs_qogirn6pro,
};
EXPORT_SYMBOL(gsp_desc_qogirn6pro);
