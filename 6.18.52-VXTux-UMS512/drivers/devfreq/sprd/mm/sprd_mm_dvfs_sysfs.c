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
 * The sysfs layer of the Spreadtrum camera memory-management DVFS driver.
 *
 * The stock vendor blob (mmdvfs.ko) exposes the per-IP tuning knobs and
 * the mmsys-level switches through two attribute groups per device:
 *
 *   <ip>_governor   set_hw_dvfs_en, set_auto_tune_en, set_dvfs_coffe,
 *                   get_ip_status, set_work_freq, set_idle_freq,
 *                   set_work_index, set_idle_index, set_fix_dvfs_value,
 *                   get_dvfs_table_info
 *   <ip>_coeff      set_gfree_wait_delay, set_freq_upd_hdsk_en,
 *                   set_freq_upd_delay_en, set_freq_upd_en_byp,
 *                   set_sw_trig_en, set_work_def_index, set_idle_def_index,
 *                   set_auto_tune_en
 *
 *   mmsys_governor  mmsys_power_ctrl, get_sys_info, set_sys_sw_dvfs_en,
 *                   get_ip_status
 *   mmsys_coeff     set_sys_dvfs_hold_en, set_sys_dvfs_clk_gate,
 *                   sys_dvfs_wait_window, set_sys_dvfs_min_volt
 *
 * The 44 handler bodies were recovered from the stock blob by
 * disassembling its exported get_*_show / set_*_store functions; the
 * field offsets in struct ip_dvfs_dev, the callback slots in
 * struct ip_dvfs_ops and the mmsys switch offsets were read out of the
 * same disassembly.  The port uses the C field names rather than the
 * vendor offsets, because the ported structures have a different layout.
 *
 * A show handler reads its field, formats it into the page buffer and
 * returns the length.  A store handler parses the value, stores it and
 * forwards it to the IP's callback so the hardware follows.
 *
 * The devicedata of a per-IP attribute is the struct ip_dvfs_dev pointer,
 * obtained with dev_get_drvdata().
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/devfreq.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "sprd_mm_dvfs.h"

/*
 * The exported handlers.  The stock blob exports all 44 individually,
 * so a consumer can reuse them; the port exports the same names.
 */
ssize_t get_dvfs_enable_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_auto_tune_en_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_work_freq_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_idle_freq_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_work_index_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_idle_index_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_fix_dvfs_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_dvfs_coffe_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_ip_status_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_dvfs_table_info_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_gfree_wait_delay_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_freq_upd_hdsk_en_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_freq_upd_delay_en_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_freq_upd_en_byp_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sw_trig_en_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_mmsys_power_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sys_status_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sys_dvfs_hold_en_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sys_sw_dvfs_en_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sys_dvfs_clk_gate_ctrl_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sys_dvfs_wait_window_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t get_sys_dvfs_min_volt_show(struct device *dev, struct device_attribute *attr, char *buf);
ssize_t set_dvfs_enable_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_auto_tune_en_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_work_freq_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_idle_freq_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_work_index_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_idle_index_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_fix_dvfs_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_dvfs_coffe_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t get_ip_status_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_dvfs_table_info_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_gfree_wait_delay_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_freq_upd_hdsk_en_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_freq_upd_delay_en_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_freq_upd_en_byp_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sw_trig_en_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_mmsys_power_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sys_status_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sys_dvfs_hold_en_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sys_sw_dvfs_en_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sys_dvfs_clk_gate_ctrl_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sys_dvfs_wait_window_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);
ssize_t set_sys_dvfs_min_volt_store(struct device *dev, struct device_attribute *attr, const char *buf, size_t count);

/* the text the stock driver returns when a field has no value yet */
static const char undefined_str[] = "undefined\n";

/*
 * The per-IP helpers.
 *
 * Every per-IP attribute is attached to the IP's platform device, so the
 * driver data is the struct ip_dvfs_dev.
 */
static struct ip_dvfs_dev *ip_dev_of(struct device *dev)
{
	return dev_get_drvdata(dev);
}

/*
 * The stock show handlers print the field value with "%d\n"; when the
 * value is not available they copy the literal "undefined\n" into the
 * page and return its length.
 */
static ssize_t show_undefined(char *buf)
{
	memcpy(buf, undefined_str, sizeof(undefined_str) - 1);
	return sizeof(undefined_str) - 1;
}

/*
 * The per-IP show handlers.
 */

ssize_t get_dvfs_enable_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->dvfs_enable);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_auto_tune_en_show(struct device *dev,
				     struct device_attribute *attr,
				     char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->auto_tune_en);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_work_freq_show(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->work_freq);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_idle_freq_show(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->idle_freq);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_work_index_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->work_index);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_idle_index_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->idle_index);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_fix_dvfs_show(struct device *dev,
				 struct device_attribute *attr,
				 char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->fix_dvfs_value);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

/*
 * The coefficient show handlers.  The stock blob prints the raw
 * coefficient values, one per line.
 */
ssize_t get_gfree_wait_delay_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->coffe.gfree_wait_delay);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_freq_upd_hdsk_en_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->coffe.freq_upd_hdsk_en);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_freq_upd_delay_en_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->coffe.freq_upd_delay_en);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_freq_upd_en_byp_show(struct device *dev,
					 struct device_attribute *attr,
					 char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->coffe.freq_upd_en_byp);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

ssize_t get_sw_trig_en_show(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops)
		ret = show_undefined(buf);
	else
		ret = sprintf(buf, "%d\n", ip_dev->coffe.sw_trig_en);
	mutex_unlock(&ip_dev->lock);

	return ret;
}

/*
 * get_dvfs_coffe_show - dump the whole coefficient block
 *
 * The stock driver prints one "%d\n" per coefficient, in the order the
 * coefficients live in the struct.
 */
ssize_t get_dvfs_coffe_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	ssize_t ret;

	mutex_lock(&ip_dev->lock);
	if (!ip_dev->ops) {
		ret = show_undefined(buf);
	} else {
		ret = sprintf(buf, "%d\n", ip_dev->coffe.gfree_wait_delay);
		ret += sprintf(buf + ret, "%d\n",
			       ip_dev->coffe.freq_upd_hdsk_en);
		ret += sprintf(buf + ret, "%d\n",
			       ip_dev->coffe.freq_upd_delay_en);
		ret += sprintf(buf + ret, "%d\n",
			       ip_dev->coffe.freq_upd_en_byp);
		ret += sprintf(buf + ret, "%d\n", ip_dev->coffe.sw_trig_en);
		ret += sprintf(buf + ret, "%d\n", ip_dev->coffe.hw_dfs_en);
		ret += sprintf(buf + ret, "%d\n", ip_dev->coffe.hw_dfs_en);
		ret += sprintf(buf + ret, "%d\n",
			       ip_dev->coffe.work_index_def);
		ret += sprintf(buf + ret, "%d\n",
			       ip_dev->coffe.idle_index_def);
	}
	mutex_unlock(&ip_dev->lock);

	return ret;
}

/*
 * get_ip_status_show - dump the live voltage and clock readback
 */
ssize_t get_ip_status_show(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 clk;
	u32 volt;
	u32 top_volt = 0;
	u32 mm_volt = 0;
	ssize_t ret;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;

	clk = ip_dev->cur_freq;
	volt = ip_dev->cur_volt;

	if (ops->get_mm_current_volt)
		ops->get_mm_current_volt(ops, &mm_volt);
	if (ops->get_top_current_volt)
		ops->get_top_current_volt(ops, &top_volt);
	mutex_unlock(&ip_dev->lock);

	ret = sprintf(buf, "module_dvfs_read_clk=%d\n", clk);
	ret += sprintf(buf + ret, "module_dvfs_read_volt=%d\n", volt);
	ret += sprintf(buf + ret, "module_dvfs_read_top_volt=%d\n", top_volt);
	ret += sprintf(buf + ret, "module_dvfs_read_mm_volt=%d\n", mm_volt);

	return ret;
}

/*
 * get_dvfs_table_info_show - dump the IP's frequency map
 */
ssize_t get_dvfs_table_info_show(struct device *dev,
					 struct device_attribute *attr,
					 char *buf)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	ssize_t ret = 0;
	int i;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;

	if (ops->get_dvfs_table)
		ops->get_dvfs_table(ops, ip_dev->table,
				    DVFS_FREQ_LEVEL_NUM);

	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++) {
		struct ip_dvfs_map_cfg *cfg = &ip_dev->table[i];

		ret += sprintf(buf + ret, "volt=%s\n",
			       cfg->volt_val ? cfg->volt_val : "unknown");
		ret += sprintf(buf + ret, "clk=%d\n", cfg->clk_level);
		ret += sprintf(buf + ret, "map_index=%d\n", cfg->map_index);
		ret += sprintf(buf + ret, "reg_add=%lu\n",
			       (unsigned long)cfg->reg_add);
		ret += sprintf(buf + ret, "fdiv_denom=%d\n", cfg->fdiv_num);
		ret += sprintf(buf + ret, "fdiv_num=%d\n", cfg->reserved2);
		ret += sprintf(buf + ret, "axi_index=%d\n", cfg->axi_index);
		ret += sprintf(buf + ret, "mtx_index=%d\n\n", cfg->mtx_index);
	}
	mutex_unlock(&ip_dev->lock);

	return ret;
}

/*
 * The per-IP store handlers.
 *
 * Each one parses the value, stores it in the per-IP state and then
 * forwards it to the IP's callback, so the hardware follows.
 */

ssize_t set_dvfs_enable_store(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->dvfs_enable = val;
	if (ops->set_hw_dvfs_en)
		ops->set_hw_dvfs_en(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_auto_tune_en_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->auto_tune_en = val;
	if (ops->set_auto_tune_en)
		ops->set_auto_tune_en(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_work_freq_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->work_freq = val;
	if (ops->set_work_freq)
		ops->set_work_freq(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_idle_freq_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->idle_freq = val;
	if (ops->set_idle_freq)
		ops->set_idle_freq(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_work_index_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->work_index = val;
	if (ops->set_ip_dvfs_work_index)
		ops->set_ip_dvfs_work_index(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_idle_index_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->idle_index = val;
	if (ops->set_ip_dvfs_idle_index)
		ops->set_ip_dvfs_idle_index(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_fix_dvfs_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->fix_dvfs_value = val;
	if (ops->set_fix_dvfs_value)
		ops->set_fix_dvfs_value(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_gfree_wait_delay_store(struct device *dev,
					   struct device_attribute *attr,
					   const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->coffe.gfree_wait_delay = val;
	if (ops->set_gfree_wait_delay)
		ops->set_gfree_wait_delay(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_freq_upd_hdsk_en_store(struct device *dev,
					   struct device_attribute *attr,
					   const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->coffe.freq_upd_hdsk_en = val;
	if (ops->set_freq_upd_hdsk_en)
		ops->set_freq_upd_hdsk_en(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_freq_upd_delay_en_store(struct device *dev,
					   struct device_attribute *attr,
					   const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->coffe.freq_upd_delay_en = val;
	if (ops->set_freq_upd_delay_en)
		ops->set_freq_upd_delay_en(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_freq_upd_en_byp_store(struct device *dev,
					  struct device_attribute *attr,
					  const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->coffe.freq_upd_en_byp = val;
	if (ops->set_freq_upd_en_byp)
		ops->set_freq_upd_en_byp(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

ssize_t set_sw_trig_en_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 val;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->coffe.sw_trig_en = val;
	if (ops->set_ip_dvfs_swtrig_en)
		ops->set_ip_dvfs_swtrig_en(ops, val);
	mutex_unlock(&ip_dev->lock);

	return count;
}

/*
 * set_dvfs_coffe_store - write the whole coefficient block
 *
 * The stock driver parses eight comma separated values and applies them
 * to the coefficient block.
 */
ssize_t set_dvfs_coffe_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	u32 vals[8];
	int n;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	n = sscanf(buf, "%u,%u,%u,%u,%u,%u,%u,%u",
		   &vals[0], &vals[1], &vals[2], &vals[3],
		   &vals[4], &vals[5], &vals[6], &vals[7]);
	if (n != 8)
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;
	ip_dev->coffe.gfree_wait_delay = vals[0];
	ip_dev->coffe.freq_upd_hdsk_en = vals[1];
	ip_dev->coffe.freq_upd_delay_en = vals[2];
	ip_dev->coffe.freq_upd_en_byp = vals[3];
	ip_dev->coffe.sw_trig_en = vals[4];
	ip_dev->coffe.hw_dfs_en = vals[5];
	ip_dev->coffe.work_index_def = vals[6];
	ip_dev->coffe.idle_index_def = vals[7];

	if (ops->set_gfree_wait_delay)
		ops->set_gfree_wait_delay(ops, vals[0]);
	if (ops->set_freq_upd_hdsk_en)
		ops->set_freq_upd_hdsk_en(ops, vals[1]);
	if (ops->set_freq_upd_delay_en)
		ops->set_freq_upd_delay_en(ops, vals[2]);
	if (ops->set_freq_upd_en_byp)
		ops->set_freq_upd_en_byp(ops, vals[3]);
	if (ops->set_ip_dvfs_swtrig_en)
		ops->set_ip_dvfs_swtrig_en(ops, vals[4]);
	if (ops->set_ip_dvfs_work_index)
		ops->set_ip_dvfs_work_index(ops, vals[6]);
	if (ops->set_ip_dvfs_idle_index)
		ops->set_ip_dvfs_idle_index(ops, vals[7]);
	mutex_unlock(&ip_dev->lock);

	return count;
}

/*
 * set_dvfs_table_info_store - write one entry of the IP's frequency map
 *
 * The stock driver parses
 *   "map_index,reg_add,fdiv_num,fdiv_denom,axi_index,mtx_index,clk,volt"
 * and applies the entry to the hardware.
 */
ssize_t set_dvfs_table_info_store(struct device *dev,
					  struct device_attribute *attr,
					  const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip_dev = ip_dev_of(dev);
	struct ip_dvfs_ops *ops;
	struct ip_dvfs_map_cfg cfg;
	u32 map_index;
	u64 reg_add;
	u32 fdiv_num;
	u32 fdiv_denom;
	u32 axi_index;
	u32 mtx_index;
	u32 clk_level;
	u32 volt_level;
	int n;

	if (!ip_dev || !ip_dev->ops)
		return -EINVAL;

	n = sscanf(buf, "%u,%llu,%u,%u,%u,%u,%u,%u",
		   &map_index, &reg_add, &fdiv_num, &fdiv_denom,
		   &axi_index, &mtx_index, &clk_level, &volt_level);
	if (n != 8)
		return -EINVAL;

	if (map_index >= DVFS_FREQ_LEVEL_NUM)
		return -EINVAL;

	mutex_lock(&ip_dev->lock);
	ops = ip_dev->ops;

	memset(&cfg, 0, sizeof(cfg));
	cfg.map_index = map_index;
	cfg.reg_add = reg_add;
	cfg.fdiv_num = fdiv_num;
	cfg.reserved2 = fdiv_denom;
	cfg.axi_index = axi_index;
	cfg.mtx_index = mtx_index;
	cfg.clk_level = clk_level;
	cfg.volt_level = volt_level;

	ip_dev->table[map_index] = cfg;

	if (ops->set_dvfs_table)
		ops->set_dvfs_table(ops, &cfg);
	mutex_unlock(&ip_dev->lock);

	return count;
}

/*
 * The stock blob uses get_ip_status_store as the store half of the
 * get_ip_status attribute; it accepts the write and does nothing.
 */
ssize_t get_ip_status_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	return count;
}

/*
 * The stock blob uses set_sys_status_store as the store half of the
 * get_sys_info attribute; it accepts the write and does nothing.
 */
ssize_t set_sys_status_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	return count;
}

/*
 * The mmsys-level handlers.
 *
 * The mmsys device carries a struct mmsys_dvfs, and the attributes are
 * attached to the mmsys platform device.
 */
static struct ip_dvfs_dev *mmsys_of(struct device *dev)
{
	return dev_get_drvdata(dev);
}

/* the mmsys operations, registered by the IP layer */
static struct ip_dvfs_ops *mmsys_ops(void)
{
	return get_ip_dvfs_ops("MMSYS_DVFS_OPS");
}

/*
 * get_mmsys_power_show - the mmsys power switch
 */
ssize_t get_mmsys_power_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	ssize_t ret;

	if (!mmsys)
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	ret = sprintf(buf, "%lu\n", (unsigned long)mmsys->power_ctrl);
	mutex_unlock(&mmsys->lock);

	return ret;
}

/*
 * set_mmsys_power_store - set the mmsys power switch
 */
ssize_t set_mmsys_power_store(struct device *dev,
				      struct device_attribute *attr,
				      const char *buf, size_t count)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	struct ip_dvfs_ops *ops = mmsys_ops();
	u64 val;

	if (!mmsys)
		return -EINVAL;

	if (kstrtou64(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	mmsys->power_ctrl = val;
	mutex_unlock(&mmsys->lock);

	if (ops && ops->set_hw_dvfs_en)
		ops->set_hw_dvfs_en(ops, (int)(val & 0xffffffff));

	return count;
}

/*
 * get_sys_status_show - the mmsys voltage and clock summary
 */
ssize_t get_sys_status_show(struct device *dev,
				   struct device_attribute *attr,
				   char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	struct ip_dvfs_ops *ops = mmsys_ops();
	struct ip_dvfs_status status;
	u32 top_volt = 0;
	u32 mm_volt = 0;
	ssize_t ret;

	if (!mmsys || !ops)
		return -EINVAL;

	memset(&status, 0, sizeof(status));
	mutex_lock(&mmsys->lock);
	if (ops->get_ip_status)
		ops->get_ip_status(ops, &status);
	if (ops->get_top_current_volt)
		ops->get_top_current_volt(ops, &top_volt);
	if (ops->get_mm_current_volt)
		ops->get_mm_current_volt(ops, &mm_volt);
	mutex_unlock(&mmsys->lock);

	ret = sprintf(buf, "ip volt(V):\nmtx_vote\tisp_vote\tfd_vote\t\t"
			   "cpp_vote\tjpg_vote\tdcam_vote\taxi_vote\tmm_vote\t\ttop\n");
	ret += sprintf(buf + ret, "%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
		       status.apsys_cur_volt ? status.apsys_cur_volt : "",
		       status.dpu_vote_volt ? status.dpu_vote_volt : "",
		       status.gsp0_vote_volt ? status.gsp0_vote_volt : "",
		       status.gsp1_vote_volt ? status.gsp1_vote_volt : "",
		       status.vsp_vote_volt ? status.vsp_vote_volt : "",
		       status.vpuenc_vote_volt ? status.vpuenc_vote_volt : "",
		       status.vdsp_vote_volt ? status.vdsp_vote_volt : "",
		       status.dpu_cur_freq ? status.dpu_cur_freq : "",
		       status.vsp_cur_freq ? status.vsp_cur_freq : "");
	ret += sprintf(buf + ret, "ip clock(Hz):\nmtx\t\tisp\t\tfd\t\t"
			   "cpp\t\tjpg\t\tdcam\t\taxi\n");
	ret += sprintf(buf + ret, "%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
		       0, 0, 0, 0, 0, 0, 0);

	return ret;
}

/*
 * The mmsys coefficient switches.
 */
ssize_t get_sys_dvfs_hold_en_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	ssize_t ret;

	if (!mmsys)
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	ret = sprintf(buf, "%d\n", mmsys->dvfs_hold_en);
	mutex_unlock(&mmsys->lock);

	return ret;
}

ssize_t set_sys_dvfs_hold_en_store(struct device *dev,
					  struct device_attribute *attr,
					  const char *buf, size_t count)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	u32 val;

	if (!mmsys)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	mmsys->dvfs_hold_en = val;
	mutex_unlock(&mmsys->lock);

	mm_dvfs_hold_onoff(val);

	return count;
}

ssize_t get_sys_sw_dvfs_en_show(struct device *dev,
				       struct device_attribute *attr,
				       char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	ssize_t ret;

	if (!mmsys)
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	ret = sprintf(buf, "%d\n", mmsys->sw_dvfs_en);
	mutex_unlock(&mmsys->lock);

	return ret;
}

ssize_t set_sys_sw_dvfs_en_store(struct device *dev,
				       struct device_attribute *attr,
				       const char *buf, size_t count)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	u32 val;

	if (!mmsys)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	mmsys->sw_dvfs_en = val;
	mutex_unlock(&mmsys->lock);

	mm_sw_dvfs_onoff(val);

	return count;
}

ssize_t get_sys_dvfs_clk_gate_ctrl_show(struct device *dev,
						struct device_attribute *attr,
						char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	ssize_t ret;

	if (!mmsys)
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	ret = sprintf(buf, "%d\n", mmsys->clk_gate_ctrl);
	mutex_unlock(&mmsys->lock);

	return ret;
}

ssize_t set_sys_dvfs_clk_gate_ctrl_store(struct device *dev,
						 struct device_attribute *attr,
						 const char *buf, size_t count)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	u32 val;

	if (!mmsys)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	mmsys->clk_gate_ctrl = val;
	mutex_unlock(&mmsys->lock);

	mmsys_sw_cgb_enable(val);

	return count;
}

ssize_t get_sys_dvfs_wait_window_show(struct device *dev,
					      struct device_attribute *attr,
					      char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	ssize_t ret;

	if (!mmsys)
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	ret = sprintf(buf, "%d\n", mmsys->wait_window);
	mutex_unlock(&mmsys->lock);

	return ret;
}

ssize_t set_sys_dvfs_wait_window_store(struct device *dev,
					      struct device_attribute *attr,
					      const char *buf, size_t count)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	u32 val;

	if (!mmsys)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	mmsys->wait_window = val;
	mutex_unlock(&mmsys->lock);

	return count;
}

ssize_t get_sys_dvfs_min_volt_show(struct device *dev,
					   struct device_attribute *attr,
					   char *buf)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	ssize_t ret;

	if (!mmsys)
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	ret = sprintf(buf, "%d\n", mmsys->min_volt);
	mutex_unlock(&mmsys->lock);

	return ret;
}

ssize_t set_sys_dvfs_min_volt_store(struct device *dev,
					   struct device_attribute *attr,
					   const char *buf, size_t count)
{
	struct ip_dvfs_dev *mmsys = mmsys_of(dev);
	u32 val;

	if (!mmsys)
		return -EINVAL;

	if (kstrtou32(buf, 0, &val))
		return -EINVAL;

	mutex_lock(&mmsys->lock);
	mmsys->min_volt = val;
	mutex_unlock(&mmsys->lock);

	return count;
}

/*
 * The attribute tables.
 *
 * 6.18 keeps the show/store handlers in struct device_attribute and puts
 * a bare struct attribute * into the group, so each entry is the
 * &dev_attr_*.attr of one DEVICE_ATTR.  ATTRIBUTE_GROUPS() then emits
 * the group and the NULL-terminated group list.
 */
/*
 * The stock attribute name on the left, the exported handler on the
 * right: DEVICE_ATTR() takes both explicitly because the vendor's
 * sysfs names do not follow the <name>_show convention.
 */

















/*
 * 6.18 has no four-argument DEVICE_ATTR(): the only macro that takes an
 * explicit show/store pair is __ATTR(), and that one produces a plain
 * struct attribute.  The 6.18 sysfs code wants struct device_attribute,
 * so the entries are written out here.  The vendor names each attribute
 * after the knob it drives ("set_work_freq") but the show function is the
 * getter ("get_work_freq_show"), so the names cannot be derived.
 */
static struct device_attribute dev_attr_set_hw_dvfs_en = {
	.attr = { .name = "set_hw_dvfs_en", .mode = 0644 },
	.show = get_dvfs_enable_show,
	.store = NULL,
};
static struct device_attribute dev_attr_set_auto_tune_en = {
	.attr = { .name = "set_auto_tune_en", .mode = 0644 },
	.show = get_auto_tune_en_show,
	.store = set_auto_tune_en_store,
};
static struct device_attribute dev_attr_set_dvfs_coffe = {
	.attr = { .name = "set_dvfs_coffe", .mode = 0644 },
	.show = get_dvfs_coffe_show,
	.store = set_dvfs_coffe_store,
};
static struct device_attribute dev_attr_get_ip_status = {
	.attr = { .name = "get_ip_status", .mode = 0644 },
	.show = get_ip_status_show,
	.store = get_ip_status_store,
};
static struct device_attribute dev_attr_set_work_freq = {
	.attr = { .name = "set_work_freq", .mode = 0644 },
	.show = get_work_freq_show,
	.store = set_work_freq_store,
};
static struct device_attribute dev_attr_set_idle_freq = {
	.attr = { .name = "set_idle_freq", .mode = 0644 },
	.show = get_idle_freq_show,
	.store = set_idle_freq_store,
};
static struct device_attribute dev_attr_set_work_index = {
	.attr = { .name = "set_work_index", .mode = 0644 },
	.show = get_work_index_show,
	.store = set_work_index_store,
};
static struct device_attribute dev_attr_set_idle_index = {
	.attr = { .name = "set_idle_index", .mode = 0644 },
	.show = get_idle_index_show,
	.store = set_idle_index_store,
};
static struct device_attribute dev_attr_set_fix_dvfs_value = {
	.attr = { .name = "set_fix_dvfs_value", .mode = 0644 },
	.show = get_fix_dvfs_show,
	.store = set_fix_dvfs_store,
};
static struct device_attribute dev_attr_get_dvfs_table_info = {
	.attr = { .name = "get_dvfs_table_info", .mode = 0644 },
	.show = get_dvfs_table_info_show,
	.store = set_dvfs_table_info_store,
};
static struct device_attribute dev_attr_set_gfree_wait_delay = {
	.attr = { .name = "set_gfree_wait_delay", .mode = 0644 },
	.show = get_gfree_wait_delay_show,
	.store = set_gfree_wait_delay_store,
};
static struct device_attribute dev_attr_set_freq_upd_hdsk_en = {
	.attr = { .name = "set_freq_upd_hdsk_en", .mode = 0644 },
	.show = get_freq_upd_hdsk_en_show,
	.store = set_freq_upd_hdsk_en_store,
};
static struct device_attribute dev_attr_set_freq_upd_delay_en = {
	.attr = { .name = "set_freq_upd_delay_en", .mode = 0644 },
	.show = get_freq_upd_delay_en_show,
	.store = set_freq_upd_delay_en_store,
};
static struct device_attribute dev_attr_set_freq_upd_en_byp = {
	.attr = { .name = "set_freq_upd_en_byp", .mode = 0644 },
	.show = get_freq_upd_en_byp_show,
	.store = set_freq_upd_en_byp_store,
};
static struct device_attribute dev_attr_set_sw_trig_en = {
	.attr = { .name = "set_sw_trig_en", .mode = 0644 },
	.show = get_sw_trig_en_show,
	.store = set_sw_trig_en_store,
};
static struct device_attribute dev_attr_set_work_def_index = {
	.attr = { .name = "set_work_def_index", .mode = 0644 },
	.show = get_work_index_show,
	.store = NULL,
};
static struct device_attribute dev_attr_set_idle_def_index = {
	.attr = { .name = "set_idle_def_index", .mode = 0644 },
	.show = get_idle_index_show,
	.store = set_idle_index_store,
};
static struct device_attribute dev_attr_mmsys_power_ctrl = {
	.attr = { .name = "mmsys_power_ctrl", .mode = 0644 },
	.show = get_mmsys_power_show,
	.store = set_mmsys_power_store,
};
static struct device_attribute dev_attr_get_sys_info = {
	.attr = { .name = "get_sys_info", .mode = 0644 },
	.show = get_sys_status_show,
	.store = set_sys_status_store,
};
static struct device_attribute dev_attr_set_sys_sw_dvfs_en = {
	.attr = { .name = "set_sys_sw_dvfs_en", .mode = 0644 },
	.show = get_sys_sw_dvfs_en_show,
	.store = set_sys_sw_dvfs_en_store,
};
static struct device_attribute dev_attr_mmsys_get_ip_status = {
	.attr = { .name = "mmsys_get_ip_status", .mode = 0644 },
	.show = get_sys_status_show,
	.store = NULL,
};
static struct device_attribute dev_attr_set_sys_dvfs_hold_en = {
	.attr = { .name = "set_sys_dvfs_hold_en", .mode = 0644 },
	.show = get_sys_dvfs_hold_en_show,
	.store = set_sys_dvfs_hold_en_store,
};
static struct device_attribute dev_attr_set_sys_dvfs_clk_gate = {
	.attr = { .name = "set_sys_dvfs_clk_gate", .mode = 0644 },
	.show = get_sys_dvfs_clk_gate_ctrl_show,
	.store = set_sys_dvfs_clk_gate_ctrl_store,
};
static struct device_attribute dev_attr_sys_dvfs_wait_window = {
	.attr = { .name = "sys_dvfs_wait_window", .mode = 0644 },
	.show = get_sys_dvfs_wait_window_show,
	.store = set_sys_dvfs_wait_window_store,
};
static struct device_attribute dev_attr_set_sys_dvfs_min_volt = {
	.attr = { .name = "set_sys_dvfs_min_volt", .mode = 0644 },
	.show = get_sys_dvfs_min_volt_show,
	.store = set_sys_dvfs_min_volt_store,
};

static struct attribute *dev_entries[] = {
	&dev_attr_set_hw_dvfs_en.attr,
	&dev_attr_set_auto_tune_en.attr,
	&dev_attr_set_dvfs_coffe.attr,
	&dev_attr_get_ip_status.attr,
	&dev_attr_set_work_freq.attr,
	&dev_attr_set_idle_freq.attr,
	&dev_attr_set_work_index.attr,
	&dev_attr_set_idle_index.attr,
	&dev_attr_set_fix_dvfs_value.attr,
	&dev_attr_get_dvfs_table_info.attr,
	NULL
};

static const struct attribute_group dev_attr_group = {
	.name = "governor",
	.attrs = dev_entries,
};

static struct attribute *coeff_entries[] = {
	&dev_attr_set_gfree_wait_delay.attr,
	&dev_attr_set_freq_upd_hdsk_en.attr,
	&dev_attr_set_freq_upd_delay_en.attr,
	&dev_attr_set_freq_upd_en_byp.attr,
	&dev_attr_set_sw_trig_en.attr,
	&dev_attr_set_work_def_index.attr,
	&dev_attr_set_idle_def_index.attr,
	&dev_attr_set_auto_tune_en.attr,
	NULL
};

static const struct attribute_group coeff_attr_group = {
	.name = "coeff",
	.attrs = coeff_entries,
};

/*
 * The mmsys attribute tables.
 */








static struct attribute *mmsys_dev_attrs[] = {
	&dev_attr_mmsys_power_ctrl.attr,
	&dev_attr_get_sys_info.attr,
	&dev_attr_set_sys_sw_dvfs_en.attr,
	&dev_attr_mmsys_get_ip_status.attr,
	NULL
};

static const struct attribute_group mmsys_dev_group = {
	.name = "governor",
	.attrs = mmsys_dev_attrs,
};

static struct attribute *mmsys_coeff_attrs[] = {
	&dev_attr_set_sys_dvfs_hold_en.attr,
	&dev_attr_set_sys_dvfs_clk_gate.attr,
	&dev_attr_sys_dvfs_wait_window.attr,
	&dev_attr_set_sys_dvfs_min_volt.attr,
	NULL
};

static const struct attribute_group mmsys_coeff_group = {
	.name = "coeff",
	.attrs = mmsys_coeff_attrs,
};

/*
 * The exported handler symbols.
 *
 * The stock blob exports the handlers individually, so a consumer can
 * reuse them; the port exports the same names.
 */
#define EXPORT_HANDLER(name)		\
	EXPORT_SYMBOL_GPL(name)

EXPORT_HANDLER(get_dvfs_enable_show);
EXPORT_HANDLER(set_dvfs_enable_store);
EXPORT_HANDLER(get_auto_tune_en_show);
EXPORT_HANDLER(set_auto_tune_en_store);
EXPORT_HANDLER(get_work_freq_show);
EXPORT_HANDLER(set_work_freq_store);
EXPORT_HANDLER(get_idle_freq_show);
EXPORT_HANDLER(set_idle_freq_store);
EXPORT_HANDLER(get_work_index_show);
EXPORT_HANDLER(set_work_index_store);
EXPORT_HANDLER(get_idle_index_show);
EXPORT_HANDLER(set_idle_index_store);
EXPORT_HANDLER(get_fix_dvfs_show);
EXPORT_HANDLER(set_fix_dvfs_store);
EXPORT_HANDLER(get_dvfs_coffe_show);
EXPORT_HANDLER(set_dvfs_coffe_store);
EXPORT_HANDLER(get_ip_status_show);
EXPORT_HANDLER(get_ip_status_store);
EXPORT_HANDLER(get_dvfs_table_info_show);
EXPORT_HANDLER(set_dvfs_table_info_store);
EXPORT_HANDLER(get_gfree_wait_delay_show);
EXPORT_HANDLER(set_gfree_wait_delay_store);
EXPORT_HANDLER(get_freq_upd_hdsk_en_show);
EXPORT_HANDLER(set_freq_upd_hdsk_en_store);
EXPORT_HANDLER(get_freq_upd_delay_en_show);
EXPORT_HANDLER(set_freq_upd_delay_en_store);
EXPORT_HANDLER(get_freq_upd_en_byp_show);
EXPORT_HANDLER(set_freq_upd_en_byp_store);
EXPORT_HANDLER(get_sw_trig_en_show);
EXPORT_HANDLER(set_sw_trig_en_store);
EXPORT_HANDLER(get_mmsys_power_show);
EXPORT_HANDLER(set_mmsys_power_store);
EXPORT_HANDLER(get_sys_status_show);
EXPORT_HANDLER(set_sys_status_store);
EXPORT_HANDLER(get_sys_dvfs_hold_en_show);
EXPORT_HANDLER(set_sys_dvfs_hold_en_store);
EXPORT_HANDLER(get_sys_sw_dvfs_en_show);
EXPORT_HANDLER(set_sys_sw_dvfs_en_store);
EXPORT_HANDLER(get_sys_dvfs_clk_gate_ctrl_show);
EXPORT_HANDLER(set_sys_dvfs_clk_gate_ctrl_store);
EXPORT_HANDLER(get_sys_dvfs_wait_window_show);
EXPORT_HANDLER(set_sys_dvfs_wait_window_store);
EXPORT_HANDLER(get_sys_dvfs_min_volt_show);
EXPORT_HANDLER(set_sys_dvfs_min_volt_store);

/*
 * mm_dvfs_sysfs_create_groups - create the sysfs groups of one device
 * @dev: the device the groups are attached to
 * @mmsys: non-NULL for the mmsys device, NULL for a camera IP
 *
 * The stock driver creates the groups from the devfreq governor's
 * start event and removes them on stop, so the attributes only exist
 * while the governor runs.  The port creates them at probe time, which
 * is what a user of the sysfs knobs expects.
 */
int mm_dvfs_sysfs_create_groups(struct device *dev, int mmsys)
{
	const struct attribute_group *groups[2];
	int ret;

	if (mmsys) {
		groups[0] = &mmsys_dev_group;
		groups[1] = &mmsys_coeff_group;
	} else {
		groups[0] = &dev_attr_group;
		groups[1] = &coeff_attr_group;
	}

	ret = sysfs_create_group(&dev->kobj, groups[0]);
	if (ret)
		return ret;

	ret = sysfs_create_group(&dev->kobj, groups[1]);
	if (ret)
		sysfs_remove_group(&dev->kobj, groups[0]);

	return ret;
}
EXPORT_SYMBOL_GPL(mm_dvfs_sysfs_create_groups);

/*
 * mm_dvfs_sysfs_remove_groups - remove the sysfs groups of one device
 */
void mm_dvfs_sysfs_remove_groups(struct device *dev, int mmsys)
{
	if (mmsys) {
		sysfs_remove_group(&dev->kobj, &mmsys_coeff_group);
		sysfs_remove_group(&dev->kobj, &mmsys_dev_group);
	} else {
		sysfs_remove_group(&dev->kobj, &coeff_attr_group);
		sysfs_remove_group(&dev->kobj, &dev_attr_group);
	}
}
EXPORT_SYMBOL_GPL(mm_dvfs_sysfs_remove_groups);
