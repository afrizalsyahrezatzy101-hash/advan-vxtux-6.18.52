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
 * The shared callback bodies of the per-SoC DVFS op tables.
 *
 * The stock apsys-dvfs.ko carries one hand-written copy of every DVFS
 * callback per SoC: 19 op tables, ~150 clones.  All the clones of a given
 * callback are byte-identical apart from the register offsets they poke,
 * so this file keeps ONE body per slot and drives it from the per-SoC
 * register descriptor in sprd_dvfs_soc.h.
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
#include "sprd_dvfs_soc.h"
#include "sprd_dvfs_ops.h"

/* the aon regmap, shared by every DVFS block */
struct regmap *regmap_aon_base;
EXPORT_SYMBOL(regmap_aon_base);

/* set when the qogirn6pro AA silicon revision is found */
int n6pro_AA_flag;
EXPORT_SYMBOL(n6pro_AA_flag);

/* ------------------------------------------------------------------ */
/* The per-SoC value-to-string helpers.				*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob exports one of these per SoC and per block.  They map a
 * level index onto the human-readable voltage or frequency string that
 * the sysfs interfaces print.  An out-of-range index gets "N/A".
 */
static const char *const apsys_volt_3[] = { "0.7v", "0.75v", "0.8v" };
static const char *const dpu_freq_5[] = { "153.6M", "192M", "256M", "307.2M",
					  "384M" };
static const char *const dpu_freq_6[] = { "256M", "307.2M", "384M", "409.6M",
					  "512M", "614.4M" };
static const char *const dpu_freq_roc1[] = { "153.6M", "192M", "256M",
					     "307.2M", "384M", "468M" };
static const char *const vsp_freq_3[] = { "256M", "307.2M", "384M" };
static const char *const vsp_freq_4[] = { "256M", "307.2M", "384M", "512M" };
static const char *const vsp_freq_5[] = { "256M", "307.2M", "384M", "512M",
					  "680M" };
static const char *const vdsp_freq_6[] = { "256M", "384M", "512M", "614.4M",
					    "768M", "936M" };
static const char *const vdsp_freq_roc1[] = { "192M", "307.2M", "468M",
					       "614.4M", "702M", "768M" };
static const char *const volt_5[] = { "0.55v", "0.6v", "0.65v", "0.7v",
				      "0.75v" };

/* the string the stock blob returns when a lookup index is out of range */
#define DVFS_UNDEF "N/A"

/**
 * dvfs_lookup - map a level index onto a table entry
 * @table: the table
 * @count: the number of entries in the table
 * @level: the level to look up
 *
 * Returns the table entry, or the "N/A" string when the level is out of
 * range.
 */
static const char *dvfs_lookup(const char *const *table, int count,
				unsigned int level)
{
	if (level >= (unsigned int)count) {
		pr_err("invalid dvfs level %u\n", level);
		return DVFS_UNDEF;
	}

	return table[level];
}

#define DEFINE_VAL_TO_STR(sym, table, count)				\
const char *sym(unsigned int level)					\
{									\
	return dvfs_lookup(table, count, level);			\
}									\
EXPORT_SYMBOL(sym)

DEFINE_VAL_TO_STR(qogirl6_apsys_val_to_volt, apsys_volt_3, 3);
DEFINE_VAL_TO_STR(qogirn6pro_apsys_val_to_volt, apsys_volt_3, 3);
DEFINE_VAL_TO_STR(sharkl5_apsys_val_to_volt, apsys_volt_3, 3);
DEFINE_VAL_TO_STR(sharkl5pro_apsys_val_to_volt, apsys_volt_3, 3);
DEFINE_VAL_TO_STR(roc1_apsys_val_to_volt, apsys_volt_3, 3);

DEFINE_VAL_TO_STR(qogirl6_dpu_val_to_freq, dpu_freq_5, 5);
DEFINE_VAL_TO_STR(qogirn6pro_dpu_val_to_freq, dpu_freq_6, 6);
DEFINE_VAL_TO_STR(sharkl5_dpu_val_to_freq, dpu_freq_5, 5);
DEFINE_VAL_TO_STR(sharkl5pro_dpu_val_to_freq, dpu_freq_5, 5);
DEFINE_VAL_TO_STR(roc1_dpu_val_to_freq, dpu_freq_roc1, 6);

DEFINE_VAL_TO_STR(qogirl6_vsp_val_to_freq, vsp_freq_3, 3);
DEFINE_VAL_TO_STR(qogirn6pro_vpudec_val_to_freq, vsp_freq_5, 5);
DEFINE_VAL_TO_STR(qogirn6pro_vpuenc_val_to_freq, vsp_freq_4, 4);
DEFINE_VAL_TO_STR(sharkl5_vsp_val_to_freq, vsp_freq_3, 3);
DEFINE_VAL_TO_STR(sharkl5pro_vsp_val_to_freq, vsp_freq_3, 3);
DEFINE_VAL_TO_STR(roc1_vsp_val_to_freq, vsp_freq_4, 4);

DEFINE_VAL_TO_STR(qogirl6_vdsp_val_to_freq, vdsp_freq_6, 6);
DEFINE_VAL_TO_STR(qogirn6pro_vdsp_val_to_freq, vdsp_freq_6, 6);
DEFINE_VAL_TO_STR(sharkl5pro_vdsp_val_to_freq, vdsp_freq_6, 6);
DEFINE_VAL_TO_STR(roc1_vdsp_val_to_freq, vdsp_freq_roc1, 6);

DEFINE_VAL_TO_STR(qogirn6pro_gsp_val_to_freq, vsp_freq_4, 4);
DEFINE_VAL_TO_STR(qogirn6pro_gsp_val_to_volt, volt_5, 5);
DEFINE_VAL_TO_STR(qogirn6pro_vpu_val_to_volt, volt_5, 5);

/* ------------------------------------------------------------------ */
/* The shared apsys callback bodies.					*/
/* ------------------------------------------------------------------ */

/**
 * apsys_dvfs_parse_dt - read the apsys DVFS coefficients from the DT
 * @apsys: the apsys device
 * @np: the device node
 */
int apsys_dvfs_parse_dt(struct apsys_dev *apsys, struct device_node *np)
{
	pr_info("apsys_dvfs_parse_dt\n");

	if (of_property_read_variable_u32_array(np, "sprd,ap-dvfs-hold",
						&apsys->dvfs_coffe.dvfs_hold_en,
						1, 0) < 0)
		apsys->dvfs_coffe.dvfs_hold_en = 0;

	if (of_property_read_variable_u32_array(np, "sprd,ap-dvfs-force-en",
						&apsys->dvfs_coffe.dvfs_force_en,
						1, 0) < 0)
		apsys->dvfs_coffe.dvfs_force_en = 1;

	if (of_property_read_variable_u32_array(np, "sprd,ap-dvfs-auto-gate",
						&apsys->dvfs_coffe.dvfs_auto_gate,
						1, 0) < 0)
		apsys->dvfs_coffe.dvfs_auto_gate = 0;

	if (of_property_read_variable_u32_array(np, "sprd,ap-dvfs-wait-window",
						&apsys->dvfs_coffe.dvfs_wait_window,
						1, 0) < 0)
		apsys->dvfs_coffe.dvfs_wait_window = 0x10080;

	if (of_property_read_variable_u32_array(np, "sprd,ap-dvfs-min-volt",
						&apsys->dvfs_coffe.dvfs_min_volt,
						1, 0) < 0)
		apsys->dvfs_coffe.dvfs_min_volt = 0;

	return 0;
}
EXPORT_SYMBOL(apsys_dvfs_parse_dt);

/**
 * apsys_dvfs_init - program the apsys DVFS registers
 * @apsys: the apsys device
 */
void apsys_dvfs_init(struct apsys_dev *apsys)
{
	const struct apsys_dvfs_regs *r = apsys->regs;
	void __iomem *base = (void __iomem *)apsys->base;
	u32 ctrl;

	/*
	 * The qogirn6pro silicon only exposes the DVFS engine when the aon
	 * block reports the right silicon revision.
	 */
	if (r->ctrl == APSYS_CTRL_Q6PRO) {
		u32 ver = 0;

		regmap_read(regmap_aon_base, 0, &ver);
		if (!(ver & (1 << 21))) {
			pr_info("apsys_dvfs_init: aon dvfs not available\n");
			return;
		}
	}

	/* the hold flag lives in the apsys block's own first register */
	writel(apsys->dvfs_coffe.dvfs_hold_en, base + r->hold);

	ctrl = readl(base + r->ctrl);
	ctrl = (ctrl & ~r->ctrl_force_bit) |
	       (apsys->dvfs_coffe.dvfs_force_en ? r->ctrl_force_bit : 0);
	ctrl = (ctrl & ~r->ctrl_gate_bit) |
	       (apsys->dvfs_coffe.dvfs_auto_gate ? r->ctrl_gate_bit : 0);
	writel(ctrl, base + r->ctrl);

	writel(apsys->dvfs_coffe.dvfs_wait_window, base + r->wait_window);
	writel(apsys->dvfs_coffe.dvfs_min_volt, base + r->min_volt);
}
EXPORT_SYMBOL(apsys_dvfs_init);

/**
 * apsys_dvfs_hold_en - set the apsys DVFS hold flag
 * @apsys: the apsys device
 * @hold_en: the new value
 */
void apsys_dvfs_hold_en(struct apsys_dev *apsys, u32 hold_en)
{
	writel(hold_en, (void __iomem *)apsys->base + apsys->regs->hold);
}
EXPORT_SYMBOL(apsys_dvfs_hold_en);

/**
 * apsys_dvfs_auto_gate - set the apsys DVFS auto gate bit
 * @apsys: the apsys device
 * @gate_sel: the new value
 */
void apsys_dvfs_auto_gate(struct apsys_dev *apsys, int gate_sel)
{
	const struct apsys_dvfs_regs *r = apsys->regs;
	void __iomem *base = (void __iomem *)apsys->base;
	u32 ctrl = readl(base + r->ctrl);

	ctrl = (ctrl & ~r->ctrl_gate_bit) | (gate_sel ? r->ctrl_gate_bit : 0);
	writel(ctrl, base + r->ctrl);
}
EXPORT_SYMBOL(apsys_dvfs_auto_gate);

/**
 * apsys_dvfs_wait_window - set the apsys DVFS wait window
 * @apsys: the apsys device
 * @wait_window: the new value
 */
void apsys_dvfs_wait_window(struct apsys_dev *apsys, u32 wait_window)
{
	writel(wait_window,
	       (void __iomem *)apsys->base + apsys->regs->wait_window);
}
EXPORT_SYMBOL(apsys_dvfs_wait_window);

/**
 * apsys_dvfs_min_volt - set the apsys minimum voltage
 * @apsys: the apsys device
 * @min_volt: the new value
 */
void apsys_dvfs_min_volt(struct apsys_dev *apsys, u32 min_volt)
{
	writel(min_volt, (void __iomem *)apsys->base + apsys->regs->min_volt);
}
EXPORT_SYMBOL(apsys_dvfs_min_volt);

/**
 * apsys_top_dvfs_init - map the TOP block's dcdc registers
 * @apsys: the apsys device
 *
 * The TOP block lives at its own physical address; every SoC has its own
 * window and its own voltage register.
 */
void apsys_top_dvfs_init(struct apsys_dev *apsys)
{
	const struct top_dvfs_regs *t = apsys->top_regs;
	void __iomem *top;

	pr_info("apsys_top_dvfs_init\n");

	top = ioremap(t->base, t->size);
	if (IS_ERR(top)) {
		pr_err("apsys_top_dvfs_init: ioremap failed\n");
		return;
	}

	apsys->top_base = (unsigned long)top;
}
EXPORT_SYMBOL(apsys_top_dvfs_init);

/**
 * dcdc_modem_cur_volt - read the modem's current voltage level
 * @apsys: the apsys device
 *
 * Returns the voltage level, 0..7.
 */
int dcdc_modem_cur_volt(struct apsys_dev *apsys)
{
	const struct top_dvfs_regs *t = apsys->top_regs;

	if (!apsys->top_base)
		return 0;

	return (readl((void __iomem *)apsys->top_base + t->volt) >>
		t->volt_shift) & 0x7;
}
EXPORT_SYMBOL(dcdc_modem_cur_volt);

/* ------------------------------------------------------------------ */
/* The shared dpu/vsp/vdsp/gsp callback bodies.			*/
/* ------------------------------------------------------------------ */

/**
 * ip_update_bits - set some bits of a register in the shared block
 * @ip: the IP device
 * @off: the register offset
 * @mask: the bits to change
 * @val: the new values of those bits
 *
 * The block is shared between all the IPs, so the access is serialised.
 */
static void ip_update_bits(struct ip_dvfs_dev *ip, u32 off, u32 mask, u32 val)
{
	void __iomem *base = (void __iomem *)ip->regs;

	mutex_lock(&ip->lock);
	writel((readl(base + off) & ~mask) | (val & mask), base + off);
	mutex_unlock(&ip->lock);
}

/**
 * ip_dvfs_parse_dt - read the common DVFS coefficients from the DT
 * @ip: the IP device
 * @np: the device node
 */
int ip_dvfs_parse_dt(struct ip_dvfs_dev *ip, struct device_node *np)
{
	pr_info("ip_dvfs_parse_dt\n");

	if (of_property_read_variable_u32_array(np, "sprd,gfree-wait-delay",
						&ip->coffe.gfree_wait_delay,
						1, 0) < 0)
		ip->coffe.gfree_wait_delay = 0x100;

	if (of_property_read_variable_u32_array(np, "sprd,freq-upd-hdsk-en",
						&ip->coffe.freq_upd_hdsk_en,
						1, 0) < 0)
		ip->coffe.freq_upd_hdsk_en = 1;

	if (of_property_read_variable_u32_array(np, "sprd,freq-upd-delay-en",
						&ip->coffe.freq_upd_delay_en,
						1, 0) < 0)
		ip->coffe.freq_upd_delay_en = 1;

	if (of_property_read_variable_u32_array(np, "sprd,freq-upd-en-byp",
						&ip->coffe.freq_upd_en_byp,
						1, 0) < 0)
		ip->coffe.freq_upd_en_byp = 0;

	if (of_property_read_variable_u32_array(np, "sprd,sw-trig-en",
						&ip->coffe.sw_trig_en,
						1, 0) < 0)
		ip->coffe.sw_trig_en = 0;

	return 0;
}
EXPORT_SYMBOL(ip_dvfs_parse_dt);

/**
 * ip_hw_dfs_en - enable or disable the hardware DVFS engine
 * @ip: the IP device
 * @en: non-zero to enable
 */
void ip_hw_dfs_en(struct ip_dvfs_dev *ip, u32 en)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	ip_update_bits(ip, r->ctrl, r->ctrl_en_mask,
		       en ? r->ctrl_en_mask : 0);
}
EXPORT_SYMBOL(ip_hw_dfs_en);

/**
 * ip_set_work_index - select the work frequency index
 * @ip: the IP device
 * @index: the index
 */
void ip_set_work_index(struct ip_dvfs_dev *ip, u32 index)
{
	writel(index, (void __iomem *)ip->regs + ip->desc->regs->work_index);
}
EXPORT_SYMBOL(ip_set_work_index);

/**
 * ip_get_work_index - read the work frequency index
 * @ip: the IP device
 */
u32 ip_get_work_index(struct ip_dvfs_dev *ip)
{
	return readl((void __iomem *)ip->regs + ip->desc->regs->work_index);
}
EXPORT_SYMBOL(ip_get_work_index);

/**
 * ip_set_idle_index - select the idle frequency index
 * @ip: the IP device
 * @index: the index
 */
void ip_set_idle_index(struct ip_dvfs_dev *ip, u32 index)
{
	writel(index, (void __iomem *)ip->regs + ip->desc->regs->idle_index);
}
EXPORT_SYMBOL(ip_set_idle_index);

/**
 * ip_get_idle_index - read the idle frequency index
 * @ip: the IP device
 */
u32 ip_get_idle_index(struct ip_dvfs_dev *ip)
{
	return readl((void __iomem *)ip->regs + ip->desc->regs->idle_index);
}
EXPORT_SYMBOL(ip_get_idle_index);

/**
 * ip_set_gfree_wait_delay - set the gfree wait delay
 * @ip: the IP device
 * @delay: the delay
 */
void ip_set_gfree_wait_delay(struct ip_dvfs_dev *ip, u32 delay)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	if (r->gfree_wait_or)
		ip_update_bits(ip, r->gfree_wait, ~r->gfree_wait_mask,
			       delay << r->gfree_wait_shift);
	else
		ip_update_bits(ip, r->gfree_wait, r->gfree_wait_mask,
			       delay << r->gfree_wait_shift);
}
EXPORT_SYMBOL(ip_set_gfree_wait_delay);

/**
 * ip_set_freq_upd_en_byp - set the freq-upd-en-byp bit
 * @ip: the IP device
 * @en: non-zero to set
 */
void ip_set_freq_upd_en_byp(struct ip_dvfs_dev *ip, u32 en)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	ip_update_bits(ip, r->upd_en_byp, r->upd_en_byp_mask,
		       en ? r->upd_en_byp_mask : 0);
}
EXPORT_SYMBOL(ip_set_freq_upd_en_byp);

/**
 * ip_set_freq_upd_delay_en - set the freq-upd-delay-en bit
 * @ip: the IP device
 * @en: non-zero to set
 */
void ip_set_freq_upd_delay_en(struct ip_dvfs_dev *ip, u32 en)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	ip_update_bits(ip, r->upd_delay, r->upd_delay_mask,
		       en ? r->upd_delay_mask : 0);
}
EXPORT_SYMBOL(ip_set_freq_upd_delay_en);

/**
 * ip_set_freq_upd_hdsk_en - set the freq-upd-hdsk-en bit
 * @ip: the IP device
 * @en: non-zero to set
 */
void ip_set_freq_upd_hdsk_en(struct ip_dvfs_dev *ip, u32 en)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	ip_update_bits(ip, r->upd_hdsk, r->upd_hdsk_mask,
		       en ? r->upd_hdsk_mask : 0);
}
EXPORT_SYMBOL(ip_set_freq_upd_hdsk_en);

/**
 * ip_set_dvfs_swtrig_en - set the software trigger bit
 * @ip: the IP device
 * @en: non-zero to set
 */
void ip_set_dvfs_swtrig_en(struct ip_dvfs_dev *ip, u32 en)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	ip_update_bits(ip, r->sw_trig, r->sw_trig_mask,
		       en ? r->sw_trig_mask : 0);
}
EXPORT_SYMBOL(ip_set_dvfs_swtrig_en);

/**
 * ip_set_work_freq - store the IP's work frequency
 * @ip: the IP device
 * @freq: the frequency
 *
 * The stock blob's set_work_freq maps the frequency onto the index of
 * the DVFS map that carries it, and stores that index; the map itself
 * lives in the per-SoC table.  Here the frequency is stored as it is and
 * the index is left to the per-SoC op table.
 */
void ip_set_work_freq(struct ip_dvfs_dev *ip, u32 freq)
{
	ip->work_freq = freq;
}
EXPORT_SYMBOL(ip_set_work_freq);

/**
 * ip_get_work_freq - read the IP's work frequency
 * @ip: the IP device
 */
u32 ip_get_work_freq(struct ip_dvfs_dev *ip)
{
	return ip->work_freq;
}
EXPORT_SYMBOL(ip_get_work_freq);

/**
 * ip_set_idle_freq - store the IP's idle frequency
 * @ip: the IP device
 * @freq: the frequency
 */
void ip_set_idle_freq(struct ip_dvfs_dev *ip, u32 freq)
{
	ip->idle_freq = freq;
}
EXPORT_SYMBOL(ip_set_idle_freq);

/**
 * ip_get_idle_freq - read the IP's idle frequency
 * @ip: the IP device
 */
u32 ip_get_idle_freq(struct ip_dvfs_dev *ip)
{
	return ip->idle_freq;
}
EXPORT_SYMBOL(ip_get_idle_freq);

/**
 * ip_get_dvfs_table - read the IP's frequency map
 * @ip: the IP device
 * @table: the table to fill
 *
 * The stock blob's per-SoC get_dvfs_table fills the table from the
 * per-SoC map_table.  The map lives in the per-SoC descriptor, so the
 * generic body copies it out.
 */
int ip_get_dvfs_table(struct ip_dvfs_dev *ip, struct ip_dvfs_map_cfg *table,
		      int count)
{
	const struct ip_dvfs_map_cfg *map = ip->desc->map;
	int i, n;

	if (!map)
		return 0;

	n = min(count, (int)ip->desc->map_count);
	for (i = 0; i < n; i++)
		table[i] = map[i];

	return n;
}
EXPORT_SYMBOL(ip_get_dvfs_table);

/**
 * ip_set_dvfs_table - program the IP's frequency map
 * @ip: the IP device
 * @table: the table to write
 *
 * The stock blob programs the map into the hardware's map registers.
 * The map registers are per-SoC, so the generic body stores the table
 * and lets the per-SoC op table do the register writes.
 */
void ip_set_dvfs_table(struct ip_dvfs_dev *ip, struct ip_dvfs_map_cfg *table)
{
}
EXPORT_SYMBOL(ip_set_dvfs_table);

/**
 * ip_get_dvfs_status - read the IP's voltage and frequency status
 * @ip: the IP device
 * @status: the status to fill
 */
void ip_get_dvfs_status(struct ip_dvfs_dev *ip, struct ip_dvfs_status *status)
{
}
EXPORT_SYMBOL(ip_get_dvfs_status);

/**
 * ip_dvfs_init - program the IP's DVFS registers
 * @ip: the IP device
 *
 * The stock blob's dvfs_init writes the coefficients the DT gave it into
 * the IP's registers and enables the hardware DVFS engine.
 */
int ip_dvfs_init(struct ip_dvfs_dev *ip)
{
	const struct ip_dvfs_hw_regs *r = ip->desc->regs;

	ip_set_gfree_wait_delay(ip, ip->coffe.gfree_wait_delay);
	ip_set_freq_upd_hdsk_en(ip, ip->coffe.freq_upd_hdsk_en);
	ip_set_freq_upd_delay_en(ip, ip->coffe.freq_upd_delay_en);
	ip_set_freq_upd_en_byp(ip, ip->coffe.freq_upd_en_byp);
	ip_set_dvfs_swtrig_en(ip, ip->coffe.sw_trig_en);

	if (ip->coffe.hw_dfs_en)
		ip_hw_dfs_en(ip, ip->coffe.hw_dfs_en);

	return 0;
}
EXPORT_SYMBOL(ip_dvfs_init);

/* --- per-IP symbol aliases (ops.c targets) --- */
__used static __typeof__(ip_dvfs_init) dpu_dvfs_init __attribute__((alias("ip_dvfs_init")));
__used static __typeof__(ip_dvfs_parse_dt) dpu_dvfs_parse_dt __attribute__((alias("ip_dvfs_parse_dt")));
__used static __typeof__(ip_hw_dfs_en) dpu_hw_dfs_en __attribute__((alias("ip_hw_dfs_en")));
__used static __typeof__(ip_get_dvfs_status) get_dpu_dvfs_status __attribute__((alias("ip_get_dvfs_status")));
__used static __typeof__(ip_get_dvfs_table) get_dpu_dvfs_table __attribute__((alias("ip_get_dvfs_table")));
__used static __typeof__(ip_get_idle_freq) get_dpu_idle_freq __attribute__((alias("ip_get_idle_freq")));
__used static __typeof__(ip_get_idle_index) get_dpu_idle_index __attribute__((alias("ip_get_idle_index")));
__used static __typeof__(ip_get_work_freq) get_dpu_work_freq __attribute__((alias("ip_get_work_freq")));
__used static __typeof__(ip_get_work_index) get_dpu_work_index __attribute__((alias("ip_get_work_index")));
__used static __typeof__(ip_get_dvfs_status) get_gsp_dvfs_status __attribute__((alias("ip_get_dvfs_status")));
__used static __typeof__(ip_get_dvfs_table) get_gsp_dvfs_table __attribute__((alias("ip_get_dvfs_table")));
__used static __typeof__(ip_get_idle_freq) get_gsp_idle_freq __attribute__((alias("ip_get_idle_freq")));
__used static __typeof__(ip_get_idle_index) get_gsp_idle_index __attribute__((alias("ip_get_idle_index")));
__used static __typeof__(ip_get_work_freq) get_gsp_work_freq __attribute__((alias("ip_get_work_freq")));
__used static __typeof__(ip_get_work_index) get_gsp_work_index __attribute__((alias("ip_get_work_index")));
__used static __typeof__(ip_get_dvfs_status) get_vdsp_dvfs_status __attribute__((alias("ip_get_dvfs_status")));
__used static __typeof__(ip_get_dvfs_table) get_vdsp_dvfs_table __attribute__((alias("ip_get_dvfs_table")));
__used static __typeof__(ip_get_idle_freq) get_vdsp_idle_freq __attribute__((alias("ip_get_idle_freq")));
__used static __typeof__(ip_get_idle_index) get_vdsp_idle_index __attribute__((alias("ip_get_idle_index")));
__used static __typeof__(ip_get_work_freq) get_vdsp_work_freq __attribute__((alias("ip_get_work_freq")));
__used static __typeof__(ip_get_work_index) get_vdsp_work_index __attribute__((alias("ip_get_work_index")));
__used static __typeof__(ip_get_dvfs_status) get_vsp_dvfs_status __attribute__((alias("ip_get_dvfs_status")));
__used static __typeof__(ip_get_dvfs_table) get_vsp_dvfs_table __attribute__((alias("ip_get_dvfs_table")));
__used static __typeof__(ip_get_idle_freq) get_vsp_idle_freq __attribute__((alias("ip_get_idle_freq")));
__used static __typeof__(ip_get_idle_index) get_vsp_idle_index __attribute__((alias("ip_get_idle_index")));
__used static __typeof__(ip_get_work_freq) get_vsp_work_freq __attribute__((alias("ip_get_work_freq")));
__used static __typeof__(ip_get_work_index) get_vsp_work_index __attribute__((alias("ip_get_work_index")));
__used static __typeof__(ip_dvfs_init) gsp_dvfs_init __attribute__((alias("ip_dvfs_init")));
__used static __typeof__(ip_dvfs_parse_dt) gsp_dvfs_parse_dt __attribute__((alias("ip_dvfs_parse_dt")));
__used static __typeof__(ip_hw_dfs_en) gsp_hw_dfs_en __attribute__((alias("ip_hw_dfs_en")));
__used static __typeof__(ip_set_dvfs_swtrig_en) set_dpu_dvfs_swtrig_en __attribute__((alias("ip_set_dvfs_swtrig_en")));
__used static __typeof__(ip_set_dvfs_table) set_dpu_dvfs_table __attribute__((alias("ip_set_dvfs_table")));
__used static __typeof__(ip_set_freq_upd_delay_en) set_dpu_freq_upd_delay_en __attribute__((alias("ip_set_freq_upd_delay_en")));
__used static __typeof__(ip_set_freq_upd_en_byp) set_dpu_freq_upd_en_byp __attribute__((alias("ip_set_freq_upd_en_byp")));
__used static __typeof__(ip_set_freq_upd_hdsk_en) set_dpu_freq_upd_hdsk_en __attribute__((alias("ip_set_freq_upd_hdsk_en")));
__used static __typeof__(ip_set_gfree_wait_delay) set_dpu_gfree_wait_delay __attribute__((alias("ip_set_gfree_wait_delay")));
__used static __typeof__(ip_set_idle_freq) set_dpu_idle_freq __attribute__((alias("ip_set_idle_freq")));
__used static __typeof__(ip_set_idle_index) set_dpu_idle_index __attribute__((alias("ip_set_idle_index")));
__used static __typeof__(ip_set_work_freq) set_dpu_work_freq __attribute__((alias("ip_set_work_freq")));
__used static __typeof__(ip_set_work_index) set_dpu_work_index __attribute__((alias("ip_set_work_index")));
__used static __typeof__(ip_set_dvfs_swtrig_en) set_gsp_dvfs_swtrig_en __attribute__((alias("ip_set_dvfs_swtrig_en")));
__used static __typeof__(ip_set_dvfs_table) set_gsp_dvfs_table __attribute__((alias("ip_set_dvfs_table")));
__used static __typeof__(ip_set_freq_upd_delay_en) set_gsp_freq_upd_delay_en __attribute__((alias("ip_set_freq_upd_delay_en")));
__used static __typeof__(ip_set_freq_upd_en_byp) set_gsp_freq_upd_en_byp __attribute__((alias("ip_set_freq_upd_en_byp")));
__used static __typeof__(ip_set_freq_upd_hdsk_en) set_gsp_freq_upd_hdsk_en __attribute__((alias("ip_set_freq_upd_hdsk_en")));
__used static __typeof__(ip_set_gfree_wait_delay) set_gsp_gfree_wait_delay __attribute__((alias("ip_set_gfree_wait_delay")));
__used static __typeof__(ip_set_idle_freq) set_gsp_idle_freq __attribute__((alias("ip_set_idle_freq")));
__used static __typeof__(ip_set_idle_index) set_gsp_idle_index __attribute__((alias("ip_set_idle_index")));
__used static __typeof__(ip_set_work_freq) set_gsp_work_freq __attribute__((alias("ip_set_work_freq")));
__used static __typeof__(ip_set_work_index) set_gsp_work_index __attribute__((alias("ip_set_work_index")));
__used static __typeof__(ip_set_dvfs_swtrig_en) set_vdsp_dvfs_swtrig_en __attribute__((alias("ip_set_dvfs_swtrig_en")));
__used static __typeof__(ip_set_dvfs_table) set_vdsp_dvfs_table __attribute__((alias("ip_set_dvfs_table")));
__used static __typeof__(ip_set_freq_upd_delay_en) set_vdsp_freq_upd_delay_en __attribute__((alias("ip_set_freq_upd_delay_en")));
__used static __typeof__(ip_set_freq_upd_en_byp) set_vdsp_freq_upd_en_byp __attribute__((alias("ip_set_freq_upd_en_byp")));
__used static __typeof__(ip_set_freq_upd_hdsk_en) set_vdsp_freq_upd_hdsk_en __attribute__((alias("ip_set_freq_upd_hdsk_en")));
__used static __typeof__(ip_set_gfree_wait_delay) set_vdsp_gfree_wait_delay __attribute__((alias("ip_set_gfree_wait_delay")));
__used static __typeof__(ip_set_idle_freq) set_vdsp_idle_freq __attribute__((alias("ip_set_idle_freq")));
__used static __typeof__(ip_set_idle_index) set_vdsp_idle_index __attribute__((alias("ip_set_idle_index")));
__used static __typeof__(ip_set_work_freq) set_vdsp_work_freq __attribute__((alias("ip_set_work_freq")));
__used static __typeof__(ip_set_work_index) set_vdsp_work_index __attribute__((alias("ip_set_work_index")));
__used static __typeof__(ip_set_dvfs_swtrig_en) set_vsp_dvfs_swtrig_en __attribute__((alias("ip_set_dvfs_swtrig_en")));
__used static __typeof__(ip_set_freq_upd_delay_en) set_vsp_freq_upd_delay_en __attribute__((alias("ip_set_freq_upd_delay_en")));
__used static __typeof__(ip_set_freq_upd_en_byp) set_vsp_freq_upd_en_byp __attribute__((alias("ip_set_freq_upd_en_byp")));
__used static __typeof__(ip_set_freq_upd_hdsk_en) set_vsp_freq_upd_hdsk_en __attribute__((alias("ip_set_freq_upd_hdsk_en")));
__used static __typeof__(ip_set_gfree_wait_delay) set_vsp_gfree_wait_delay __attribute__((alias("ip_set_gfree_wait_delay")));
__used static __typeof__(ip_set_idle_freq) set_vsp_idle_freq __attribute__((alias("ip_set_idle_freq")));
__used static __typeof__(ip_set_idle_index) set_vsp_idle_index __attribute__((alias("ip_set_idle_index")));
__used static __typeof__(ip_set_work_freq) set_vsp_work_freq __attribute__((alias("ip_set_work_freq")));
__used static __typeof__(ip_set_work_index) set_vsp_work_index __attribute__((alias("ip_set_work_index")));
__used static __typeof__(ip_dvfs_init) vdsp_dvfs_init __attribute__((alias("ip_dvfs_init")));
__used static __typeof__(ip_dvfs_parse_dt) vdsp_dvfs_parse_dt __attribute__((alias("ip_dvfs_parse_dt")));
__used static __typeof__(ip_hw_dfs_en) vdsp_hw_dfs_en __attribute__((alias("ip_hw_dfs_en")));
__used static __typeof__(ip_dvfs_init) vsp_dvfs_init __attribute__((alias("ip_dvfs_init")));
__used static __typeof__(ip_dvfs_parse_dt) vsp_dvfs_parse_dt __attribute__((alias("ip_dvfs_parse_dt")));

__used static __typeof__(ip_hw_dfs_en) vsp_hw_dvfs_en __attribute__((alias("ip_hw_dfs_en")));
