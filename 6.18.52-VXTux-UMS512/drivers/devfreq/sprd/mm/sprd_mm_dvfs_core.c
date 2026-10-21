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
 * The common core of the Spreadtrum camera memory-management DVFS driver.
 *
 * The stock vendor blob (mmdvfs.ko) implements one devfreq profile per
 * camera IP (mtx, isp, fd, cpp, jpg, dcam, dcam_axi, dcam_if) plus the
 * mmsys-level profile, all sharing a single mm register block.  Each IP
 * differs only in
 *   - the register window it owns inside the mm block,
 *   - its frequency/voltage table,
 *   - the name it registers under.
 *
 * This file holds everything that is genuinely shared: the ops registry,
 * the notifier chain, the register accessors and the devfreq profile
 * callbacks.  The per-IP tables and register windows live in
 * sprd_mm_dvfs_ip.c.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/delay.h>
#include <linux/devfreq.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/pm_opp.h>
#include <linux/regmap.h>
#include <linux/slab.h>

#include "sprd_mm_dvfs.h"

/* the mm register block, mapped by mmsys_dvfs_probe() */
void *g_mmreg_map;

/* serialises every access to the shared mm registers */
struct mutex mmsys_glob_reg_lock;

/* the registry of the per-IP operations */
struct list_head ip_dvfs_ops_list = LIST_HEAD_INIT(ip_dvfs_ops_list);
struct mutex ip_dvfsops_list_lock;

/* the mmsys power event chain */
/* 6.18 dropped BLOCKING_NOTIFIER_INIT(): the head is a plain list head
 * and the notifier chain is registered with
 * blocking_notifier_chain_register() at probe time.
 */
struct blocking_notifier_head mmsys_notifier_list;

/* set while the mmsys block is powered on */
int mm_power_flag;

/* the top-block regmap, for the voltage readback */
static struct regmap *mmsys_top_regmap;

/*
 * The mm register block accessors.  Every one of them takes
 * mmsys_glob_reg_lock, because the block is shared between all the IPs.
 */
static u32 mm_read(u32 off)
{
	return readl(g_mmreg_map + off);
}

static void mm_write(u32 off, u32 val)
{
	writel(val, g_mmreg_map + off);
}

static void mm_update_bits(u32 off, u32 mask, u32 val)
{
	mutex_lock(&mmsys_glob_reg_lock);
	mm_write(off, (mm_read(off) & ~mask) | (val & mask));
	mutex_unlock(&mmsys_glob_reg_lock);
}

/**
 * register_ip_dvfs_ops - add an IP's operations to the registry
 * @ops: the operations to register
 *
 * Returns 0 on success, -EINVAL if the IP is already registered.
 */
int register_ip_dvfs_ops(struct ip_dvfs_ops *ops)
{
	struct ip_dvfs_ops *pos;

	if (!ops || !ops->name[0])
		return -EINVAL;

	mutex_lock(&ip_dvfsops_list_lock);
	list_for_each_entry(pos, &ip_dvfs_ops_list, list) {
		if (!strcmp(pos->name, ops->name)) {
			mutex_unlock(&ip_dvfsops_list_lock);
			pr_err("dvfs ops: %s already registered\n", ops->name);
			return -EINVAL;
		}
	}
	list_add_tail(&ops->list, &ip_dvfs_ops_list);
	mutex_unlock(&ip_dvfsops_list_lock);

	pr_info("dvfs ops: %s\n", ops->name);
	return 0;
}
EXPORT_SYMBOL_GPL(register_ip_dvfs_ops);

/**
 * unregister_ip_dvfs_ops - remove an IP's operations from the registry
 * @ops: the operations to remove
 */
int unregister_ip_dvfs_ops(struct ip_dvfs_ops *ops)
{
	if (!ops)
		return -EINVAL;

	mutex_lock(&ip_dvfsops_list_lock);
	list_del(&ops->list);
	mutex_unlock(&ip_dvfsops_list_lock);

	return 0;
}
EXPORT_SYMBOL_GPL(unregister_ip_dvfs_ops);

/**
 * get_ip_dvfs_ops - look up an IP's operations by name
 * @name: the IP's name, e.g. "MTX_DVFS_OPS"
 *
 * Returns the operations, or NULL if no IP registered under that name.
 */
struct ip_dvfs_ops *get_ip_dvfs_ops(const char *name)
{
	struct ip_dvfs_ops *pos;

	if (!name)
		return NULL;

	mutex_lock(&ip_dvfsops_list_lock);
	list_for_each_entry(pos, &ip_dvfs_ops_list, list) {
		if (!strcmp(pos->name, name)) {
			mutex_unlock(&ip_dvfsops_list_lock);
			return pos;
		}
	}
	mutex_unlock(&ip_dvfsops_list_lock);

	return NULL;
}
EXPORT_SYMBOL_GPL(get_ip_dvfs_ops);

int mmsys_register_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&mmsys_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(mmsys_register_notifier);

int mmsys_unregister_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&mmsys_notifier_list, nb);
}
EXPORT_SYMBOL_GPL(mmsys_unregister_notifier);

int mmsys_notify_call_chain(int state)
{
	return blocking_notifier_call_chain(&mmsys_notifier_list, state, NULL);
}
EXPORT_SYMBOL_GPL(mmsys_notify_call_chain);

/*
 * The mmsys-level knobs.  These are the global switches that gate the
 * whole camera DVFS path.
 */
void mmsys_adjust_target_freq(void)
{
	pr_info("mmsys_adjust_target_freq\n");
}
EXPORT_SYMBOL_GPL(mmsys_adjust_target_freq);

void mmsys_set_fix_dvfs_value(u32 value)
{
	/* bit 31 selects the fixed value, bits 2:0 the level */
	if (mmsys_top_regmap)
		regmap_update_bits(mmsys_top_regmap, 0,
				   BIT(31) | 0x7,
				   (value >> 31) | ((value & 0x7) << 1));
	pr_info("mmsys_set_fix_dvfs_value\n");
}
EXPORT_SYMBOL_GPL(mmsys_set_fix_dvfs_value);

/**
 * top_mm_dvfs_current_volt - read the current voltage from the top block
 *
 * Returns the voltage level, 0..7, or 0 if the read failed.
 */
u32 top_mm_dvfs_current_volt(void)
{
	u32 val = 0;
	int ret;

	msleep(1);
	if (mmsys_top_regmap) {
		ret = regmap_read(mmsys_top_regmap, 0x20, &val);
		if (ret) {
			pr_err("top_mm_dvfs_current_volt: read failed\n");
			return 0;
		}
	}
	pr_info("top_mm_dvfs_current_volt\n");
	return (val >> 20) & 0x7;
}
EXPORT_SYMBOL_GPL(top_mm_dvfs_current_volt);

/**
 * mm_dvfs_read_current_volt - read the current voltage from the mm block
 *
 * Returns the voltage level, 0..7.
 */
u32 mm_dvfs_read_current_volt(void)
{
	u32 val;

	msleep(1);
	val = mm_read(0x38);
	pr_info("mm_dvfs_read_current_volt\n");
	return (val >> 24) & 0x7;
}
EXPORT_SYMBOL_GPL(mm_dvfs_read_current_volt);

void mm_sw_dvfs_onoff(int on)
{
	mm_update_bits(0x30, 0x40, on ? 0x40 : 0);
}
EXPORT_SYMBOL_GPL(mm_sw_dvfs_onoff);

void mmdcdc_sw_dvfs_onoff(int on)
{
	mm_update_bits(0x30, 0x40, on ? 0x40 : 0);
}
EXPORT_SYMBOL_GPL(mmdcdc_sw_dvfs_onoff);

void mmsys_sw_cgb_enable(int en)
{
	mm_update_bits(0x30, 0x2, en ? 0x2 : 0);
}
EXPORT_SYMBOL_GPL(mmsys_sw_cgb_enable);

void mm_dvfs_hold_onoff(int en)
{
	mm_update_bits(0xc, 0x2, en ? 0x2 : 0);
}
EXPORT_SYMBOL_GPL(mm_dvfs_hold_onoff);

/*
 * The devfreq profile shared by every IP.
 */

/**
 * dvfs_target - the devfreq target callback
 *
 * The requested frequency is rounded to the nearest operating point and
 * the IP is reprogrammed when the frequency actually changes.
 */
int dvfs_target(struct device *dev, unsigned long *freq, u32 flags)
{
	struct ip_dvfs_dev *ip_dev = dev_get_drvdata(dev);
	struct dev_pm_opp *opp;
	unsigned long target;
	unsigned long volt;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	opp = devfreq_recommended_opp(dev, freq, flags);
	if (IS_ERR(opp)) {
		dev_err(dev, "Failed to find opp for %lu KHz\n", *freq);
		return PTR_ERR(opp);
	}

	target = dev_pm_opp_get_freq(opp);
	volt = dev_pm_opp_get_voltage(opp);
	dev_pm_opp_put(opp);

	mutex_lock(&ip_dev->lock);
	if (ip_dev->cur_freq != target) {
		if (ip_dev->ops->updata_target_freq)
			ip_dev->ops->updata_target_freq(ip_dev->ops, target,
							 volt, 0);
		ip_dev->cur_freq = target;
		ip_dev->cur_volt = volt;
	}
	mutex_unlock(&ip_dev->lock);

	return 0;
}
EXPORT_SYMBOL_GPL(dvfs_target);

int dvfs_get_dev_status(struct device *dev, struct devfreq_dev_status *status)
{
	struct ip_dvfs_dev *ip_dev = dev_get_drvdata(dev);
	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	/*
	 * 6.18 shrank devfreq_dev_status to total_time/busy_time/
	 * current_frequency plus a driver-private pointer: there is no
	 * min_freq/max_freq/governor field any more, and
	 * devfreq_event_data()/devfreq_event_get_event() are gone. The
	 * window and the governor now live on struct devfreq as
	 * scaling_min_freq/scaling_max_freq and ->governor.
	 */
	status->current_frequency = ip_dev->cur_freq;
	status->private_data = ip_dev;
	return 0;
}
EXPORT_SYMBOL_GPL(dvfs_get_dev_status);

int dvfs_get_cur_freq(struct device *dev, unsigned long *freq)
{
	struct ip_dvfs_dev *ip_dev = dev_get_drvdata(dev);

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	*freq = ip_dev->cur_freq;
	return 0;
}
EXPORT_SYMBOL_GPL(dvfs_get_cur_freq);

/**
 * dvfs_notify_callback - the devfreq notification callback
 *
 * The mmsys power chain drives the per-IP reinitialisation: the IP's
 * registers are lost when the block is powered down, so they have to be
 * programmed again on the way back up.
 */
int dvfs_notify_callback(struct notifier_block *nb, unsigned long action,
			  void *data)
{
	struct ip_dvfs_dev *ip_dev = container_of(nb, struct ip_dvfs_dev,
						  notifier);
	struct ip_dvfs_ops *ops = ip_dev->ops;

	if (!ops)
		return 0;

	switch (action) {
	case SPRD_MM_DVFS_NOTIFY_POWER_ON:
		if (ops->dvfs_init)
			ops->dvfs_init(ops);
		break;
	case SPRD_MM_DVFS_NOTIFY_POWER_OFF:
		ip_dev->dvfs_enable = 0;
		break;
	default:
		return -EINVAL;
	}

	return NOTIFY_OK;
}
EXPORT_SYMBOL_GPL(dvfs_notify_callback);

EXPORT_SYMBOL(g_mmreg_map);
EXPORT_SYMBOL(mm_power_flag);
EXPORT_SYMBOL(mmsys_glob_reg_lock);
