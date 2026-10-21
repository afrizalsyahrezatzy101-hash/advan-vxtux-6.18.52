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
 * The per-SoC op tables are shared; only the register descriptor and
 * the DT node differ per SoC, so the whole per-IP driver is generated
 * from one template below, exactly like the mm driver's IP_DRIVER.
 */

#ifndef __SPRD_DVFS_IP_H__
#define __SPRD_DVFS_IP_H__

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/types.h>

#include "sprd_dvfs_soc.h"

/* the DT compatible of the apsys block itself; the IP drivers look it up */
#define APSYS_DVFS_NODE_NAME	"apsys-dvfs"

/* the ops list of one IP; one per IP, like the stock blob's */
#define IP_DVFS_HEAD_DECL(ip)	extern struct list_head ip##_dvfs_head;

/**
 * struct ip_dvfs_ops - the per-IP callback table
 *
 * The slot order is the stock blob's, so that the per-SoC op tables
 * below can be written out slot by slot.  Every entry is optional; a
 * NULL entry means "this SoC does not implement it".
 */
struct ip_dvfs_ops {
	/* initialization interface */
	int  (*parse_dt)(struct ip_dvfs_priv *priv, struct device_node *np);
	int  (*parse_pll)(struct ip_dvfs_priv *priv, struct device *dev);
	int  (*dvfs_init)(struct ip_dvfs_priv *priv);
	void (*hw_dfs_en)(struct ip_dvfs_priv *priv, u32 dfs_en);

	/* work-idle dvfs index ops */
	void (*set_work_index)(struct ip_dvfs_priv *priv, int index);
	int  (*get_work_index)(struct ip_dvfs_priv *priv);
	void (*set_idle_index)(struct ip_dvfs_priv *priv, int index);
	int  (*get_idle_index)(struct ip_dvfs_priv *priv);

	/* work-idle dvfs freq ops */
	void (*set_work_freq)(struct ip_dvfs_priv *priv, u32 freq);
	u32  (*get_work_freq)(struct ip_dvfs_priv *priv);
	void (*set_idle_freq)(struct ip_dvfs_priv *priv, u32 freq);
	u32  (*get_idle_freq)(struct ip_dvfs_priv *priv);

	/* work-idle dvfs map ops */
	int  (*get_dvfs_table)(struct ip_dvfs_priv *priv,
			       struct ip_dvfs_map_cfg *dvfs_table);
	void (*set_dvfs_table)(struct ip_dvfs_priv *priv,
			       struct ip_dvfs_map_cfg *dvfs_table);
	void (*get_dvfs_status)(struct ip_dvfs_priv *priv,
				struct ip_dvfs_status *dvfs_status);

	/* coffe setting ops */
	void (*set_gfree_wait_delay)(struct ip_dvfs_priv *priv, u32 para);
	void (*set_freq_upd_en_byp)(struct ip_dvfs_priv *priv, u32 enable);
	void (*set_freq_upd_delay_en)(struct ip_dvfs_priv *priv, u32 enable);
	void (*set_freq_upd_hdsk_en)(struct ip_dvfs_priv *priv, u32 enable);
	void (*set_dvfs_swtrig_en)(struct ip_dvfs_priv *priv, u32 enable);
};

/**
 * struct ip_dvfs_match_data - the DT match data of one IP
 * @ver:       the SoC name, e.g. "sharkl5pro"
 * @max_level: the number of frequency levels this IP has
 * @ops:       the per-SoC op table
 */
struct ip_dvfs_match_data {
	const char *ver;
	u32 max_level;
	struct ip_dvfs_ops *ops;
};

/*
 * The runtime state of one IP.
 *
 * The stock blob keeps a private structure per IP whose layout differs
 * slightly (the vsp one is bigger, because it also carries the encoder
 * state).  Only the fields the shared callback bodies actually touch
 * are needed here, so one type serves all four IPs.
 */
struct ip_dvfs_priv {
	struct device *dev;		/* the IP's platform device	*/
	struct device_node *np;		/* its DT node			*/
	struct apsys_dev *apsys;	/* the shared apsys block	*/
	const struct soc_dvfs_desc *desc;	/* the per-SoC descriptor	*/
	struct ip_dvfs_ops *ops;	/* the per-SoC op table		*/

	struct ip_dvfs_coffe coffe;	/* the DT-read coefficients	*/
	struct ip_dvfs_status status;	/* the last read status		*/

	struct mutex lock;		/* serialises the register writes */

	/*
	 * The two frequency slots of the IP.  Which one is live depends on
	 * whether the IP is in its work state or its idle state; the stock
	 * blob keeps them at +0x3e0 and +0x3e4 of its private structure.
	 */
	u32 work_freq;
	u32 idle_freq;
	set_freq_type freq_type;

	int dvfs_enable;		/* the software DVFS enable flag	*/

	/* set while the IP is powered; the notify callback drops the
	 * frequency changes that arrive while it is off */
	int powered;

	struct devfreq *devfreq;	/* the devfreq device		*/
	struct notifier_block nb;	/* the layer-change notifier	*/
};

/* the notifier chains of the IPs */
extern struct atomic_notifier_head dpu_dvfs_chain;
extern struct blocking_notifier_head vsp_dvfs_chain;
extern struct blocking_notifier_head vpuenc_dvfs_chain;
extern struct atomic_notifier_head vdsp_dvfs_chain;
extern struct atomic_notifier_head gsp_dvfs_chain;

/* ------------------------------------------------------------------ */
/* The shared callback bodies.						*/
/* ------------------------------------------------------------------ */

int ip_dvfs_parse_dt(struct ip_dvfs_priv *priv, struct device_node *np);
int ip_dvfs_init(struct ip_dvfs_priv *priv);
void ip_hw_dfs_en(struct ip_dvfs_priv *priv, u32 en);
void ip_set_work_index(struct ip_dvfs_priv *priv, int index);
int ip_get_work_index(struct ip_dvfs_priv *priv);
void ip_set_idle_index(struct ip_dvfs_priv *priv, int index);
int ip_get_idle_index(struct ip_dvfs_priv *priv);
void ip_set_work_freq(struct ip_dvfs_priv *priv, u32 freq);
u32 ip_get_work_freq(struct ip_dvfs_priv *priv);
void ip_set_idle_freq(struct ip_dvfs_priv *priv, u32 freq);
u32 ip_get_idle_freq(struct ip_dvfs_priv *priv);
int ip_get_dvfs_table(struct ip_dvfs_priv *priv,
		      struct ip_dvfs_map_cfg *dvfs_table);
void ip_set_dvfs_table(struct ip_dvfs_priv *priv,
		       struct ip_dvfs_map_cfg *dvfs_table);
void ip_get_dvfs_status(struct ip_dvfs_priv *priv,
			struct ip_dvfs_status *dvfs_status);
void ip_set_gfree_wait_delay(struct ip_dvfs_priv *priv, u32 delay);
void ip_set_freq_upd_en_byp(struct ip_dvfs_priv *priv, u32 en);
void ip_set_freq_upd_delay_en(struct ip_dvfs_priv *priv, u32 en);
void ip_set_freq_upd_hdsk_en(struct ip_dvfs_priv *priv, u32 en);
void ip_set_dvfs_swtrig_en(struct ip_dvfs_priv *priv, u32 en);

/* the shared probe / governor / sysfs bodies */
int ip_dvfs_probe(struct ip_dvfs_priv *priv, struct platform_device *pdev,
		  struct devfreq_dev_profile *profile,
		  struct devfreq_governor *gov,
		  struct ip_dvfs_ops *ops,
		  const struct soc_dvfs_desc *desc,
		  struct notifier_head *chain);
int ip_dvfs_remove(struct ip_dvfs_priv *priv, struct platform_device *pdev);
int ip_dvfs_suspend(struct ip_dvfs_priv *priv, struct device *dev);
int ip_dvfs_resume(struct ip_dvfs_priv *priv, struct device *dev);
int ip_dvfs_target(struct ip_dvfs_priv *priv, unsigned long *freq, u32 flags);
int ip_dvfs_get_dev_status(struct ip_dvfs_priv *priv,
			   struct devfreq_dev_status *stat);
int ip_dvfs_get_cur_freq(struct ip_dvfs_priv *priv, unsigned long *freq);
int ip_gov_get_target(struct ip_dvfs_priv *priv, struct devfreq *df,
		      unsigned long *freq);
int ip_gov_event_handler(struct ip_dvfs_priv *priv, struct devfreq *df,
			 unsigned int event, void *data);
int ip_dvfs_notify_callback(struct ip_dvfs_priv *priv,
			    struct notifier_block *nb,
			    unsigned long action, void *data);

/* the shared sysfs bodies */
ssize_t ip_dvfs_table_show(struct ip_dvfs_priv *priv, char *buf);
ssize_t ip_dvfs_status_show(struct ip_dvfs_priv *priv, char *buf);
ssize_t ip_dvfs_index_show(struct ip_dvfs_priv *priv, char *buf,
			   int (*get)(struct ip_dvfs_priv *));
ssize_t ip_dvfs_index_store(struct ip_dvfs_priv *priv, const char *buf,
			    size_t count, void (*set)(struct ip_dvfs_priv *, int));
ssize_t ip_dvfs_freq_show(struct ip_dvfs_priv *priv, char *buf,
			  u32 (*get)(struct ip_dvfs_priv *));
ssize_t ip_dvfs_freq_store(struct ip_dvfs_priv *priv, const char *buf,
			   size_t count, void (*set)(struct ip_dvfs_priv *, u32));
ssize_t ip_dvfs_hw_dfs_show(struct ip_dvfs_priv *priv, char *buf);
ssize_t ip_dvfs_hw_dfs_store(struct ip_dvfs_priv *priv, const char *buf,
			     size_t count);
ssize_t ip_dvfs_enable_show(struct ip_dvfs_priv *priv, char *buf);
ssize_t ip_dvfs_enable_store(struct ip_dvfs_priv *priv, const char *buf,
			      size_t count);

#endif /* __SPRD_DVFS_IP_H__ */
