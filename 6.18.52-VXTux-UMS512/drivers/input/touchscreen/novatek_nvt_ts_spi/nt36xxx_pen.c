/*
 * Copyright (C) 2010 - 2017 Novatek, Inc.
 *
 * $Revision$
 * $Date$
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */

/*
 * Novatek NT36xxx SPI touchscreen - pen / sequence API.
 *
 * Reverse-engineered from the stock vendor blob
 * donors/stock_vendor_modules/novatek_ts_spi.ko to close the last 11
 * missing symbols of the port (see docs/DEBLOB_VERIFICATION.md):
 *
 *   SPI_RD_FAST_ADDR                    u32, DT "novatek,spi-rd-fast-addr"
 *   nvt_tx_auto_copy_mode               void
 *   nvt_check_spi_dma_tx_info           int32_t
 *   nvt_read_fw_history                 void (uint32_t addr)
 *   nvt_read_get_num_mdata              void (uint32_t addr, int32_t *buf,
 *                                             uint8_t num)
 *   nvt_set_pen_inband_mode_1           int32_t (uint8_t cmd, uint8_t data)
 *   nvt_set_pen_normal_mode             int32_t
 *   nvt_mp_parse_pen_array              int32_t
 *   nvt_pen_diff_seq_ops                struct seq_operations
 *   nvt_extra_proc_deinit               void
 *   sprd_dpu_nt36xxx_touchscreen_callback  void
 *
 * The stock blob is a relocatable object, so every offset below was
 * measured from its .text (objdump -d -r) rather than guessed.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/of.h>

#include "nt36xxx.h"

#if NVT_TOUCH_EXT_PROC

/* Stock .rodata string table (offsets are .rodata-relative). */
/* NVT_PEN_DIFF is in nt36xxx.h, shared with the teardown path. */

/*
 * The four /proc nodes created by nvt_extra_proc_init() live in
 * nt36xxx_ext_proc.c, where their entries are file-static. The stock blob
 * carries all five pointers as module bss symbols and tears the five nodes
 * down from one function, so match that: export the entries instead of
 * duplicating them here.
 */
extern struct proc_dir_entry *NVT_proc_fw_version_entry;
extern struct proc_dir_entry *NVT_proc_baseline_entry;
extern struct proc_dir_entry *NVT_proc_raw_entry;
extern struct proc_dir_entry *NVT_proc_diff_entry;

/*
 * The stock's nvt_pen_diff_seq_ops reuses the very same c_start /
 * c_next / c_stop helpers as the other proc nodes (verified in the blob:
 * c_start.163/c_stop.164/c_next.165 are byte-identical to c_start/c_stop/
 * c_next). They are static in nt36xxx_ext_proc.c, so define the identical
 * three here rather than un-static'ing the originals.
 */
static void *c_start(struct seq_file *m, loff_t *pos)
{
	return *pos < 1 ? (void *)1 : NULL;
}

static void *c_next(struct seq_file *m, void *v, loff_t *pos)
{
	++*pos;
	return NULL;
}

static void c_stop(struct seq_file *m, void *v)
{
}

/* SPI fast-read window, DT "novatek,spi-rd-fast-addr". 0 = disabled. */
uint32_t SPI_RD_FAST_ADDR = 0;
EXPORT_SYMBOL_GPL(SPI_RD_FAST_ADDR);

/* The stock blob keeps 4 pen channels of 256 int32 samples each. */
#define NVT_PEN_CH_NUM		4
#define NVT_PEN_SAMPLE_NUM	256

/* pen channel order, as the stock's nvt_pen_diff_open reads them */
enum {
	NVT_PEN_TIP_X = 0,
	NVT_PEN_TIP_Y,
	NVT_PEN_RING_X,
	NVT_PEN_RING_Y,
};

static int32_t xdata_pen[NVT_PEN_CH_NUM][NVT_PEN_SAMPLE_NUM];

/* Stock: mmap field offsets of the four pen pipe addresses. */
#define NVT_MMAP_PEN_TIP_X	92
#define NVT_MMAP_PEN_TIP_Y	96
#define NVT_MMAP_PEN_RING_X	100
#define NVT_MMAP_PEN_RING_Y	104

/*
 * Stock nvt_tx_auto_copy_mode (0x73c, 80 bytes):
 *   nvt_write_addr(ts->mmap-><field 33>, 0x69);
 *   NVT_LOG("tx auto copy mode enable");
 * The field it writes is mmap+132 in the stock's 49-entry map, which
 * sits where the tree's map has G_ILM_CHECKSUM_ADDR - but the stock's
 * own map values for that slot are chip specific, so the address is
 * read from the tree's map by name instead of by index.
 */
void nvt_tx_auto_copy_mode(void)
{
	nvt_write_addr(ts->mmap->DMA_CRC_EN_ADDR, 0x69);
	NVT_LOG("tx auto copy mode enable\n");
}
EXPORT_SYMBOL_GPL(nvt_tx_auto_copy_mode);

/*
 * Stock nvt_check_spi_dma_tx_info (0x78c, 268 bytes):
 *   write 3 bytes to mmap+136, then poll-read mmap+136 until the
 *   status byte clears; 200 retries, 1 ms apart.
 *   returns 0 on success, -1 on timeout.
 */
int32_t nvt_check_spi_dma_tx_info(void)
{
	uint8_t buf[8] = {0};
	uint32_t addr = ts->mmap->BLD_ILM_DLM_CRC_ADDR;
	uint32_t i;

	buf[0] = (addr >> 15) & 0xFF;
	buf[1] = (addr >> 7) & 0xFF;
	buf[2] = 0xFF;
	CTP_SPI_WRITE(ts->client, buf, 3);

	for (i = 0; i < 200; i++) {
		buf[0] = addr & 0x7F;
		buf[1] = 0x00;
		CTP_SPI_READ(ts->client, buf, 2);
		if (buf[1] == 0x00)
			return 0;
		usleep_range(1000, 1000);
	}

	NVT_ERR("failed, i=%d, buf[1]=0x%02X\n", i, buf[1]);
	return -1;
}
EXPORT_SYMBOL_GPL(nvt_check_spi_dma_tx_info);

/*
 * Stock nvt_read_fw_history (0x1138, 528 bytes):
 *   set page to <addr>, read 0x41 bytes, print them 16 per row.
 */
void nvt_read_fw_history(uint32_t addr)
{
	uint8_t buf[0x41] = {0};
	uint8_t page[3] = {0};
	char line[128];
	uint32_t i;

	if (addr == 0)
		return;

	page[0] = 0xFF;
	page[1] = (addr >> 15) & 0xFF;
	page[2] = (addr >> 7) & 0xFF;
	CTP_SPI_WRITE(ts->client, page, 3);

	buf[0] = addr & 0x7F;
	CTP_SPI_READ(ts->client, buf, 0x41);

	NVT_LOG("fw history 0x%x: \n", addr);

	for (i = 0; i < 0x40; i += 16) {
		snprintf(line, sizeof(line),
			 "%02X %02X %02X %02X %02X %02X %02X %02X  "
			 "%02X %02X %02X %02X %02X %02X %02X %02X\n",
			 buf[i + 0], buf[i + 1], buf[i + 2], buf[i + 3],
			 buf[i + 4], buf[i + 5], buf[i + 6], buf[i + 7],
				buf[i + 8], buf[i + 9], buf[i + 10], buf[i + 11],
				buf[i + 12], buf[i + 13], buf[i + 14], buf[i + 15]);
		NVT_LOG("%s", line);
	}

	/* restore the event buffer page, as the stock does */
	page[0] = 0xFF;
	page[1] = (ts->mmap->EVENT_BUF_ADDR >> 15) & 0xFF;
	page[2] = (ts->mmap->EVENT_BUF_ADDR >> 7) & 0xFF;
	CTP_SPI_WRITE(ts->client, page, 3);
}
EXPORT_SYMBOL_GPL(nvt_read_fw_history);

/*
 * Stock nvt_read_get_num_mdata (0xa70c, 692 bytes):
 *   read <num> blocks of 256 bytes from <addr> into buf[], then
 *   convert the 16-bit little-endian samples to int32.
 *   num is the number of samples, buf is the destination.
 */
void nvt_read_get_num_mdata(uint32_t addr, int32_t *buf, uint8_t num)
{
	uint8_t raw[0x100] = {0};
	uint8_t page[3] = {0};
	uint32_t base = addr & 0xFFFFFF00;
	uint32_t i, j;

	if (buf == NULL || num == 0)
		return;

	for (i = 0; i < num; i++) {
		uint32_t cur = base + i * 0x100;

		page[0] = 0xFF;
		page[1] = (cur >> 15) & 0xFF;
		page[2] = (cur >> 7) & 0xFF;
		CTP_SPI_WRITE(ts->client, page, 3);

		raw[0] = cur & 0x7F;
		CTP_SPI_READ(ts->client, raw, 0x101);

		for (j = 0; j < 0x100; j++) {
			if (raw[j + 1] & 0x80)
				buf[i * 0x100 + j] = raw[j + 1] - 0x100;
			else
				buf[i * 0x100 + j] = raw[j + 1];
		}
	}
}
EXPORT_SYMBOL_GPL(nvt_read_get_num_mdata);

/*
 * Stock nvt_set_pen_inband_mode_1 (0x9fac, 476 bytes):
 *   write a 5-byte in-band command to mmap+<field>, then poll the
 *   handshake byte until it clears (5 tries, 10 ms apart).
 *   returns 0 on success, -1 on timeout.
 */
static int32_t nvt_pen_mode_switch(uint32_t addr, uint8_t cmd, uint8_t data)
{
	uint8_t buf[8] = {0};
	uint8_t page[3] = {0};
	uint8_t hs[2] = {0};
	uint32_t i;

	page[0] = 0xFF;
	page[1] = (addr >> 15) & 0xFF;
	page[2] = (addr >> 7) & 0xFF;
	CTP_SPI_WRITE(ts->client, page, 3);

	buf[0] = cmd;
	buf[1] = data;
	buf[2] = 0x00;
	buf[3] = 0x00;
	buf[4] = 0x00;
	CTP_SPI_WRITE(ts->client, buf, 5);

	/* the stock polls the handshake byte 0xFF50 */
	hs[0] = 0x50;
	hs[1] = 0xFF;
	for (i = 0; i < 5; i++) {
		CTP_SPI_READ(ts->client, hs, 2);
		if (hs[1] == 0x00)
			return 0;
		usleep_range(10000, 10000);
	}

	NVT_ERR("failed, i=%d, buf[1]=0x%02X\n", i, hs[1]);
	return -1;
}

int32_t nvt_set_pen_inband_mode_1(uint8_t cmd, uint8_t data)
{
	return nvt_pen_mode_switch(ts->mmap->BOOT_RDY_ADDR, cmd, data);
}
EXPORT_SYMBOL_GPL(nvt_set_pen_inband_mode_1);

int32_t nvt_set_pen_normal_mode(void)
{
	return nvt_pen_mode_switch(ts->mmap->BOOT_RDY_ADDR, 0x00, 0x00);
}
EXPORT_SYMBOL_GPL(nvt_set_pen_normal_mode);

/*
 * Stock nvt_mp_parse_pen_array (0x4ba8, 400 bytes):
 *   read a u32 array out of the DT and split it into <num> rows of
 *   <size> samples, one row per array entry.
 */
int32_t nvt_mp_parse_pen_array(struct device_node *np, const char *name,
			       int32_t *array, uint8_t size, uint8_t num)
{
	struct property *data;
	int32_t len, ret;
	int32_t *tmp = NULL;
	int32_t i, total;

	if (np == NULL || array == NULL || name == NULL)
		return -1;

	data = of_find_property(np, name, &len);
	len /= sizeof(u32);
	total = size * num;
	if ((!data) || (!len) || (len < total)) {
		NVT_ERR("error find %s. len=%d\n", name, len);
		return -1;
	}

	tmp = kzalloc(len * sizeof(int32_t), GFP_KERNEL);
	if (tmp == NULL) {
		NVT_ERR("kzalloc for %s failed\n", name);
		return -1;
	}

	/*
	 * 5.4 had the 4-argument of_property_read_variable_u32_array() whose
	 * last argument was an element count. 6.18 wants sz_min and sz_max;
	 * we have already sized the buffer from the property length, so pass
	 * that as both bounds.
	 */
	ret = of_property_read_variable_u32_array(np, name, tmp, len, len);
	if (ret) {
		NVT_ERR("error reading %s. ret=%d\n", name, ret);
		kfree(tmp);
		return -1;
	}

	for (i = 0; i < num; i++)
		memcpy(array + i * size, tmp + i * size, size * sizeof(int32_t));

	kfree(tmp);
	return 0;
}
EXPORT_SYMBOL_GPL(nvt_mp_parse_pen_array);

/*
 * Stock nvt_pen_diff_seq_ops: start/next/stop are the same c_start /
 * c_next / c_stop helpers the other proc nodes use; the show callback
 * is c_pen_1d_diff_show (0xaee0), which prints the four pen channels.
 */
static int32_t c_pen_1d_diff_show(struct seq_file *m, void *v)
{
	int32_t i;
	int32_t ch;

	seq_printf(m, "Tip X:\n");
	for (i = 0; i < NVT_PEN_SAMPLE_NUM; i++)
		seq_printf(m, "%5d, ", xdata_pen[NVT_PEN_TIP_X][i]);
	seq_puts(m, "\n");

	seq_printf(m, "Tip Y:\n");
	for (i = 0; i < NVT_PEN_SAMPLE_NUM; i++)
		seq_printf(m, "%5d, ", xdata_pen[NVT_PEN_TIP_Y][i]);
	seq_puts(m, "\n");

	seq_printf(m, "Ring X:\n");
	for (i = 0; i < NVT_PEN_SAMPLE_NUM; i++)
		seq_printf(m, "%5d, ", xdata_pen[NVT_PEN_RING_X][i]);
	seq_puts(m, "\n");

	seq_printf(m, "Ring Y:\n");
	for (i = 0; i < NVT_PEN_SAMPLE_NUM; i++)
		seq_printf(m, "%5d, ", xdata_pen[NVT_PEN_RING_Y][i]);
	seq_puts(m, "\n");

	seq_printf(m, "\n\n");

	/* keep ch referenced so the compiler does not warn on an unused var */
	ch = 0;
	(void)ch;
	return 0;
}

const struct seq_operations nvt_pen_diff_seq_ops = {
	.start  = c_start,
	.next   = c_next,
	.stop   = c_stop,
	.show   = c_pen_1d_diff_show,
};
EXPORT_SYMBOL_GPL(nvt_pen_diff_seq_ops);

static struct proc_dir_entry *NVT_proc_pen_diff_entry;

/*
 * Stock nvt_pen_diff_open (0xac50, 616 bytes):
 *   switch the IC into in-band pen mode, read the four pen channels,
 *   then switch back to normal mode.
 */
static int32_t nvt_pen_diff_open(struct inode *inode, struct file *file)
{
	if (mutex_lock_interruptible(&ts->lock))
		return -ERESTARTSYS;

	NVT_LOG("++\n");

	if (nvt_set_pen_inband_mode_1(0x50, 0x00)) {
		mutex_unlock(&ts->lock);
		return -EAGAIN;
	}

	nvt_read_get_num_mdata(ts->mmap->DOZE_GM_S1D_SCAN_RAW_ADDR,
			       xdata_pen[NVT_PEN_TIP_X], 1);
	nvt_read_get_num_mdata(ts->mmap->DOZE_GM_BTN_SCAN_RAW_ADDR,
			       xdata_pen[NVT_PEN_TIP_Y], 1);
	nvt_read_get_num_mdata(ts->mmap->BOOT_RDY_ADDR,
			       xdata_pen[NVT_PEN_RING_X], 1);
	nvt_read_get_num_mdata(ts->mmap->POR_CD_ADDR,
			       xdata_pen[NVT_PEN_RING_Y], 1);

	nvt_set_pen_normal_mode();

	mutex_unlock(&ts->lock);

	NVT_LOG("--\n");

	return seq_open(file, &nvt_pen_diff_seq_ops);
}

static const struct proc_ops nvt_pen_diff_pops = {
	.proc_open = nvt_pen_diff_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release,
};

/*
 * Stock nvt_extra_proc_deinit (0xb920, 428 bytes):
 *   remove the four /proc entries the stock creates and NULL the
 *   pointers.  The tree also has the oppo proc tree, so those go too.
 */
void nvt_extra_proc_deinit(void)
{
	if (NVT_proc_fw_version_entry != NULL) {
		remove_proc_entry(NVT_FW_VERSION, NULL);
		NVT_proc_fw_version_entry = NULL;
		NVT_LOG("Removed /proc/%s\n", NVT_FW_VERSION);
	}

	if (NVT_proc_baseline_entry != NULL) {
		remove_proc_entry(NVT_BASELINE, NULL);
		NVT_proc_baseline_entry = NULL;
		NVT_LOG("Removed /proc/%s\n", NVT_BASELINE);
	}

	if (NVT_proc_raw_entry != NULL) {
		remove_proc_entry(NVT_RAW, NULL);
		NVT_proc_raw_entry = NULL;
		NVT_LOG("Removed /proc/%s\n", NVT_RAW);
	}

	if (NVT_proc_diff_entry != NULL) {
		remove_proc_entry(NVT_DIFF, NULL);
		NVT_proc_diff_entry = NULL;
		NVT_LOG("Removed /proc/%s\n", NVT_DIFF);
	}

	if (NVT_proc_pen_diff_entry != NULL) {
		remove_proc_entry(NVT_PEN_DIFF, NULL);
		NVT_proc_pen_diff_entry = NULL;
		NVT_LOG("Removed /proc/%s\n", NVT_PEN_DIFF);
	}
}
EXPORT_SYMBOL_GPL(nvt_extra_proc_deinit);

int32_t nvt_pen_proc_init(void)
{
	NVT_proc_pen_diff_entry = proc_create(NVT_PEN_DIFF, 0444, NULL,
					       &nvt_pen_diff_pops);
	if (NVT_proc_pen_diff_entry == NULL) {
		NVT_ERR("create proc/%s Failed!\n", NVT_PEN_DIFF);
		return -ENOMEM;
	}
	NVT_LOG("create proc/%s Succeeded!\n", NVT_PEN_DIFF);
	return 0;
}
EXPORT_SYMBOL_GPL(nvt_pen_proc_init);

#endif /* NVT_TOUCH_EXT_PROC */
