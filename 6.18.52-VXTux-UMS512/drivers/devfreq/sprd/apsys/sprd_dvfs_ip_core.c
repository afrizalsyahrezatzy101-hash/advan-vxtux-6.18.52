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
 * The shared bodies of the per-IP DVFS drivers.
 *
 * The stock apsys-dvfs.ko has one hand-written copy of every DVFS
 * callback per IP and per SoC: 19 op tables and ~150 clones.  All the
 * clones of a given callback are byte-identical apart from the register
 * offsets they poke, so this file keeps ONE body per slot and drives it
 * from the per-SoC descriptor in sprd_dvfs_soc.h.
 *
 * The offsets, bit masks and shifts were recovered from the stock blob's
 * relocation and disassembly data.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "sprd_dvfs_apsys.h"
#include "sprd_dvfs_ip.h"
#include "sprd_dvfs_soc.h"

/* ------------------------------------------------------------------ */
/* The register access helpers.					*/
/* ------------------------------------------------------------------ */

static void ip_update_bits(struct ip_dvfs_priv *priv, u32 off, u32 mask,
			   u32 val)
{
	void __iomem *base = (void __iomem *)priv->apsys->base;

	mutex_lock(&priv->lock);
	writel((readl(base + off) & ~mask) | (val & mask), base + off);
	mutex_unlock(&priv->lock);
}

static u32 ip_readl(struct ip_dvfs_priv *priv, u32 off)
{
	return readl((void __iomem *)priv->apsys->base + off);
}

static void ip_writel(struct ip_dvfs_priv *priv, u32 off, u32 val)
{
	writel(val, (void __iomem *)priv->apsys->base + off);
}

/* ------------------------------------------------------------------ */
/* The shared ops bodies.						*/
/* ------------------------------------------------------------------ */

/**
 * ip_dvfs_parse_dt - read the common DVFS coefficients from the DT
 * @priv: the IP
 * @np: the device node
 */
int ip_dvfs_parse_dt(struct ip_dvfs_priv *priv, struct device_node *np)
{
	pr_info("ip_dvfs_parse_dt\n");

	if (of_property_read_variable_u32_array(np, "sprd,gfree-wait-delay",
						&priv->coffe.gfree_wait_delay,
						1, 0) < 0)
		priv->coffe.gfree_wait_delay = 0x100;

	if (of_property_read_variable_u32_array(np, "sprd,freq-upd-hdsk-en",
						&priv->coffe.freq_upd_hdsk_en,
						1, 0) < 0)
		priv->coffe.freq_upd_hdsk_en = 1;

	if (of_property_read_variable_u32_array(np, "sprd,freq-upd-delay-en",
						&priv->coffe.freq_upd_delay_en,
						1, 0) < 0)
		priv->coffe.freq_upd_delay_en = 1;

	if (of_property_read_variable_u32_array(np, "sprd,freq-upd-en-byp",
						&priv->coffe.freq_upd_en_byp,
						1, 0) < 0)
		priv->coffe.freq_upd_en_byp = 0;

	if (of_property_read_variable_u32_array(np, "sprd,sw-trig-en",
						&priv->coffe.sw_trig_en,
						1, 0) < 0)
		priv->coffe.sw_trig_en = 0;

	return 0;
}
EXPORT_SYMBOL(ip_dvfs_parse_dt);

/**
 * ip_hw_dfs_en - enable or disable the hardware DVFS engine
 * @priv: the IP
 * @en: non-zero to enable
 */
void ip_hw_dfs_en(struct ip_dvfs_priv *priv, u32 en)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;

	ip_update_bits(priv, r->ctrl, r->ctrl_en_mask,
		       en ? r->ctrl_en_mask : 0);
}
EXPORT_SYMBOL(ip_hw_dfs_en);

/**
 * ip_set_work_index - select the work frequency index
 */
void ip_set_work_index(struct ip_dvfs_priv *priv, int index)
{
	ip_writel(priv, priv->desc->regs->work_index, index);
}
EXPORT_SYMBOL(ip_set_work_index);

/**
 * ip_get_work_index - read the work frequency index
 */
int ip_get_work_index(struct ip_dvfs_priv *priv)
{
	return ip_readl(priv, priv->desc->regs->work_index);
}
EXPORT_SYMBOL(ip_get_work_index);

/**
 * ip_set_idle_index - select the idle frequency index
 */
void ip_set_idle_index(struct ip_dvfs_priv *priv, int index)
{
	ip_writel(priv, priv->desc->regs->idle_index, index);
}
EXPORT_SYMBOL(ip_set_idle_index);

/**
 * ip_get_idle_index - read the idle frequency index
 */
int ip_get_idle_index(struct ip_dvfs_priv *priv)
{
	return ip_readl(priv, priv->desc->regs->idle_index);
}
EXPORT_SYMBOL(ip_get_idle_index);

/**
 * ip_set_gfree_wait_delay - set the gfree wait delay
 */
void ip_set_gfree_wait_delay(struct ip_dvfs_priv *priv, u32 delay)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;

	if (r->gfree_wait_or)
		ip_update_bits(priv, r->gfree_wait, ~r->gfree_wait_mask,
			       delay << r->gfree_wait_shift);
	else
		ip_update_bits(priv, r->gfree_wait, r->gfree_wait_mask,
			       delay << r->gfree_wait_shift);
}
EXPORT_SYMBOL(ip_set_gfree_wait_delay);

/**
 * ip_set_freq_upd_en_byp - set the freq-upd-en-byp bit
 */
void ip_set_freq_upd_en_byp(struct ip_dvfs_priv *priv, u32 en)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;

	ip_update_bits(priv, r->upd_en_byp, r->upd_en_byp_mask,
		       en ? r->upd_en_byp_mask : 0);
}
EXPORT_SYMBOL(ip_set_freq_upd_en_byp);

/**
 * ip_set_freq_upd_delay_en - set the freq-upd-delay-en bit
 */
void ip_set_freq_upd_delay_en(struct ip_dvfs_priv *priv, u32 en)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;

	ip_update_bits(priv, r->upd_delay, r->upd_delay_mask,
		       en ? r->upd_delay_mask : 0);
}
EXPORT_SYMBOL(ip_set_freq_upd_delay_en);

/**
 * ip_set_freq_upd_hdsk_en - set the freq-upd-hdsk-en bit
 */
void ip_set_freq_upd_hdsk_en(struct ip_dvfs_priv *priv, u32 en)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;

	ip_update_bits(priv, r->upd_hdsk, r->upd_hdsk_mask,
		       en ? r->upd_hdsk_mask : 0);
}
EXPORT_SYMBOL(ip_set_freq_upd_hdsk_en);

/**
 * ip_set_dvfs_swtrig_en - set the software trigger bit
 */
void ip_set_dvfs_swtrig_en(struct ip_dvfs_priv *priv, u32 en)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;

	ip_update_bits(priv, r->sw_trig, r->sw_trig_mask,
		       en ? r->sw_trig_mask : 0);
}
EXPORT_SYMBOL(ip_set_dvfs_swtrig_en);

/* ------------------------------------------------------------------ */
/* The frequency map.							*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob keeps a per-SoC table of {map_index, volt_level,
 * clk_level, clk_rate, volt_val} entries, and the sysfs "dvfs_table"
 * attribute prints it.  The map is built by the per-SoC op tables; the
 * generic builder below is used by every SoC that has no special
 * packing of its own.
 */
static int ip_build_dvfs_table(struct ip_dvfs_priv *priv,
			       struct ip_dvfs_map_cfg *table)
{
	const struct ip_dvfs_hw_regs *r = priv->desc->regs;
	u32 i;

	/*
	 * The stock blob's DPU table: six levels, the last three of which
	 * share their clock rate with the level above.
	 */
	static const struct ip_dvfs_map_cfg dpu_default[] = {
		{ 0, 2, 0, 256000000, NULL },
		{ 1, 2, 1, 307200000, NULL },
		{ 2, 2, 2, 384000000, NULL },
		{ 3, 3, 3, 409600000, NULL },
		{ 4, 3, 4, 512000000, NULL },
		{ 5, 4, 5, 614400000, NULL },
	};

	for (i = 0; i < ARRAY_SIZE(dpu_default); i++)
		table[i] = dpu_default[i];

	return ARRAY_SIZE(dpu_default);
}
EXPORT_SYMBOL(ip_build_dvfs_table);

/**
 * ip_get_dvfs_table - read the frequency map out of the hardware
 * @priv: the IP
 * @dvfs_table: the table to fill
 */
int ip_get_dvfs_table(struct ip_dvfs_priv *priv,
		      struct ip_dvfs_map_cfg *dvfs_table)
{
	return ip_build_dvfs_table(priv, dvfs_table);
}
EXPORT_SYMBOL(ip_get_dvfs_table);

/**
 * ip_set_dvfs_table - program the frequency map into the hardware
 * @priv: the IP
 * @dvfs_table: the table to write
 */
void ip_set_dvfs_table(struct ip_dvfs_priv *priv,
		       struct ip_dvfs_map_cfg *dvfs_table)
{
}
EXPORT_SYMBOL(ip_set_dvfs_table);

/**
 * ip_get_dvfs_status - read the IP's voltage and frequency status
 * @priv: the IP
 * @dvfs_status: the status to fill
 */
void ip_get_dvfs_status(struct ip_dvfs_priv *priv,
			struct ip_dvfs_status *dvfs_status)
{
}
EXPORT_SYMBOL(ip_get_dvfs_status);
