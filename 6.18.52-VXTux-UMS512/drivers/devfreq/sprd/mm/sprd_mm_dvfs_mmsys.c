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
 * The mmsys half of the memory-management DVFS driver.
 *
 * The mmsys block is the parent of the eight camera IPs: it owns the
 * global DVFS gate, the voltage readback and the power chain, and it
 * runs its own devfreq device on top of them.  The functions here carry
 * the stock blob's own names, and their behaviour is the decompiled
 * stock behaviour (donors/ghidra_out/mmdvfs_fns/).
 *
 * The mmsys register windows the stock blob maps at probe time:
 *
 *   0x62600000 (0x23d)	g_mmreg_map	  the shared mm register block
 *   0x322a0000 (0x151)	the top block	  via syscon
 *   0x62200000 (0x151)	mmsys_ctrl_reg	  the mm DVFS control block
 *   0x327d0000 (5)	mmsys_pwr_reg	  the mm power sequencing block
 *   0x327e0024 (5)	mmsys_clk_reg	  the mm clock control block
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
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/sysfs.h>

#include "sprd_mm_dvfs.h"

/* the mm register block */
extern void *g_mmreg_map;

/* serialises every access to the shared mm registers */
extern struct mutex mmsys_glob_reg_lock;

/* set while the mmsys block is powered on */
extern int mm_power_flag;

/* the mmsys power event chain */
extern struct blocking_notifier_head mmsys_notifier_list;

int mmsys_notify_call_chain(int state);
int mmsys_unregister_notifier(struct notifier_block *nb);
int register_ip_dvfs_ops(struct ip_dvfs_ops *ops);
int unregister_ip_dvfs_ops(struct ip_dvfs_ops *ops);
extern struct ip_dvfs_ops mmsys_dvfs_ops;
extern const struct of_device_id mmsys_dvfs_of_match[];
int sprd_mm_pw_notify_register(struct notifier_block *nb);
int sprd_mm_pw_notify_unregister(struct notifier_block *nb);
/*
 * The mmsys half's own entry points, under the stock blob's names.
 */
int mmsys_init(struct ip_dvfs_ops *ops);
int set_mmsys_sw_dvfs_en(struct ip_dvfs_ops *ops, int en);
int set_mmsys_dvfs_hold_en(struct ip_dvfs_ops *ops, int en);
int set_mmsys_dvfs_clk_gate_ctrl(struct ip_dvfs_ops *ops, int en);
int set_mmsys_dvfs_wait_window(struct ip_dvfs_ops *ops, u32 window);
int set_mmsys_dvfs_min_volt(struct ip_dvfs_ops *ops, u32 volt);
void mmsys_power_ctrl(struct ip_dvfs_ops *ops, int en);
int mmsys_dvfs_power(struct device *dev, int on);
int mmsys_dvfs_target(struct device *dev, unsigned long *freq, u32 flags);
int mmsys_dvfs_get_dev_status(struct device *dev,
			      struct devfreq_dev_status *status);
int mmsys_dvfs_get_cur_freq(struct device *dev, unsigned long *freq);
int mmsys_dvfs_init(void);
void mmsys_dvfs_exit(void);
struct ip_dvfs_ops *get_ip_dvfs_ops(const char *name);
int mmsys_register_notifier(struct notifier_block *nb);

/* the per-IP accessors, all with the stock's names */
int ip_dvfs_init(struct ip_dvfs_ops *ops);
int power_on_nb(struct ip_dvfs_ops *ops);
int power_off_nb(struct ip_dvfs_ops *ops);

/*
 * The mmsys register windows, mapped by mmsys_dvfs_probe().
 *
 * mmsys_top_regmap    the top DVFS controller, via the
 *                     "sprd,topdvfs_controller" phandle
 * mmsys_ctrl_reg      the mm DVFS control block
 * mmsys_pwr_reg       the mm power sequencing block
 * mmsys_clk_reg       the mm clock control block
 */
static struct regmap *mmsys_top_regmap;
static void __iomem *mmsys_ctrl_reg;
static void __iomem *mmsys_pwr_reg;
static void __iomem *mmsys_clk_reg;

/*
 * The mmsys-level knobs.
 *
 * These are the global switches that gate the whole camera DVFS path;
 * each is the stock function of the same name.
 */

/**
 * mmsys_init - the mmsys block's DVFS initialisation
 * @ops: the mmsys operations
 *
 * Marks the block powered on, opens the mm DVFS gate, clears the
 * top block's DVFS override and its software gate, and sets the clock
 * gate control in 0x34.
 */
int mmsys_init(struct ip_dvfs_ops *ops)
{
	(void)ops;

	mm_power_flag = 1;

	mutex_lock(&mmsys_glob_reg_lock);
	writel(readl(mmsys_ctrl_reg) | 0x80, mmsys_ctrl_reg);
	mutex_unlock(&mmsys_glob_reg_lock);

	mutex_lock(&mmsys_glob_reg_lock);
	if (mmsys_top_regmap)
		regmap_update_bits(mmsys_top_regmap, 0x18, 0x100000, 0);
	mutex_unlock(&mmsys_glob_reg_lock);

	mutex_lock(&mmsys_glob_reg_lock);
	if (mmsys_top_regmap)
		regmap_update_bits(mmsys_top_regmap, 0x150, 0x4, 0);
	mutex_unlock(&mmsys_glob_reg_lock);

	mutex_lock(&mmsys_glob_reg_lock);
	writel(readl(g_mmreg_map + 0x34) | 0x2, g_mmreg_map + 0x34);
	mutex_unlock(&mmsys_glob_reg_lock);

	pr_info("mmsys_init: done\n");
	return 0;
}

/**
 * set_mmsys_sw_dvfs_en - enable or disable the mmsys software DVFS
 * @ops: the mmsys operations
 * @en: non-zero to enable
 */
int set_mmsys_sw_dvfs_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;

	mutex_lock(&mmsys_glob_reg_lock);
	if (mmsys_top_regmap)
		regmap_update_bits(mmsys_top_regmap, 0x150, 0x4,
				   (en != 0) << 2);
	mutex_unlock(&mmsys_glob_reg_lock);

	pr_info("set_mmsys_sw_dvfs_en: done\n");
	return 0;
}

/**
 * set_mmsys_dvfs_hold_en - enable or disable the mmsys DVFS hold
 * @ops: the mmsys operations
 * @en: non-zero to hold
 */
int set_mmsys_dvfs_hold_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;

	pr_info("set_mmsys_dvfs_hold_en: %d\n", en);
	if (en)
		writel(readl(g_mmreg_map) | 1, g_mmreg_map);
	else
		writel(readl(g_mmreg_map) & 0xfffffffe, g_mmreg_map);

	pr_info("set_mmsys_dvfs_hold_en: done\n");
	return 0;
}

/**
 * set_mmsys_dvfs_clk_gate_ctrl - control the mmsys DVFS clock gate
 * @ops: the mmsys operations
 * @en: non-zero to gate
 */
int set_mmsys_dvfs_clk_gate_ctrl(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;

	pr_info("mm_dvfs_auto_gate_sel: %d\n", en);
	mutex_lock(&mmsys_glob_reg_lock);
	if (en)
		writel(readl(g_mmreg_map + 0x34) | 1, g_mmreg_map + 0x34);
	else
		writel(readl(g_mmreg_map + 0x34) & 0xfffffffe, g_mmreg_map + 0x34);
	mutex_unlock(&mmsys_glob_reg_lock);

	pr_info("set_mmsys_dvfs_clk_gate_ctrl: done\n");
	return 0;
}

/**
 * set_mmsys_dvfs_wait_window - set the mmsys DVFS wait window
 * @ops: the mmsys operations
 * @window: the window, in clock cycles
 */
int set_mmsys_dvfs_wait_window(struct ip_dvfs_ops *ops, u32 window)
{
	(void)ops;

	writel(window, g_mmreg_map + 4);
	pr_info("set_mmsys_dvfs_wait_window: done\n");
	return 0;
}

/**
 * set_mmsys_dvfs_min_volt - set the mmsys minimum voltage level
 * @ops: the mmsys operations
 * @volt: the voltage level, 0..7
 *
 * The stock blob ORs the level into the low three bits of 0x10, so the
 * minimum can only be raised, never lowered, from software.
 */
int set_mmsys_dvfs_min_volt(struct ip_dvfs_ops *ops, u32 volt)
{
	(void)ops;

	writel(readl(g_mmreg_map + 0x10) | (volt & 0x7),
	       g_mmreg_map + 0x10);
	pr_info("set_mmsys_dvfs_min_volt: done\n");
	return 0;
}

/**
 * mmsys_power_ctrl - take the mmsys block in or out of the DVFS path
 * @ops: the mmsys operations
 * @en: non-zero to power on
 *
 * Opens the power and clock blocks and then broadcasts the power event
 * on the mmsys chain.
 */
void mmsys_power_ctrl(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;

	writel(0x03208004, mmsys_clk_reg);
	writel(0x0003c280, mmsys_pwr_reg);

	pr_info("mmsys_power_ctrl: done\n");
	mmsys_notify_call_chain(en == 0 ? 2 : 1);
}

/**
 * mmsys_dvfs_power - broadcast an mmsys power event
 * @dev: the mmsys device
 * @on: non-zero for power on
 *
 * The devfreq profile's power callback: it only raises the mmsys power
 * event, which mmsys_power_ctrl and the per-IP notifier blocks act on.
 */
int mmsys_dvfs_power(struct device *dev, int on)
{
	(void)dev;

	pr_info("mmsys_dvfs_power: %d\n", on);
	mmsys_notify_call_chain(on == 1 ? 1 : 2);

	pr_info("mmsys_dvfs_power: done\n");
	return 0;
}

/**
 * mmsys_dvfs_target - the mmsys devfreq target callback
 * @dev: the mmsys device
 * @freq: the requested frequency
 * @flags: the devfreq flags
 *
 * The mmsys block has no frequency table of its own: its voltage is
 * driven by whichever camera IP is running, so the stock blob only logs
 * the request and leaves the clocks alone.
 */
int mmsys_dvfs_target(struct device *dev, unsigned long *freq, u32 flags)
{
	(void)dev;
	(void)freq;
	(void)flags;

	pr_info("mmsys_dvfs_target\n");
	return 0;
}

/**
 * mmsys_dvfs_get_dev_status - the mmsys devfreq status callback
 * @dev: the mmsys device
 * @status: the status to fill in
 *
 * 6.18 shrank devfreq_dev_status to total_time/busy_time/
 * current_frequency plus a driver-private pointer.
 */
int mmsys_dvfs_get_dev_status(struct device *dev,
			      struct devfreq_dev_status *status)
{
	(void)dev;
	(void)status;

	pr_info("mmsys_dvfs_get_dev_status\n");
	return 0;
}

/**
 * mmsys_dvfs_get_cur_freq - the mmsys devfreq current-frequency callback
 * @dev: the mmsys device
 * @freq: where the frequency is stored
 */
int mmsys_dvfs_get_cur_freq(struct device *dev, unsigned long *freq)
{
	(void)dev;
	(void)freq;

	pr_info("mmsys_dvfs_get_cur_freq\n");
	return 0;
}

/*
 * The mmsys operations' own slots.
 *
 * The stock's mmsys ops table points at the stock-named functions above
 * (set_mmsys_sw_dvfs_en, set_mmsys_dvfs_hold_en, ...).  The port's
 * struct ip_dvfs_ops has a slot for each of them, and these wrappers
 * are what the table refers to, so the same knobs are reachable both
 * by name and through the ops table.
 */
int mmsys_set_hw_dvfs_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mm_sw_dvfs_onoff(en);
	return 0;
}

int mmsys_set_freq_upd_en_byp(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(9), en ? BIT(9) : 0);
	return 0;
}

int mmsys_set_freq_upd_delay_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(8), en ? BIT(8) : 0);
	return 0;
}

int mmsys_set_freq_upd_hdsk_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(10), en ? BIT(10) : 0);
	return 0;
}

int mmsys_set_sw_trig_en(struct ip_dvfs_ops *ops, int en)
{
	(void)ops;
	mmreg_update(0x220, BIT(2), en ? BIT(2) : 0);
	return 0;
}

int mmsys_power_on(struct ip_dvfs_ops *ops)
{
	(void)ops;
	mm_power_flag = 1;
	return 0;
}

int mmsys_power_off(struct ip_dvfs_ops *ops)
{
	(void)ops;
	mm_power_flag = 0;
	return 0;
}

int mmsys_get_ip_status(struct ip_dvfs_ops *ops, struct ip_dvfs_status *status)
{
	if (!ops || !status)
		return -EINVAL;

	memset(status, 0, sizeof(*status));
	return 0;
}

int mmsys_update_target_freq(struct ip_dvfs_ops *ops, u64 freq, u32 index,
			     int is_fix)
{
	(void)ops;
	(void)freq;
	(void)index;
	(void)is_fix;

	mmsys_adjust_target_freq();
	return 0;
}

int mmsys_top_current_volt(struct ip_dvfs_ops *ops, u32 *volt)
{
	if (!ops || !volt)
		return -EINVAL;

	*volt = top_mm_dvfs_current_volt();
	return 0;
}

int mmsys_mm_current_volt(struct ip_dvfs_ops *ops, u32 *volt)
{
	if (!ops || !volt)
		return -EINVAL;

	*volt = mm_dvfs_read_current_volt();
	return 0;
}

/*
 * The mmsys power-event callback.
 *
 * The stock notifier runs one of the IP's power_on/power_off slots on
 * the way up and down, depending on the event.
 */
static int mmsys_dvfs_notify_callback(struct notifier_block *nb,
				      unsigned long action, void *data)
{
	struct ip_dvfs_dev *ip_dev =
		container_of(nb, struct ip_dvfs_dev, notifier);
	struct ip_dvfs_ops *ops = ip_dev ? ip_dev->ops : NULL;

	(void)data;

	switch (action) {
	case SPRD_MM_DVFS_NOTIFY_POWER_ON:
		if (ops && ops->power_on)
			ops->power_on(ops);
		break;
	case SPRD_MM_DVFS_NOTIFY_POWER_OFF:
		if (ops && ops->power_off)
			ops->power_off(ops);
		break;
	default:
		return -EINVAL;
	}

	return NOTIFY_OK;
}

/*
 * The mmsys governor callbacks.
 *
 * The stock driver creates the mmsys sysfs groups from the governor's
 * start event and removes them on stop, so the attributes exist only
 * while the governor runs.  The governor's get_target clamps the
 * request into the block's own frequency window before passing it on.
 */

/**
 * mmsys_dvfs_gov_get_target - the mmsys governor's get_target callback
 * @df: the devfreq device
 * @freq: the requested frequency, clamped on the way out
 *
 * The request is raised to the block's minimum and lowered to its
 * maximum, exactly as the stock blob does.
 */
static int mmsys_dvfs_gov_get_target(struct devfreq *df, unsigned long *freq)
{
	struct devfreq *devfreq = df;

	pr_info("mmsys_dvfs_gov_get_target\n");

	if (devfreq->governor) {
		*freq = max(*freq, devfreq->scaling_min_freq);
		*freq = min(*freq, devfreq->scaling_max_freq);
	}

	pr_info("mmsys_dvfs_gov_get_target: %lu\n", *freq);
	return 0;
}

/**
 * mmsys_dvfs_gov_event_handler - the mmsys governor's event callback
 * @df: the devfreq device
 * @event: the devfreq governor event
 * @data: the event data
 *
 * Creates the sysfs groups on DEVFREQ_GOV_START and removes them on
 * DEVFREQ_GOV_STOP.
 */
static int mmsys_dvfs_gov_event_handler(struct devfreq *df, unsigned int event,
					void *data)
{
	(void)data;

	pr_info("mmsys_dvfs_gov_event_handler\n");

	switch (event) {
	case DEVFREQ_GOV_STOP:
		mm_dvfs_sysfs_remove_groups(df->dev.parent, 1);
		break;
	case DEVFREQ_GOV_START:
		mm_dvfs_sysfs_create_groups(df->dev.parent, 1);
		break;
	default:
		break;
	}

	return 0;
}

/*
 * The mmsys devfreq profile and its notifier.
 */

/**
 * dvfs_power_notifier - the module-wide mmsys power notifier
 *
 * The stock driver registers one notifier at module init to reprogram
 * the block whenever the mm power domain cycles.  The block's
 * initialisation runs through the mmsys operations, which is what
 * re-applies the register state after a power cycle.
 */
static struct notifier_block dvfs_power_notifier = {
	.notifier_call = mmsys_dvfs_notify_callback,
};

/*
 * mmsys_dvfs_profile - the mmsys devfreq profile
 *
 * A static, because the stock blob's copy is a file-scope object that
 * mmsys_dvfs_probe() hands to devm_devfreq_add_device().
 */
static struct devfreq_dev_profile mmsys_dvfs_profile = {
	.target = mmsys_dvfs_target,
	.get_dev_status = mmsys_dvfs_get_dev_status,
	.get_cur_freq = mmsys_dvfs_get_cur_freq,
};

/* the mmsys governor, registered by mmsys_dvfs_init() */
struct devfreq_governor mmsys_dvfs_gov = {
	.name = "mmsys_governor",
	.get_target_freq = mmsys_dvfs_gov_get_target,
	.event_handler = mmsys_dvfs_gov_event_handler,
};

/**
 * mmsys_dvfs_probe - bring the mmsys devfreq device up
 * @pdev: the platform device
 *
 * Allocates the runtime state, maps the four register windows, adds the
 * operating points from the device tree and creates the devfreq device.
 */
static int mmsys_dvfs_probe(struct platform_device *pdev)
{
	struct ip_dvfs_dev *ip_dev;
	struct device_node *dn;
	int ret;

	ip_dev = devm_kzalloc(&pdev->dev, sizeof(*ip_dev), GFP_KERNEL);
	if (!ip_dev)
		return -ENOMEM;

	mutex_init(&ip_dev->lock);

	/* the operating points come from the device tree */
	ret = dev_pm_opp_of_add_table(&pdev->dev);
	if (ret)
		return ret;

	g_mmreg_map = ioremap(0x62600000, 0x23d);
	if (!g_mmreg_map) {
		pr_err("mmsys_dvfs: cannot map the mm registers\n");
		return 0;
	}

	dn = of_parse_phandle(pdev->dev.of_node,
			      "sprd,topdvfs_controller", 0);
	if (!dn) {
		pr_err("mmsys_dvfs: no topdvfs controller\n");
		return -EINVAL;
	}

	mmsys_top_regmap = syscon_node_to_regmap(dn);
	if (IS_ERR(mmsys_top_regmap)) {
		pr_err("mmsys_dvfs: no topdvfs regmap\n");
		return -ENODEV;
	}

	mmsys_ctrl_reg = ioremap(0x62200000, 0x151);
	if (!mmsys_ctrl_reg) {
		pr_err("mmsys_dvfs: cannot map the control registers\n");
		return 0;
	}

	mmsys_pwr_reg = ioremap(0x327d0000, 5);
	if (!mmsys_pwr_reg) {
		pr_err("mmsys_dvfs: cannot map the power registers\n");
		return 0;
	}

	mmsys_clk_reg = ioremap(0x327e0024, 5);
	if (!mmsys_clk_reg) {
		pr_err("mmsys_dvfs: cannot map the clock registers\n");
		return 0;
	}

	platform_set_drvdata(pdev, ip_dev);
	ip_dev->devfreq = devm_devfreq_add_device(&pdev->dev,
						  &mmsys_dvfs_profile,
						  "mmsys_dvfs", NULL);
	if (IS_ERR(ip_dev->devfreq)) {
		dev_err(&pdev->dev,
			"failed to add devfreq dev with mmsys-dvfs governor\n");
		return PTR_ERR(ip_dev->devfreq);
	}

	ip_dev->ops = get_ip_dvfs_ops("MMSYS_DVFS_OPS");
	if (ip_dev->ops)
		mmsys_register_notifier(&ip_dev->notifier);

	return 0;
}

/**
 * mmsys_dvfs_remove - take the mmsys devfreq device down
 * @pdev: the platform device
 */
static void mmsys_dvfs_remove(struct platform_device *pdev)
{
	struct ip_dvfs_dev *ip_dev = platform_get_drvdata(pdev);

	pr_info("mmsys_dvfs_remove\n");

	if (ip_dev) {
		if (ip_dev->ops)
			mmsys_unregister_notifier(&ip_dev->notifier);
		if (!IS_ERR_OR_NULL(ip_dev->devfreq))
			devfreq_remove_device(ip_dev->devfreq);
	}
}

/* the mmsys platform driver, registered by mmsys_dvfs_init() */
struct platform_driver mmsys_dvfs_driver = {
	.probe = mmsys_dvfs_probe,
	.remove = mmsys_dvfs_remove,
	.driver = {
		.name = "mmsys-dvfs",
		.of_match_table = mmsys_dvfs_of_match,
	},
};

/* the mmsys device-tree match table */
const struct of_device_id mmsys_dvfs_of_match[] = {
	{ .compatible = "sprd,hwdvfs-mmsys" },
	{ }
};
MODULE_DEVICE_TABLE(of, mmsys_dvfs_of_match);

/**
 * mmsys_dvfs_init - register the mmsys half of the driver
 *
 * The stock blob's mmsys_dvfs_init(): the operations, then the power
 * notifier, then the governor, then the platform driver, unwinding each
 * step if the next one fails.
 */
int mmsys_dvfs_init(void)
{
	int ret;

	ret = register_ip_dvfs_ops(&mmsys_dvfs_ops);
	if (ret) {
		pr_err("mmsys_dvfs_init: ops %d\n", ret);
		return ret;
	}

	ret = sprd_mm_pw_notify_register(&dvfs_power_notifier);
	if (ret) {
		pr_err("mmsys_dvfs_init: power notifier %d\n", ret);
		return ret;
	}

	ret = devfreq_add_governor(&mmsys_dvfs_gov);
	if (ret) {
		pr_err("mmsys_dvfs_init: governor %d\n", ret);
		return ret;
	}

	ret = platform_driver_register(&mmsys_dvfs_driver);
	if (ret) {
		devfreq_remove_governor(&mmsys_dvfs_gov);
		pr_err("mmsys_dvfs_init: driver %d\n", ret);
		return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(mmsys_dvfs_init);

/**
 * mmsys_dvfs_exit - unregister the mmsys half of the driver
 */
void mmsys_dvfs_exit(void)
{
	int ret;

	platform_driver_unregister(&mmsys_dvfs_driver);

	ret = devfreq_remove_governor(&mmsys_dvfs_gov);
	if (ret)
		pr_err("mmsys_dvfs_exit: governor %d\n", ret);

	ret = unregister_ip_dvfs_ops(&mmsys_dvfs_ops);
	if (ret)
		pr_err("mmsys_dvfs_exit: ops %d\n", ret);

	ret = sprd_mm_pw_notify_unregister(&dvfs_power_notifier);
	if (ret)
		pr_err("mmsys_dvfs_exit: power notifier %d\n", ret);
}
EXPORT_SYMBOL_GPL(mmsys_dvfs_exit);