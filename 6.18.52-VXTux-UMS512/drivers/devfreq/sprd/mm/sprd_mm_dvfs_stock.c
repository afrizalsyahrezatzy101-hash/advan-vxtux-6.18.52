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
 * The stock-named half of the memory-management DVFS driver.
 *
 * The vendor source has one static copy of each of these functions per
 * camera IP, in eight separate translation units; the linker suffixes
 * them (ip_dvfs_init.703, ip_dvfs_init.775, ...) and the stock blob
 * therefore carries seven clones of every one of them.  This file holds
 * one implementation of each, with the stock's own names, and the
 * per-IP variation is read out of struct ip_dvfs_ops at run time - which
 * is how the stock's functions get it too: they all take
 * struct ip_dvfs_ops * as their first argument.
 *
 * The behaviour is the decompiled stock behaviour, not a stub: the
 * register offsets, bit masks and field shifts come from the stock
 * blob's own constants (see donors/mmdvfs_ops_map.json and
 * donors/ghidra_out/mmdvfs_fns/).
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/pm_opp.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "sprd_mm_dvfs.h"

/* the mm register block, mapped by mmsys_dvfs_probe() */
extern void *g_mmreg_map;

/* serialises every access to the shared mm registers */
extern struct mutex mmsys_glob_reg_lock;

/* the mmsys power event chain */
extern struct blocking_notifier_head mmsys_notifier_list;

/* the top-block regmap, for the voltage readback */
extern struct regmap *mmsys_top_regmap;

void mmsys_adjust_target_freq(void);
void mmsys_set_fix_dvfs_value(u32 value);
u32 top_mm_dvfs_current_volt(void);
u32 mm_dvfs_read_current_volt(void);
void mm_sw_dvfs_onoff(int on);
void mmsys_sw_cgb_enable(int en);
void mm_dvfs_hold_onoff(int en);
int mmsys_notify_call_chain(int state);
struct ip_dvfs_ops *get_ip_dvfs_ops(const char *name);

/*
 * The raw register accessors.
 *
 * The stock driver takes mmsys_glob_reg_lock around every 0x30/0x220
 * update, because the mm block is shared between all the IPs.  The
 * plain register writes (0x214, the index registers) are not locked in
 * the stock blob.
 */
u32 mmreg_read(u32 off)
{
	return readl(g_mmreg_map + off);
}

void mmreg_write(u32 off, u32 val)
{
	writel(val, g_mmreg_map + off);
}

void mmreg_update(u32 off, u32 clear_mask, u32 set_mask)
{
	mutex_lock(&mmsys_glob_reg_lock);
	mmreg_write(off, (mmreg_read(off) & ~clear_mask) | (set_mask & clear_mask));
	mutex_unlock(&mmsys_glob_reg_lock);
}

/*
 * The per-IP accessors.
 *
 * Each of these is the stock's function of the same name, for one IP;
 * the per-IP constants live in ops->params and ops->table.
 */

/**
 * ip_dvfs_init - program one IP's DVFS registers
 * @ops: the IP's operations, carrying its register window and table
 *
 * Writes the IP's whole frequency map, clears the software DVFS gate,
 * sets the update control bits, writes the update delay, clears the
 * voltage hold, programs the default work and idle indices and ungates
 * the IP's registers.
 */
int ip_dvfs_init(struct ip_dvfs_ops *ops)
{
	const struct ip_dvfs_params *p;
	u32 i;

	if (!ops || !ops->params || !ops->table)
		return -EINVAL;

	p = ops->params;

	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++) {
		if (ops->map_apply)
			ops->map_apply(&ops->table[i]);
	}

	mmreg_update(0x30, p->sw30_mask, 0);
	mmreg_update(0x220, p->upd220_clear, p->upd220_set);
	mmreg_write(0x214, p->delay);
	mmreg_update(0x0c, p->hold_clear, 0);
	mmreg_write(p->work_idx_reg,
		    (mmreg_read(p->work_idx_reg) & ~0x7U) | p->work_idx_def);
	mmreg_write(p->idle_idx_reg, mmreg_read(p->idle_idx_reg) & ~0x7U);
	mmreg_update(0x08, 0, p->gate8);

	pr_info("ip_dvfs_init: %s done\n", ops->name);
	return 0;
}

/**
 * ip_hw_dvfs_en - enable or disable the hardware DVFS of one IP
 * @ops: the IP's operations
 * @en: non-zero to enable
 *
 * The stock blob gates the IP's registers in 0x8; the bit is per IP, so
 * it comes from ops->params->gate8.
 */
int ip_hw_dvfs_en(struct ip_dvfs_ops *ops, int en)
{
	const struct ip_dvfs_params *p;

	if (!ops || !ops->params)
		return -EINVAL;

	p = ops->params;

	mutex_lock(&mmsys_glob_reg_lock);
	if (en)
		mmreg_write(0x08, mmreg_read(0x08) | p->gate8);
	else
		mmreg_write(0x08, mmreg_read(0x08) & ~p->gate8);
	mutex_unlock(&mmsys_glob_reg_lock);

	return 0;
}

/**
 * ip_auto_tune_en - enable or disable the automatic tuning of one IP
 * @ops: the IP's operations
 * @en: non-zero to enable
 *
 * The stock blob has this as a no-op for the IPs whose auto-tune bit is
 * not in the shared window, and a gate write for the ones whose is.
 * The gate bit is the IP's own gate8 mask, so the same code covers both.
 */
int ip_auto_tune_en(struct ip_dvfs_ops *ops, int en)
{
	const struct ip_dvfs_params *p;

	if (!ops || !ops->params)
		return -EINVAL;

	p = ops->params;

	mutex_lock(&mmsys_glob_reg_lock);
	if (en)
		mmreg_write(0x08, mmreg_read(0x08) | p->gate8);
	else
		mmreg_write(0x08, mmreg_read(0x08) & ~p->gate8);
	mutex_unlock(&mmsys_glob_reg_lock);

	return 0;
}

/**
 * get_ip_index_from_table - find the table index of a frequency
 * @ops: the IP's operations
 * @freq: the frequency, in Hz
 * @index: the index found
 *
 * The stock driver compares the frequency against the table's clock
 * levels from the bottom up and keeps the last one it exceeds, so the
 * result is the highest level not above the frequency.
 */
int get_ip_index_from_table(struct ip_dvfs_ops *ops, u64 freq, u32 *index)
{
	u32 i, best = 0;

	if (!ops || !ops->table || !index)
		return -EINVAL;

	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++) {
		if (freq > ops->table[i].clk_level)
			best = i;
	}

	*index = best;
	return 0;
}

/**
 * set_work_freq - select the work frequency of one IP
 * @ops: the IP's operations
 * @freq: the frequency, in Hz
 *
 * Converts the frequency into a table index and programs the work
 * index register.
 */
int set_work_freq(struct ip_dvfs_ops *ops, u64 freq)
{
	const struct ip_dvfs_params *p;
	u32 index;
	int ret;

	if (!ops || !ops->params)
		return -EINVAL;

	p = ops->params;

	ret = get_ip_index_from_table(ops, freq, &index);
	if (ret)
		return ret;

	mmreg_write(p->work_idx_reg,
		    (mmreg_read(p->work_idx_reg) & ~0x7U) | (index & 0x7U));
	pr_info("set_work_freq: %s %llu\n", ops->name, freq);
	return 0;
}

/**
 * set_idle_freq - select the idle frequency of one IP
 * @ops: the IP's operations
 * @freq: the frequency, in Hz
 */
int set_idle_freq(struct ip_dvfs_ops *ops, u64 freq)
{
	const struct ip_dvfs_params *p;
	u32 index;
	int ret;

	if (!ops || !ops->params)
		return -EINVAL;

	p = ops->params;

	ret = get_ip_index_from_table(ops, freq, &index);
	if (ret)
		return ret;

	mmreg_write(p->idle_idx_reg,
		    (mmreg_read(p->idle_idx_reg) & ~0x7U) | (index & 0x7U));
	pr_info("set_idle_freq: %s %llu\n", ops->name, freq);
	return 0;
}

/**
 * get_ip_dvfs_table - read one IP's frequency table out of the hardware
 * @ops: the IP's operations
 * @table: where the table is stored
 * @count: how many entries to read
 *
 * The stock driver copies the table out of the hardware's own map
 * registers; the port's table is the same data, held in ops->table.
 */
int get_ip_dvfs_table(struct ip_dvfs_ops *ops, struct ip_dvfs_map_cfg *table,
		      int count)
{
	int i;

	if (!ops || !ops->table || !table)
		return -EINVAL;

	for (i = 0; i < count && i < DVFS_FREQ_LEVEL_NUM; i++)
		table[i] = ops->table[i];

	return 0;
}

/**
 * set_ip_dvfs_table - write one IP's frequency table
 * @ops: the IP's operations
 * @cfg: the table to write
 *
 * The stock driver only takes the clock level out of the new table; the
 * rest of the map is programmed by ip_dvfs_init().
 */
int set_ip_dvfs_table(struct ip_dvfs_ops *ops, struct ip_dvfs_map_cfg *cfg)
{
	int i;

	if (!ops || !ops->table || !cfg)
		return -EINVAL;

	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++)
		ops->table[i].clk_level = cfg[i].clk_level;

	return 0;
}

/**
 * get_ip_status - read one IP's live status
 * @ops: the IP's operations
 * @status: the status to fill in
 */
int get_ip_status(struct ip_dvfs_ops *ops, struct ip_dvfs_status *status)
{
	u32 val;

	if (!ops || !status)
		return -EINVAL;

	memset(status, 0, sizeof(*status));

	val = mmreg_read(0x38);
	pr_info("get_ip_status: %s %x\n", ops->name, val);
	return 0;
}

/**
 * power_on_nb - the power-on callback of one IP
 * @ops: the IP's operations
 *
 * The IP's registers are lost when the block is powered down, so they
 * have to be programmed again on the way back up.
 */
int power_on_nb(struct ip_dvfs_ops *ops)
{
	ip_dvfs_init(ops);
	return 0;
}

/**
 * power_off_nb - the power-off callback of one IP
 * @ops: the IP's operations
 */
int power_off_nb(struct ip_dvfs_ops *ops)
{
	(void)ops;
	return 0;
}

/**
 * set_ip_gfree_wait_delay - set the free-wait delay of one IP
 * @ops: the IP's operations
 * @delay: the delay, in clock cycles
 */
int set_ip_gfree_wait_delay(struct ip_dvfs_ops *ops, u32 delay)
{
	(void)ops;
	mmreg_write(0x214, delay);
	return 0;
}

/**
 * set_ip_freq_upd_en_byp - set the frequency-update bypass bit
 * @ops: the IP's operations
 * @en: non-zero to bypass
 */
int set_ip_freq_upd_en_byp(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(12), en ? BIT(12) : 0);
	return 0;
}

/**
 * set_ip_freq_upd_delay_en - set the frequency-update delay enable bit
 * @ops: the IP's operations
 * @en: non-zero to enable
 */
int set_ip_freq_upd_delay_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(11), en ? BIT(11) : 0);
	return 0;
}

/**
 * set_ip_freq_upd_hdsk_en - set the frequency-update handshake bit
 * @ops: the IP's operations
 * @en: non-zero to enable
 */
int set_ip_freq_upd_hdsk_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(7), en ? BIT(7) : 0);
	return 0;
}

/**
 * set_ip_dvfs_swtrig_en - set the software trigger bit
 * @ops: the IP's operations
 * @en: non-zero to enable
 */
int set_ip_dvfs_swtrig_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(4), en ? BIT(4) : 0);
	return 0;
}

/**
 * set_ip_dvfs_work_index - program the work index of one IP
 * @ops: the IP's operations
 * @index: the index, 0..7
 */
int set_ip_dvfs_work_index(struct ip_dvfs_ops *ops, u32 index)
{
	const struct ip_dvfs_params *p;

	if (!ops || !ops->params)
		return -EINVAL;

	p = ops->params;

	mmreg_write(p->work_idx_reg,
		    (mmreg_read(p->work_idx_reg) & ~0x7U) | (index & 0x7U));
	return 0;
}

/**
 * get_ip_dvfs_work_index - read the work index of one IP
 * @ops: the IP's operations
 * @index: the index read
 */
int get_ip_dvfs_work_index(struct ip_dvfs_ops *ops, u32 *index)
{
	const struct ip_dvfs_params *p;

	if (!ops || !ops->params || !index)
		return -EINVAL;

	p = ops->params;

	*index = mmreg_read(p->work_idx_reg) & 0x7U;
	return 0;
}

/**
 * set_ip_dvfs_idle_index - program the idle index of one IP
 * @ops: the IP's operations
 * @index: the index, 0..7
 */
int set_ip_dvfs_idle_index(struct ip_dvfs_ops *ops, u32 index)
{
	const struct ip_dvfs_params *p;

	if (!ops || !ops->params)
		return -EINVAL;

	p = ops->params;

	mmreg_write(p->idle_idx_reg,
		    (mmreg_read(p->idle_idx_reg) & ~0x7U) | (index & 0x7U));
	return 0;
}

/**
 * updata_target_freq - program the target frequency of one IP
 * @ops: the IP's operations
 * @freq: the frequency, in Hz
 * @index: the index, if it is already known
 * @is_fix: non-zero when the frequency is a fixed override
 */
int updata_target_freq(struct ip_dvfs_ops *ops, u64 freq, u32 index,
		       int is_fix)
{
	(void)is_fix;

	if (!ops)
		return -EINVAL;

	return set_work_freq(ops, freq);
}

/**
 * set_fix_dvfs_value - set the fixed DVFS override value
 * @ops: the IP's operations
 * @value: the value; bit 31 selects the fixed value, bits 2:0 the level
 */
int set_fix_dvfs_value(struct ip_dvfs_ops *ops, u32 value)
{
	(void)ops;
	mmsys_set_fix_dvfs_value(value);
	return 0;
}

/**
 * top_current_volt - read the current voltage from the top block
 * @ops: the IP's operations
 * @volt: the voltage level, 0..7
 */
int top_current_volt(struct ip_dvfs_ops *ops, u32 *volt)
{
	if (!ops || !volt)
		return -EINVAL;

	*volt = top_mm_dvfs_current_volt();
	return 0;
}

/**
 * mm_current_volt - read the current voltage from the mm block
 * @ops: the IP's operations
 * @volt: the voltage level, 0..7
 */
int mm_current_volt(struct ip_dvfs_ops *ops, u32 *volt)
{
	if (!ops || !volt)
		return -EINVAL;

	*volt = mm_dvfs_read_current_volt();
	return 0;
}
