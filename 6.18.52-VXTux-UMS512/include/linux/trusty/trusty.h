/*
 * Copyright (C) 2013 Google, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __LINUX_TRUSTY_TRUSTY_H
#define __LINUX_TRUSTY_TRUSTY_H

#include <linux/kernel.h>
#include <linux/kconfig.h>
#include <linux/trusty/sm_err.h>
#include <linux/device.h>
#include <linux/pagemap.h>


/*
 * IS_ENABLED(), not #ifdef CONFIG_TRUSTY.
 *
 * CONFIG_TRUSTY is tristate. When it is =m, autoconf.h defines
 * CONFIG_TRUSTY_MODULE and NOT CONFIG_TRUSTY, so #ifdef CONFIG_TRUSTY
 * is false and the header fell through to the static inline stub
 * branch - while drivers/trusty/trusty-core.c still defines the real
 * functions. The two collided:
 *
 *   trusty-core.c:145:5: error: redefinition of 'trusty_fast_call32'
 *   trusty-core.c:155:5: error: redefinition of 'trusty_fast_call64'
 *   trusty-core.c:293:5: error: redefinition of 'trusty_std_call32'
 *
 * IS_ENABLED(CONFIG_TRUSTY) is true for =y AND for =m, so the
 * declaration branch is taken in both cases. The stub branch is then
 * only reached when the driver really is not built at all, which is
 * what it is for.
 */
#if IS_ENABLED(CONFIG_TRUSTY)
s32 trusty_std_call32(struct device *dev, u32 smcnr, u32 a0, u32 a1, u32 a2);
s32 trusty_fast_call32(struct device *dev, u32 smcnr, u32 a0, u32 a1, u32 a2);
s32 trusty_fast_call32_power(u32 smcnr, u32 a0, u32 a1, u32 a2);

#ifdef CONFIG_64BIT
s64 trusty_fast_call64(struct device *dev, u64 smcnr, u64 a0, u64 a1, u64 a2);
#endif
#else
static inline s32 trusty_std_call32(struct device *dev, u32 smcnr,
				    u32 a0, u32 a1, u32 a2)
{
	return SM_ERR_UNDEFINED_SMC;
}
static inline s32 trusty_fast_call32(struct device *dev, u32 smcnr,
				     u32 a0, u32 a1, u32 a2)
{
	return SM_ERR_UNDEFINED_SMC;
}
#ifdef CONFIG_64BIT
static inline s64 trusty_fast_call64(struct device *dev,
				     u64 smcnr, u64 a0, u64 a1, u64 a2)
{
	return SM_ERR_UNDEFINED_SMC;
}
#endif
#endif

struct notifier_block;
enum {
	TRUSTY_CALL_PREPARE,
	TRUSTY_CALL_RETURNED,
	/*
	 * Nilai ketiga ada di donor (work/donors_20260930/bocchi_unisoc54,
	 * drivers/trusty/trusty.h) dan DIPAKAI dua berkas yang diport:
	 * trusty-core.c:209 (atomic_notifier_call_chain saat secure OS panic)
	 * dan trusty-log.c:119/155 (dump log ke console). Tanpa nilai ini kedua
	 * berkas tidak bisa dikompilasi.
	 */
	TRUSTY_CALL_PANIC,
};
int trusty_call_notifier_register(struct device *dev,
				  struct notifier_block *n);
int trusty_call_notifier_unregister(struct device *dev,
				    struct notifier_block *n);
const char *trusty_version_str_get(struct device *dev);
u32 trusty_get_api_version(struct device *dev);

struct ns_mem_page_info {
	uint64_t attr;
};

int trusty_encode_page_info(struct ns_mem_page_info *inf,
			    struct page *page, pgprot_t pgprot);

int trusty_call32_mem_buf(struct device *dev, u32 smcnr,
			  struct page *page,  u32 size,
			  pgprot_t pgprot);

struct trusty_nop {
	struct list_head node;
	u32 args[3];
};

static inline void trusty_nop_init(struct trusty_nop *nop,
				   u32 arg0, u32 arg1, u32 arg2) {
	INIT_LIST_HEAD(&nop->node);
	nop->args[0] = arg0;
	nop->args[1] = arg1;
	nop->args[2] = arg2;
}

void trusty_enqueue_nop(struct device *dev, struct trusty_nop *nop);
void trusty_dequeue_nop(struct device *dev, struct trusty_nop *nop);

#endif
