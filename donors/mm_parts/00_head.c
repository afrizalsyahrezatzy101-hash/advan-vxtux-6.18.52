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
 * The per-IP variants of the memory-management DVFS driver.
 *
 * Every camera IP (mtx, isp, fd, cpp, jpg, dcam_if, dcam_axi, plus the
 * mmsys block itself) owns a slice of the shared mm register window and
 * a frequency/voltage table.  The callbacks are the same in every
 * variant; only the register window, the field packing and the table
 * differ, so the variants are generated from one template below.
 *
 * The per-IP data (register offsets, bit masks, default indices, table)
 * was recovered from the stock mmdvfs.ko by disassembling the eight
 * ip_dvfs_init() clones and the eight <ip>_dvfs_ops tables.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/devfreq.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/slab.h>

#include "governor.h"
#include "sprd_mm_dvfs.h"

static u32 mm_read(u32 off)
{
	return readl(g_mmreg_map + off);
}

static void mm_write(u32 off, u32 val)
{
	writel(val, g_mmreg_map + off);
}

static void mm_update(u32 off, u32 clear_mask, u32 set_mask)
{
	mutex_lock(&mmsys_glob_reg_lock);
	if (clear_mask)
		mm_write(off, mm_read(off) & clear_mask);
	if (set_mask)
		mm_write(off, mm_read(off) | set_mask);
	mutex_unlock(&mmsys_glob_reg_lock);
}

/*
 * The frequency maps, one per IP, extracted from the stock blob.  The
 * field order is map_index, reserved, reg_add, fdiv_num, clk_level,
 * volt_level, axi_index, mtx_index, reserved2, volt_val.
 */

