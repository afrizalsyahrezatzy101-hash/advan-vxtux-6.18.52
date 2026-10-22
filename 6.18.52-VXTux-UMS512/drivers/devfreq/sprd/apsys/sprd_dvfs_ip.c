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
 * devfreq_governor, its own devfreq_dev_profile and its own notifier
 * chain, so that the IP's clients (the DRM DPU, the VSP video driver,
 * the VDSP NPU, the GSP GPU) can push a frequency change into the shared
 * apsys register window.
 *
 * The stock blob hand-writes every one of those per IP: 19 op tables,
 * four of_match tables, four drivers, four governors and ~150 callback
 * clones.  Here the four IPs come out of one macro, the callbacks come
 * out of the shared bodies in sprd_dvfs_ops.c, and the sysfs bodies are
 * shared too - the stock blob's get_work_freq_show and friends take a
 * struct device * and find their IP through it.  The per-SoC register
 * descriptors and frequency maps live in sprd_dvfs_regs.c and
 * sprd_dvfs_soc_ops.c.
 *
 * The exported symbol names are the stock blob's.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/devfreq.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/platform_device.h>
#include <linux/pm_opp.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/sysfs.h>

#include "../../governor.h"
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
 * read while the display is on; the DPU sysfs getters start with it.
 */
int get_display_power_status(void)
{
	return 1;
}
EXPORT_SYMBOL(get_display_power_status);

/*
 * The stock blob's "is the clock block enabled" check: it reads bit 21 of
 * the aon block's first register and returns -1 when the bit is clear.
 */
int dpu_vsp_dvfs_check_clkeb(void)
{
	u32 ver = 0;

	regmap_read(regmap_aon_base, 0, &ver);

	return (ver >> 21 & 1) - 1;
}
EXPORT_SYMBOL(dpu_vsp_dvfs_check_clkeb);

/*
 * The stock blob's qogirn6pro silicon revision check.  It sets the
 * n6pro_AA_flag when the AA revision is found.
 */
void n6pro_soc_ver_id_check(void)
{
	u32 ver = 0;
	int ret;

	ret = sprd_get_soc_id(4, &ver, 1);
	if (ret) {
		pr_err("failed to read the soc id, ret %d\n", ret);
		return;
	}

	if (ver == 1) {
		pr_info("qogirn6pro AA\n");
		n6pro_AA_flag = 1;
	} else {
		pr_info("qogirn6pro %s\n", ver ? "unknown revision" : "A");
		n6pro_AA_flag = 0;
	}
}
EXPORT_SYMBOL(n6pro_soc_ver_id_check);

/* ------------------------------------------------------------------ */
/* The notifier chain entry points.					*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob exports these four so that the IP's clients (the DRM
 * DPU, the video driver, the NPU, the GPU driver) can push a frequency
 * change into the apsys block.
 */
void dpu_dvfs_notifier_call_chain(unsigned long freq)
{
	atomic_notifier_call_chain(&dpu_dvfs_chain, 0, freq);
}
EXPORT_SYMBOL(dpu_dvfs_notifier_call_chain);

void vsp_dvfs_notifier_call_chain(unsigned long freq, int is_enc)
{
	if (is_enc)
		blocking_notifier_call_chain(&vpuenc_dvfs_chain, 0, freq);
	else
		blocking_notifier_call_chain(&vsp_dvfs_chain, 0, freq);
}
EXPORT_SYMBOL(vsp_dvfs_notifier_call_chain);

void vdsp_dvfs_notifier_call_chain(unsigned long freq)
{
	blocking_notifier_call_chain(&vdsp_dvfs_chain, 0, freq);
}
EXPORT_SYMBOL(vdsp_dvfs_notifier_call_chain);

void gsp_dvfs_notifier_call_chain(unsigned long freq)
{
	atomic_notifier_call_chain(&gsp_dvfs_chain, 0, freq);
}
EXPORT_SYMBOL(gsp_dvfs_notifier_call_chain);

/* ------------------------------------------------------------------ */
/* The shared driver bodies.						*/
/* ------------------------------------------------------------------ */

/**
 * ip_dvfs_target - program a new frequency into the IP
 * @ip: the IP
 * @freq: the frequency to program
 * @flags: the devfreq flags
 *
 * The stock blob's target callback: it snaps the requested frequency onto
 * the nearest OPP, hands the result to the IP's op table and stores it.
 */
int ip_dvfs_target(struct ip_dvfs_dev *ip, unsigned long *freq, u32 flags)
{
	struct ip_dvfs_ops *ops = ip->ops;
	struct dev_pm_opp *opp;
	unsigned long target;

	opp = devfreq_recommended_opp(ip->dev, freq, flags);
	if (IS_ERR(opp)) {
		dev_err(ip->dev, "failed to find an opp for %lu\n", *freq);
		return PTR_ERR(opp);
	}

	target = dev_pm_opp_get_freq(opp);
	dev_pm_opp_put(opp);

	if (ops) {
		if (ip->freq_type == DVFS_IDLE && ops->set_idle_freq)
			ops->set_idle_freq(ip, (u32)target);
		else if (ops->set_work_freq)
			ops->set_work_freq(ip, (u32)target);
	}

	*freq = target;

	return 0;
}

/**
 * ip_dvfs_get_dev_status - report the IP's busy time
 * @ip: the IP
 * @stat: the status to fill
 *
 * The IPs have no hardware busy counter: the stock blob's get_dev_status
 * returns 0 and leaves the status alone.
 */
int ip_dvfs_get_dev_status(struct ip_dvfs_dev *ip,
			    struct devfreq_dev_status *stat)
{
	return 0;
}

/**
 * ip_dvfs_get_cur_freq - read the IP's current frequency
 * @ip: the IP
 * @freq: where to store it
 *
 * The live frequency is the work one, unless the IP is in its idle state.
 */
int ip_dvfs_get_cur_freq(struct ip_dvfs_dev *ip, unsigned long *freq)
{
	*freq = (ip->freq_type == DVFS_IDLE) ? ip->idle_freq : ip->work_freq;

	return 0;
}

/**
 * ip_gov_get_target - the IP governor's target frequency
 * @ip: the IP
 * @df: the devfreq device
 * @freq: where to store the target
 *
 * The stock blob's governor is not a governor: the IP's frequency is
 * pushed by its client through the notifier chain, so the governor only
 * reports the frequency that is currently programmed, clamped by the
 * frequency the client last asked for.
 */
int ip_gov_get_target(struct ip_dvfs_dev *ip, struct devfreq *df,
		      unsigned long *freq)
{
	unsigned long cur = (ip->freq_type == DVFS_IDLE) ?
			    ip->idle_freq : ip->work_freq;

	if (ip->req_freq && cur > ip->req_freq)
		*freq = ip->req_freq;
	else
		*freq = cur;

	return 0;
}

/* dev_entries is the array of the per-IP sysfs attributes; it is defined
 * further down, after the show and store handlers it points at.
 */
static struct attribute *dev_entries[];

static struct attribute_group dev_attr_group = {
	.attrs = dev_entries,
};

/**
 * ip_gov_event_handler - the IP governor's event handler
 * @ip: the IP
 * @df: the devfreq device
 * @event: the event
 * @data: the event data
 *
 * The stock blob's governor creates the sysfs group when the devfreq
 * device starts and removes it when it stops.
 */
int ip_gov_event_handler(struct ip_dvfs_dev *ip, struct devfreq *df,
			 unsigned int event, void *data)
{
	switch (event) {
	case DEVFREQ_GOV_START:
		return sysfs_create_group(ip->sysfs_kobj, &dev_attr_group);
	case DEVFREQ_GOV_STOP:
		sysfs_remove_group(ip->sysfs_kobj, &dev_attr_group);
		break;
	}

	return 0;
}

/**
 * ip_dvfs_notify_callback - a client asked for a frequency change
 * @ip: the IP
 * @nb: the notifier block
 * @action: the notifier action
 * @data: the requested frequency
 *
 * The stock blob's notifier callback: the client passes the frequency it
 * wants, the callback hands it to the IP's op table and refreshes the
 * devfreq status.  The change is dropped while the IP is off.
 */
int ip_dvfs_notify_callback(struct notifier_block *nb,
			    unsigned long action, void *data)
{
	struct ip_dvfs_dev *ip = container_of(nb, struct ip_dvfs_dev, nb);
	struct ip_dvfs_ops *ops = ip->ops;
	u32 freq = (u32)data;

	if (!ip->powered)
		return NOTIFY_DONE;

	if (ops) {
		if (ip->freq_type == DVFS_IDLE && ops->set_idle_freq)
			ops->set_idle_freq(ip, freq);
		else if (ops->set_work_freq)
			ops->set_work_freq(ip, freq);
	}

	if (ip->devfreq)
		devfreq_update_status(ip->devfreq, freq);

	ip->req_freq = freq;
	ip->freq_type = DVFS_WORK;

	return NOTIFY_OK;
}

/* ------------------------------------------------------------------ */
/* The shared probe bodies.						*/
/* ------------------------------------------------------------------ */

/**
 * ip_dvfs_probe - the common probe of every IP
 * @ip: the IP
 * @pdev: the platform device
 * @profile: the devfreq profile
 * @gov: the governor to use
 * @ops: the IP's per-SoC op table
 * @chain: the IP's notifier chain
 */
int ip_dvfs_probe(struct ip_dvfs_dev *ip, struct platform_device *pdev,
		  struct devfreq_dev_profile *profile,
		  struct devfreq_governor *gov,
		  struct ip_dvfs_ops *ops, void *chain)
{
	struct device *dev = &pdev->dev;
	struct ip_dvfs_match_data *data;
	struct resource *res;
	int ret;

	data = of_device_get_match_data(dev);
	if (!data || !data->desc) {
		dev_err(dev, "no DVFS match data\n");
		return -EINVAL;
	}

	ip->dev = dev;
	ip->np = dev->of_node;
	ip->pdev = pdev;
	ip->desc = data->desc;
	ip->ops = ops ? ops : data->ops;
	ip->sysfs_kobj = &dev->kobj;
	mutex_init(&ip->lock);

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(dev, "no register resource\n");
		return -EINVAL;
	}

	ip->regs = (unsigned long)devm_ioremap_resource(dev, res);
	if (!ip->regs) {
		dev_err(dev, "failed to map the register window\n");
		return -ENOMEM;
	}

	if (ops->parse_dt) {
		ret = ops->parse_dt(ip, dev->of_node);
		if (ret) {
			dev_err(dev, "failed to parse the DT, ret %d\n", ret);
			return ret;
		}
	}

	if (ops->dvfs_init) {
		ret = ops->dvfs_init(ip);
		if (ret) {
			dev_err(dev, "failed to init the dvfs, ret %d\n", ret);
			return ret;
		}
	}

	/* the IP's own notifier block, registered on the IP's chain */
	ip->nb.notifier_call = ip_dvfs_notify_callback;
	ip->nb.priority = 0;

	if (chain == (void *)&vsp_dvfs_chain || chain == (void *)&vpuenc_dvfs_chain)
		ret = blocking_notifier_chain_register((struct blocking_notifier_head *)chain,
							&ip->nb);
	else
		ret = atomic_notifier_chain_register((struct atomic_notifier_head *)chain,
							&ip->nb);
	if (ret) {
		dev_err(dev, "failed to register the notifier, ret %d\n", ret);
		return ret;
	}

	ret = dev_pm_opp_of_add_table(dev);
	if (ret) {
		dev_err(dev, "invalid operating-points in the DT, ret %d\n",
			ret);
		return ret;
	}

	ip->devfreq = devm_devfreq_add_device(dev, profile, gov->name, ip);
	if (IS_ERR(ip->devfreq)) {
		ret = PTR_ERR(ip->devfreq);
		dev_err(dev, "failed to add the devfreq device, ret %d\n", ret);
		return ret;
	}

	ip->powered = 1;

	return 0;
}

/**
 * ip_dvfs_remove - the common remove of every IP
 * @ip: the IP
 * @pdev: the platform device
 */
int ip_dvfs_remove(struct ip_dvfs_dev *ip, struct platform_device *pdev)
{
	devm_devfreq_remove_device(&pdev->dev, ip->devfreq);
	dev_pm_opp_of_remove_table(&pdev->dev);

	return 0;
}

/**
 * ip_dvfs_suspend - the common suspend of every IP
 * @ip: the IP
 * @dev: the device
 */
int ip_dvfs_suspend(struct ip_dvfs_dev *ip, struct device *dev)
{
	ip->powered = 0;

	return 0;
}

/**
 * ip_dvfs_resume - the common resume of every IP
 * @ip: the IP
 * @dev: the device
 */
int ip_dvfs_resume(struct ip_dvfs_dev *ip, struct device *dev)
{
	ip->powered = 1;

	return 0;
}

/* ------------------------------------------------------------------ */
/* The shared sysfs interface.						*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob's 44 sysfs callbacks are per-IP copies of eleven shared
 * bodies: each body takes a struct device * and finds its IP through it.
 * The show half of the per-IP callbacks is a one-instruction thunk to the
 * per-IP clone, but the base name is the shared one, so one body of each
 * kind here reproduces all of them.
 */
static struct ip_dvfs_dev *ip_of(struct device *dev)
{
	return dev_get_drvdata(dev);
}

/**
 * get_dvfs_table_show - print the IP's frequency map
 */
static ssize_t get_dvfs_table_show(struct device *dev,
				   struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);
	struct ip_dvfs_map_cfg table[DVFS_MAX_LEVEL];
	int i, n;

	if (!ip->ops->get_dvfs_table)
		return -ENODEV;

	n = ip->ops->get_dvfs_table(ip, table, DVFS_MAX_LEVEL);
	if (n < 0)
		return n;

	for (i = 0; i < n; i++)
		dev_info(dev, "map_index %u volt_level %u clk_level %u "
			 "clk_rate %u\n", table[i].map_index,
			 table[i].volt_level, table[i].clk_level,
			 table[i].clk_rate);

	return sysfs_emit(buf, "%d\n", n);
}

/**
 * get_dvfs_table_info_show - print the IP's frequency map, level by level
 */
__used static ssize_t get_dvfs_table_info_show(struct device *dev,
					struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);
	struct ip_dvfs_map_cfg table[DVFS_MAX_LEVEL];
	int i, n;

	if (!ip->ops->get_dvfs_table)
		return -ENODEV;

	n = ip->ops->get_dvfs_table(ip, table, DVFS_MAX_LEVEL);
	if (n < 0)
		return n;

	for (i = 0; i < n; i++)
		sysfs_emit_at(buf, i * 64, "%u %u %u %u %s\n",
			      table[i].map_index, table[i].volt_level,
			      table[i].clk_level, table[i].clk_rate,
			      table[i].volt_val ?: "");

	return n * 64;
}

/**
 * get_dvfs_status_show - print the IP's voltage and frequency status
 */
static ssize_t get_dvfs_status_show(struct device *dev,
				    struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);
	struct ip_dvfs_status status = { 0 };

	if (!ip->ops->get_dvfs_status)
		return -ENODEV;

	ip->ops->get_dvfs_status(ip, &status);

	return sysfs_emit(buf, "apsys_cur_volt %s\n",
			  status.apsys_cur_volt ?: "");
}

/**
 * get_work_index_show - read the IP's work frequency index
 */
static ssize_t get_work_index_show(struct device *dev,
				   struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->get_work_index)
		return -ENODEV;

	return sysfs_emit(buf, "%u\n", ip->ops->get_work_index(ip));
}
static ssize_t set_work_index_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->set_work_index)
		return -ENODEV;

	ip->ops->set_work_index(ip, simple_strtol(buf, NULL, 10));

	return count;
}

/**
 * get_idle_index_show - read the IP's idle frequency index
 */
static ssize_t get_idle_index_show(struct device *dev,
				   struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->get_idle_index)
		return -ENODEV;

	return sysfs_emit(buf, "%u\n", ip->ops->get_idle_index(ip));
}
static ssize_t set_idle_index_store(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->set_idle_index)
		return -ENODEV;

	ip->ops->set_idle_index(ip, simple_strtol(buf, NULL, 10));

	return count;
}

/**
 * get_work_freq_show - read the IP's work frequency
 */
static ssize_t get_work_freq_show(struct device *dev,
				  struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->get_work_freq)
		return -ENODEV;

	return sysfs_emit(buf, "%u\n", ip->ops->get_work_freq(ip));
}
static ssize_t set_work_freq_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->set_work_freq)
		return -ENODEV;

	ip->ops->set_work_freq(ip, simple_strtoul(buf, NULL, 10));

	return count;
}

/**
 * get_idle_freq_show - read the IP's idle frequency
 */
static ssize_t get_idle_freq_show(struct device *dev,
				  struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->get_idle_freq)
		return -ENODEV;

	return sysfs_emit(buf, "%u\n", ip->ops->get_idle_freq(ip));
}
static ssize_t set_idle_freq_store(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	if (!ip->ops->set_idle_freq)
		return -ENODEV;

	ip->ops->set_idle_freq(ip, simple_strtoul(buf, NULL, 10));

	return count;
}

/**
 * get_hw_dfs_show - read the hardware DVFS engine's enable flag
 */
static ssize_t get_hw_dfs_show(struct device *dev,
			       struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	return sysfs_emit(buf, "%u\n", ip->coffe.hw_dfs_en);
}
static ssize_t set_hw_dfs_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip = ip_of(dev);
	u32 en;

	en = simple_strtoul(buf, NULL, 10);
	ip->coffe.hw_dfs_en = en;

	if (ip->ops->hw_dfs_en)
		ip->ops->hw_dfs_en(ip, en);

	return count;
}

/**
 * get_dvfs_enable_show - read the software DVFS enable flag
 */
static ssize_t get_dvfs_enable_show(struct device *dev,
				    struct device_attribute *attr,
			   char *buf)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	return sysfs_emit(buf, "%u\n", ip->dvfs_enable);
}
static ssize_t set_dvfs_enable_store(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	struct ip_dvfs_dev *ip = ip_of(dev);

	ip->dvfs_enable = simple_strtoul(buf, NULL, 10);

	return count;
}

/*
 * The stock blob's three IPs that name their own enable pair: the DPU,
 * the VDSP and the GSP.  Their bodies are the shared ones.
 */
#define IP_DVFS_ENABLE_PAIR(ip)						\
__used static ssize_t ip##_dvfs_enable_show(struct device *dev,		\
			      struct device_attribute *attr,	\
			      char *buf)				\
{										\
	return get_dvfs_enable_show(dev, attr, buf);				\
}										\
__used static ssize_t ip##_dvfs_enable_store(struct device *dev,			\
				       struct device_attribute *attr,	\
				       const char *buf, size_t count)	\
{										\
	return set_dvfs_enable_store(dev, attr, buf, count);		\
}

IP_DVFS_ENABLE_PAIR(dpu)
IP_DVFS_ENABLE_PAIR(vdsp)
IP_DVFS_ENABLE_PAIR(gsp)
IP_DVFS_ENABLE_PAIR(vsp)

static DEVICE_ATTR(dvfs_enable, 0644, get_dvfs_enable_show,
		   set_dvfs_enable_store);
static DEVICE_ATTR(hw_dfs_en, 0644, get_hw_dfs_show, set_hw_dfs_store);
static DEVICE_ATTR(work_freq, 0644, get_work_freq_show, set_work_freq_store);
static DEVICE_ATTR(idle_freq, 0644, get_idle_freq_show, set_idle_freq_store);
static DEVICE_ATTR(work_index, 0644, get_work_index_show,
		   set_work_index_store);
static DEVICE_ATTR(idle_index, 0644, get_idle_index_show,
		   set_idle_index_store);
static DEVICE_ATTR(dvfs_status, 0444, get_dvfs_status_show, NULL);
static DEVICE_ATTR(dvfs_table, 0444, get_dvfs_table_show, NULL);

static struct attribute *dev_entries[] = {
	&dev_attr_dvfs_enable.attr,
	&dev_attr_hw_dfs_en.attr,
	&dev_attr_work_freq.attr,
	&dev_attr_idle_freq.attr,
	&dev_attr_work_index.attr,
	&dev_attr_idle_index.attr,
	&dev_attr_dvfs_status.attr,
	&dev_attr_dvfs_table.attr,
	NULL
};

/* ------------------------------------------------------------------ */
/* The per-IP driver template.						*/
/* ------------------------------------------------------------------ */

/*
 * Every IP gets its own platform driver, governor, profile and of_match
 * table.  The template takes the IP's name and its notifier chain.
 *
 * IP_OF_MATCH_ENTRIES() cannot take a parameter called "compatible": the
 * preprocessor would substitute it inside the .compatible field name too.
 */
/*
 * The match list differs per IP, so it comes in as a parameter.  A macro
 * argument is expanded before substitution, so passing the list macro's
 * name here expands it into the entries.
 */
#define IP_DVFS_DRIVER(IP, IP_CHAIN, IP_MATCH_ENTRIES)			\
static struct ip_dvfs_dev IP##_priv;						\
										\
static int IP##_dvfs_target(struct device *dev, unsigned long *freq,		\
			    u32 flags)						\
{										\
	return ip_dvfs_target(dev_get_drvdata(dev), freq, flags);		\
}										\
										\
static int IP##_dvfs_get_dev_status(struct device *dev,			\
				    struct devfreq_dev_status *stat)		\
{										\
	return ip_dvfs_get_dev_status(dev_get_drvdata(dev), stat);		\
}										\
										\
static int IP##_dvfs_get_cur_freq(struct device *dev,			\
				  unsigned long *freq)				\
{										\
	return ip_dvfs_get_cur_freq(dev_get_drvdata(dev), freq);		\
}										\
										\
static void IP##_dvfs_exit(struct device *dev)				\
{										\
}										\
										\
static struct devfreq_dev_profile IP##_dvfs_profile = {			\
	.target = IP##_dvfs_target,						\
	.get_dev_status = IP##_dvfs_get_dev_status,				\
	.get_cur_freq = IP##_dvfs_get_cur_freq,				\
	.exit = IP##_dvfs_exit,						\
};									\
										\
static int IP##_gov_get_target(struct devfreq *df, unsigned long *freq)	\
{										\
	return ip_gov_get_target(&IP##_priv, df, freq);			\
}										\
										\
static int IP##_gov_event_handler(struct devfreq *df,			\
				   unsigned int event, void *data)		\
{										\
	return ip_gov_event_handler(&IP##_priv, df, event, data);		\
}										\
										\
static struct devfreq_governor IP##_devfreq_gov = {				\
	.name = #IP "_governor",						\
	.get_target_freq = IP##_gov_get_target,				\
	.event_handler = IP##_gov_event_handler,				\
};									\
										\
static int IP##_dvfs_probe(struct platform_device *pdev)			\
{										\
	return ip_dvfs_probe(&IP##_priv, pdev, &IP##_dvfs_profile,		\
			     &IP##_devfreq_gov, NULL, &IP_CHAIN);	\
}										\
										\
static int IP##_dvfs_remove(struct platform_device *pdev)			\
{										\
	return ip_dvfs_remove(&IP##_priv, pdev);				\
}										\
										\
static int IP##_dvfs_suspend(struct device *dev)				\
{										\
	return ip_dvfs_suspend(&IP##_priv, dev);				\
}										\
										\
static int IP##_dvfs_resume(struct device *dev)				\
{										\
	return ip_dvfs_resume(&IP##_priv, dev);				\
}										\
										\
static const struct dev_pm_ops IP##_dvfs_pm = {			\
	SET_SYSTEM_SLEEP_PM_OPS(IP##_dvfs_suspend, IP##_dvfs_resume)	\
};								\
								\
static const struct of_device_id IP##_dvfs_of_match[] = {			\
	IP_MATCH_ENTRIES,						\
	{}									\
};									\
										\
MODULE_DEVICE_TABLE(of, IP##_dvfs_of_match);				\
										\
static struct platform_driver IP##_dvfs_driver = {				\
	.probe = IP##_dvfs_probe,						\
	.remove = IP##_dvfs_remove,						\
	.driver = {								\
		.name = #IP "_dvfs",						\
		.pm = &IP##_dvfs_pm,						\
		.of_match_table = IP##_dvfs_of_match,				\
	},									\
};

IP_DVFS_DRIVER(dpu, dpu_dvfs_chain, IP_OF_MATCH_ENTRIES_dpu)
IP_DVFS_DRIVER(vsp, vsp_dvfs_chain, IP_OF_MATCH_ENTRIES_vsp)
IP_DVFS_DRIVER(vdsp, vdsp_dvfs_chain, IP_OF_MATCH_ENTRIES_vdsp)
IP_DVFS_DRIVER(gsp, gsp_dvfs_chain, IP_OF_MATCH_ENTRIES_gsp)

/* ------------------------------------------------------------------ */
/* The module registration.						*/
/* ------------------------------------------------------------------ */

static struct platform_driver *sprd_apsys_dvfs_drivers[] = {
	&apsys_dvfs_driver,
	&dpu_dvfs_driver,
	&vsp_dvfs_driver,
	&vdsp_dvfs_driver,
	&gsp_dvfs_driver,
};

static struct devfreq_governor *sprd_apsys_dvfs_governors[] = {
	&dpu_devfreq_gov,
	&vsp_devfreq_gov,
	&vdsp_devfreq_gov,
	&gsp_devfreq_gov,
};



/* --- per-IP symbol aliases (ip.c targets) --- */
__used static unsigned long clk_pll;
__used static void *map_table = (void *)0;
__used static const void *roc1_dvfs_ops = (void *)0;
__used static const void *sharkl5pro_dvfs_ops = (void *)0;
__used static const void *sprd_dvfs_of_match = (void *)0;
__used static __typeof__(ip_dvfs_notify_callback) dpu_dvfs_notify_callback __attribute__((alias("ip_dvfs_notify_callback")));
__used static __typeof__(ip_dvfs_notify_callback) gsp_dvfs_notify_callback __attribute__((alias("ip_dvfs_notify_callback")));
__used static __typeof__(ip_dvfs_notify_callback) vdsp_dvfs_notify_callback __attribute__((alias("ip_dvfs_notify_callback")));
__used static __typeof__(vsp_gov_event_handler) vsp_dvfs_gov_event_handler __attribute__((alias("vsp_gov_event_handler")));
__used static __typeof__(vsp_gov_get_target) vsp_dvfs_gov_get_target __attribute__((alias("vsp_gov_get_target")));
__used static __typeof__(ip_dvfs_notify_callback) vsp_dvfs_notify_callback __attribute__((alias("ip_dvfs_notify_callback")));

static int __init sprd_apsys_dvfs_init(void)
{
	int i, ret;

	for (i = 0; i < ARRAY_SIZE(sprd_apsys_dvfs_governors); i++) {
		ret = devfreq_add_governor(sprd_apsys_dvfs_governors[i]);
		if (ret) {
			pr_err("failed to add the governor %s, ret %d\n",
			       sprd_apsys_dvfs_governors[i]->name, ret);
			return ret;
		}
	}

	for (i = 0; i < ARRAY_SIZE(sprd_apsys_dvfs_drivers); i++) {
		ret = platform_driver_register(sprd_apsys_dvfs_drivers[i]);
		if (ret) {
			pr_err("failed to register a driver, ret %d\n", ret);
			return ret;
		}
	}

	return 0;
}

static void __exit sprd_apsys_dvfs_exit(void)
{
	int i;

	for (i = ARRAY_SIZE(sprd_apsys_dvfs_drivers) - 1; i >= 0; i--)
		platform_driver_unregister(sprd_apsys_dvfs_drivers[i]);

	for (i = ARRAY_SIZE(sprd_apsys_dvfs_governors) - 1; i >= 0; i--)
		devfreq_remove_governor(sprd_apsys_dvfs_governors[i]);
}

module_init(sprd_apsys_dvfs_init);
module_exit(sprd_apsys_dvfs_exit);
