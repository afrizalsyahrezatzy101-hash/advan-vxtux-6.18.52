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
 * The per-SoC register descriptors of the DVFS blocks, defined in
 * sprd_dvfs_regs.c.
 */

#ifndef __SPRD_DVFS_REGS_H__
#define __SPRD_DVFS_REGS_H__

#include "sprd_dvfs_soc.h"

extern const struct apsys_dvfs_regs apsys_regs;
extern const struct apsys_dvfs_regs apsys_regs_qogirn6pro;
extern const struct top_dvfs_regs top_regs_sharkl5pro;
extern const struct top_dvfs_regs top_regs_qogirl6;
extern const struct top_dvfs_regs top_regs_qogirn6pro;
extern const struct top_dvfs_regs top_regs_sharkl5;
extern const struct top_dvfs_regs top_regs_roc1;
extern const struct ip_dvfs_hw_regs dpu_regs_sharkl5pro;
extern const struct ip_dvfs_hw_regs dpu_regs_qogirl6;
extern const struct ip_dvfs_hw_regs dpu_regs_sharkl5;
extern const struct ip_dvfs_hw_regs dpu_regs_roc1;
extern const struct ip_dvfs_hw_regs dpu_regs_qogirn6pro;
extern const struct ip_dvfs_hw_regs vsp_regs_sharkl5pro;
extern const struct ip_dvfs_hw_regs vsp_regs_qogirl6;
extern const struct ip_dvfs_hw_regs vsp_regs_sharkl5;
extern const struct ip_dvfs_hw_regs vsp_regs_roc1;
extern const struct ip_dvfs_hw_regs vsp_regs_qogirn6pro_vpudec;
extern const struct ip_dvfs_hw_regs vsp_regs_qogirn6pro_vpuenc;
extern const struct ip_dvfs_hw_regs vdsp_regs_sharkl5pro;
extern const struct ip_dvfs_hw_regs vdsp_regs_roc1;
extern const struct ip_dvfs_hw_regs gsp_regs_qogirn6pro;
extern const struct soc_dvfs_desc dpu_desc_sharkl5pro;
extern const struct soc_dvfs_desc dpu_desc_qogirl6;
extern const struct soc_dvfs_desc dpu_desc_sharkl5;
extern const struct soc_dvfs_desc dpu_desc_roc1;
extern const struct soc_dvfs_desc dpu_desc_qogirn6pro;
extern const struct soc_dvfs_desc vsp_desc_sharkl5pro;
extern const struct soc_dvfs_desc vsp_desc_qogirl6;
extern const struct soc_dvfs_desc vsp_desc_sharkl5;
extern const struct soc_dvfs_desc vsp_desc_roc1;
extern const struct soc_dvfs_desc vsp_desc_qogirn6pro_vpudec;
extern const struct soc_dvfs_desc vsp_desc_qogirn6pro_vpuenc;
extern const struct soc_dvfs_desc vdsp_desc_sharkl5pro;
extern const struct soc_dvfs_desc vdsp_desc_roc1;
extern const struct soc_dvfs_desc gsp_desc_qogirn6pro;

#endif /* __SPRD_DVFS_REGS_H__ */
