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
 * The per-IP devfreq devices of the apsys block.
 *
 * The stock apsys-dvfs.ko owns one devfreq device per IP: dpu, vsp, vdsp
 * and gsp.  Each of them is a platform driver with its own of_match
 * table, its own devfreq_governor, its own devfreq_dev_profile and its
 * own notifier chain, so that the IP's clients (the DRM DPU, the VSP
 * video driver, the VDSP NPU, the GSP GPU) can push a frequency change
 * into the shared apsys register window.
 *
 * The per-SoC op tables are shared; only the register descriptor and the
 * frequency map differ per SoC, so the whole per-IP driver is generated
 * from one template in sprd_dvfs_ip.c.
 */

#ifndef __SPRD_DVFS_IP_H__
#define __SPRD_DVFS_IP_H__

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/types.h>

#include "sprd_dvfs_apsys.h"
#include "sprd_dvfs_soc.h"

/* the DT compatible of the apsys block itself; the IP drivers look it up */
#define APSYS_DVFS_NODE_NAME	"apsys-dvfs"

/* the largest frequency map any IP has */
#define DVFS_MAX_LEVEL		8

/**
 * struct ip_dvfs_ops - the per-IP callback table
 *
 * The slot order is the stock blob's, so that the per-SoC tables in
 * sprd_dvfs_soc_ops.c can be written out slot by slot.  Every entry is
 * optional; a NULL entry means "this IP does not implement it".
 */
struct ip_dvfs_ops {
	/* initialization interface */
	int  (*parse_dt)(struct ip_dvfs_dev *ip, struct device_node *np);
	int  (*parse_pll)(struct ip_dvfs_dev *ip, struct device *dev);
	int  (*dvfs_init)(struct ip_dvfs_dev *ip);
	void (*hw_dfs_en)(struct ip_dvfs_dev *ip, u32 en);

	/* work-idle dvfs index ops */
	void (*set_work_index)(struct ip_dvfs_dev *ip, u32 index);
	u32  (*get_work_index)(struct ip_dvfs_dev *ip);
	void (*set_idle_index)(struct ip_dvfs_dev *ip, u32 index);
	u32  (*get_idle_index)(struct ip_dvfs_dev *ip);

	/* work-idle dvfs freq ops */
	void (*set_work_freq)(struct ip_dvfs_dev *ip, u32 freq);
	u32  (*get_work_freq)(struct ip_dvfs_dev *ip);
	void (*set_idle_freq)(struct ip_dvfs_dev *ip, u32 freq);
	u32  (*get_idle_freq)(struct ip_dvfs_dev *ip);

	/* work-idle dvfs map ops */
	int  (*get_dvfs_table)(struct ip_dvfs_dev *ip,
			       struct ip_dvfs_map_cfg *table, int count);
	void (*set_dvfs_table)(struct ip_dvfs_dev *ip,
			       struct ip_dvfs_map_cfg *table);
	void (*get_dvfs_status)(struct ip_dvfs_dev *ip,
				struct ip_dvfs_status *status);

	/* coffe setting ops */
	void (*set_gfree_wait_delay)(struct ip_dvfs_dev *ip, u32 delay);
	void (*set_freq_upd_en_byp)(struct ip_dvfs_dev *ip, u32 en);
	void (*set_freq_upd_delay_en)(struct ip_dvfs_dev *ip, u32 en);
	void (*set_freq_upd_hdsk_en)(struct ip_dvfs_dev *ip, u32 en);
	void (*set_dvfs_swtrig_en)(struct ip_dvfs_dev *ip, u32 en);

	/* the vsp's extra: push a frequency change into the DVFS map */
	void (*updata_target_freq)(struct ip_dvfs_dev *ip, u32 freq,
				   int freq_type);
};

/**
 * struct ip_dvfs_match_data - the DT match data of one IP
 * @ver:       the SoC name, e.g. "sharkl5pro"
 * @max_level: the number of frequency levels this IP has
 * @ops:       the per-SoC op table
 * @desc:      the per-SoC register descriptor
 * @map:       the per-SoC frequency map
 * @map_count: the number of entries in @map
 */
struct ip_dvfs_match_data {
	const char *ver;
	u32 max_level;
	struct ip_dvfs_ops *ops;
	const struct soc_dvfs_desc *desc;
	const struct ip_dvfs_map_cfg *map;
	u32 map_count;
};

/* the ops list of one IP; one per IP, like the stock blob's */
extern struct list_head dpu_dvfs_head;
extern struct list_head vsp_dvfs_head;
extern struct list_head vdsp_dvfs_head;
extern struct list_head gsp_dvfs_head;

/* the notifier chains of the IPs */
extern struct atomic_notifier_head dpu_dvfs_chain;
extern struct blocking_notifier_head vsp_dvfs_chain;
extern struct blocking_notifier_head vpuenc_dvfs_chain;
extern struct atomic_notifier_head vdsp_dvfs_chain;
extern struct atomic_notifier_head gsp_dvfs_chain;

/* the shared callback bodies, in sprd_dvfs_ops.c */
int  ip_dvfs_parse_dt(struct ip_dvfs_dev *ip, struct device_node *np);
int  ip_dvfs_init(struct ip_dvfs_dev *ip);
void ip_hw_dfs_en(struct ip_dvfs_dev *ip, u32 en);
void ip_set_work_index(struct ip_dvfs_dev *ip, u32 index);
u32  ip_get_work_index(struct ip_dvfs_dev *ip);
void ip_set_idle_index(struct ip_dvfs_dev *ip, u32 index);
u32  ip_get_idle_index(struct ip_dvfs_dev *ip);
void ip_set_work_freq(struct ip_dvfs_dev *ip, u32 freq);
u32  ip_get_work_freq(struct ip_dvfs_dev *ip);
void ip_set_idle_freq(struct ip_dvfs_dev *ip, u32 freq);
u32  ip_get_idle_freq(struct ip_dvfs_dev *ip);
int  ip_get_dvfs_table(struct ip_dvfs_dev *ip, struct ip_dvfs_map_cfg *table,
		       int count);
void ip_set_dvfs_table(struct ip_dvfs_dev *ip, struct ip_dvfs_map_cfg *table);
void ip_get_dvfs_status(struct ip_dvfs_dev *ip, struct ip_dvfs_status *status);
void ip_set_gfree_wait_delay(struct ip_dvfs_dev *ip, u32 delay);
void ip_set_freq_upd_en_byp(struct ip_dvfs_dev *ip, u32 en);
void ip_set_freq_upd_delay_en(struct ip_dvfs_dev *ip, u32 en);
void ip_set_freq_upd_hdsk_en(struct ip_dvfs_dev *ip, u32 en);
void ip_set_dvfs_swtrig_en(struct ip_dvfs_dev *ip, u32 en);

/* the shared probe / governor bodies, in sprd_dvfs_ip.c */
int  ip_dvfs_probe(struct ip_dvfs_dev *ip, struct platform_device *pdev,
		    struct devfreq_dev_profile *profile,
		    struct devfreq_governor *gov,
		    struct ip_dvfs_ops *ops, struct notifier_head *chain);
int  ip_dvfs_remove(struct ip_dvfs_dev *ip, struct platform_device *pdev);
int  ip_dvfs_suspend(struct ip_dvfs_dev *ip, struct device *dev);
int  ip_dvfs_resume(struct ip_dvfs_dev *ip, struct device *dev);
int  ip_dvfs_target(struct ip_dvfs_dev *ip, unsigned long *freq, u32 flags);
int  ip_dvfs_get_dev_status(struct ip_dvfs_dev *ip,
			    struct devfreq_dev_status *stat);
int  ip_dvfs_get_cur_freq(struct ip_dvfs_dev *ip, unsigned long *freq);
int  ip_gov_get_target(struct ip_dvfs_dev *ip, struct devfreq *df,
		       unsigned long *freq);
int  ip_gov_event_handler(struct ip_dvfs_dev *ip, struct devfreq *df,
			  unsigned int event, void *data);
int  ip_dvfs_notify_callback(struct ip_dvfs_dev *ip,
			     struct notifier_block *nb,
			     unsigned long action, void *data);

/* ------------------------------------------------------------------ */
/* The DT match tables.						*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob's of_match tables, recovered from the DT compatible
 * strings it carries.
 *
 * NOTE: the macro parameter must NOT be called "compatible" - the
 * preprocessor would substitute it inside the .compatible field name
 * too, and the build would fail.
 */
#define IP_OF_MATCH_ENTRIES(ip)						\
	{ .compatible = "sprd,hwdvfs-" #ip "-sharkl5",			\
	  .data = sharkl5_##ip##_dvfs },				\
	{ .compatible = "sprd,hwdvfs-" #ip "-roc1",			\
	  .data = roc1_##ip##_dvfs },					\
	{ .compatible = "sprd,hwdvfs-" #ip "-sharkl5pro",		\
	  .data = sharkl5pro_##ip##_dvfs },				\
	{ .compatible = "sprd,hwdvfs-" #ip "-qogirl6",			\
	  .data = qogirl6_##ip##_dvfs },

#endif /* __SPRD_DVFS_IP_H__ */
