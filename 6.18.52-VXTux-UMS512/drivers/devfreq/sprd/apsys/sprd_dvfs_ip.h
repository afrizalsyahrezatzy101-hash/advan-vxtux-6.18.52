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

	struct ip_dvfs_coffe coffe;	/* the DT-read coefficients	*/

	struct devfreq *devfreq;	/* the devfreq device		*/
	struct notifier_block nb;	/* the layer-change notifier	*/

	/*
	 * The two frequency slots of the IP.  Which one is live depends on
	 * whether the IP is in its work state or its idle state; the stock
	 * blob keeps them at +0x3e0 and +0x3e4 of its private structure.
	 */
	u32 work_freq;
	u32 idle_freq;

	/* set while the IP is powered; the notify callback drops the
	 * frequency changes that arrive while it is off */
	int powered;
};

/*
 * The devfreq governor of one IP.
 *
 * The stock blob's governor ("<ip>_governor") picks the IP's current
 * frequency: the live one (work or idle), clamped by the frequency the
 * client last asked for.
 */
struct ip_dvfs_gov_data {
	struct ip_dvfs_priv *priv;
	unsigned long req_freq;	/* the frequency the client asked for */
};

#endif /* __SPRD_DVFS_IP_H__ */
