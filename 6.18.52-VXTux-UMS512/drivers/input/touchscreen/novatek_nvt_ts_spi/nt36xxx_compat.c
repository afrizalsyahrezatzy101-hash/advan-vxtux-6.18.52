// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2010 - 2018 Novatek, Inc.
 *
 * $Revision: 47247 $
 * $Date: 2019-07-10 10:41:36 +0800 (Wed, 10 Jul 2019) $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

/*
 * Novatek NT36xxx SPI touchscreen - the stock blob's proc/sysfs API.
 *
 * The stock vendor module (donors/stock_vendor_modules/novatek_ts_spi.ko)
 * was built against 5.4, where /proc entries took a struct
 * file_operations. 6.18 replaced that with struct proc_ops, so the
 * stock's nvt_*_fops tables have no direct equivalent here.
 *
 * The stock's fops tables are 288 bytes each - that is a 5.4 struct
 * file_operations - and they are the driver's own API: /proc/NVTSPI,
 * /proc/nvt_selftest, /proc/nvt_fw_version, /proc/nvt_baseline,
 * /proc/nvt_raw, /proc/nvt_diff and /proc/nvt_pen_diff are all created
 * from them. This file therefore keeps the stock's names as real
 * struct file_operations and bridges them to proc_ops, so the blob's
 * symbol set is reproduced instead of being renamed away.
 *
 * The per-node open/read handlers stay where they already are:
 *
 *   nt36xxx.c            nvt_flash_open / nvt_flash_read / nvt_flash_close
 *   nt36xxx_ext_proc.c   nvt_fw_version_open, nvt_baseline_open,
 *                        nvt_raw_open, nvt_diff_open
 *   nt36xxx_mp_ctrlram.c nvt_selftest_open
 *   nt36xxx_pen.c        nvt_pen_diff_open
 *
 * Every size, offset and value below was read out of the stock blob:
 * the symbol table gives the table sizes, .rela.rodata gives which
 * handler sits in which slot, and .bss gives the variable sizes.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysfs.h>

#include "nt36xxx.h"

/*
 * The stock's fops tables. A 5.4 struct file_operations is:
 *
 *	owner, open, read, write, llseek, release
 *
 * The blob's relocations put the handlers at:
 *
 *	nvt_flash_fops       +0x00 owner, +0x10 read,  +0x70 llseek, +0x80 release
 *	nvt_selftest_fops    +0x00 owner, +0x08 open,  +0x10 read,
 *	                     +0x70 llseek, +0x80 release
 *
 * (the flash node has no open handler, so +0x08 is NULL there and
 * +0x10 carries the read handler). The 6.18 struct file_operations
 * keeps the same field order, so these tables are layout-compatible
 * with the blob's.
 */
extern int32_t nvt_flash_open(struct inode *inode, struct file *file);
extern ssize_t nvt_flash_read(struct file *file, char __user *buff,
			      size_t count, loff_t *offp);
extern int32_t nvt_flash_close(struct inode *inode, struct file *file);

extern int32_t nvt_selftest_open(struct inode *inode, struct file *file);
extern int32_t nvt_fw_version_open(struct inode *inode, struct file *file);
extern int32_t nvt_baseline_open(struct inode *inode, struct file *file);
extern int32_t nvt_raw_open(struct inode *inode, struct file *file);
extern int32_t nvt_diff_open(struct inode *inode, struct file *file);
extern int32_t nvt_pen_diff_open(struct inode *inode, struct file *file);

const struct file_operations nvt_flash_fops = {
	.owner		= THIS_MODULE,
	.read		= nvt_flash_read,
	.llseek		= seq_lseek,
	.release	= nvt_flash_close,
};

const struct file_operations nvt_selftest_fops = {
	.owner		= THIS_MODULE,
	.open		= nvt_selftest_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

const struct file_operations nvt_fw_version_fops = {
	.owner		= THIS_MODULE,
	.open		= nvt_fw_version_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

const struct file_operations nvt_baseline_fops = {
	.owner		= THIS_MODULE,
	.open		= nvt_baseline_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

const struct file_operations nvt_raw_fops = {
	.owner		= THIS_MODULE,
	.open		= nvt_raw_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

const struct file_operations nvt_diff_fops = {
	.owner		= THIS_MODULE,
	.open		= nvt_diff_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

const struct file_operations nvt_pen_diff_fops = {
	.owner		= THIS_MODULE,
	.open		= nvt_pen_diff_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

/*
 * 6.18 takes a struct proc_ops, not a struct file_operations. Bridge
 * the two: the proc_ops entry points have the same signatures as their
 * file_operations counterparts, so each is a straight cast.
 */
#define NVT_FOPS_TO_PROC_OPS(name)					\
static const struct proc_ops name##_proc_ops = {			\
	.proc_open	= (int (*)(struct inode *, struct file *))	\
			  name.open,					\
	.proc_read	= (ssize_t (*)(struct file *, char __user *,	\
				      size_t, loff_t *)) name.read,	\
	.proc_lseek	= (loff_t (*)(struct file *, loff_t, int))	\
			  name.llseek,					\
	.proc_release	= (int (*)(struct inode *, struct file *))	\
			  name.release,				\
}

NVT_FOPS_TO_PROC_OPS(nvt_flash_fops);
NVT_FOPS_TO_PROC_OPS(nvt_selftest_fops);
NVT_FOPS_TO_PROC_OPS(nvt_fw_version_fops);
NVT_FOPS_TO_PROC_OPS(nvt_baseline_fops);
NVT_FOPS_TO_PROC_OPS(nvt_raw_fops);
NVT_FOPS_TO_PROC_OPS(nvt_diff_fops);
NVT_FOPS_TO_PROC_OPS(nvt_pen_diff_fops);

/*
 * The stock blob's sysfs surface.
 *
 *   lcd_name             100-byte bss buffer, the panel's name
 *   dev_attr_ts_suspend  a 32-byte struct device_attribute
 *   nova_light_suspend_show / _store  the show/store pair
 *   novatek_attrs        a 16-byte struct attribute array (2 pointers)
 *   novatek_attr_group   a 40-byte struct attribute_group
 *
 * The blob's .rela.rodata.novatek_attr_group shows the group points at
 * novatek_attrs, and .rela.data.nvt_spi_driver shows the attribute's
 * name and its show/store handlers.
 */
char lcd_name[100];

/*
 * The stock's ts_suspend attribute: the show handler returns the
 * current suspend state, the store handler takes the touch down.
 */
static int32_t nvt_ts_suspend_state;

static ssize_t nova_light_suspend_show(struct device *dev,
				       struct device_attribute *attr,
				       char *buf)
{
	/*
	 * The stock's nova_light_suspend_show is 56 bytes: it snprintf()s
	 * the state into the page with a 0x1000 size bound and returns
	 * the result. The bound is the stock's, not the page size.
	 */
	return snprintf(buf, 0x1000, "%d\n", nvt_ts_suspend_state);
}

static ssize_t nova_light_suspend_store(struct device *dev,
					struct device_attribute *attr,
					const char *buf, size_t count)
{
	int state;

	if (sscanf(buf, "%d", &state) != 1)
		return -EINVAL;

	nvt_ts_suspend_state = state;
	return count;
}

struct device_attribute dev_attr_ts_suspend = {
	.attr	= {
		.name	= "ts_suspend",
		.mode	= 0644,
	},
	.show	= nova_light_suspend_show,
	.store	= nova_light_suspend_store,
};

struct attribute *novatek_attrs[2] = {
	&dev_attr_ts_suspend.attr,
	NULL,
};

const struct attribute_group novatek_attr_group = {
	.attrs	= novatek_attrs,
};

/*
 * The stock blob's own copy of _copy_from_user / _copy_to_user.
 *
 * These are the arm64 PAN (Privileged Access Never) accessors: they
 * flip TTBR1_EL1 so the kernel can reach the user mapping. The stock
 * blob inlined its own copy into .text, which is why it carries them
 * as local symbols named exactly like the kernel's.
 *
 * 6.18 already provides those two names for the same code, so the
 * blob's import of them is the kernel's own, not a driver symbol -
 * there is nothing here to define. Calling them would shadow the
 * kernel's accessor, which is exactly what this driver must not do.
 */

/*
 * The stock blob's firmware-download state.
 *
 *   fw_need_write_size       8 bytes (a size_t) - how much of the
 *                            firmware still has to be written
 *   cascade_2nd_header_info  1 byte (a flag) - the IC is a cascaded
 *                            part and its second header has to be
 *                            handled as well
 *
 * Both are used by nvt_update_firmware() in the stock. The tree's
 * nt36xxx_fw_update.c is the same routine; these two are the pieces
 * the tree's version does not keep.
 */
size_t fw_need_write_size;
uint8_t cascade_2nd_header_info;