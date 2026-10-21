// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2026 Spreadtrum Communications Inc.
 *
 * sprd_time_sync - the time correction device.
 *
 * A character device at /dev/sprd_time_sync hands out the various
 * clocks of the system as one snapshot, and the same snapshot can be
 * pushed to userspace as a uevent.
 *
 * The snapshot is seven 64 bit words:
 *
 *   [0] unused
 *   [1] CLOCK_MONOTONIC in nanoseconds
 *   [2] CLOCK_BOOTTIME in nanoseconds
 *   [3] CLOCK_REALTIME in seconds
 *   [4] the arm64 generic timer counter
 *   [5] the 26MHz systimer counter
 *   [6] the timezone, as minuteswest and dsttime
 */

#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/mod_devicetable.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/time.h>
#include <linux/uaccess.h>

#include <asm/timex.h>

#include <linux/sprd_systimer.h>

/* Defined below; exported under this name by the stock blob. */
void sprd_send_ap_time(void);

#define SPRD_TIME_SYNC_DEV_NAME	"sprd_time_sync"
#define SPRD_TIME_SYNC_CLASS_NAME	"sprd_time_sync"

/* the size of the snapshot that is read out */
#define SPRD_TIME_SYNC_DATA_SIZE	(7 * sizeof(u64))

/* the size of the uevent environment */
#define SPRD_TIME_SYNC_ENV_SIZE		100

struct sprd_time_sync_data {
	u64 unused;
	u64 mono_ns;
	u64 boot_ns;
	u64 real_s;
	u64 ts_cnt;
	u64 systimer_cnt;
	s32 minuteswest;
	s32 dsttime;

	/* the platform device that owns this instance */
	struct platform_device *pdev;
	/* the char device that was registered for it */
	struct cdev cdev;
	/* the device node */
	struct device *dev;
	/* the major and minor of the char device */
	dev_t devt;
};

static struct sprd_time_sync_data *tcd_dev;

static void sprd_time_sync_fill(struct sprd_time_sync_data *tcd)
{
	tcd->mono_ns = ktime_get_mono_fast_ns();
	tcd->boot_ns = ktime_get_boot_fast_ns();
	tcd->real_s = ktime_get_real_seconds();
	tcd->ts_cnt = arch_timer_read_counter();
	tcd->systimer_cnt = sprd_systimer_read();
	tcd->minuteswest = sys_tz.tz_minuteswest;
	tcd->dsttime = sys_tz.tz_dsttime;
}

/*
 * Push the current snapshot to userspace as a uevent on the device
 * node, so that a listener can pick the time up without polling.
 */
void sprd_send_ap_time(void)
{
	char env[SPRD_TIME_SYNC_ENV_SIZE];
	char env_boottime[SPRD_TIME_SYNC_ENV_SIZE];
	char env_realtime[SPRD_TIME_SYNC_ENV_SIZE];
	char env_tscnt[SPRD_TIME_SYNC_ENV_SIZE];
	char env_sysfrt[SPRD_TIME_SYNC_ENV_SIZE];
	char env_minuteswest[SPRD_TIME_SYNC_ENV_SIZE];
	char env_dsttime[SPRD_TIME_SYNC_ENV_SIZE];
	char *envp[] = {
		env, env_boottime, env_realtime, env_tscnt,
		env_sysfrt, env_minuteswest, env_dsttime,
	};

	if (!tcd_dev) {
		printk("sprd_time_sync: tcd_dev is not ready\n");
		return;
	}

	sprd_time_sync_fill(tcd_dev);

	snprintf(env, sizeof(env), "monotime_ns=%lld", tcd_dev->mono_ns);
	snprintf(env_boottime, sizeof(env_boottime), "boottime_ns=%lld",
		 tcd_dev->boot_ns);
	snprintf(env_realtime, sizeof(env_realtime), "realtime_s=%lld",
		 tcd_dev->real_s);
	snprintf(env_tscnt, sizeof(env_tscnt), "ts_cnt=%lld",
		 tcd_dev->ts_cnt);
	snprintf(env_sysfrt, sizeof(env_sysfrt), "sysfrt_cnt=%lld",
		 tcd_dev->systimer_cnt);
	snprintf(env_minuteswest, sizeof(env_minuteswest), "minuteswest=%d",
		 tcd_dev->minuteswest);
	snprintf(env_dsttime, sizeof(env_dsttime), "dsttime=%d",
		 tcd_dev->dsttime);

	kobject_uevent_env(&tcd_dev->dev->kobj, 2, envp);

	dev_info(tcd_dev->dev,
		 "send %s %s %s %s %s %s %s to userspace\n",
		 env, env_boottime, env_realtime, env_tscnt,
		 env_sysfrt, env_minuteswest, env_dsttime);
}
EXPORT_SYMBOL(sprd_send_ap_time);

static ssize_t sprd_time_sync_read(struct file *file, char __user *buf,
				    size_t count, loff_t *ppos)
{
	struct sprd_time_sync_data data = { 0 };

	if (!tcd_dev)
		return 0;

	sprd_time_sync_fill(&data);

	if (copy_to_user(buf, &data, SPRD_TIME_SYNC_DATA_SIZE))
		return -EFAULT;

	return SPRD_TIME_SYNC_DATA_SIZE;
}

static int sprd_time_sync_open(struct inode *inode, struct file *file)
{
	file->private_data = tcd_dev;
	return 0;
}

static int sprd_time_sync_release(struct inode *inode, struct file *file)
{
	return 0;
}

static const struct file_operations sprd_time_sync_fops = {
	.owner = THIS_MODULE,
	.read = sprd_time_sync_read,
	.open = sprd_time_sync_open,
	.release = sprd_time_sync_release,
	.llseek = default_llseek,
};

struct class *sprd_time_sync_class;
EXPORT_SYMBOL_GPL(sprd_time_sync_class);

static int sprd_time_sync_probe(struct platform_device *pdev)
{
	struct sprd_time_sync_data *tcd;
	dev_t devt;
	int ret;

	dev_info(&pdev->dev, "start parse sprd_time_sync\n");

	sprd_time_sync_class = class_create(SPRD_TIME_SYNC_CLASS_NAME);
	if (IS_ERR(sprd_time_sync_class)) {
		ret = PTR_ERR(sprd_time_sync_class);
		dev_err(&pdev->dev,
			"Failed to parse sprd_time_sync device tree, ret=%d\n",
			ret);
		return ret;
	}

	tcd = devm_kzalloc(&pdev->dev, sizeof(*tcd), GFP_KERNEL);
	if (!tcd)
		return -ENOMEM;

	tcd->pdev = pdev;
	platform_set_drvdata(pdev, tcd);

	ret = alloc_chrdev_region(&devt, 0, 1, SPRD_TIME_SYNC_DEV_NAME);
	if (ret) {
		dev_err(&pdev->dev, "Failed to alloc sprd_time_sync chrdev\n");
		goto err_class;
	}

	cdev_init(&tcd->cdev, &sprd_time_sync_fops);
	tcd->cdev.owner = THIS_MODULE;
	ret = cdev_add(&tcd->cdev, devt, 1);
	if (ret) {
		dev_err(&pdev->dev, "Failed to add sprd_time_sync cdev\n");
		goto err_chrdev;
	}

	tcd->devt = devt;
	tcd->dev = device_create(sprd_time_sync_class, NULL, devt, NULL, SPRD_TIME_SYNC_DEV_NAME);
	if (IS_ERR(tcd->dev)) {
		ret = PTR_ERR(tcd->dev);
		dev_err(&pdev->dev, "Failed to create sprd_time_sync device\n");
		goto err_cdev;
	}

	tcd_dev = tcd;

	dev_info(&pdev->dev, "success parse sprd_time_sync\n");

	return 0;

err_cdev:
	cdev_del(&tcd->cdev);
err_chrdev:
	unregister_chrdev_region(devt, 1);
err_class:
	class_destroy(sprd_time_sync_class);
	return ret;
}

static void sprd_time_sync_remove(struct platform_device *pdev)
{
	struct sprd_time_sync_data *tcd = platform_get_drvdata(pdev);

	if (tcd) {
		device_destroy(tcd->dev->class, tcd->devt);
		cdev_del(&tcd->cdev);
		unregister_chrdev_region(tcd->devt, 1);
		tcd = NULL;
	}
	if (tcd_dev)
		tcd_dev = NULL;
}

static const struct of_device_id sprd_time_sync_match_table[] = {
	{ .compatible = "sprd,time-sync" },
	{ }
};
MODULE_DEVICE_TABLE(of, sprd_time_sync_match_table);

static struct platform_driver sprd_time_sync_driver = {
	.probe = sprd_time_sync_probe,
	.remove = sprd_time_sync_remove,
	.driver = {
		.name = "sprd_time_sync",
		.of_match_table = sprd_time_sync_match_table,
	},
};

static int __init sprd_time_sync_init(void)
{
	return platform_driver_register(&sprd_time_sync_driver);
}

static void __exit sprd_time_sync_exit(void)
{
	platform_driver_unregister(&sprd_time_sync_driver);
}

module_init(sprd_time_sync_init);
module_exit(sprd_time_sync_exit);

MODULE_DESCRIPTION("sprd_time_sync driver");
MODULE_AUTHOR("Catdeo Zhang & Ruifeng Zhang");
MODULE_LICENSE("GPL");
