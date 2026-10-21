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
 * The per-SoC register descriptors of the apsys/dpu/vsp/vdsp/gsp DVFS
 * blocks.
 *
 * The stock apsys-dvfs.ko carries one hand-written copy of every DVFS
 * callback per SoC (19 op tables, ~150 clones).  All the clones of a given
 * callback are byte-identical apart from the register offsets they poke,
 * so the tree keeps ONE shared callback body per slot and drives it from
 * the descriptor below.  The offsets were recovered from the stock blob's
 * relocation and disassembly data.
 *
 * UMS512 is sharkl5pro; that is the variant that has to be functionally
 * correct.  The other descriptors are carried so that the exported symbol
 * surface of the stock blob is preserved.
 */

#ifndef __SPRD_DVFS_SOC_H__
#define __SPRD_DVFS_SOC_H__

#include <linux/types.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/device.h>
#include <linux/platform_device.h>

struct device_node;
struct devfreq;
struct platform_device;

/*
 * One entry of an IP's frequency map: the map index, the voltage level
 * and the clock level it selects, and the clock rate that results.
 */
struct ip_dvfs_map_cfg {
	u32 map_index;
	u32 volt_level;
	u32 clk_level;
	u32 clk_rate;
	char *volt_val;
};

typedef enum {
	DVFS_WORK = 0,
	DVFS_IDLE,
} set_freq_type;

/*
 * The apsys block's own registers.
 *
 * The apsys callbacks take the apsys device, whose "base" is a pointer to
 * the apsys register block.  On the top of that the driver keeps a small
 * context: the "hold" flag, the "wait window" and the "min volt" values
 * live in the apsys block's own header.
 */
/*
 * The force/auto-gate register offset of the qogirn6pro silicon.
 * Every other SoC uses 0x3c.
 */
#define APSYS_CTRL_Q6PRO	0x50

struct apsys_dvfs_regs {
	u32 hold;		/* ap-dvfs-hold		*/
	u32 wait_window;	/* ap-dvfs-wait-window	*/
	u32 min_volt;		/* ap-dvfs-min-volt	*/
	u32 ctrl;		/* the ctrl register holding force/auto_gate */
	u32 ctrl_force_bit;	/* bit of ctrl that carries force_en */
	u32 ctrl_gate_bit;	/* bit of ctrl that carries auto_gate */
};

/*
 * The TOP block's dcdc registers.  The TOP block is a separate ioremap,
 * so the descriptor carries its physical base and size.
 */
struct top_dvfs_regs {
	u32 base;		/* physical base of the top block */
	u32 size;		/* its size */
	u32 volt;		/* the register that carries the modem voltage */
	u32 volt_shift;		/* the shift of the voltage field in it */
};

/*
 * The generic DVFS engine register window.  Every IP (dpu, vsp, vdsp, gsp)
 * owns one window inside a shared register block; the shared block is
 * reached through the apsys device.
 */
struct ip_dvfs_hw_regs {
	/* the "enable" bit inside the control register */
	u32 ctrl;		/* control register		*/
	u32 ctrl_en_mask;	/* the hw-dfs-en bit mask	*/

	/* the frequency-index registers */
	u32 work_index;		/* the work frequency index	*/
	u32 idle_index;		/* the idle frequency index	*/

	/* the coefficient registers */
	u32 gfree_wait;		/* gfree-wait-delay		*/
	u32 gfree_wait_mask;	/* its field mask		*/
	u32 gfree_wait_shift;	/* its field shift		*/
	u32 gfree_wait_or;	/* non-zero: the field is OR-ed in, not masked */
	u32 upd_en_byp;		/* freq-upd-en-byp		*/
	u32 upd_en_byp_mask;	/* its field mask		*/
	u32 upd_en_byp_shift;	/* its field shift		*/
	u32 upd_delay;		/* freq-upd-delay-en		*/
	u32 upd_delay_mask;	/* its field mask		*/
	u32 upd_delay_shift;	/* its field shift		*/
	u32 upd_hdsk;		/* freq-upd-hdsk-en		*/
	u32 upd_hdsk_mask;	/* its field mask		*/
	u32 upd_hdsk_shift;	/* its field shift		*/
	u32 sw_trig;		/* sw-trig-en			*/
	u32 sw_trig_mask;	/* its field mask		*/
	u32 sw_trig_shift;	/* its field shift		*/

	/* the clock-divider registers programmed at init time */
	u32 div0;
	u32 div1;
	u32 div2;
	u32 div3;
	u32 div4;
	u32 div5;		/* only roc1's dpu has a sixth */
};

/*
 * The per-SoC descriptor of one DVFS block.  One of these exists per
 * SoC and per block, and the shared callback bodies read it through the
 * device they are handed.
 */
struct soc_dvfs_desc {
	const char *name;	/* the SoC name, e.g. "sharkl5pro"	*/
	const struct ip_dvfs_hw_regs *regs;

	/*
	 * The IP's frequency map: the list of (index, voltage level, clock
	 * level, clock rate) entries the hardware can be programmed with.
	 * The stock blob keeps one such table per IP and per SoC.
	 */
	const struct ip_dvfs_map_cfg *map;
	u32 map_count;
};

/*
 * The runtime state of one DVFS IP (dpu, vsp, vdsp, gsp).  The stock blob
 * keeps the same state inside its per-IP private structures; here it is
 * one shared type, because the shared callback bodies only need these
 * fields.
 */
/*
 * The DT-read coefficients of one DVFS IP (dpu, vsp, vdsp, gsp).  The
 * stock blob keeps the same values inside its per-IP private structures;
 * here they are one shared type.
 */
struct ip_dvfs_coffe {
	u32 gfree_wait_delay;
	u32 freq_upd_hdsk_en;
	u32 freq_upd_delay_en;
	u32 freq_upd_en_byp;
	u32 sw_trig_en;
	u32 hw_dfs_en;
	u32 work_index_def;
	u32 idle_index_def;
};

/*
 * The runtime state of one DVFS IP.
 *
 * The stock blob keeps this in four separate ~1 KiB private structures,
 * one per IP, whose layouts differ slightly (the vsp one is bigger, it
 * also carries the encoder state).  Only the fields the shared callback
 * bodies actually touch are kept here, so one type serves all four IPs.
 */
struct ip_dvfs_dev {
	struct device *dev;		/* the IP's platform device	*/
	struct device_node *np;		/* its DT node			*/
	unsigned long regs;		/* the IP's register window	*/
	const struct soc_dvfs_desc *desc;	/* the per-SoC descriptor	*/

	struct ip_dvfs_coffe coffe;	/* the DT-read coefficients	*/

	struct mutex lock;		/* serialises the register writes */

	/*
	 * The two frequency slots.  Which one is live depends on whether the
	 * IP is in its work state or its idle state; the stock blob keeps
	 * them at +0x3e0 and +0x3e4 of its private structure.
	 */
	u32 work_freq;
	u32 idle_freq;
	set_freq_type freq_type;

	int dvfs_enable;		/* the software DVFS enable flag	*/

	/* the frequency the client last asked for through the notifier;
	 * the governor clamps the current frequency by it */
	unsigned long req_freq;

	/* set while the IP is powered; the notify callback drops the
	 * frequency changes that arrive while it is off */
	int powered;

	struct devfreq *devfreq;	/* the devfreq device		*/
	struct notifier_block nb;	/* the layer-change notifier	*/
	struct platform_device *pdev;	/* its platform device		*/
	struct kobject *sysfs_kobj;	/* where the sysfs group goes	*/
};

#endif /* __SPRD_DVFS_SOC_H__ */
