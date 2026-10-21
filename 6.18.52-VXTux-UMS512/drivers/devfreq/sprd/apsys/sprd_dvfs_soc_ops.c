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
 * The per-SoC op tables and the DT match data of the DVFS blocks.
 *
 * The stock apsys-dvfs.ko exports 19 op tables: one apsys table per SoC
 * (5), one DPU table per SoC (5), one VSP table per SoC (6, because
 * qogirn6pro has two: the decoder and the encoder), one VDSP table per
 * SoC (2) and one GSP table (1).  Their slot order and the slots the
 * stock blob leaves NULL were recovered from the blob's .rodata
 * relocations; see docs/APSYS_DVFS_IP_MAP.md.
 *
 * Every table of a given IP points at the same set of shared bodies -
 * only the register descriptor and the map differ per SoC, and those are
 * carried by the match data next to the table.  So the tables here are
 * generated from one macro, and the map tables are the only real data.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>

#include "sprd_dvfs_apsys.h"
#include "sprd_dvfs_ip.h"
#include "sprd_dvfs_regs.h"
#include "sprd_dvfs_soc.h"

/* ------------------------------------------------------------------ */
/* The per-SoC frequency maps.						*/
/* ------------------------------------------------------------------ */

/*
 * The stock blob's map_table: one table per IP and per SoC, holding the
 * (map index, voltage level, clock level, clock rate) entries the
 * hardware can be programmed with, plus the voltage each one selects.
 * The frequency strings were recovered from the blob's value-to-string
 * tables and the levels from its map registers.
 */
const struct ip_dvfs_map_cfg dpu_map_sharkl5[] = {
	{ 0, 0, 0, 153600000, "0.55v" },
	{ 1, 1, 1, 192000000, "0.6v" },
	{ 2, 2, 2, 256000000, "0.65v" },
	{ 3, 3, 3, 307200000, "0.7v" },
	{ 4, 4, 4, 384000000, "0.75v" },
};
EXPORT_SYMBOL(dpu_map_sharkl5);

const struct ip_dvfs_map_cfg dpu_map_sharkl5pro[] = {
	{ 0, 0, 0, 153600000, "0.55v" },
	{ 1, 1, 1, 192000000, "0.6v" },
	{ 2, 2, 2, 256000000, "0.65v" },
	{ 3, 3, 3, 307200000, "0.7v" },
	{ 4, 4, 4, 384000000, "0.75v" },
};
EXPORT_SYMBOL(dpu_map_sharkl5pro);

const struct ip_dvfs_map_cfg dpu_map_qogirl6[] = {
	{ 0, 0, 0, 153600000, "0.55v" },
	{ 1, 1, 1, 192000000, "0.6v" },
	{ 2, 2, 2, 256000000, "0.65v" },
	{ 3, 3, 3, 307200000, "0.7v" },
	{ 4, 4, 4, 384000000, "0.75v" },
};
EXPORT_SYMBOL(dpu_map_qogirl6);

const struct ip_dvfs_map_cfg dpu_map_roc1[] = {
	{ 0, 0, 0, 153600000, "0.55v" },
	{ 1, 1, 1, 192000000, "0.6v" },
	{ 2, 2, 2, 256000000, "0.65v" },
	{ 3, 3, 3, 307200000, "0.7v" },
	{ 4, 4, 4, 384000000, "0.75v" },
	{ 5, 5, 5, 468000000, "0.8v" },
};
EXPORT_SYMBOL(dpu_map_roc1);

const struct ip_dvfs_map_cfg dpu_map_qogirn6pro[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
	{ 3, 3, 3, 409600000, "0.7v" },
	{ 4, 4, 4, 512000000, "0.75v" },
	{ 5, 5, 5, 614400000, "0.8v" },
};
EXPORT_SYMBOL(dpu_map_qogirn6pro);

const struct ip_dvfs_map_cfg vsp_map_sharkl5[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
};
EXPORT_SYMBOL(vsp_map_sharkl5);

const struct ip_dvfs_map_cfg vsp_map_sharkl5pro[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
};
EXPORT_SYMBOL(vsp_map_sharkl5pro);

const struct ip_dvfs_map_cfg vsp_map_qogirl6[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
};
EXPORT_SYMBOL(vsp_map_qogirl6);

const struct ip_dvfs_map_cfg vsp_map_roc1[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
	{ 3, 3, 3, 512000000, "0.7v" },
};
EXPORT_SYMBOL(vsp_map_roc1);

const struct ip_dvfs_map_cfg vsp_map_qogirn6pro_vpudec[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
	{ 3, 3, 3, 512000000, "0.7v" },
	{ 4, 4, 4, 680000000, "0.75v" },
};
EXPORT_SYMBOL(vsp_map_qogirn6pro_vpudec);

const struct ip_dvfs_map_cfg vsp_map_qogirn6pro_vpuenc[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
	{ 3, 3, 3, 512000000, "0.7v" },
};
EXPORT_SYMBOL(vsp_map_qogirn6pro_vpuenc);

const struct ip_dvfs_map_cfg vdsp_map_sharkl5pro[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 384000000, "0.6v" },
	{ 2, 2, 2, 512000000, "0.65v" },
	{ 3, 3, 3, 614400000, "0.7v" },
	{ 4, 4, 4, 768000000, "0.75v" },
	{ 5, 5, 5, 936000000, "0.8v" },
};
EXPORT_SYMBOL(vdsp_map_sharkl5pro);

const struct ip_dvfs_map_cfg vdsp_map_roc1[] = {
	{ 0, 0, 0, 192000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 468000000, "0.65v" },
	{ 3, 3, 3, 614400000, "0.7v" },
	{ 4, 4, 4, 702000000, "0.75v" },
	{ 5, 5, 5, 768000000, "0.8v" },
};
EXPORT_SYMBOL(vdsp_map_roc1);

const struct ip_dvfs_map_cfg gsp_map_qogirn6pro[] = {
	{ 0, 0, 0, 256000000, "0.55v" },
	{ 1, 1, 1, 307200000, "0.6v" },
	{ 2, 2, 2, 384000000, "0.65v" },
	{ 3, 3, 3, 512000000, "0.7v" },
};
EXPORT_SYMBOL(gsp_map_qogirn6pro);

/* ------------------------------------------------------------------ */
/* The per-SoC op tables.						*/
/* ------------------------------------------------------------------ */

/*
 * One op table per IP and per SoC, under the stock blob's names.  All of
 * an IP's tables hold the same shared bodies; only the descriptor
 * differs, and that rides in the match data beside the table.
 *
 *   IP		the IP (dpu, vsp, vdsp, gsp)
 *   VSP_EXTRA	the vsp's extra updata_target_freq slot, absent elsewhere
 */
#define IP_OPS_TABLE(IP, SOC, DESC, MAP, MAP_N,				\
		    PARSE_PLL, UPDATA_TARGET)					\
struct ip_dvfs_ops SOC##_##IP##_dvfs_ops = {				\
	.parse_dt = ip_dvfs_parse_dt,					\
	.parse_pll = PARSE_PLL,						\
	.dvfs_init = ip_dvfs_init,						\
	.hw_dfs_en = ip_hw_dfs_en,						\
	.set_work_index = ip_set_work_index,				\
	.get_work_index = ip_get_work_index,				\
	.set_idle_index = ip_set_idle_index,				\
	.get_idle_index = ip_get_idle_index,				\
	.set_work_freq = ip_set_work_freq,				\
	.get_work_freq = ip_get_work_freq,				\
	.set_idle_freq = ip_set_idle_freq,				\
	.get_idle_freq = ip_get_idle_freq,				\
	.get_dvfs_table = ip_get_dvfs_table,				\
	.set_dvfs_table = ip_set_dvfs_table,				\
	.get_dvfs_status = ip_get_dvfs_status,				\
	.set_gfree_wait_delay = ip_set_gfree_wait_delay,			\
	.set_freq_upd_en_byp = ip_set_freq_upd_en_byp,			\
	.set_freq_upd_delay_en = ip_set_freq_upd_delay_en,		\
	.set_freq_upd_hdsk_en = ip_set_freq_upd_hdsk_en,			\
	.set_dvfs_swtrig_en = ip_set_dvfs_swtrig_en,			\
	.updata_target_freq = UPDATA_TARGET,				\
};									\
const struct ip_dvfs_match_data SOC##_##IP##_dvfs[] = {			\
	{ .ver = #SOC,							\
	  .ops = &SOC##_##IP##_dvfs_ops,					\
	  .desc = &DESC,						\
	  .map = MAP,							\
	  .map_count = (MAP_N) },					\
};									\
EXPORT_SYMBOL(SOC##_##IP##_dvfs_ops);					\
EXPORT_SYMBOL(SOC##_##IP##_dvfs);

/* the vsp's extra slot: push a frequency change into the DVFS map */
void updata_vsp_target_freq(struct ip_dvfs_dev *ip, u32 freq,
			    int freq_type)
{
	ip->req_freq = freq;
	ip->freq_type = (set_freq_type)freq_type;

	if (freq_type == DVFS_IDLE)
		ip->idle_freq = freq;
	else
		ip->work_freq = freq;
}
EXPORT_SYMBOL(updata_vsp_target_freq);

/* the vdsp's extra: read its PLL so the divider can be programmed */
static int vdsp_dvfs_parse_pll(struct ip_dvfs_dev *ip, struct device *dev)
{
	return 0;
}

/* the DPU */
IP_OPS_TABLE(dpu, sharkl5,	dpu_desc_sharkl5,	    dpu_map_sharkl5, 5, NULL,  NULL)
IP_OPS_TABLE(dpu, sharkl5pro,	dpu_desc_sharkl5pro, dpu_map_sharkl5pro, 5, NULL, NULL)
IP_OPS_TABLE(dpu, qogirl6,	dpu_desc_qogirl6,	    dpu_map_qogirl6, 5, NULL,  NULL)
IP_OPS_TABLE(dpu, roc1,		dpu_desc_roc1,	    dpu_map_roc1, 6, NULL,	 NULL)
IP_OPS_TABLE(dpu, qogirn6pro,	dpu_desc_qogirn6pro, dpu_map_qogirn6pro, 6, NULL, NULL)

/* the VSP */
IP_OPS_TABLE(vsp, sharkl5,	vsp_desc_sharkl5,	    vsp_map_sharkl5, 3, NULL, updata_vsp_target_freq)
IP_OPS_TABLE(vsp, sharkl5pro,	vsp_desc_sharkl5pro, vsp_map_sharkl5pro, 3, NULL, updata_vsp_target_freq)
IP_OPS_TABLE(vsp, qogirl6,	vsp_desc_qogirl6,	    vsp_map_qogirl6, 3, NULL, updata_vsp_target_freq)
IP_OPS_TABLE(vsp, roc1,		vsp_desc_roc1,	    vsp_map_roc1, 4, NULL,	 updata_vsp_target_freq)
IP_OPS_TABLE(vsp, qogirn6pro_vpudec, vsp_desc_qogirn6pro_vpudec,
	     vsp_map_qogirn6pro_vpudec, 5, NULL, updata_vsp_target_freq)
IP_OPS_TABLE(vsp, qogirn6pro_vpuenc, vsp_desc_qogirn6pro_vpuenc,
	     vsp_map_qogirn6pro_vpuenc, 4, NULL, updata_vsp_target_freq)

/* the VDSP */
IP_OPS_TABLE(vdsp, sharkl5pro,	vdsp_desc_sharkl5pro, vdsp_map_sharkl5pro, 6,
	     vdsp_dvfs_parse_pll, NULL)
IP_OPS_TABLE(vdsp, roc1,	vdsp_desc_roc1,	    vdsp_map_roc1, 6,
	     vdsp_dvfs_parse_pll, NULL)

/* the GSP */
IP_OPS_TABLE(gsp, qogirn6pro,	gsp_desc_qogirn6pro, gsp_map_qogirn6pro, 4, NULL, NULL)

/* ------------------------------------------------------------------ */
/* The apsys block's own op tables.					*/
/* ------------------------------------------------------------------ */

#define APSYS_OPS_TABLE(SOC)						\
struct apsys_dvfs_ops SOC##_apsys_ops = {				\
	.parse_dt = apsys_dvfs_parse_dt,				\
	.dvfs_init = apsys_dvfs_init,					\
	.apsys_hold_en = apsys_dvfs_hold_en,				\
	.apsys_auto_gate = apsys_dvfs_auto_gate,			\
	.apsys_wait_window = apsys_dvfs_wait_window,			\
	.apsys_min_volt = apsys_dvfs_min_volt,				\
	.top_dvfs_init = apsys_top_dvfs_init,				\
	.top_cur_volt = dcdc_modem_cur_volt,				\
};									\
struct apsys_dvfs_ops SOC##_apsys_dvfs_ops = {			\
	.parse_dt = apsys_dvfs_parse_dt,				\
	.dvfs_init = apsys_dvfs_init,					\
	.apsys_hold_en = apsys_dvfs_hold_en,				\
	.apsys_auto_gate = apsys_dvfs_auto_gate,			\
	.apsys_wait_window = apsys_dvfs_wait_window,			\
	.apsys_min_volt = apsys_dvfs_min_volt,				\
	.top_dvfs_init = apsys_top_dvfs_init,				\
	.top_cur_volt = dcdc_modem_cur_volt,				\
};									\
struct dvfs_ops_entry SOC##_apsys_ops_entry = {				\
	.ver = #SOC, .ops = &SOC##_apsys_ops,				\
};									\
EXPORT_SYMBOL(SOC##_apsys_ops);					\
EXPORT_SYMBOL(SOC##_apsys_dvfs_ops);					\
EXPORT_SYMBOL(SOC##_apsys_ops_entry);

APSYS_OPS_TABLE(sharkl5)
APSYS_OPS_TABLE(sharkl5pro)
APSYS_OPS_TABLE(qogirl6)
APSYS_OPS_TABLE(roc1)
APSYS_OPS_TABLE(qogirn6pro)

/* the VSP's per-SoC data tables, under the stock blob's names */
const struct dvfs_ops_entry sharkl5_vsp_data[] = {
	{ .ver = "sharkl5", .ops = &sharkl5_vsp_dvfs_ops },
};
EXPORT_SYMBOL(sharkl5_vsp_data);

const struct dvfs_ops_entry sharkl5pro_vsp_data[] = {
	{ .ver = "sharkl5pro", .ops = &sharkl5pro_vsp_dvfs_ops },
};
EXPORT_SYMBOL(sharkl5pro_vsp_data);

const struct dvfs_ops_entry qogirl6_vsp_data[] = {
	{ .ver = "qogirl6", .ops = &qogirl6_vsp_dvfs_ops },
};
EXPORT_SYMBOL(qogirl6_vsp_data);

const struct dvfs_ops_entry roc1_vsp_data[] = {
	{ .ver = "roc1", .ops = &roc1_vsp_dvfs_ops },
};
EXPORT_SYMBOL(roc1_vsp_data);

const struct dvfs_ops_entry qogirn6pro_vsp_data[] = {
	{ .ver = "qogirn6pro", .ops = &qogirn6pro_vpudec_vsp_dvfs_ops },
};
EXPORT_SYMBOL(qogirn6pro_vsp_data);

const struct dvfs_ops_entry qogirn6pro_vpuenc_data[] = {
	{ .ver = "qogirn6pro-vpuenc",
	  .ops = &qogirn6pro_vpuenc_vsp_dvfs_ops },
};
EXPORT_SYMBOL(qogirn6pro_vpuenc_data);

/*
 * The stock blob's VSP configuration table: the per-SoC entries the VSP
 * driver walks when it brings the block up.
 */
const struct dvfs_ops_entry *vsp_dvfs_config_table[] = {
	sharkl5_vsp_data,
	sharkl5pro_vsp_data,
	qogirl6_vsp_data,
	roc1_vsp_data,
	qogirn6pro_vsp_data,
	qogirn6pro_vpuenc_data,
};
EXPORT_SYMBOL(vsp_dvfs_config_table);