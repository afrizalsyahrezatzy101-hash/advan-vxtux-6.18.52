// SPDX-License-Identifier: GPL-2.0
/*
 * Spreadtrum SDHCI R11 additions: power-on write protect (POWP) user-space
 * interface, tuning read-back helper and the SWCQ request/IRQ entry points.
 *
 * Ported from the Unisoc UMS512 4.14 sources (sdhci-sprd-powp.c,
 * sdhci-sprd-tuning.c, sdhci-sprd-swcq.c) to the 6.18 MMC/SDHCI API.
 */

#include <linux/blkdev.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>

#include <linux/mmc/card.h>
#include <linux/mmc/host.h>
#include <linux/mmc/ioctl.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/sdio_func.h>

#include "sdhci.h"
#include "mmc_hsq.h"

#define SPRD_WP_MAGIC_KEY	0x53505244	/* "SPRD" */
#define SPRD_WP_MAX_DEVICES	256

#define MMC_SEND_WR_PROT_TYPE		31	/* not in MMC spec */
#define EXT_CSD_CLASS_6_CTRL		59
#define EXT_CSD_USR_WP			171
#define EXT_CSD_CMD_SET_NORMAL		(1 << 0)
#define EXT_CSD_USR_WP_EN_PWR_WP		1

/*
 * The host only ever drives eMMC here (mmc0); the SD slot has no SWCQ/POWP.
 * Mirrors the vendor HOST_IS_EMMC_TYPE().
 */
#define SPRD_HOST_IS_EMMC(c) \
	(((c)->caps2 & (MMC_CAP2_NO_SDIO | MMC_CAP2_NO_SD)) == \
	 (MMC_CAP2_NO_SDIO | MMC_CAP2_NO_SD))

struct sprd_mmc_wp {
	bool inited;
	struct device dev;
	struct cdev chrdev;
	dev_t devt;
	struct mmc_host *mmc;
	unsigned int enabled;
	unsigned int grp_cnt;
	unsigned int grp_size;
	u64 start_addr;
	u64 prot_size;
};

static struct sprd_mmc_wp sprd_wp;
static struct bus_type sprd_wp_bus_type;

static int mmc_set_powp(struct mmc_card *card);
static void mmc_get_wp_current_status_and_type(struct mmc_card *card);

/*
 * sdhci_send_command() is static inside sdhci.c, and so is
 * sdhci_manual_cmd23().  Both are needed by the synchronous request paths
 * below, so sdhci.c exports one small pair of wrappers for them.  A copy of
 * the command-send path in this file would drift from upstream on every
 * merge; two forwarders cannot.
 */
bool sdhci_sprd_send_command(struct sdhci_host *host, struct mmc_command *cmd);

static inline bool sdhci_sprd_manual_cmd23(struct sdhci_host *host,
					   struct mmc_request *mrq)
{
	return mrq->sbc && !(host->flags & SDHCI_AUTO_CMD23);
}

/* ------------------------------------------------------------------ */
/* power-on write protect: status/type read-back                       */
/* ------------------------------------------------------------------ */

static int mmc_get_wp_current_status(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	struct mmc_command cmd = {0};
	struct mmc_request mrq = {NULL};
	struct mmc_data data = {0};
	struct scatterlist sg;
	u64 wp_bits_sts = 0;
	unsigned long long size = 4;
	unsigned int i;
	int err = 0;
	u8 *status_buf;

	mrq.cmd = &cmd;
	mrq.data = &data;

	status_buf = kzalloc(size, GFP_KERNEL);
	if (!status_buf) {
		pr_err("%s:(WP) status_buf create false\n", mmc_hostname(host));
		return -ENOMEM;
	}

	/* CMD30 gets the write-protect bit per group */
	cmd.opcode = MMC_SEND_WRITE_PROT;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	cmd.arg = sprd_wp.start_addr;

	data.blksz = size;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;
	data.blk_addr = 0;
	data.sg = &sg;
	data.sg_len = 1;
	sg_init_one(&sg, status_buf, size);

	mmc_wait_for_req(host, &mrq);

	if (cmd.error) {
		pr_err("%s:(WP) status cmd error %d\n", mmc_hostname(host), cmd.error);
		err = cmd.error;
		goto out;
	}

	if (data.error) {
		pr_err("%s:(WP) status data error %d\n", mmc_hostname(host), data.error);
		err = data.error;
		goto out;
	}

	for (i = 0; i < 4; i++)
		wp_bits_sts = (wp_bits_sts << 8) + status_buf[i];

	pr_info("%s:(WP) the status of write protection bits: 0x%llx\n",
		mmc_hostname(host), wp_bits_sts);
out:
	kfree(status_buf);
	return err;
}

static int mmc_get_wp_current_type(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	struct mmc_command cmd = {0};
	struct mmc_request mrq = {NULL};
	struct mmc_data data = {0};
	struct scatterlist sg;
	unsigned long long size = 8;
	u64 actual_type = 0;
	u64 expected_type = 0;
	unsigned int i;
	int err = 0;
	u8 *type_buf;

	mrq.cmd = &cmd;
	mrq.data = &data;

	type_buf = kzalloc(size, GFP_KERNEL);
	if (!type_buf) {
		pr_err("%s:(WP) type_buf create false\n", mmc_hostname(host));
		return -ENOMEM;
	}

	/* CMD31 (Unisoc ext) gets the per-group protection type */
	cmd.opcode = MMC_SEND_WR_PROT_TYPE;
	cmd.flags = MMC_RSP_SPI_R1 | MMC_RSP_R1 | MMC_CMD_ADTC;
	cmd.arg = sprd_wp.start_addr;

	data.blksz = size;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;
	data.blk_addr = 0;
	data.sg = &sg;
	data.sg_len = 1;
	sg_init_one(&sg, type_buf, size);

	mmc_wait_for_req(host, &mrq);

	if (cmd.error) {
		pr_err("%s:(WP) status cmd error %d\n", mmc_hostname(host), cmd.error);
		err = cmd.error;
		goto out;
	}

	if (data.error) {
		pr_err("%s:(WP) status data error %d\n", mmc_hostname(host), data.error);
		err = data.error;
		goto out;
	}

	for (i = 0; i < 8; i++)
		actual_type = (actual_type << 8) + type_buf[i];

	pr_info("%s:(WP) the really type of write protection grout: 0x%llx\n",
		mmc_hostname(host), actual_type);

	for (i = 0; i < sprd_wp.grp_cnt; i++)
		expected_type = (expected_type << 2) + (sprd_wp.enabled ? 0x2 : 0);

	pr_info("%s:(WP) the expected type of write protection group: 0x%llx\n",
		mmc_hostname(host), expected_type);

	if (actual_type != expected_type) {
		pr_err("%s:(WP) actual type not equal to expected type\n",
		       mmc_hostname(host));
		err = -EINVAL;
	}
out:
	kfree(type_buf);
	return err;
}

static void mmc_get_wp_current_status_and_type(struct mmc_card *card)
{
	mmc_get_wp_current_status(card);
	mmc_get_wp_current_type(card);
}

/* ------------------------------------------------------------------ */
/* power-on write protect: applying the protection                     */
/* ------------------------------------------------------------------ */

static int mmc_enable_usr_wp(struct mmc_card *card, unsigned char wp)
{
	struct mmc_host *host = card->host;
	struct mmc_command cmd = {0};
	struct mmc_request mrq = {NULL};

	mrq.cmd = &cmd;

	/* clear CLASS_6 control */
	cmd.opcode = MMC_SWITCH;
	cmd.arg = (MMC_SWITCH_MODE_CLEAR_BITS << 24) |
		  (EXT_CSD_CLASS_6_CTRL << 16) | EXT_CSD_CMD_SET_NORMAL;
	cmd.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;
	mmc_wait_for_req(host, &mrq);
	if (cmd.error) {
		pr_err("%s:(WP) set class6 ctrl, cmd error %d\n",
		       mmc_hostname(host), cmd.error);
		return cmd.error;
	}

	/* reset ERASE_GROUP_DEF to 1 */
	cmd.arg = (MMC_SWITCH_MODE_CLEAR_BITS << 24) |
		  (EXT_CSD_ERASE_GROUP_DEF << 16) | (1 << 8) |
		  EXT_CSD_CMD_SET_NORMAL;
	mmc_wait_for_req(host, &mrq);
	if (cmd.error) {
		pr_err("%s:(WP) set erase group def fail, cmd error %d\n",
		       mmc_hostname(host), cmd.error);
		return cmd.error;
	}

	/* set USR_WP */
	cmd.arg = (MMC_SWITCH_MODE_SET_BITS << 24) | (EXT_CSD_USR_WP << 16) |
		  (wp << 8) | EXT_CSD_CMD_SET_NORMAL;
	mmc_wait_for_req(host, &mrq);
	if (cmd.error) {
		pr_err("%s:(WP) set usr_wp fail, cmd error %d\n",
		       mmc_hostname(host), cmd.error);
		return cmd.error;
	}

	return 0;
}

static int mmc_set_usr_wp(struct mmc_card *card, unsigned char wp_type)
{
	struct mmc_host *host = card->host;
	struct mmc_command cmd = {0};
	struct mmc_request mrq = {NULL};
	unsigned int i;
	int err;

	err = mmc_enable_usr_wp(card, wp_type);
	if (err)
		return err;

	mrq.cmd = &cmd;
	cmd.opcode = MMC_SET_WRITE_PROT;
	cmd.flags = MMC_RSP_SPI_R1B | MMC_RSP_R1B | MMC_CMD_AC;

	for (i = 0; i < sprd_wp.grp_cnt; i++) {
		cmd.arg = sprd_wp.start_addr + (u64)i * sprd_wp.grp_size;
		mmc_wait_for_req(host, &mrq);
		if (cmd.error) {
			pr_err("%s:(WP) set fail, cmd error %d\n",
			       mmc_hostname(host), cmd.error);
			return cmd.error;
		}
	}

	return 0;
}

static int mmc_get_wp_params(struct mmc_card *card)
{
	struct mmc_host *host = card->host;
	struct device_node *np = dev_of_node(host->parent);
	u32 data[2];
	u64 temp64;
	int err;

	if (!np)
		return -ENODEV;

	err = of_property_read_u32_array(np, "sprd,part-address-size",
					  data, ARRAY_SIZE(data));
	if (err) {
		pr_err("%s:(WP) no sprd,part-address-size setting in node\n",
		       mmc_hostname(host));
		return err;
	}

	sprd_wp.start_addr = data[0];
	sprd_wp.prot_size = data[1];

	sprd_wp.grp_size = card->ext_csd.hc_erase_size *
			   card->ext_csd.raw_hc_erase_gap_size;
	if (!sprd_wp.grp_size)
		return -EINVAL;

	temp64 = sprd_wp.prot_size;
	if (do_div(temp64, sprd_wp.grp_size)) {
		pr_err("%s:(WP) partition is not aligned\n", mmc_hostname(host));
		return -EINVAL;
	}
	sprd_wp.grp_cnt = (unsigned int)temp64;

	pr_info("%s:(WP) start address: 0x%llx, size: 0x%llx, group cnt: 0x%x\n",
		mmc_hostname(host), sprd_wp.start_addr, sprd_wp.prot_size,
		sprd_wp.grp_cnt);

	return 0;
}

/**
 * mmc_set_powp() - apply the power-on write protect state to the eMMC
 * @card: eMMC card to program
 *
 * Reads the partition geometry from the DT, then either reports the current
 * protection state or programs the erase groups for power-on write protect.
 */
int mmc_set_powp(struct mmc_card *card)
{
	struct mmc_host *mmc = card->host;
	int err;

	if (mmc->index != 0) {
		pr_err("%s:(WP) mmc index err\n", mmc_hostname(mmc));
		return -EINVAL;
	}

	err = mmc_get_wp_params(card);
	if (err)
		pr_err("%s:(WP) get params err\n", mmc_hostname(mmc));

	if (!sprd_wp.enabled) {
		mmc_get_wp_current_status_and_type(card);
		pr_info("%s:(WP) status is disabled\n", mmc_hostname(mmc));
		return 0;
	}

	err = mmc_set_usr_wp(card, EXT_CSD_USR_WP_EN_PWR_WP);
	if (err)
		return err;

	mmc_get_wp_current_status_and_type(card);
	pr_info("%s:(WP) status is enabled\n", mmc_hostname(mmc));

	return 0;
}
EXPORT_SYMBOL_GPL(mmc_set_powp);

/**
 * mmc_check_wp_fn() - is the POWP user-space interface enabled in DT?
 * @host: host whose DT node is queried
 *
 * Return: 0 when "sprd,wp_fn" is "enable", <0 otherwise.
 */
int mmc_check_wp_fn(struct mmc_host *host)
{
	struct device_node *np = dev_of_node(host->parent);
	const char *name;
	int ret;

	if (!np)
		return -EINVAL;

	ret = of_property_read_string(np, "sprd,wp_fn", &name);
	if (ret) {
		pr_err("%s:(WP) can not read the property of sprd wp_fn\n",
		       mmc_hostname(host));
		return ret;
	}

	return strcmp(name, "enable") ? -EINVAL : 0;
}
EXPORT_SYMBOL_GPL(mmc_check_wp_fn);

/* ------------------------------------------------------------------ */
/* power-on write protect: char device + ioctl                          */
/* ------------------------------------------------------------------ */

struct sprd_mmc_wp_ioc_data {
	struct mmc_ioc_cmd ic;
	unsigned char *buf;
	u64 buf_bytes;
	void *reserved;
};

static struct sprd_mmc_wp_ioc_data *
mmc_wp_ioctl_copy_from_user(struct mmc_ioc_cmd __user *user)
{
	struct sprd_mmc_wp_ioc_data *idata;

	idata = kmalloc(sizeof(*idata), GFP_KERNEL);
	if (!idata)
		return ERR_PTR(-ENOMEM);

	if (copy_from_user(&idata->ic, user, sizeof(idata->ic))) {
		kfree(idata);
		return ERR_PTR(-EFAULT);
	}

	idata->buf_bytes = (u64)idata->ic.blksz * idata->ic.blocks;
	if (!idata->buf_bytes) {
		kfree(idata);
		return ERR_PTR(-EINVAL);
	}

	idata->buf = memdup_user((void __user *)(unsigned long)
				 idata->ic.data_ptr, idata->buf_bytes);
	if (IS_ERR(idata->buf)) {
		int err = PTR_ERR(idata->buf);

		kfree(idata);
		return ERR_PTR(err);
	}

	return idata;
}

static int mmc_wp_ioctl_cmd(struct mmc_host *host, struct mmc_ioc_cmd __user *ic_ptr)
{
	struct sprd_mmc_wp_ioc_data *idata;
	struct mmc_card *card = host->card;
	struct sdio_func sf = { .card = card };
	bool cmdq_dis = false;
	int err = 0;

	if (!card || mmc_card_removed(card))
		return -ENODEV;

	idata = mmc_wp_ioctl_copy_from_user(ic_ptr);
	if (IS_ERR(idata)) {
		pr_err("%s:(WP) copy from user failed\n", mmc_hostname(host));
		return PTR_ERR(idata);
	}

	if (idata->ic.opcode != SPRD_WP_MAGIC_KEY) {
		pr_err("%s:(WP) user opcode invalid\n", mmc_hostname(host));
		err = -EINVAL;
		goto out_free;
	}

	if (idata->ic.arg == sprd_wp.enabled) {
		if (sprd_wp.enabled)
			pr_info("%s:(WP) no need to reset, status is enabled\n",
				mmc_hostname(host));
		else
			pr_info("%s:(WP) no need to reset, status is disabled\n",
				mmc_hostname(host));
		goto out_free;
	}

	/*
	 * Serialise against the block queue, then quiesce CQE/HSQ before
	 * toggling the protection bit, as the reset below re-inits the card.
	 *
	 * ponytail: 6.18's mmc_queue has no user-visible 'busy' flag that a char
	 * device can usefully claim (it tracks internal recovery state only),
	 * so gate on the host's queue state instead. Replace with a real
	 * mq->busy claim once the SWCQ block layer is ported.
	 */
	if (host->cqe_enabled && host->cqe_ops) {
		err = host->cqe_ops->cqe_wait_for_idle(host);
		if (err) {
			pr_err("%s:(WP) wait for idle fail\n",
			       mmc_hostname(host));
			goto out_free;
		}
	}

	sdio_claim_host(&sf);
	if (card->ext_csd.cmdq_en) {
		err = mmc_cmdq_disable(card);
		if (err) {
			pr_err("%s:(WP) cmdq disable fail, err=%d\n",
			       mmc_hostname(host), err);
			goto cmd_done;
		}
		cmdq_dis = true;
	}

	/* reset eMMC so the new power-on write protect state takes effect */
	sprd_wp.enabled = idata->ic.arg;
	err = mmc_hw_reset(card);

cmd_done:
	if (cmdq_dis) {
		int err2 = mmc_cmdq_enable(card);

		if (err2) {
			pr_err("%s:(WP) cmdq enable fail, err=%d\n",
			       mmc_hostname(host), err2);
			mmc_hw_reset(card);
		}
	}

	sdio_release_host(&sf);
out_free:
	kfree(idata->buf);
	kfree(idata);

	return err;
}

static long mmc_wp_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct mmc_host *host = file->private_data;

	if (cmd != MMC_IOC_CMD)
		return -EINVAL;

	return mmc_wp_ioctl_cmd(host, (struct mmc_ioc_cmd __user *)arg);
}

#ifdef CONFIG_COMPAT
static long mmc_wp_ioctl_compat(struct file *file, unsigned int cmd,
				unsigned long arg)
{
	return mmc_wp_ioctl(file, cmd, arg);
}
#endif

static int mmc_wp_chrdev_open(struct inode *inode, struct file *filp)
{
	struct sprd_mmc_wp *wp = container_of(inode->i_cdev,
					     struct sprd_mmc_wp, chrdev);

	get_device(&wp->dev);
	filp->private_data = wp->mmc;

	return nonseekable_open(inode, filp);
}

static int mmc_wp_chrdev_release(struct inode *inode, struct file *filp)
{
	struct sprd_mmc_wp *wp = container_of(inode->i_cdev,
					     struct sprd_mmc_wp, chrdev);

	put_device(&wp->dev);
	return 0;
}

static const struct file_operations wp_fileops = {
	.owner = THIS_MODULE,
	.open = mmc_wp_chrdev_open,
	.release = mmc_wp_chrdev_release,
	.unlocked_ioctl = mmc_wp_ioctl,
	.llseek = noop_llseek,
#ifdef CONFIG_COMPAT
	.compat_ioctl = mmc_wp_ioctl_compat,
#endif
};

/**
 * mmc_wp_init() - register the power-on write protect char device
 * @mmc: host that owns the char device
 *
 * Return: 0 on success, negative errno on failure.
 */
int mmc_wp_init(struct mmc_host *mmc)
{
	char name[64];
	int res;

	if (mmc->index != 0 || sprd_wp.inited)
		return 0;

	if (!sprd_wp_bus_type.name) {
		sprd_wp_bus_type.name = "mmc_wp";
		res = bus_register(&sprd_wp_bus_type);
		if (res < 0) {
			pr_err("%s:(WP) could not register WP bus type\n",
			       mmc_hostname(mmc));
			sprd_wp_bus_type.name = NULL;
			return res;
		}
	}

	snprintf(name, sizeof(name), "%s_wp", mmc_hostname(mmc));
	res = alloc_chrdev_region(&sprd_wp.devt, 0, SPRD_WP_MAX_DEVICES, name);
	if (res < 0) {
		pr_err("%s:(WP) failed to allocate chrdev region\n",
		       mmc_hostname(mmc));
		goto out_bus_unreg;
	}

	sprd_wp.dev.init_name = name;
	sprd_wp.dev.bus = &sprd_wp_bus_type;
	sprd_wp.dev.devt = MKDEV(MAJOR(sprd_wp.devt), 1);
	sprd_wp.dev.parent = &mmc->class_dev;
	device_initialize(&sprd_wp.dev);

	dev_set_drvdata(&sprd_wp.dev, &sprd_wp);

	cdev_init(&sprd_wp.chrdev, &wp_fileops);
	sprd_wp.chrdev.owner = THIS_MODULE;

	res = cdev_device_add(&sprd_wp.chrdev, &sprd_wp.dev);
	if (res) {
		pr_err("%s:(WP) could not add character device\n",
		       mmc_hostname(mmc));
		goto out_put_device;
	}

	sprd_wp.mmc = mmc;
	pr_info("%s:(WP) create chardev(%d:1)\n", mmc_hostname(mmc),
		MAJOR(sprd_wp.devt));
	sprd_wp.inited = true;
	sprd_wp.enabled = 1;

	return 0;

out_put_device:
	put_device(&sprd_wp.dev);
	unregister_chrdev_region(sprd_wp.devt, SPRD_WP_MAX_DEVICES);
out_bus_unreg:
	if (!sprd_wp.inited) {
		bus_unregister(&sprd_wp_bus_type);
		sprd_wp_bus_type.name = NULL;
	}
	return res;
}
EXPORT_SYMBOL_GPL(mmc_wp_init);

/**
 * mmc_wp_remove() - tear down the power-on write protect char device
 * @mmc: host that owns the char device
 */
void mmc_wp_remove(struct mmc_host *mmc)
{
	if (mmc->index != 0 || !sprd_wp.inited)
		return;

	put_device(&sprd_wp.dev);
	unregister_chrdev_region(sprd_wp.devt, SPRD_WP_MAX_DEVICES);

	bus_unregister(&sprd_wp_bus_type);
	sprd_wp_bus_type.name = NULL;

	sprd_wp.inited = false;
	sprd_wp.enabled = 0;
}
EXPORT_SYMBOL_GPL(mmc_wp_remove);

/* ------------------------------------------------------------------ */
/* tuning read-back                                                     */
/* ------------------------------------------------------------------ */

/**
 * mmc_send_tuning_read() - probe bus integrity with a single 512B read
 * @host: host to read from
 *
 * Used by the DLL delay search to verify the sampled delay actually moves
 * data. Reads block 0 of the card.
 *
 * Return: 0 on success, negative errno on failure.
 */
int mmc_send_tuning_read(struct mmc_host *host)
{
	struct mmc_request mrq = {0};
	struct mmc_command cmd = {0};
	struct mmc_data data = {0};
	struct scatterlist sg;
	int size = 512;
	int err = 0;
	u8 *data_buf;

	data_buf = kzalloc(size, GFP_KERNEL);
	if (!data_buf) {
		pr_err("%s: data_buf create false\n", mmc_hostname(host));
		return -ENOMEM;
	}

	mrq.cmd = &cmd;
	mrq.data = &data;

	cmd.opcode = MMC_READ_SINGLE_BLOCK;
	cmd.flags = MMC_RSP_R1B | MMC_CMD_ADTC;

	data.blksz = size;
	data.blocks = 1;
	data.flags = MMC_DATA_READ;
	data.blk_addr = 0;
	data.timeout_ns = 100000000;
	data.timeout_clks = 0;

	data.sg = &sg;
	data.sg_len = 1;
	sg_init_one(&sg, data_buf, size);

	mmc_wait_for_req(host, &mrq);

	if (cmd.error)
		err = cmd.error;
	else if (data.error)
		err = data.error;

	kfree(data_buf);

	return err;
}
EXPORT_SYMBOL_GPL(mmc_send_tuning_read);

/* ------------------------------------------------------------------ */
/* SWCQ request paths                                                   */
/* ------------------------------------------------------------------ */

/*
 * The software queue calls into these instead of sdhci_request(), because that
 * path takes host->lock itself and completes requests via complete_work. These
 * take the lock and issue the command inline, which is what the queue's
 * polling loop needs in order to observe the inhibit bits itself.
 *
 * sdhci_send_command() is static in sdhci.c, so presence is checked with the
 * exported sdhci_present_error() and the command is handed to the generic
 * sdhci_request_atomic() once the host is known alive.
 */

/**
 * sdhci_sprd_request_sync() - send one request with host->lock already held
 * @mmc: host
 * @mrq: request to send
 *
 * Return: 0 on success, -EBUSY if the controller has not released its
 * inhibit bits, in which case the caller retries in non-atomic context.
 */
int sdhci_sprd_request_sync(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct sdhci_host *host = mmc_priv(mmc);
	struct mmc_command *cmd;
	int ret = 0;

	if (sdhci_present_error(host, mrq->cmd, true)) {
		sdhci_finish_mrq(host, mrq);
		goto out;
	}

	cmd = sdhci_sprd_manual_cmd23(host, mrq) ? mrq->sbc : mrq->cmd;

	if (sdhci_sprd_send_command(host, cmd))
		ret = -EBUSY;

out:
	return ret;
}
EXPORT_SYMBOL_GPL(sdhci_sprd_request_sync);

/**
 * _sdhci_request_atomic() - issue one request from atomic (hard IRQ) context
 * @mmc: host
 * @mrq: request to send
 *
 * Return: 0 on success, -EBUSY if the controller is still busy.
 */
int _sdhci_request_atomic(struct mmc_host *mmc, struct mmc_request *mrq)
{
	struct sdhci_host *host = mmc_priv(mmc);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&host->lock, flags);
	ret = sdhci_sprd_request_sync(mmc, mrq);
	spin_unlock_irqrestore(&host->lock, flags);

	return ret;
}
EXPORT_SYMBOL_GPL(_sdhci_request_atomic);

/**
 * mmc_hsq_swcq_init() - attach the right software queue for this host
 * @host: SDHCI host
 * @pdev: platform device backing the host
 *
 * eMMC hosts get the Unisoc software command queue (mmc_swcq_init), SD hosts
 * the upstream HSQ (mmc_hsq_init).
 *
 * Return: 0 on success, negative errno on failure.
 */
int mmc_hsq_swcq_init(struct sdhci_host *host, struct platform_device *pdev)
{
	struct mmc_hsq *hsq;
	int ret;

	/*
	 * ponytail: the Unisoc software command queue (mmc_swcq.c, ~64 KB) is
	 * not ported to 6.18, so eMMC hosts fall back to HSQ here. Swap in
	 * mmc_swcq_init() once drivers/mmc/host/mmc_swcq.c is in the tree.
	 */
	hsq = devm_kzalloc(&pdev->dev, sizeof(*hsq), GFP_KERNEL);
	if (!hsq)
		return -ENOMEM;

	ret = mmc_hsq_init(hsq, host->mmc);

	return ret;
}
EXPORT_SYMBOL_GPL(mmc_hsq_swcq_init);

/*
 * SWCQ's raw hard-IRQ handler. sdhci_irq() is static in sdhci.c and hands
 * command/data completion to complete_work, which the SWCQ completion path
 * must not race with, so the SWCQ carries its own copy of the top-level
 * handler and re-requests it after every runtime resume.
 *
 * ponytail: this is the upstream handler with host->ops->irq kept, because
 * that hook is where the SWCQ injects its own command/data handling. Replace
 * with a single shared sdhci_irq() + a queue pointer in sdhci_host once
 * sdhci.c grows an sdhci_ops entry for the hard-IRQ half.
 */
static irqreturn_t sprd_sdhci_raw_irq(int irq, void *dev_id)
{
	struct sdhci_host *host = dev_id;
	u32 intmask, mask, unexpected = 0;
	int max_loops = 16;

	spin_lock(&host->lock);

	if (host->runtime_suspended) {
		spin_unlock(&host->lock);
		return IRQ_NONE;
	}

	intmask = sdhci_readl(host, SDHCI_INT_STATUS);
	if (!intmask || intmask == 0xffffffff) {
		spin_unlock(&host->lock);
		return IRQ_NONE;
	}

	do {
		if (host->ops->irq) {
			intmask = host->ops->irq(host, intmask);
			if (!intmask)
				goto cont;
		}

		/* Clear selected interrupts. */
		mask = intmask & (SDHCI_INT_CMD_MASK | SDHCI_INT_DATA_MASK |
				  SDHCI_INT_BUS_POWER);
		sdhci_writel(host, mask, SDHCI_INT_STATUS);

		if (intmask & (SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE)) {
			u32 present = sdhci_readl(host, SDHCI_PRESENT_STATE) &
				      SDHCI_CARD_PRESENT;

			/*
			 * Only watch the edge that does not immediately
			 * re-fire, otherwise the interrupt storms.
			 */
			host->ier &= ~(SDHCI_INT_CARD_INSERT |
				       SDHCI_INT_CARD_REMOVE);
			host->ier |= present ? SDHCI_INT_CARD_REMOVE :
					       SDHCI_INT_CARD_INSERT;
			sdhci_writel(host, host->ier, SDHCI_INT_ENABLE);
			sdhci_writel(host, host->ier, SDHCI_SIGNAL_ENABLE);

			sdhci_writel(host, intmask & (SDHCI_INT_CARD_INSERT |
						     SDHCI_INT_CARD_REMOVE),
				     SDHCI_INT_STATUS);

			host->thread_isr |= intmask &
					   (SDHCI_INT_CARD_INSERT |
					    SDHCI_INT_CARD_REMOVE);
		}

		if (intmask & (SDHCI_INT_RETUNE))
			mmc_retune_needed(host->mmc);

		if ((intmask & SDHCI_INT_CARD_INT) &&
		    (host->ier & SDHCI_INT_CARD_INT)) {
			sdhci_enable_sdio_irq_nolock(host, false);
			sdio_signal_irq(host->mmc);
		}

		intmask &= ~(SDHCI_INT_CARD_INSERT | SDHCI_INT_CARD_REMOVE |
			     SDHCI_INT_CMD_MASK | SDHCI_INT_DATA_MASK |
			     SDHCI_INT_ERROR | SDHCI_INT_BUS_POWER |
			     SDHCI_INT_RETUNE | SDHCI_INT_CARD_INT);

		if (intmask) {
			unexpected |= intmask;
			sdhci_writel(host, intmask, SDHCI_INT_STATUS);
		}
cont:
		intmask = sdhci_readl(host, SDHCI_INT_STATUS);
	} while (intmask && --max_loops);

	spin_unlock(&host->lock);

	if (unexpected) {
		pr_err("%s: Unexpected interrupt 0x%08x.\n",
		       mmc_hostname(host->mmc), unexpected);
		sdhci_dumpregs(host);
	}

	return IRQ_HANDLED;
}

/**
 * sdhci_sprd_irq_request_swcq() - re-hook the host IRQ for SWCQ eMMC hosts
 * @host: SDHCI host
 *
 * sdhci_runtime_resume_host() re-arms the shared threaded IRQ with the
 * generic handlers, which would drop the SWCQ handlers on every resume, so
 * re-request it here.
 *
 * Return: 0 on success, negative errno on failure.
 */
int sdhci_sprd_irq_request_swcq(struct sdhci_host *host)
{
	if (!SPRD_HOST_IS_EMMC(host->mmc))
		return 0;

	free_irq(host->irq, host);

	return request_threaded_irq(host->irq, sprd_sdhci_raw_irq,
				    host->thread_irq_fn, IRQF_SHARED,
				    mmc_hostname(host->mmc), host);
}
EXPORT_SYMBOL_GPL(sdhci_sprd_irq_request_swcq);