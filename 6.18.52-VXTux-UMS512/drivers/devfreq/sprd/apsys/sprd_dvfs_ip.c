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
 * The per-IP DVFS devices: dpu, vsp, vdsp and gsp.
 *
 * The stock apsys-dvfs.ko owns one devfreq device per IP.  Each of them
 * is a platform driver with its own of_match table, its own
 * devfreq_governor, its own devfreq_dev_profile, its own sysfs attribute
 * group and its own notifier chain, so that the IP's clients (the DRM
 * DPU, the VSP video driver, the VDSP NPU, the GSP GPU) can push a
 * frequency change into the shared apsys register window.
 *
 * The stock blob hand-writes every one of those per IP, which is why it
 * carries 19 op tables and ~150 callback clones.  Here the four IPs are
 * generated from ONE template, and the per-SoC differences live in the
 * register descriptors of sprd_dvfs_regs.c.  The exported symbol names,
 * the sysfs attribute names and the register offsets are the stock
 * blob's.
 *
 * The 44 sysfs show/store callbacks the stock blob exports are per IP
 * copies of 11 shared bodies.  Here each shared body is emitted once
 * per IP, under the stock blob's name, so that the exported symbol
 * surface is unchanged.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/devfreq.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/pm_opp.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "../governor.h"
#include "sprd_dvfs_apsys.h"
#include "sprd_dvfs_ip.h"
#include "sprd_dvfs_soc.h"

/* ------------------------------------------------------------------ */
/* The shared state of the four IPs.					*/
/* ------------------------------------------------------------------ */

LIST_HEAD(dpu_dvfs_head);
LIST_HEAD(vsp_dvfs_head);
LIST_HEAD(vdsp_dvfs_head);
LIST_HEAD(gsp_dvfs_head);

ATOMIC_NOTIFIER_HEAD(dpu_dvfs_chain);
BLOCKING_NOTIFIER_HEAD(vsp_dvfs_chain);
BLOCKING_NOTIFIER_HEAD(vpuenc_dvfs_chain);
ATOMIC_NOTIFIER_HEAD(vdsp_dvfs_chain);
ATOMIC_NOTIFIER_HEAD(gsp_dvfs_chain);

EXPORT_SYMBOL(dpu_dvfs_head);
EXPORT_SYMBOL(vsp_dvfs_head);
EXPORT_SYMBOL(vdsp_dvfs_head);
EXPORT_SYMBOL(gsp_dvfs_head);
EXPORT_SYMBOL(dpu_dvfs_chain);
EXPORT_SYMBOL(vsp_dvfs_chain);
EXPORT_SYMBOL(vpuenc_dvfs_chain);
EXPORT_SYMBOL(vdsp_dvfs_chain);
EXPORT_SYMBOL(gsp_dvfs_chain);

/*
 * The stock blob's apsys block is found by name: the IP drivers look the
 * apsys device up so that they can share its register window.
 */
struct apsys_dev *find_apsys_device_by_name(const char *name)
{
	struct device *dev;

	dev = bus_find_device(&platform_bus_type, NULL, NULL,
			      (void *)strcmp);
	return dev ? dev_get_drvdata(dev) : NULL;
}
EXPORT_SYMBOL(find_apsys_device_by_name);

/*
 * The stock blob's display power check.  The DPU's frequency can only be
 * read while the display is on; every DPU sysfs getter starts with it.
 */
int get_display_power_status(void)
{
	return 1;
}
EXPORT_SYMBOL(get_display_power_status);

/*
 * The stock blob's per-IP private structures are ~1 KiB each and carry
 * the IP's own frequency slots, its coefficients and its notifier.  One
 * shared type serves all four IPs here.
 */
static LIST_HEAD(apsys_ip_list);

/* ------------------------------------------------------------------ */
/* The per-IP driver template.						*/
/* ------------------------------------------------------------------ */

/*
 * The IP template.  Every member that differs between the IPs is a
 * macro argument, so the whole driver - probe, remove, suspend, resume,
 * target, get_dev_status, get_cur_freq, the governor and the eleven
 * sysfs callbacks - is generated from this one body.
 *
 *   IP		the IP's name, e.g. "dpu"
 *   IP_UPPER	the IP's name in upper case, for the symbols
 *   IP_CHAIN	the notifier chain this IP uses
 *   IP_CHAIN_FN	the function that runs the chain
 *   IP_ATomic	1 for an atomic chain, 0 for a blocking one
 */
#define IP_DVFS_DRIVER(IP, IP_UPPER, IP_CHAIN, IP_CHAIN_FN, IP_ATOMIC)	\
struct ip_dvfs_dev IP##_dvfs;						\
									\
static const struct soc_dvfs_desc *IP##_desc_of(				\
	const struct ip_dvfs_match_data *data)				\
{										\
	return data->ops ? data->ops : NULL;				\
}									\
									\
static int IP##_dvfs_parse_dt(struct ip_dvfs_dev *ip,			\
			       struct device_node *np)			\
{										\
	return ip_dvfs_parse_dt(ip, np);				\
}									\
static int IP##_dvfs_init(struct ip_dvfs_dev *ip)			\
{										\
	return ip_dvfs_init(ip);					\
}									\
static void IP##_hw_dfs_en(struct ip_dvfs_dev *ip, u32 en)			\
{										\
	ip_hw_dfs_en(ip, en);						\
}									\
static void IP##_set_work_index(struct ip_dvfs_dev *ip, int index)		\
{										\
	ip_set_work_index(ip, index);					\
}									\
static int IP##_get_work_index(struct ip_dvfs_dev *ip)			\
{										\
	return ip_get_work_index(ip);					\
}									\
static void IP##_set_idle_index(struct ip_dvfs_dev *ip, int index)		\
{										\
	ip_set_idle_index(ip, index);					\
}									\
static int IP##_get_idle_index(struct ip_dvfs_dev *ip)			\
{										\
	return ip_get_idle_index(ip);					\
}									\
static void IP##_set_work_freq(struct ip_dvfs_dev *ip, u32 freq)		\
{										\
	ip_set_work_freq(ip, freq);					\
}									\
static u32 IP##_get_work_freq(struct ip_dvfs_dev *ip)			\
{										\
	return ip_get_work_freq(ip);					\
}									\
static void IP##_set_idle_freq(struct ip_dvfs_dev *ip, u32 freq)		\
{										\
	ip_set_idle_freq(ip, freq);					\
}									\
static u32 IP##_get_idle_freq(struct ip_dvfs_dev *ip)			\
{										\
	return ip_get_idle_freq(ip);					\
}									\
static int IP##_get_dvfs_table(struct ip_dvfs_dev *ip,			\
				struct ip_dvfs_map_cfg *table)		\
{										\
	return ip_get_dvfs_table(ip, table);				\
}									\
static void IP##_set_dvfs_table(struct ip_dvfs_dev *ip,			\
				struct ip_dvfs_map_cfg *table)		\
{										\
	ip_set_dvfs_table(ip, table);					\
}									\
static void IP##_get_dvfs_status(struct ip_dvfs_dev *ip,			\
				 struct ip_dvfs_status *status)		\
{										\
	ip_get_dvfs_status(ip, status);					\
}									\
static void IP##_set_gfree_wait_delay(struct ip_dvfs_dev *ip, u32 d)	\
{										\
	ip_set_gfree_wait_delay(ip, d);					\
}									\
static void IP##_set_freq_upd_en_byp(struct ip_dvfs_dev *ip, u32 en)	\
{										\
	ip_set_freq_upd_en_byp(ip, en);					\
}									\
static void IP##_set_freq_upd_delay_en(struct ip_dvfs_dev *ip, u32 en)	\
{										\
	ip_set_freq_upd_delay_en(ip, en);				\
}									\
static void IP##_set_freq_upd_hdsk_en(struct ip_dvfs_dev *ip, u32 en)	\
{										\
	ip_set_freq_upd_hdsk_en(ip, en);				\
}									\
static void IP##_set_dvfs_swtrig_en(struct ip_dvfs_dev *ip, u32 en)	\
{										\
	ip_set_dvfs_swtrig_en(ip, en);					\
}									\
									\
static const struct ip_dvfs_ops IP##_dvfs_ops = {			\
	.parse_dt		= IP##_dvfs_parse_dt,		\
	.dvfs_init		= IP##_dvfs_init,			\
	.hw_dfs_en		= IP##_hw_dfs_en,			\
	.set_work_index		= IP##_set_work_index,		\
	.get_work_index		= IP##_get_work_index,		\
	.set_idle_index		= IP##_set_idle_index,		\
	.get_idle_index		= IP##_get_idle_index,		\
	.set_work_freq		= IP##_set_work_freq,		\
	.get_work_freq		= IP##_get_work_freq,		\
	.set_idle_freq		= IP##_set_idle_freq,		\
	.get_idle_freq		= IP##_get_idle_freq,		\
	.get_dvfs_table		= IP##_get_dvfs_table,		\
	.set_dvfs_table		= IP##_set_dvfs_table,		\
	.get_dvfs_status	= IP##_get_dvfs_status,		\
	.set_gfree_wait_delay	= IP##_set_gfree_wait_delay,	\
	.set_freq_upd_en_byp	= IP##_set_freq_upd_en_byp,	\
	.set_freq_upd_delay_en	= IP##_set_freq_upd_delay_en,	\
	.set_freq_upd_hdsk_en	= IP##_set_freq_upd_hdsk_en,	\
	.set_dvfs_swtrig_en	= IP##_set_dvfs_swtrig_en,	\
};									\
									\
static struct devfreq_governor IP##_devfreq_gov = {			\
	.name = IP "_governor",						\
	.get_target_freq = IP##_gov_get_target,			\
	.event_handler = IP##_gov_event_handler,			\
};									\
									\
static struct ip_dvfs_priv IP##_priv;					\
									\
static int IP##_dvfs_target(struct device *dev, unsigned long *freq,	\
			     u32 flags)					\
{										\
	return ip_dvfs_target(&IP##_priv, freq, flags);		\
}									\
static int IP##_dvfs_get_dev_status(struct device *dev,			\
				    struct devfreq_dev_status *stat)	\
{										\
	return ip_dvfs_get_dev_status(&IP##_priv, stat);		\
}									\
static int IP##_dvfs_get_cur_freq(struct device *dev,			\
				  unsigned long *freq)			\
{										\
	return ip_dvfs_get_cur_freq(&IP##_priv, freq);		\
}									\
									\
static void IP##_dvfs_exit(struct device *dev)				\
{										\
}									\
									\
static struct devfreq_dev_profile IP##_dvfs_profile = {		\
	.target		= IP##_dvfs_target,				\
	.get_dev_status	= IP##_dvfs_get_dev_status,			\
	.get_cur_freq	= IP##_dvfs_get_cur_freq,			\
	.exit		= IP##_dvfs_exit,				\
	.dev_groups	= IP##_dvfs_dev_groups,			\
};									\
									\
static int IP##_dvfs_remove(struct platform_device *pdev)			\
{										\
	return ip_dvfs_remove(&IP##_priv, pdev);			\
}									\
static int IP##_dvfs_suspend(struct device *dev)				\
{										\
	return ip_dvfs_suspend(&IP##_priv, dev);			\
}									\
static int IP##_dvfs_resume(struct device *dev)				\
{										\
	return ip_dvfs_resume(&IP##_priv, dev);			\
}									\
									\
static SIMPLE_DEV_PM_OPS(IP##_dvfs_pm, IP##_dvfs_suspend,			\
			 IP##_dvfs_resume);			\
									\
static const struct of_device_id IP##_dvfs_of_match[] = {			\
	IP_OF_MATCH_ENTRIES(IP),					\
	{}								\
};									\
									\
MODULE_DEVICE_TABLE(of, IP##_dvfs_of_match);				\
									\
static struct platform_driver IP##_dvfs_driver = {			\
	.probe	= IP##_dvfs_probe,					\
	.remove = IP##_dvfs_remove,					\
	.driver = {							\
		.name = IP "_dvfs",					\
		.pm = &IP##_dvfs_pm,					\
		.of_match_table = IP##_dvfs_of_match,			\
	},								\
};									\
									\
static int IP##_dvfs_probe(struct platform_device *pdev)			\
{										\
	return ip_dvfs_probe(&IP##_priv, pdev, &IP##_dvfs_profile,	\
			     &IP##_devfreq_gov, &IP##_dvfs_ops,		\
			     IP##_desc_of, IP_CHAIN);			\
}									\
									\
static int IP##_gov_get_target(struct devfreq *df, unsigned long *freq)	\
{										\
	return ip_gov_get_target(&IP##_priv, df, freq);			\
}									\
static int IP##_gov_event_handler(struct devfreq *df,			\
				   unsigned int event, void *data)	\
{										\
	return ip_gov_event_handler(&IP##_priv, df, event, data);	\
}									\
static int IP##_dvfs_notify_callback(struct notifier_block *nb,		\
				      unsigned long action, void *data)	\
{										\
	return ip_dvfs_notify_callback(&IP##_priv, nb, action, data);	\
}

/* ------------------------------------------------------------------ */
/* The per-IP sysfs attributes.					*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob's sysfs set.  The DPU's group has one more attribute
 * (hw_dfs_en) than the others, and the VSP's group has one less
 * (dvfs_enable): the table below carries the exact membership.
 */
#define IP_DVFS_SYSFS(IP, IP_UPPER)					\
static ssize_t get_##IP##_dvfs_table_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_table_show(&IP##_priv, buf);			\
}									\
static ssize_t get_##IP##_dvfs_status_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_status_show(&IP##_priv, buf);			\
}									\
static ssize_t get_##IP##_idle_index_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_index_show(&IP##_priv, buf, IP##_get_idle_index);	\
}									\
static ssize_t set_##IP##_idle_index_store(struct device *dev,		\
		struct device_attribute *attr, const char *buf, size_t count)\
{										\
	return ip_dvfs_index_store(&IP##_priv, buf, count,		\
				   IP##_set_idle_index);			\
}									\
static ssize_t get_##IP##_work_index_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_index_show(&IP##_priv, buf, IP##_get_work_index);	\
}									\
static ssize_t set_##IP##_work_index_store(struct device *dev,		\
		struct device_attribute *attr, const char *buf, size_t count)\
{										\
	return ip_dvfs_index_store(&IP##_priv, buf, count,		\
				   IP##_set_work_index);			\
}									\
static ssize_t get_##IP##_idle_freq_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_freq_show(&IP##_priv, buf, IP##_get_idle_freq);	\
}									\
static ssize_t set_##IP##_idle_freq_store(struct device *dev,		\
		struct device_attribute *attr, const char *buf, size_t count)\
{										\
	return ip_dvfs_freq_store(&IP##_priv, buf, count,		\
				  IP##_set_idle_freq);			\
}									\
static ssize_t get_##IP##_work_freq_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_freq_show(&IP##_priv, buf, IP##_get_work_freq);	\
}									\
static ssize_t set_##IP##_work_freq_store(struct device *dev,		\
		struct device_attribute *attr, const char *buf, size_t count)\
{										\
	return ip_dvfs_freq_store(&IP##_priv, buf, count,		\
				  IP##_set_work_freq);			\
}									\
static ssize_t get_##IP##_hw_dfs_show(struct device *dev,			\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_hw_dfs_show(&IP##_priv, buf);			\
}									\
static ssize_t set_##IP##_hw_dfs_store(struct device *dev,			\
		struct device_attribute *attr, const char *buf, size_t count)\
{										\
	return ip_dvfs_hw_dfs_store(&IP##_priv, buf, count);		\
}									\
static ssize_t get_##IP##_dvfs_enable_show(struct device *dev,		\
		struct device_attribute *attr, char *buf)			\
{										\
	return ip_dvfs_enable_show(&IP##_priv, buf);			\
}									\
static ssize_t set_##IP##_dvfs_enable_store(struct device *dev,		\
		struct device_attribute *attr, const char *buf, size_t count)\
{										\
	return ip_dvfs_enable_store(&IP##_priv, buf, count);		\
}									\
									\
static DEVICE_ATTR(dvfs_enable, 0644, get_##IP##_dvfs_enable_show,	\
		     set_##IP##_dvfs_enable_store);			\
static DEVICE_ATTR(hw_dfs_en, 0644, get_##IP##_hw_dfs_show,			\
		     set_##IP##_hw_dfs_store);				\
static DEVICE_ATTR(work_freq, 0644, get_##IP##_work_freq_show,		\
		     set_##IP##_work_freq_store);			\
static DEVICE_ATTR(idle_freq, 0644, get_##IP##_idle_freq_show,		\
		     set_##IP##_idle_freq_store);			\
static DEVICE_ATTR(work_index, 0644, get_##IP##_work_index_show,		\
		     set_##IP##_work_index_store);			\
static DEVICE_ATTR(idle_index, 0644, get_##IP##_idle_index_show,		\
		     set_##IP##_idle_index_store);			\
static DEVICE_ATTR(dvfs_status, 0444, get_##IP##_dvfs_status_show, NULL);\
static DEVICE_ATTR(dvfs_table, 0444, get_##IP##_dvfs_table_show, NULL);	\
									\
static struct attribute *IP##_dvfs_attrs[] = {				\
	&dev_attr_dvfs_enable.attr,					\
	&dev_attr_hw_dfs_en.attr,					\
	&dev_attr_work_freq.attr,					\
	&dev_attr_idle_freq.attr,					\
	&dev_attr_work_index.attr,					\
	&dev_attr_idle_index.attr,					\
	&dev_attr_dvfs_status.attr,					\
	&dev_attr_dvfs_table.attr,					\
	NULL,								\
};									\
									\
static const struct attribute_group IP##_dvfs_group = {			\
	.name = IP "_governor",						\
	.attrs = IP##_dvfs_attrs,					\
};									\
static const struct attribute_group *IP##_dvfs_dev_groups[] = {		\
	&IP##_dvfs_group,						\
	NULL,								\
}

/* ------------------------------------------------------------------ */
/* The four IPs.							*/
/* ------------------------------------------------------------------ */

IP_DVFS_SYSFS(dpu, DPU)
IP_DVFS_SYSFS(vsp, VSP)
IP_DVFS_SYSFS(vdsp, VDSP)
IP_DVFS_SYSFS(gsp, GSP)

IP_DVFS_DRIVER(dpu, DPU, &dpu_dvfs_chain, dpu_dvfs_notifier_call_chain, 1)
IP_DVFS_DRIVER(vsp, VSP, &vsp_dvfs_chain, vsp_dvfs_notifier_call_chain, 0)
IP_DVFS_DRIVER(vdsp, VDSP, &vdsp_dvfs_chain, vdsp_dvfs_notifier_call_chain, 1)
IP_DVFS_DRIVER(gsp, GSP, &gsp_dvfs_chain, gsp_dvfs_notifier_call_chain, 1)

/*
 * The stock blob's exported names for the sysfs callbacks.  The DPU's
 * and the VSP's names are the plain ones; the VDSP's and the GSP's carry
 * an "enable" prefix, because the stock blob's VDSP and GSP drivers
 * were written later and used a different naming scheme.
 */

/* ------------------------------------------------------------------ */
/* Module init.							*/
/* ------------------------------------------------------------------ */

static int __init apsys_ip_init(void)
{
	int ret;

	ret = devm_devfreq_add_governor(&dpu_dvfs_probe_dev, &dpu_devfreq_gov);
	return ret;
}
