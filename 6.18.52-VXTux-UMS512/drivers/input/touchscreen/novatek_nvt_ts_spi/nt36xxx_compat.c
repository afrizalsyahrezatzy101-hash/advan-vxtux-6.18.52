// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2010 - 2018 Novatek, Inc.
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
 */

/*
 * Novatek NT36xxx SPI touchscreen - the stock blob's 5.4-shaped API.
 *
 * donors/stock_vendor_modules/novatek_ts_spi.ko was built against 5.4,
 * where a /proc entry took a struct file_operations. Its seven nvt_*_fops
 * tables are 288 bytes each, which is a 5.4 struct file_operations, and
 * the blob's .rela.rodata.novatek_attr_group pins each table's slots:
 *
 *	nvt_flash_fops       llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *	nvt_selftest_fops    llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *	nvt_fw_version_fops  llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *	nvt_baseline_fops    llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *	nvt_raw_fops         llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *	nvt_diff_fops        llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *	nvt_pen_diff_fops    llseek +0x08, read  +0x10, open    +0x70, release +0x80
 *
 * 6.18 replaced that with struct proc_ops. This file keeps the stock's
 * seven tables as real struct file_operations so the blob's symbol set
 * is reproduced under its own names, then bridges each to a proc_ops
 * that the tree's proc_create() sites use.
 *
 * Per-node handlers stay where they already live:
 *   nt36xxx.c            nvt_flash_open / nvt_flash_read / nvt_flash_close
 *   nt36xxx_ext_proc.c   nvt_fw_version_open, nvt_baseline_open,
 *                        nvt_raw_open, nvt_diff_open
 *   nt36xxx_mp_ctrlram.c nvt_selftest_open
 *   nt36xxx_pen.c        nvt_pen_diff_open
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

/*
 * The stock's own tables, reconstructed from its relocation graph: every
 * table's +0x70 carries the node's own open handler and +0x80 the
 * matching release, and +0x08/+0x10 are seq_lseek/seq_read for the six
 * seq nodes. nvt_flash_fops is the exception - the blob has no open
 * handler for it, so +0x08 stays empty, +0x10 is nvt_flash_read, +0x70
 * is nvt_flash_open and +0x80 is nvt_flash_close.
 */
const struct file_operations nvt_flash_fops = {
	.owner		= THIS_MODULE,
	.read		= nvt_flash_read,
	.open		= nvt_flash_open,
	.release	= nvt_flash_close,
};

const struct file_operations nvt_selftest_fops = {
	.owner		= THIS_MODULE,
	.llseek		= seq_lseek,
	.read		= seq_read,
	.open		= nvt_selftest_open,
	.release	= seq_release,
};

const struct file_operations nvt_fw_version_fops = {
	.owner		= THIS_MODULE,
	.llseek		= seq_lseek,
	.read		= seq_read,
	.open		= nvt_fw_version_open,
	.release	= seq_release,
};

const struct file_operations nvt_baseline_fops = {
	.owner		= THIS_MODULE,
	.llseek		= seq_lseek,
	.read		= seq_read,
	.open		= nvt_baseline_open,
	.release	= seq_release,
};

const struct file_operations nvt_raw_fops = {
	.owner		= THIS_MODULE,
	.llseek		= seq_lseek,
	.read		= seq_read,
	.open		= nvt_raw_open,
	.release	= seq_release,
};

const struct file_operations nvt_diff_fops = {
	.owner		= THIS_MODULE,
	.llseek		= seq_lseek,
	.read		= seq_read,
	.open		= nvt_diff_open,
	.release	= seq_release,
};

const struct file_operations nvt_pen_diff_fops = {
	.owner		= THIS_MODULE,
	.llseek		= seq_lseek,
	.read		= seq_read,
	.open		= nvt_pen_diff_open,
	.release	= seq_release,
};

/*
 * 6.18 takes a struct proc_ops. The proc_ops entry points have the same
 * signatures as their file_operations counterparts, so each is a direct
 * cast.
 */
/*
 * The blob's nvt_flash_fops has no llseek slot at all, so reading
 * name.llseek back out of the struct is a read of an uninitialised field
 * and gcc rejects it as a non-constant initializer.  Pass the four entry
 * points in explicitly instead.
 */
#define NVT_FOPS_TO_PROC_OPS(name, open_fn, read_fn, lseek_fn, rel_fn)	\
const struct proc_ops name##_proc_ops = {			\
	.proc_open	= open_fn,					\
	.proc_read	= read_fn,					\
	.proc_lseek	= lseek_fn,					\
	.proc_release	= rel_fn,					\
}

NVT_FOPS_TO_PROC_OPS(nvt_flash_fops, nvt_flash_open, nvt_flash_read, NULL, nvt_flash_close);
NVT_FOPS_TO_PROC_OPS(nvt_selftest_fops, nvt_selftest_open, seq_read, seq_lseek, seq_release);
NVT_FOPS_TO_PROC_OPS(nvt_fw_version_fops, nvt_fw_version_open, seq_read, seq_lseek, seq_release);
NVT_FOPS_TO_PROC_OPS(nvt_baseline_fops, nvt_baseline_open, seq_read, seq_lseek, seq_release);
NVT_FOPS_TO_PROC_OPS(nvt_raw_fops, nvt_raw_open, seq_read, seq_lseek, seq_release);
NVT_FOPS_TO_PROC_OPS(nvt_diff_fops, nvt_diff_open, seq_read, seq_lseek, seq_release);
NVT_FOPS_TO_PROC_OPS(nvt_pen_diff_fops, nvt_pen_diff_open, seq_read, seq_lseek, seq_release);

/*
 * The stock blob's sysfs surface.
 *
 *   lcd_name             100-byte bss buffer, the panel's name
 *   dev_attr_ts_suspend  a 32-byte struct device_attribute
 *   nova_light_suspend_show / _store  the show/store pair
 *   novatek_attrs        a 16-byte struct attribute * array (2 slots)
 *   novatek_attr_group   a 40-byte struct attribute_group
 *
 * The blob's .rela.rodata.novatek_attr_group shows +0x18 (attrs) pointing
 * at novatek_attrs, and .rela.data.nvt_spi_driver shows the attribute's
 * name string plus its show and store handlers.
 */
char lcd_name[100];

/*
 * The stock's ts_suspend attribute. nova_light_suspend_store() is 1012
 * bytes of real code in the blob: it kstrtouint()s the argument, then
 * for a suspend request disables the IRQ, cancels the delayed work,
 * cancels the ESD check, takes the ts mutex, writes the shutdown
 * command over SPI, reports the touch-up transitions and re-enables the
 * IRQ; for a resume request it clears the ESD check and requeues the
 * work. That is the tree's nvt_ts_suspend() / nvt_ts_resume(), split
 * across the value, and nt36xxx.c's nvt_sys_suspend_store() is the
 * forwarder that reaches them.
 */
extern ssize_t nvt_sys_suspend_store(const char *buf, size_t count);

static ssize_t nova_light_suspend_store(struct device *dev,
					 struct device_attribute *attr,
					 const char *buf, size_t count)
{
	return nvt_sys_suspend_store(buf, count);
}

/*
 * The stock's nova_light_suspend_show() is 56 bytes: it snprintf()s the
 * current state with a 0x1000 bound and returns the result. That bound is
 * the stock's, not the page size, so keep it.
 */
extern int32_t nvt_suspend_state;

static ssize_t nova_light_suspend_show(struct device *dev,
					struct device_attribute *attr,
					char *buf)
{
	return snprintf(buf, 0x1000, "%d\n", nvt_suspend_state);
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
 * The stock blob's own _copy_from_user() / _copy_to_user().
 *
 * The blob was built against 5.4, where arm64's copy_from_user() was a
 * macro over a local _copy_from_user() that toggled PAN around the
 * access; 6.18 moved that into __arch_copy_from_user() and made
 * copy_from_user() a plain function, so the blob's symbol no longer has
 * a separate body upstream. Both stock blobs here are local 't' clones
 * of the header macro, and both are pure wrappers:
 *
 *	_copy_from_user(to, from, n)  ->  copy_from_user(to, from, n)
 *	_copy_to_user(to, from, n)    ->  copy_to_user(to, from, n)
 *
 * Same access, same semantics, blob's names kept. See also
 * drivers/mmc/host/sdhci-sprd-ext.c, which keeps the blob's
 * _copy_from_user() for the same reason.
 */
unsigned long _copy_from_user(void *to, const void __user *from,
			       unsigned long n)
{
	if (copy_from_user(to, from, n))
		return n;

	return 0;
}

unsigned long _copy_to_user(void __user *to, const void *from,
			     unsigned long n)
{
	if (copy_to_user(to, from, n))
		return n;

	return 0;
}

/*
 * The stock blob's firmware-download state.
 *
 *   fw_need_write_size       8 bytes - how much of the firmware still
 *                            has to be written
 *   cascade_2nd_header_info  1 byte - the IC is a cascaded part and its
 *                            second header has to be handled as well
 *
 * Both belong to nvt_update_firmware() in the stock. The tree's
 * nt36xxx_fw_update.c is the same routine but tracks the write position
 * in locals, so keep the blob's names as the shared state.
 */
size_t fw_need_write_size;
uint8_t cascade_2nd_header_info;