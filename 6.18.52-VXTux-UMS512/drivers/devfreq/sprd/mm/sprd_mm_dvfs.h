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

#ifndef __SPRD_MM_DVFS_H__
#define __SPRD_MM_DVFS_H__

#include <linux/device.h>
#include <linux/devfreq.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/mutex.h>
#include <linux/notifier.h>
#include <linux/types.h>

#define DVFS_VOLT_LEVEL_NUM	8
#define DVFS_FREQ_LEVEL_NUM	8

/* the mmsys power event states, as broadcast on mmsys_notifier_list */
#define SPRD_MM_DVFS_NOTIFY_POWER_ON	1
#define SPRD_MM_DVFS_NOTIFY_POWER_OFF	2

/* the DVFS map of one camera IP */
struct ip_dvfs_map_cfg {
	u32 map_index;		/* index into the hardware's frequency map */
	u32 reserved;		/* padding */
	u64 reg_add;		/* the register offset that selects this point */
	u32 fdiv_num;		/* the frequency divider numerator */
	u32 clk_level;		/* the resulting clock, in Hz */
	u32 volt_level;		/* the voltage level, 0..7 */
	u32 axi_index;		/* the axi clock index */
	u32 mtx_index;		/* the mtx clock index */
	u32 reserved2;		/* padding */
	char *volt_val;		/* human readable voltage, for sysfs */
};

/* the live state of one camera IP */
struct ip_dvfs_status {
	char *apsys_cur_volt;
	char *dpu_vote_volt;
	char *gsp0_vote_volt;
	char *gsp1_vote_volt;
	char *vsp_vote_volt;
	char *vpuenc_vote_volt;
	char *vdsp_vote_volt;
	char *dpu_cur_freq;
	char *gsp0_cur_freq;
	char *gsp1_cur_freq;
	char *vsp_cur_freq;
	char *vpuenc_cur_freq;
	char *vdsp_cur_freq;
	u32 vdsp_edap_div;
	u32 vdsp_m0_div;
};

/* the hardware DVFS coefficients of one IP */
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

/* the per-IP register window inside the mm register block */
struct ip_dvfs_regs {
	u32 work_index;		/* the work frequency index register */
	u32 idle_index;		/* the idle frequency index register */
	u32 idx_shift;		/* the index field shift in those registers */
};

/*
 * The per-IP register parameters, recovered from the stock blob.
 *
 * The stock driver has one copy of each per-IP callback and gets the
 * variation from the constant tables that sit next to it; the port has
 * one translation unit, so the same data is carried in struct
 * ip_dvfs_ops and the single set of callbacks reads it from there.  That
 * is also the shape of the stock's own functions: they take
 * struct ip_dvfs_ops * as their first argument.
 *
 * sw30_mask	bits to clear in 0x30 (the software DVFS gate)
 * upd220_clear	bits to clear in 0x220 (the update control)
 * upd220_set	bits to set in 0x220
 * delay		the value written to 0x214 (the update delay)
 * hold_clear	bits to clear in 0x0c (the voltage hold), 0 = leave alone
 * work_idx_reg	the work index register, and its default value
 * idle_idx_reg	the idle index register, and its default value
 * gate8		the bits set in 0x8 to clock-gate this IP's registers
 */
struct ip_dvfs_params {
	u32 sw30_mask;
	u32 upd220_clear;
	u32 upd220_set;
	u32 delay;
	u32 hold_clear;
	u32 work_idx_reg;
	u32 idle_idx_reg;
	u32 work_idx_def;
	u32 idle_idx_def;
	u32 gate8;
};

/**
 * struct ip_dvfs_ops - the operations of one camera IP
 *
 * The IP identifies itself by name; the rest are the callbacks the core
 * calls into.  Every IP variant shares the same callback set, only the
 * register window and the frequency table differ.
 */
struct ip_dvfs_ops {
	struct list_head list;
	/* "dcam_axi_DVFS_OPS" is 16 characters; char[16] drops the last one. */
	char name[20];
	u32 ver;

	/*
	 * The per-IP data the stock driver keeps in the constant tables
	 * next to its callbacks.  The port has one translation unit, so
	 * the callbacks read the variation from here instead of from
	 * eight copies of themselves.
	 */
	const struct ip_dvfs_params *params;
	struct ip_dvfs_map_cfg *table;
	/* the bit layout of one map register, per IP */
	void (*map_apply)(struct ip_dvfs_map_cfg *cfg);

	/* the IP's own dvfs knobs */
	int (*dvfs_init)(struct ip_dvfs_ops *ops);
	int (*set_hw_dvfs_en)(struct ip_dvfs_ops *ops, int en);
	int (*set_auto_tune_en)(struct ip_dvfs_ops *ops, u32 en);
	int (*set_work_freq)(struct ip_dvfs_ops *ops, u64 freq);
	int (*set_idle_freq)(struct ip_dvfs_ops *ops, u64 freq);

	/* the IP's live state */
	int (*get_dvfs_table)(struct ip_dvfs_ops *ops,
			      struct ip_dvfs_map_cfg *table, int count);
	int (*set_dvfs_table)(struct ip_dvfs_ops *ops,
			      struct ip_dvfs_map_cfg *cfg);
	int (*get_ip_status)(struct ip_dvfs_ops *ops, struct ip_dvfs_status *status);

	/* power management */
	int (*power_on)(struct ip_dvfs_ops *ops);
	int (*power_off)(struct ip_dvfs_ops *ops);
	int (*set_gfree_wait_delay)(struct ip_dvfs_ops *ops, u32 delay);

	/* the update controls */
	int (*set_freq_upd_en_byp)(struct ip_dvfs_ops *ops, int en);
	int (*set_freq_upd_delay_en)(struct ip_dvfs_ops *ops, int en);
	int (*set_freq_upd_hdsk_en)(struct ip_dvfs_ops *ops, int en);
	int (*set_ip_dvfs_swtrig_en)(struct ip_dvfs_ops *ops, int en);

	/* index and frequency */
	int (*set_ip_dvfs_work_index)(struct ip_dvfs_ops *ops, u32 index);
	int (*get_ip_dvfs_work_index)(struct ip_dvfs_ops *ops, u32 *index);
	int (*set_ip_dvfs_idle_index)(struct ip_dvfs_ops *ops, u32 index);
	int (*get_ip_index_from_table)(struct ip_dvfs_ops *ops, u64 freq,
				       u32 *index);
	int (*get_ip_index_from_table2)(struct ip_dvfs_ops *ops, u64 freq,
					u32 *index);

	/* the mmsys-level knobs */
	int (*updata_target_freq)(struct ip_dvfs_ops *ops, u64 freq,
				  u32 index, int is_fix);
	int (*set_fix_dvfs_value)(struct ip_dvfs_ops *ops, u32 value);
	int (*get_top_current_volt)(struct ip_dvfs_ops *ops, u32 *volt);
	int (*get_mm_current_volt)(struct ip_dvfs_ops *ops, u32 *volt);
};

/* the per-IP runtime state, allocated with the devfreq device */
struct ip_dvfs_dev {
	struct ip_dvfs_ops *ops;
	struct devfreq *devfreq;
	struct ip_dvfs_status status;
	struct ip_dvfs_coffe coffe;
	struct ip_dvfs_map_cfg table[DVFS_FREQ_LEVEL_NUM];

	u32 dvfs_enable;
	u32 auto_tune_en;
	u32 work_freq;
	u32 idle_freq;
	u32 work_index;
	u32 idle_index;
	u32 fix_dvfs_value;

	/*
	 * The mmsys-level switches, only used by the mmsys device.  Their
	 * names and their role come from the stock blob: the power
	 * control value, the software DVFS gate, the DVFS hold, the clock
	 * gate control, the wait window and the minimum voltage.
	 */
	u64 power_ctrl;
	u32 sw_dvfs_en;
	u32 dvfs_hold_en;
	u32 clk_gate_ctrl;
	u32 wait_window;
	u32 min_volt;

	u64 cur_freq;
	u64 cur_volt;

	struct notifier_block notifier;
	struct mutex lock;
};

/* the mmsys-level devfreq device */
struct mmsys_dvfs {
	struct device *dev;
	struct ip_dvfs_dev *ip_dev;
	struct devfreq *devfreq;
	struct notifier_block notifier;
	struct mutex lock;
};

/* the core's global state, shared by every IP */
extern void *g_mmreg_map;
extern struct mutex mmsys_glob_reg_lock;
extern struct list_head ip_dvfs_ops_list;
extern struct mutex ip_dvfsops_list_lock;
extern struct blocking_notifier_head mmsys_notifier_list;
extern int mm_power_flag;

/*
 * The shared mm register window accessors, defined in
 * sprd_mm_dvfs_stock.c.  Every update takes mmsys_glob_reg_lock,
 * because the block is shared between all the IPs.
 */
u32 mmreg_read(u32 off);
void mmreg_write(u32 off, u32 val);
void mmreg_update(u32 off, u32 clear_mask, u32 set_mask);

int register_ip_dvfs_ops(struct ip_dvfs_ops *ops);
int unregister_ip_dvfs_ops(struct ip_dvfs_ops *ops);
struct ip_dvfs_ops *get_ip_dvfs_ops(const char *name);

int mmsys_register_notifier(struct notifier_block *nb);
int mmsys_unregister_notifier(struct notifier_block *nb);
int mmsys_notify_call_chain(int state);

void mmsys_adjust_target_freq(void);
void mmsys_set_fix_dvfs_value(u32 value);
u32 top_mm_dvfs_current_volt(void);
u32 mm_dvfs_read_current_volt(void);

void mm_sw_dvfs_onoff(int on);
void mmdcdc_sw_dvfs_onoff(int on);
void mmsys_sw_cgb_enable(int en);
void mm_dvfs_hold_onoff(int en);

/* the mmsys devfreq profile's callbacks */
int dvfs_target(struct device *dev, unsigned long *freq, u32 flags);
int dvfs_get_dev_status(struct device *dev, struct devfreq_dev_status *status);
int dvfs_get_cur_freq(struct device *dev, unsigned long *freq);
int dvfs_notify_callback(struct notifier_block *nb, unsigned long action,
			  void *data);

/* the sysfs tuning knobs, created per device at probe time */
int mm_dvfs_sysfs_create_groups(struct device *dev, int mmsys);
void mm_dvfs_sysfs_remove_groups(struct device *dev, int mmsys);

#endif /* __SPRD_MM_DVFS_H__ */
