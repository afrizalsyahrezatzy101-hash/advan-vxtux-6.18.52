// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Spreadtrum Communications Inc.
 *
 * Unisoc systimer and sysfrt driver.
 *
 * Two free running counters live in the always-on domain:
 *
 *   sprd,syst-timer   a 26MHz counter, u32, read at base + 0xc
 *   sprd,sysfrt-timer a 32kHz counter, u64, read at base + 0xc / + 0x4
 *
 * The counters are not latched when they are read, so converting one
 * into a wall clock time needs a reference point: the boot time and
 * the counter value that was read at that moment. A hrtimer re-reads
 * both every 600 seconds so the reference stays fresh, and the
 * systimer_seq counter tells a reader that it caught the reference
 * while it was being updated.
 */

#include <linux/clocksource.h>
#include <linux/delay.h>
#include <linux/hrtimer.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/module.h>

#define SYSTIMER_VALUE_OFFSET	0xc
#define SYSTIMER_REG_SIZE	0x10

/* the reference is re-read every 600 seconds */
#define SYSTIMER_SYNC_INTERVAL	600ULL * NSEC_PER_SEC

struct sprd_systimer_convert {
	u64 last_cnt;		/* the counter value at the reference */
	u64 mult;		/* ns per counter tick, as a fraction */
	u32 shift;
	u64 boot_time;		/* the boot time of the reference */
};

static void __iomem *sprd_systimer_addr_base;
static void __iomem *sprd_sysfrt_addr_base;

/* the systimer counter is 26MHz, the sysfrt counter is 32kHz */
static u32 systimer_mult;
static u32 systimer_shift;
static u32 sysfrt_mult;
static u32 sysfrt_shift;

static u64 cnter_to_boottime;
static u32 systimer_cnt;
static u64 sysfrt_cnt;

/* bumped around every update of the reference, so a reader can tell
 * that it read a torn reference
 */
static u32 systimer_seq;

/* the hrtimer that keeps the reference fresh */
static struct hrtimer cnt_to_boot_timer;

static inline u32 sprd_systimer_read_raw(void)
{
	if (!sprd_systimer_addr_base)
		return 0;
	return readl(sprd_systimer_addr_base + SYSTIMER_VALUE_OFFSET);
}

static inline u64 sprd_sysfrt_read_raw(void)
{
	if (!sprd_sysfrt_addr_base)
		return 0;
	return (u64)readl(sprd_sysfrt_addr_base + 0x4) |
	       ((u64)readl(sprd_sysfrt_addr_base + SYSTIMER_VALUE_OFFSET) << 32);
}

u32 sprd_systimer_read(void)
{
	return sprd_systimer_read_raw();
}

u64 sprd_sysfrt_read(void)
{
	return sprd_sysfrt_read_raw();
}
EXPORT_SYMBOL(sprd_systimer_read);
EXPORT_SYMBOL(sprd_sysfrt_read);

/*
 * Convert a counter value into a boot time in nanoseconds.
 *
 * timer_type 0 is the 26MHz systimer, 1 is the 32kHz sysfrt counter.
 */
u64 sprd_systimer_to_boottime(u64 cnt, int timer_type)
{
	u64 delta;

	if (timer_type == 1) {
		if (!sprd_sysfrt_addr_base)
			return 0;
		smp_rmb();
		delta = cnt - sysfrt_cnt;
		return (delta * sysfrt_mult >> sysfrt_shift) + cnter_to_boottime;
	} else if (timer_type == 0) {
		if (!sprd_systimer_addr_base)
			return 0;
		smp_rmb();
		delta = (u32)(cnt - systimer_cnt);
		return (delta * systimer_mult >> systimer_shift) + cnter_to_boottime;
	}
	return 0;
}
EXPORT_SYMBOL(sprd_systimer_to_boottime);

/*
 * Copy the conversion parameters out for a reader that wants to do
 * the conversion itself. The seq counter guards against a torn read.
 *
 *   para[0] the boot time of the reference
 *   para[1] the systimer counter at the reference
 *   para[2] the sysfrt counter at the reference
 *   para[3] the systimer multiplier
 *   para[4] the sysfrt multiplier
 */
void get_convert_para(u64 *para)
{
	u32 seq;

	if (!para)
		return;

	do {
		seq = systimer_seq;
		smp_rmb();
		para[0] = cnter_to_boottime;
		para[1] = systimer_cnt;
		para[2] = sysfrt_cnt;
		para[3] = systimer_mult;
		para[4] = sysfrt_mult;
		smp_rmb();
	} while (systimer_seq != seq);
}
EXPORT_SYMBOL(get_convert_para);

static enum hrtimer_restart sync_cnter_boottime(struct hrtimer *timer)
{
	systimer_seq++;
	smp_wmb();
	cnter_to_boottime = ktime_get_boot_fast_ns();
	systimer_cnt = sprd_systimer_read_raw();
	sysfrt_cnt = sprd_sysfrt_read_raw();
	smp_wmb();
	systimer_seq++;

	hrtimer_forward_now(timer, SYSTIMER_SYNC_INTERVAL);
	return HRTIMER_RESTART;
}

static int __init sprd_systimer_probe(void)
{
	struct device_node *np;
	void __iomem *base;

	np = of_find_compatible_node(NULL, NULL, "sprd,syst-timer");
	if (np) {
		base = of_iomap(np, 0);
		if (!base) {
			pr_err("sprd_systimer: Can't map sprd systimer reg!\n");
		} else {
			sprd_systimer_addr_base = base;
			clocks_calc_mult_shift(&systimer_mult, &systimer_shift,
					       1000, 1000000000, 10);
		}
	}

	np = of_find_compatible_node(NULL, NULL, "sprd,sysfrt-timer");
	if (np) {
		base = of_iomap(np, 0);
		if (!base) {
			pr_err("sprd_systimer: Can't map sprd sysfrt reg!\n");
		} else {
			sprd_sysfrt_addr_base = base;
			clocks_calc_mult_shift(&sysfrt_mult, &sysfrt_shift,
					       0x8000, 1000000000, 10);
		}
	}

	if (!sprd_systimer_addr_base && !sprd_sysfrt_addr_base) {
		pr_err("sprd_systimer: no systimer or sysfrt dts node.\n");
		return 0;
	}

	cnter_to_boottime = ktime_get_boot_fast_ns();
	systimer_cnt = sprd_systimer_read_raw();
	sysfrt_cnt = sprd_sysfrt_read_raw();

	hrtimer_setup(&cnt_to_boot_timer, sync_cnter_boottime,
			  CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrtimer_start_range_ns(&cnt_to_boot_timer, SYSTIMER_SYNC_INTERVAL,
				0, HRTIMER_MODE_REL);

	return 0;
}

static int __init sprd_systimer_init(void)
{
	return sprd_systimer_probe();
}

module_init(sprd_systimer_init);

MODULE_DESCRIPTION("Unisoc systimer and sysfrt Driver");
MODULE_LICENSE("GPL");
