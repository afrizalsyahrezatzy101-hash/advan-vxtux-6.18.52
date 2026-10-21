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
 * The operation tables of the per-SoC DVFS blocks.
 *
 * The stock apsys-dvfs.ko exports one <soc>_<block>_dvfs_ops table per
 * SoC and per block (19 in total).  Every table is an array of function
 * pointers; the stock blob fills them with per-SoC clones of the same
 * body.  Here the bodies are shared and the per-SoC differences live in
 * a register descriptor, so each table is a plain initialiser.
 *
 * The slot order of every table was recovered from the stock blob's
 * relocation table.  It is NOT the same for every block, so each block
 * gets its own struct below.
 */

#ifndef __SPRD_DVFS_OPS_H__
#define __SPRD_DVFS_OPS_H__

#include <linux/types.h>

#include "sprd_dvfs_apsys.h"
#include "sprd_dvfs_soc.h"

/*
 * The apsys operation table.  Nine slots, in the order the stock blob
 * uses them.  Slot 3, apsys_force_en, is not implemented by the stock
 * blob on any SoC, which is why the per-SoC initialisers leave it NULL.
 */
/* struct apsys_dvfs_ops lives in sprd_dvfs_apsys.h */

/*
 * The dpu operation table.  21 slots.  Slots 13 and 14 are not
 * implemented by the stock blob, which is why the per-SoC initialisers
 * leave them NULL.
 */
struct dpu_dvfs_ops {
	int (*parse_dt)(struct ip_dvfs_dev *dpu, struct device_node *np);
	int (*dvfs_init)(struct ip_dvfs_dev *dpu);
	void (*hw_dfs_en)(struct ip_dvfs_dev *dpu, u32 en);

	void (*set_work_index)(struct ip_dvfs_dev *dpu, u32 index);
	u32 (*get_work_index)(struct ip_dvfs_dev *dpu);
	void (*set_idle_index)(struct ip_dvfs_dev *dpu, u32 index);
	u32 (*get_idle_index)(struct ip_dvfs_dev *dpu);

	void (*set_work_freq)(struct ip_dvfs_dev *dpu, u32 freq);
	u32 (*get_work_freq)(struct ip_dvfs_dev *dpu);
	void (*set_idle_freq)(struct ip_dvfs_dev *dpu, u32 freq);
	u32 (*get_idle_freq)(struct ip_dvfs_dev *dpu);

	int (*get_dvfs_table)(struct ip_dvfs_dev *dpu,
			      struct ip_dvfs_map_cfg *table, int count);
	void (*set_dvfs_table)(struct ip_dvfs_dev *dpu,
				struct ip_dvfs_map_cfg *table);
	void (*get_dvfs_coffe)(struct ip_dvfs_dev *dpu,
			       struct ip_dvfs_coffe *coffe);
	void (*set_dvfs_coffe)(struct ip_dvfs_dev *dpu,
			       struct ip_dvfs_coffe *coffe);
	void (*get_dvfs_status)(struct ip_dvfs_dev *dpu,
				struct ip_dvfs_status *status);

	void (*set_gfree_wait_delay)(struct ip_dvfs_dev *dpu, u32 delay);
	void (*set_freq_upd_en_byp)(struct ip_dvfs_dev *dpu, u32 en);
	void (*set_freq_upd_delay_en)(struct ip_dvfs_dev *dpu, u32 en);
	void (*set_freq_upd_hdsk_en)(struct ip_dvfs_dev *dpu, u32 en);
	void (*set_dvfs_swtrig_en)(struct ip_dvfs_dev *dpu, u32 en);
};

/*
 * The vsp operation table.  19 slots; unlike the dpu it has no
 * set_dvfs_table / set_dvfs_coffe, and it adds the target frequency
 * updater.
 */
struct vsp_dvfs_ops {
	void (*parse_dt)(struct ip_dvfs_dev *vsp, struct device_node *np);
	int (*dvfs_init)(struct ip_dvfs_dev *vsp);
	void (*hw_dvfs_en)(struct ip_dvfs_dev *vsp, u32 en);

	void (*set_work_freq)(struct ip_dvfs_dev *vsp, u32 freq);
	u32 (*get_work_freq)(struct ip_dvfs_dev *vsp);
	void (*set_idle_freq)(struct ip_dvfs_dev *vsp, u32 freq);
	u32 (*get_idle_freq)(struct ip_dvfs_dev *vsp);

	void (*get_dvfs_table)(struct ip_dvfs_dev *vsp,
			       struct ip_dvfs_map_cfg *table, int count);
	void (*get_dvfs_status)(struct ip_dvfs_dev *vsp,
				struct ip_dvfs_status *status);

	void (*set_gfree_wait_delay)(struct ip_dvfs_dev *vsp, u32 delay);
	void (*set_freq_upd_en_byp)(struct ip_dvfs_dev *vsp, u32 en);
	void (*set_freq_upd_delay_en)(struct ip_dvfs_dev *vsp, u32 en);
	void (*set_freq_upd_hdsk_en)(struct ip_dvfs_dev *vsp, u32 en);
	void (*set_dvfs_swtrig_en)(struct ip_dvfs_dev *vsp, u32 en);

	void (*set_work_index)(struct ip_dvfs_dev *vsp, u32 index);
	u32 (*get_work_index)(struct ip_dvfs_dev *vsp);
	void (*set_idle_index)(struct ip_dvfs_dev *vsp, u32 index);
	u32 (*get_idle_index)(struct ip_dvfs_dev *vsp);

	void (*updata_target_freq)(struct ip_dvfs_dev *vsp, u32 freq,
				   int freq_type);
};

/*
 * The vdsp operation table.  22 slots; it has a PLL parse in slot 1 and
 * three unimplemented slots at 14, 15 and 16.
 */
struct vdsp_dvfs_ops {
	int (*parse_dt)(struct ip_dvfs_dev *vdsp, struct device_node *np);
	int (*parse_pll)(struct ip_dvfs_dev *vdsp, struct device *dev);
	int (*dvfs_init)(struct ip_dvfs_dev *vdsp);
	void (*hw_dfs_en)(struct ip_dvfs_dev *vdsp, u32 en);

	void (*set_work_index)(struct ip_dvfs_dev *vdsp, u32 index);
	u32 (*get_work_index)(struct ip_dvfs_dev *vdsp);
	void (*set_idle_index)(struct ip_dvfs_dev *vdsp, u32 index);
	u32 (*get_idle_index)(struct ip_dvfs_dev *vdsp);

	void (*set_work_freq)(struct ip_dvfs_dev *vdsp, u32 freq);
	u32 (*get_work_freq)(struct ip_dvfs_dev *vdsp);
	void (*set_idle_freq)(struct ip_dvfs_dev *vdsp, u32 freq);
	u32 (*get_idle_freq)(struct ip_dvfs_dev *vdsp);

	int (*get_dvfs_table)(struct ip_dvfs_dev *vdsp,
			      struct ip_dvfs_map_cfg *table, int count);
	void (*set_dvfs_table)(struct ip_dvfs_dev *vdsp,
				struct ip_dvfs_map_cfg *table);
	void (*get_dvfs_coffe)(struct ip_dvfs_dev *vdsp,
			       struct ip_dvfs_coffe *coffe);
	void (*set_dvfs_coffe)(struct ip_dvfs_dev *vdsp,
			       struct ip_dvfs_coffe *coffe);
	void (*get_dvfs_status)(struct ip_dvfs_dev *vdsp,
				struct ip_dvfs_status *status);

	void (*set_gfree_wait_delay)(struct ip_dvfs_dev *vdsp, u32 delay);
	void (*set_freq_upd_en_byp)(struct ip_dvfs_dev *vdsp, u32 en);
	void (*set_freq_upd_delay_en)(struct ip_dvfs_dev *vdsp, u32 en);
	void (*set_freq_upd_hdsk_en)(struct ip_dvfs_dev *vdsp, u32 en);
	void (*set_dvfs_swtrig_en)(struct ip_dvfs_dev *vdsp, u32 en);
};

/* the gsp operation table; same shape as the dpu one */
struct gsp_dvfs_ops {
	int (*parse_dt)(struct ip_dvfs_dev *gsp, struct device_node *np);
	int (*dvfs_init)(struct ip_dvfs_dev *gsp);
	void (*hw_dfs_en)(struct ip_dvfs_dev *gsp, u32 en);

	void (*set_work_index)(struct ip_dvfs_dev *gsp, u32 index);
	u32 (*get_work_index)(struct ip_dvfs_dev *gsp);
	void (*set_idle_index)(struct ip_dvfs_dev *gsp, u32 index);
	u32 (*get_idle_index)(struct ip_dvfs_dev *gsp);

	void (*set_work_freq)(struct ip_dvfs_dev *gsp, u32 freq);
	u32 (*get_work_freq)(struct ip_dvfs_dev *gsp);
	void (*set_idle_freq)(struct ip_dvfs_dev *gsp, u32 freq);
	u32 (*get_idle_freq)(struct ip_dvfs_dev *gsp);

	int (*get_dvfs_table)(struct ip_dvfs_dev *gsp,
			      struct ip_dvfs_map_cfg *table, int count);
	void (*set_dvfs_table)(struct ip_dvfs_dev *gsp,
				struct ip_dvfs_map_cfg *table);
	void (*get_dvfs_coffe)(struct ip_dvfs_dev *gsp,
			       struct ip_dvfs_coffe *coffe);
	void (*set_dvfs_coffe)(struct ip_dvfs_dev *gsp,
			       struct ip_dvfs_coffe *coffe);
	void (*get_dvfs_status)(struct ip_dvfs_dev *gsp,
				struct ip_dvfs_status *status);

	void (*set_gfree_wait_delay)(struct ip_dvfs_dev *gsp, u32 delay);
	void (*set_freq_upd_en_byp)(struct ip_dvfs_dev *gsp, u32 en);
	void (*set_freq_upd_delay_en)(struct ip_dvfs_dev *gsp, u32 en);
	void (*set_freq_upd_hdsk_en)(struct ip_dvfs_dev *gsp, u32 en);
	void (*set_dvfs_swtrig_en)(struct ip_dvfs_dev *gsp, u32 en);
};

#endif /* __SPRD_DVFS_OPS_H__ */
