// SPDX-License-Identifier: GPL-2.0
/*
 * sprd-dmaengine-pcm.c
 *
 * Spreadtrum DMA-engine based ASoC platform (PCM) driver.
 *
 * Re-implementation of the vendor blob sprd-dmaengine-pcm.ko
 * (kernel 5.4.210, aarch64, built with Android clang 12.0.5) for the
 * VXTux 6.18.52 tree.
 *
 * The original vendor source was
 *   bsp/modules/kernel5.4/audio_driver/sprd/platform/dmaengine_pcm/
 *   sprd-dmaengine-pcm.c
 * (path recovered from the blob's .rodata).
 *
 * The 5.4 blob used the legacy ASoC "platform" API
 * (snd_soc_platform / snd_soc_platform_driver / snd_soc_pcm_ops).
 * 6.18 removed those; the equivalent is the component API:
 *   snd_soc_platform_driver  ->  snd_soc_component_driver
 *   .pcm_new                 ->  .pcm_construct
 *   snd_soc_register_platform->  devm_snd_soc_register_component
 *   .open/.close/.hw_params/.hw_free/.prepare/.trigger/.pointer/.mmap
 *     now take (struct snd_soc_component *, ...) as first argument.
 *
 * Copyright (C) 2015 SpreadTrum Ltd.
 * Copyright (C) 2026 VXTux port
 */

#include <linux/dma-mapping.h>
#include <linux/dmaengine.h>
#include <linux/dma/sprd-dma.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/scatterlist.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/suspend.h>
#include <sound/core.h>
#include <sound/info.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-component.h>

#include "agdsp_access.h"
#include "audio_mem.h"
#include "sprd-asoc-common.h"
#include "sprd-dmaengine-pcm.h"
#include "sprd-platform-pcm-routing.h"

#define DRV_NAME			"sprd-pcm-dma"
#define SPRD_PCM_DRIVER_NAME		"sprd-pcm-audio"

/* Maximum number of DMA channels used by one PCM stream (L/R). */
#define SPRD_PCM_DMA_CHAN_MAX	2

/*
 * Debug/feature knobs exposed through the /proc/asound cardX/sprd-dmaengine
 * proc entry. Same names and semantics as the vendor blob; note the blob
 * keeps test_normal_cap_use_iram in .data (initialised to 1) and
 * test_force_stop in .bss, so they are declared the same way here.
 *
 * Symbol visibility matches the stock blob exactly:
 *   test_force_stop              -> global .bss  (B)
 *   test_ddr_buffer_size         -> local  .bss  (b)
 *   debug_pointer_log            -> local  .bss  (b)
 *   test_normal_cap_use_iram     -> local  .data (d)
 */
static int test_ddr_buffer_size;
static int test_normal_cap_use_iram = 1;
int test_force_stop;
static int debug_pointer_log;

/*
 * Global PM/DMA bookkeeping shared by all PCM substreams.
 * Field layout follows the vendor's struct audio_pm_dma.
 */
struct audio_pm_dma *pm_dma;

/*
 * The vendor blob initialises the two mutexes with the classic
 * mutex_init() macro inside its init_pm_dma() helper, which produced the
 * lockdep static keys named "init_pm_dma.__key" and "init_pm_dma.__key.3".
 * Reproduce that shape (and the symbol names) with explicit keys.
 */
static struct lock_class_key init_pm_dma___key __asm__("init_pm_dma.__key");
static struct lock_class_key init_pm_dma___key_3 __asm__("init_pm_dma.__key.3");

struct sprd_pcm_dma_channel {
	struct dma_chan *chan;
	struct dma_async_tx_descriptor *desc;
	dma_cookie_t cookie;
};

/*
 * Per-substream private data. Mirrors the vendor's "sprd_runtime_data"
 * so that the recovered behaviour is preserved 1:1.
 */
struct sprd_runtime_data {
	struct snd_pcm_substream *substream;
	struct device *dev;

	/* per channel (0 = left, 1 = right) */
	struct sprd_pcm_dma_channel chan[SPRD_PCM_DMA_CHAN_MAX];
	int hw_chan;			/* number of channels in use */

	/* DMA link-list configuration (one per channel) */
	struct sprd_dma_cfg *dma_cfg;
	void *dma_cfg_virt[2];
	u32 dma_cfg_phy[2];

	/* PCM buffer mapping (AP side) */
	void *dma_buf_virt;
	u32 dma_buf_phy;
	u32 dma_buf_size;

	/* stream geometry */
	int channels;
	int bytes_per_frame;
	int period_bytes;
	int buffer_bytes;
	int periods;
	int datawidth;
	int src_step;
	int des_step;
	int fragmens_len;
	int src_burst;
	int dst_burst;
	int slave_id;
	int transfer_type;		/* 1 = mem->dev, 2 = dev->mem */
	u32 dma_config_flag;
	u32 transcation_len;

	/* pointer tracking */
	int pre_pointer[2];
	int sel_max;
	int dma_addr_offset;

	/* the FE DAI id the stream was opened with */
	int dai_id;

	/* pm glue */
	int no_pm_cnt;
};


/* ------------------------------------------------------------------ */
/* Forward declarations (the stock blob exported these as global       */
/* symbols, so they need prototypes to satisfy -Wmissing-prototypes).   */
/* ------------------------------------------------------------------ */

int sprd_pcm_new(struct snd_soc_component *component,
		 struct snd_soc_pcm_runtime *rtd);
int sprd_pcm_open(struct snd_soc_component *component,
		  struct snd_pcm_substream *substream);
int sprd_pcm_close(struct snd_soc_component *component,
		   struct snd_pcm_substream *substream);
int sprd_pcm_hw_params(struct snd_soc_component *component,
		       struct snd_pcm_substream *substream,
		       struct snd_pcm_hw_params *params);
int sprd_pcm_hw_free(struct snd_soc_component *component,
		     struct snd_pcm_substream *substream);
int sprd_pcm_prepare(struct snd_soc_component *component,
		     struct snd_pcm_substream *substream);
int sprd_pcm_trigger(struct snd_soc_component *component,
		     struct snd_pcm_substream *substream, int cmd);
snd_pcm_uframes_t sprd_pcm_pointer(struct snd_soc_component *component,
				   struct snd_pcm_substream *substream);
int sprd_pcm_mmap(struct snd_soc_component *component,
		  struct snd_pcm_substream *substream,
		  struct vm_area_struct *vma);
void sprd_pcm_dma_step_config(struct sprd_runtime_data *rt_data,
			      int is_playback);
int sprd_pcm_config_dma(struct snd_soc_component *component,
			struct snd_pcm_substream *substream,
			struct sprd_runtime_data *rt_data, u32 *buf_phy);
void sprd_pcm_dma_buf_done(struct snd_pcm_substream *substream);
void sprd_pcm_platform_proc_read(struct snd_info_entry *entry,
				 struct snd_info_buffer *buffer);
void sprd_pcm_platform_proc_write(struct snd_info_entry *entry,
				  struct snd_info_buffer *buffer);
int sprd_snd_platform_probe(struct snd_card *card);
void sprd_snd_platform_remove(struct snd_card *card);
int sprd_soc_platform_probe(struct platform_device *pdev);
void sprd_soc_platform_remove(struct platform_device *pdev);

/* ------------------------------------------------------------------ */
/* Hardware description tables (recovered from the blob's .rodata)     */
/* ------------------------------------------------------------------ */

/*
 * sprd_i2s_pcm_hardware / sprd_pcm_hardware_v1, decoded from the blob's
 * .rodata (19 x u32 each, the 5.4 struct snd_pcm_hardware layout).
 *
 * 6.18's struct snd_pcm_hardware dropped the 5.4 buffer_bytes_min /
 * period_step / buffer_step / resample fields, so only the fields that
 * still exist are carried over.
 */
/*
 * __used: the stock blob carries this table even though nothing in it
 * references the table either - objdump of the blob finds no reference to
 * sprd_i2s_pcm_hardware at all.  gcc drops an unreferenced static const from
 * the object, so the port had to keep it explicitly to carry the same symbol.
 */
static const struct snd_pcm_hardware __used sprd_i2s_pcm_hardware = {
	.info = SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID |
		SNDRV_PCM_INFO_INTERLEAVED | SNDRV_PCM_INFO_PAUSE |
		SNDRV_PCM_INFO_RESUME | SNDRV_PCM_INFO_NO_PERIOD_WAKEUP,
	.rates = SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |
		 SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |
		 SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 |
		 SNDRV_PCM_RATE_48000,
	.period_bytes_max = 64 * 1024,
	.fifo_size = 1,
};

static const struct snd_pcm_hardware sprd_pcm_hardware_v1 = {
	.info = SNDRV_PCM_INFO_MMAP | SNDRV_PCM_INFO_MMAP_VALID |
		SNDRV_PCM_INFO_INTERLEAVED | SNDRV_PCM_INFO_PAUSE |
		SNDRV_PCM_INFO_RESUME | SNDRV_PCM_INFO_NO_PERIOD_WAKEUP |
		SNDRV_PCM_INFO_NONINTERLEAVED,
	.rates = SNDRV_PCM_RATE_8000 | SNDRV_PCM_RATE_11025 |
		 SNDRV_PCM_RATE_16000 | SNDRV_PCM_RATE_22050 |
		 SNDRV_PCM_RATE_32000 | SNDRV_PCM_RATE_44100 |
		 SNDRV_PCM_RATE_48000,
	.period_bytes_max = 233472,
	.fifo_size = 1,
};

/* ------------------------------------------------------------------ */
/* Small helpers                                                      */
/* ------------------------------------------------------------------ */

static const char *sprd_pcm_stream_str(int stream)
{
	return stream == SNDRV_PCM_STREAM_PLAYBACK ? "Playback" : "Captrue";
}

/*
 * The blob only took the PM lock for the "normal" (dai id 0 / 0x0b)
 * platform in playback mode. Reproduce that condition so the lock
 * nesting matches the vendor driver.
 */
static bool sprd_pcm_use_pm_lock(struct sprd_runtime_data *rt_data, int stream)
{
	if (!pm_dma)
		return false;

	if (rt_data->dai_id != 0x0b && rt_data->dai_id != 0)
		return false;

	return stream == SNDRV_PCM_STREAM_PLAYBACK;
}

/* ------------------------------------------------------------------ */
/* PM notifier                                                        */
/* ------------------------------------------------------------------ */

static void sprd_pcm_pm_normal_dma_chan_release(struct sprd_runtime_data *rt_data)
{
	int i;

	if (!rt_data)
		return;

	for (i = 0; i < rt_data->hw_chan; i++) {
		struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];

		if (!ch->chan) {
			pr_info("%s: %d has released\n", __func__, i);
			continue;
		}

		pr_info("%s: release id=%d\n", __func__, i);

		spin_lock(&pm_dma->pm_splk_dma_prot);
		ch->chan = NULL;
		ch->desc = NULL;
		ch->cookie = 0;
		spin_unlock(&pm_dma->pm_splk_dma_prot);

		dma_release_channel(ch->chan);
	}
}

static int sprd_pcm_pm_notifier(struct notifier_block *nb, unsigned long action,
				void *data)
{
	struct audio_pm_dma *pm = container_of(nb, struct audio_pm_dma, pm_nb);
	struct sprd_runtime_data *rt_data;

	switch (action) {
	case PM_SUSPEND_PREPARE:
		pr_info("%s, PM_SUSPEND_PREPARE.\n", __func__);
		break;
	case PM_POST_SUSPEND:
		pr_info("%s, PM_POST_SUSPEND.\n", __func__);
		mutex_lock(&pm->pm_mtx_dma_prot);
		rt_data = pm->normal_rtd;
		if (rt_data) {
			mutex_lock(&pm->pm_mtx_cnt);
			sprd_pcm_pm_normal_dma_chan_release(rt_data);
			mutex_unlock(&pm->pm_mtx_cnt);
		}
		mutex_unlock(&pm->pm_mtx_dma_prot);
		break;
	default:
		break;
	}

	return NOTIFY_DONE;
}

/* ------------------------------------------------------------------ */
/* DMA channel request / release                                      */
/* ------------------------------------------------------------------ */

static int sprd_pcm_request_dma_channel(struct snd_soc_component *component,
					 struct sprd_runtime_data *rt_data,
					 char *const chan_names[SPRD_PCM_DMA_CHAN_MAX],
					 int channels)
{
	struct device *dev = component->dev;
	int i;

	if (channels > SPRD_PCM_DMA_CHAN_MAX) {
		dev_err(dev, "channel count(%d) is greater than %d\n",
			channels, SPRD_PCM_DMA_CHAN_MAX);
		return -EINVAL;
	}

	if (channels < 1) {
		dev_err(dev, "channel count(%d) is less than 1\n", channels);
		return -EINVAL;
	}

	for (i = 0; i < channels; i++) {
		struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];

		if (!chan_names[i]) {
			dev_err(dev,
				"%s, dma_data->used_dma_channel_name[i] == NULL\n",
				__func__);
			return -EINVAL;
		}

		ch->chan = dma_request_slave_channel(dev, chan_names[i]);
		if (!ch->chan) {
			dev_err(dev, "request dma channel(%s) failed\n",
				chan_names[i]);
			return -ENODEV;
		}

		pr_info("%s, Register Callback func for DMA chan ID %d\n",
			__func__, i);
	}

	rt_data->hw_chan = channels;

	return 0;
}

static void sprd_pcm_release_dma_channel(struct sprd_runtime_data *rt_data)
{
	int i;

	if (!rt_data)
		return;

	for (i = 0; i < SPRD_PCM_DMA_CHAN_MAX; i++) {
		struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];

		if (ch->chan) {
			spin_lock(&pm_dma->pm_splk_dma_prot);
			dma_release_channel(ch->chan);
			ch->chan = NULL;
			ch->desc = NULL;
			spin_unlock(&pm_dma->pm_splk_dma_prot);
		}
	}

	rt_data->hw_chan = 0;
}

/* ------------------------------------------------------------------ */
/* DMA step configuration                                             */
/* ------------------------------------------------------------------ */

/*
 * sprd_pcm_dma_step_config() - compute src/des step from the data width.
 *
 * The blob sets, for a "normal" (non burst) transfer:
 *   step = 4 for a single channel, 2 for two channels
 * and for a burst transfer:
 *   step = 4 (16 bit samples), 8 (32 bit samples)
 */
void sprd_pcm_dma_step_config(struct sprd_runtime_data *rt_data,
				      int is_playback)
{
	unsigned int step;

	if (rt_data->transfer_type == 0) {
		step = 4;
		if (rt_data->channels != 1)
			step = 2;

		if (is_playback) {
			rt_data->src_step = 0;
			rt_data->des_step = step;
		} else {
			rt_data->src_step = step;
			rt_data->des_step = 0;
		}
	} else {
		switch (rt_data->datawidth) {
		case 2:
			step = 4;
			break;
		case 4:
			step = 8;
			break;
		default:
			pr_err("%s: err: datawidth %u is not supported yet.\n",
				__func__, rt_data->datawidth);
			step = 0;
			break;
		}

		if (is_playback) {
			rt_data->src_step = 0;
			rt_data->des_step = step;
		} else {
			rt_data->src_step = step;
			rt_data->des_step = 0;
		}
	}

	pr_debug("%s: desc->src_step  =%u, des_step=%u, datawidth =%u\n",
		 __func__, rt_data->src_step, rt_data->des_step,
		 rt_data->datawidth);
}

/* ------------------------------------------------------------------ */
/* DMA configuration                                                  */
/* ------------------------------------------------------------------ */

/*
 * sprd_pcm_config_dma() - build the SPRD DMA link-list descriptors.
 *
 * The SPRD DMA controller runs in link-list mode: the driver fills a
 * memory area with one struct sprd_dma_chn_hw per fragment and hands the
 * physical/virtual pair to the DMA engine through struct sprd_dma_linklist.
 *
 * The blob walks the buffer in "fragments" of fragmens_len bytes, one
 * scatterlist entry per fragment, and computes the source/destination
 * addresses per channel.
 */
int sprd_pcm_config_dma(struct snd_soc_component *component,
				struct snd_pcm_substream *substream,
				struct sprd_runtime_data *rt_data,
				u32 *buf_phy)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct device *dev = component->dev;
	int is_playback = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	struct sprd_dma_linklist link;
	struct dma_slave_config config;
	struct scatterlist *sg;
	u32 frag = rt_data->fragmens_len;
	u32 offset = 0;
	u32 remaining;
	int sg_num;
	int i, ret;

	if (rt_data->hw_chan <= 0) {
		dev_err(dev, "%s: no dma\n", __func__);
		return -EINVAL;
	}

	/* The link-list area holds one descriptor per fragment per channel. */
	sg_num = runtime->periods;
	if (sg_num < 1)
		sg_num = 1;

	for (i = 0; i < rt_data->hw_chan; i++) {
		struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];
		struct sprd_dma_cfg *cfg = &rt_data->dma_cfg[i];

		cfg->transcation_len = frag;
		cfg->datawidth = rt_data->datawidth;
		cfg->src_step = rt_data->src_step;
		cfg->des_step = rt_data->des_step;
		cfg->fragmens_len = frag;
		cfg->sg_num = sg_num;

		if (is_playback) {
			config.direction = DMA_MEM_TO_DEV;
			config.src_addr = audio_addr_ap2dsp(DDR32,
					buf_phy[i], false);
			config.dst_addr = rt_data->slave_id;
			cfg->config.src_addr = config.src_addr;
			cfg->config.dst_addr = config.dst_addr;
		} else {
			config.direction = DMA_DEV_TO_MEM;
			config.src_addr = rt_data->slave_id;
			config.dst_addr = audio_addr_ap2dsp(DDR32,
					buf_phy[i], false);
			cfg->config.src_addr = config.src_addr;
			cfg->config.dst_addr = config.dst_addr;
		}

		config.src_addr_width = rt_data->datawidth;
		config.dst_addr_width = rt_data->datawidth;
		config.src_maxburst = rt_data->src_burst;
		config.dst_maxburst = rt_data->dst_burst;
		cfg->config = config;

		ret = dmaengine_slave_config(ch->chan, &config);
		if (ret) {
			dev_err(dev,
				"%s, DMA chan ID %d config is failed!\n",
				__func__, i);
			return ret;
		}
	}

	/* One scatterlist per channel, one entry per fragment. */
	sg = devm_kcalloc(dev, SPRD_PCM_DMA_CHAN_MAX,
			  sizeof(*sg) * sg_num, GFP_KERNEL);
	if (!sg) {
		dev_err(dev, "ERR: dma_pdata alloc failed!\n");
		return -ENOMEM;
	}

	for (i = 0; i < rt_data->hw_chan; i++) {
		struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];
		struct scatterlist *s = &sg[i * sg_num];
		struct sprd_dma_cfg *cfg = &rt_data->dma_cfg[i];
		u32 addr = buf_phy[i];

		sg_init_table(s, sg_num);
		cfg->sg = s;

		remaining = rt_data->buffer_bytes;
		offset = 0;
		for (ret = 0; ret < sg_num; ret++) {
			u32 len = min(frag, remaining);

			sg_dma_address(&s[ret]) = addr + offset;
			sg_dma_len(&s[ret]) = len;
			offset += len;
			remaining -= len;
			if (!remaining)
				break;
		}
		cfg->sg_num = ret ? ret : 1;

		/* link-list memory for this channel */
		link.virt_addr = (unsigned long)rt_data->dma_cfg_virt[i];
		link.phy_addr = rt_data->dma_cfg_phy[i];
		link.wrap_addr = 0;
		cfg->ll_cfg = link;

		ch->desc = ch->chan->device->device_prep_slave_sg(
				ch->chan, s, cfg->sg_num,
				is_playback ? DMA_MEM_TO_DEV : DMA_DEV_TO_MEM,
				SPRD_DMA_FLAGS(SPRD_DMA_CHN_MODE_NONE,
					       SPRD_DMA_NO_TRG,
					       SPRD_DMA_FRAG_REQ,
					       SPRD_DMA_TRANS_INT),
				&link);
		if (!ch->desc) {
			dev_err(dev,
				"%s dma_tx_des[0] is null ret=%#x, %d\n",
				__func__, 0, i);
			return -ENOMEM;
		}
	}

	return 0;
}

/* ------------------------------------------------------------------ */
/* Component callbacks                                                */
/* ------------------------------------------------------------------ */

int sprd_pcm_new(struct snd_soc_component *component,
			struct snd_soc_pcm_runtime *rtd)
{
	struct device *dev = component->dev;

	/*
	 * The blob used a 64 bit DMA mask on the platform device and then
	 * forced the stream down to 32 bit addressing. Keep the 32 bit mask.
	 */
	dma_set_mask(dev, DMA_BIT_MASK(32));

	return 0;
}

static void sprd_pcm_free_dma_buffers(struct snd_soc_component *component,
				      struct snd_soc_pcm_runtime *rtd)
{
	pr_debug("%s\n", __func__);
}

int sprd_pcm_open(struct snd_soc_component *component,
			 struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct device *dev = component->dev;
	struct sprd_runtime_data *rt_data;
	struct snd_soc_dai *dai;
	int dai_id;

	/*
	 * The blob keyed everything off the FE DAI id the stream was
	 * opened with: the I2S/TDM variants (0x124 / 0x129 in the 5.4
	 * tree) use the i2s hw params table; everything else uses the
	 * generic one. The VXTux FE DAI uses the FE_DAI_ID_* enum
	 * instead, where NORMAL_AP01 is the "normal" platform.
	 */
	dai_id = FE_DAI_ID_NORMAL_AP01;
	for_each_component_dais(component, dai) {
		dai_id = dai->id;
		break;
	}

	rt_data = devm_kzalloc(dev, sizeof(*rt_data), GFP_KERNEL);
	if (!rt_data) {
		dev_err(dev, "ERR:dma_pdata alloc failed!\n");
		return -ENOMEM;
	}

	rt_data->substream = substream;
	rt_data->dev = dev;
	rt_data->dai_id = dai_id;
	runtime->private_data = rt_data;

	pr_info("%s %s Open %s\n", __func__,
		sprd_pcm_stream_str(substream->stream), DRV_NAME);

	return 0;
}

int sprd_pcm_close(struct snd_soc_component *component,
			  struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct device *dev = component->dev;
	struct sprd_runtime_data *rt_data = runtime->private_data;

	if (!rt_data)
		return -EINVAL;

	pr_info("%s %s Close %s\n", __func__,
		sprd_pcm_stream_str(substream->stream), DRV_NAME);

	if (pm_dma) {
		mutex_lock(&pm_dma->pm_mtx_dma_prot);
		mutex_lock(&pm_dma->pm_mtx_cnt);
		if (rt_data->no_pm_cnt > 0)
			rt_data->no_pm_cnt--;
		mutex_unlock(&pm_dma->pm_mtx_cnt);
		mutex_unlock(&pm_dma->pm_mtx_dma_prot);
	}

	devm_kfree(dev, rt_data);
	runtime->private_data = NULL;

	return 0;
}

int sprd_pcm_hw_params(struct snd_soc_component *component,
			      struct snd_pcm_substream *substream,
			      struct snd_pcm_hw_params *params)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct snd_soc_pcm_runtime *rtd = snd_soc_substream_to_rtd(substream);
	struct sprd_runtime_data *rt_data = runtime->private_data;
	struct sprd_pcm_dma_params *dma_params;
	struct snd_soc_dai *dai;
	int is_playback = substream->stream == SNDRV_PCM_STREAM_PLAYBACK;
	u32 buf_phy[SPRD_PCM_DMA_CHAN_MAX];
	int ret;

	if (!rt_data)
		return -EINVAL;

	rt_data->channels = params_channels(params);
	rt_data->buffer_bytes = params_buffer_bytes(params);
	rt_data->period_bytes = params_period_bytes(params);
	rt_data->periods = params_periods(params);
	rt_data->bytes_per_frame =
		div_u64(params_buffer_bytes(params),
			max(params_channels(params), 1));
	rt_data->datawidth = params_width(params) / 8;
	rt_data->transfer_type = is_playback ? 1 : 2;

	snd_soc_set_runtime_hwparams(substream, &sprd_pcm_hardware_v1);

	sprd_pcm_dma_step_config(rt_data, is_playback);

	/*
	 * The FE DAI publishes the DMA description (channel names, device
	 * physical addresses, fragment length, data width) through
	 * snd_soc_dai_set_dma_data() in its own hw_params. Pick it up
	 * here - the platform is the only consumer.
	 */
	dma_params = NULL;
	for_each_component_dais(component, dai) {
		dma_params = snd_soc_dai_get_dma_data(dai, substream);
		if (dma_params)
			break;
	}
	if (!dma_params && rtd && rtd->dais)
		dma_params = snd_soc_dai_get_dma_data(rtd->dais[0], substream);

	if (!dma_params) {
		/* No DMA description: the stream runs without DMA. */
		dev_warn(component->dev, "no dma parameters setting\n");
		return 0;
	}

	rt_data->fragmens_len = dma_params->desc.fragmens_len;
	rt_data->datawidth = dma_params->desc.datawidth;
	rt_data->slave_id = dma_params->dev_paddr[0];
	rt_data->src_burst = dma_params->desc.fragmens_len;
	rt_data->dst_burst = dma_params->desc.fragmens_len;

	rt_data->dma_cfg = devm_kcalloc(component->dev, SPRD_PCM_DMA_CHAN_MAX,
					sizeof(*rt_data->dma_cfg), GFP_KERNEL);
	if (!rt_data->dma_cfg)
		return -ENOMEM;

	ret = sprd_pcm_request_dma_channel(component, rt_data,
					   dma_params->used_dma_channel_name,
					   rt_data->channels);
	if (ret)
		return ret;

	/* The PCM buffer itself lives in the shared audio memory pool. */
	rt_data->dma_buf_size = rt_data->buffer_bytes;
	rt_data->dma_buf_phy = audio_mem_alloc(DDR32, &rt_data->dma_buf_size);
	if (!rt_data->dma_buf_phy) {
		dev_err(component->dev, "ERR: audio_mem_alloc failed!\n");
		return -ENOMEM;
	}
	rt_data->dma_buf_virt = audio_mem_vmap(rt_data->dma_buf_phy,
					       rt_data->dma_buf_size, 1);
	if (!rt_data->dma_buf_virt) {
		dev_err(component->dev, "ERR: audio_mem_vmap failed!\n");
		return -ENOMEM;
	}

	memset(buf_phy, 0, sizeof(buf_phy));
	buf_phy[0] = rt_data->dma_buf_phy;

	return sprd_pcm_config_dma(component, substream, rt_data, buf_phy);
}

int sprd_pcm_hw_free(struct snd_soc_component *component,
			    struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sprd_runtime_data *rt_data = runtime->private_data;

	if (!rt_data)
		return 0;

	if (pm_dma) {
		mutex_lock(&pm_dma->pm_mtx_dma_prot);
		mutex_lock(&pm_dma->pm_mtx_cnt);
		if (sprd_pcm_use_pm_lock(rt_data, substream->stream))
			pm_dma->normal_rtd = rt_data;
		mutex_unlock(&pm_dma->pm_mtx_cnt);
		mutex_unlock(&pm_dma->pm_mtx_dma_prot);
	}

	sprd_pcm_release_dma_channel(rt_data);

	if (rt_data->dma_buf_virt) {
		audio_mem_unmap(rt_data->dma_buf_virt);
		rt_data->dma_buf_virt = NULL;
	}
	if (rt_data->dma_buf_phy) {
		audio_mem_free(DDR32, rt_data->dma_buf_phy,
			       rt_data->dma_buf_size);
		rt_data->dma_buf_phy = 0;
	}

	return 0;
}

int sprd_pcm_prepare(struct snd_soc_component *component,
			     struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sprd_runtime_data *rt_data = runtime->private_data;

	if (rt_data)
		rt_data->no_pm_cnt = 1;

	return 0;
}

int sprd_pcm_trigger(struct snd_soc_component *component,
			     struct snd_pcm_substream *substream, int cmd)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct device *dev = component->dev;
	struct sprd_runtime_data *rt_data = runtime->private_data;
	int i, ret;

	if (!rt_data)
		return -EINVAL;

	switch (cmd) {
	case SNDRV_PCM_TRIGGER_START:
	case SNDRV_PCM_TRIGGER_RESUME:
	case SNDRV_PCM_TRIGGER_PAUSE_RELEASE:
		for (i = 0; i < rt_data->hw_chan; i++) {
			struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];

			if (!ch->chan || !ch->desc)
				continue;

			ch->cookie = dmaengine_submit(ch->desc);
			ret = dma_submit_error(ch->cookie);
			if (ret) {
				dev_err(dev, "failed to submit dma request: %d\n",
					ret);
				return ret;
			}
			dma_async_issue_pending(ch->chan);
		}
		break;
	case SNDRV_PCM_TRIGGER_STOP:
	case SNDRV_PCM_TRIGGER_SUSPEND:
	case SNDRV_PCM_TRIGGER_PAUSE_PUSH:
		for (i = 0; i < rt_data->hw_chan; i++) {
			struct sprd_pcm_dma_channel *ch = &rt_data->chan[i];

			if (ch->chan)
				dmaengine_terminate_async(ch->chan);
		}
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

snd_pcm_uframes_t sprd_pcm_pointer(struct snd_soc_component *component,
					  struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sprd_runtime_data *rt_data = runtime->private_data;
	snd_pcm_uframes_t bytes = 0;

	if (!rt_data || !rt_data->hw_chan)
		return 0;

	bytes = (snd_pcm_uframes_t)rt_data->pre_pointer[0];

	return bytes_to_frames(runtime, bytes);
}

int sprd_pcm_mmap(struct snd_soc_component *component,
			 struct snd_pcm_substream *substream,
			 struct vm_area_struct *vma)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sprd_runtime_data *rt_data = runtime->private_data;

	if (!rt_data || !rt_data->dma_buf_virt)
		return -EINVAL;

	/* 6.18: vm_flags is const, use the vm_flags_set() helpers. */
	vm_flags_set(vma, VM_IO | VM_READ | VM_WRITE | VM_MAYREAD |
			     VM_MAYWRITE | VM_MAYEXEC);

	/* DMA buffers are device memory: map it uncached, as the in-tree
	 * ASoC DMA drivers do (see sound/soc/loongson/loongson_dma.c).
	 */
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	return remap_pfn_range(vma, vma->vm_start,
			       rt_data->dma_buf_phy >> PAGE_SHIFT,
			       vma->vm_end - vma->vm_start,
			       vma->vm_page_prot);
}

void sprd_pcm_dma_buf_done(struct snd_pcm_substream *substream)
{
	struct snd_pcm_runtime *runtime = substream->runtime;
	struct sprd_runtime_data *rt_data = runtime->private_data;

	/*
	 * The blob tracked which of the two DMA channels reported the
	 * period completion and only raised the ALSA period elapsed
	 * notification once, when the primary channel fired.
	 */
	if (!rt_data)
		return;

	if (rt_data->transfer_type == 1) {
		snd_pcm_period_elapsed(substream);
		return;
	}

	snd_pcm_period_elapsed(substream);
}

/* ------------------------------------------------------------------ */
/* ALSA proc entry ("sprd-dmaengine")                                 */
/* ------------------------------------------------------------------ */

void sprd_pcm_platform_proc_read(struct snd_info_entry *entry,
					struct snd_info_buffer *buffer)
{
	snd_iprintf(buffer, "test_ddr_buffer_size %#x\n", test_ddr_buffer_size);
	snd_iprintf(buffer, "test_normal_cap_use_iram %#x\n",
		    test_normal_cap_use_iram);
	snd_iprintf(buffer, "debug_pointer_log %#x\n", debug_pointer_log);
}

void sprd_pcm_platform_proc_write(struct snd_info_entry *entry,
					 struct snd_info_buffer *buffer)
{
	char line[64];
	char cmd[32];
	unsigned int val;
	int ret;

	while ((ret = snd_info_get_line(buffer, line, sizeof(line))) == 0) {
		if (sscanf(line, "%s %x", cmd, &val) != 2)
			continue;

		if (!strcmp(cmd, "test_ddr_buffer_size")) {
			test_ddr_buffer_size = val;
			pr_info("test_ddr_buffer_size =%#x\n",
				test_ddr_buffer_size);
		} else if (!strcmp(cmd, "test_normal_cap_use_iram")) {
			test_normal_cap_use_iram = val;
			pr_info("test_normal_cap_use_iram =%#x\n",
				test_normal_cap_use_iram);
		} else if (!strcmp(cmd, "test_force_stop")) {
			test_force_stop = val;
			pr_info("test_force_stop =%#x\n", test_force_stop);
		} else if (!strcmp(cmd, "debug_pointer_log")) {
			debug_pointer_log = val;
			pr_info("debug_pointer_log=%#x\n", debug_pointer_log);
		}
	}
}

int sprd_snd_platform_probe(struct snd_card *card)
{
	struct snd_info_entry *entry;

	entry = snd_info_create_card_entry(card, "sprd-dmaengine",
					   card->proc_root);
	if (!entry)
		return -ENOMEM;

	/* 6.18: the text ops live in the "c" union member. */
	entry->private_data = card;
	entry->mode |= 0220;
	entry->c.text.read = sprd_pcm_platform_proc_read;
	entry->c.text.write = sprd_pcm_platform_proc_write;

	return 0;
}

void sprd_snd_platform_remove(struct snd_card *card)
{
}

/* ------------------------------------------------------------------ */
/* Platform component registration                                    */
/* ------------------------------------------------------------------ */

/*
 * 6.18 component .ioctl takes the component as its first argument, while
 * snd_pcm_lib_ioctl() only takes the substream. Wrap it.
 */
static int sprd_pcm_ioctl(struct snd_soc_component *component,
			  struct snd_pcm_substream *substream,
			  unsigned int cmd, void *arg)
{
	return snd_pcm_lib_ioctl(substream, cmd, arg);
}

/*
 * The stock blob exported two data symbols that described the platform:
 *
 *   sprd_pcm_ops   - the legacy struct snd_soc_pcm_ops (open/close/ioctl/
 *                    hw_params/hw_free/prepare/trigger/pointer/mmap)
 *   sprd_soc_platform - the legacy struct snd_soc_platform
 *
 * 6.18 deleted both legacy types, so the ops table is kept here as a
 * private mirror of the component driver's callbacks. It keeps the same
 * symbol name and the same slot order, so anything that used to look at
 * the blob's layout still resolves.
 */
struct sprd_pcm_ops {
	int (*open)(struct snd_soc_component *component,
		    struct snd_pcm_substream *substream);
	int (*close)(struct snd_soc_component *component,
		     struct snd_pcm_substream *substream);
	int (*ioctl)(struct snd_soc_component *component,
		     struct snd_pcm_substream *substream,
		     unsigned int cmd, void *arg);
	int (*hw_params)(struct snd_soc_component *component,
			 struct snd_pcm_substream *substream,
			 struct snd_pcm_hw_params *params);
	int (*hw_free)(struct snd_soc_component *component,
		       struct snd_pcm_substream *substream);
	int (*prepare)(struct snd_soc_component *component,
		       struct snd_pcm_substream *substream);
	int (*trigger)(struct snd_soc_component *component,
		       struct snd_pcm_substream *substream, int cmd);
	snd_pcm_uframes_t (*pointer)(struct snd_soc_component *component,
				    struct snd_pcm_substream *substream);
	int (*mmap)(struct snd_soc_component *component,
		    struct snd_pcm_substream *substream,
		    struct vm_area_struct *vma);
};

struct sprd_pcm_ops sprd_pcm_ops = {
	.open		= sprd_pcm_open,
	.close		= sprd_pcm_close,
	.ioctl		= sprd_pcm_ioctl,
	.hw_params	= sprd_pcm_hw_params,
	.hw_free	= sprd_pcm_hw_free,
	.prepare	= sprd_pcm_prepare,
	.trigger	= sprd_pcm_trigger,
	.pointer	= sprd_pcm_pointer,
	.mmap		= sprd_pcm_mmap,
};

/*
 * The legacy struct snd_soc_platform: a name plus the PCM lifecycle
 * hooks. Kept as a data symbol with the stock name and the same
 * callback set.
 */
struct sprd_soc_platform_data {
	const char *name;
	int (*probe)(struct snd_card *card);
	void (*remove)(struct snd_card *card);
	int (*pcm_new)(struct snd_soc_component *component,
		       struct snd_soc_pcm_runtime *rtd);
	void (*pcm_free_dma_buffers)(struct snd_soc_component *component,
				     struct snd_soc_pcm_runtime *rtd);
	const struct sprd_pcm_ops *ops;
};

struct sprd_soc_platform_data sprd_soc_platform = {
	.name			= DRV_NAME,
	.probe			= sprd_snd_platform_probe,
	.remove			= sprd_snd_platform_remove,
	.pcm_new		= sprd_pcm_new,
	.pcm_free_dma_buffers	= sprd_pcm_free_dma_buffers,
	.ops			= &sprd_pcm_ops,
};

static const struct snd_soc_component_driver sprd_soc_component_driver = {
	.name		= DRV_NAME,
	.pcm_construct	= sprd_pcm_new,
	.open		= sprd_pcm_open,
	.close		= sprd_pcm_close,
	.hw_params	= sprd_pcm_hw_params,
	.hw_free	= sprd_pcm_hw_free,
	.prepare	= sprd_pcm_prepare,
	.trigger	= sprd_pcm_trigger,
	.pointer	= sprd_pcm_pointer,
	.mmap		= sprd_pcm_mmap,
	.ioctl		= sprd_pcm_ioctl,
};

int sprd_soc_platform_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct snd_soc_component *component;
	int ret;

	if (!dev->of_node) {
		dev_err(dev, "ERR: device_node is NULL!\n");
		return -EINVAL;
	}

	pm_dma = devm_kzalloc(dev, sizeof(*pm_dma), GFP_KERNEL);
	if (!pm_dma) {
		dev_err(dev, "ERR: pm_dma alloc failed!\n");
		return -ENOMEM;
	}

	__mutex_init(&pm_dma->pm_mtx_dma_prot, "&pm_dma->pm_mtx_dma_prot",
		     &init_pm_dma___key);
	__mutex_init(&pm_dma->pm_mtx_cnt, "&pm_dma->pm_mtx_cnt",
		     &init_pm_dma___key_3);
	spin_lock_init(&pm_dma->pm_splk_dma_prot);

	pm_dma->pm_nb.notifier_call = sprd_pcm_pm_notifier;
	ret = register_pm_notifier(&pm_dma->pm_nb);
	if (ret) {
		dev_err(dev, "Register pm notifier error!\n");
		return ret;
	}

	ret = devm_snd_soc_register_component(dev, &sprd_soc_component_driver,
					     NULL, 0);
	if (ret) {
		dev_err(dev, "could not register platform:%d\n", ret);
		return ret;
	}

	/*
	 * The stock blob registered the "sprd-dmaengine" proc entry from
	 * the card's platform probe. In the component API the card is
	 * known from the component, so create the entry here.
	 */
	component = snd_soc_lookup_component(dev, NULL);
	if (component && component->card)
		sprd_soc_platform.probe(component->card->snd_card);

	return 0;
}

void sprd_soc_platform_remove(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;

	snd_soc_unregister_component(dev);
	if (pm_dma)
		unregister_pm_notifier(&pm_dma->pm_nb);
}

static const struct of_device_id sprd_pcm_of_match[] = {
	{ .compatible = "unisoc,sharkl5-pcm-platform", },
	{ .compatible = "unisoc,sharkl5pro-pcm-platform", },
	{ .compatible = "unisoc,roc1-pcm-platform", },
	{ .compatible = "unisoc,qogirl6-pcm-platform", },
	{ .compatible = "unisoc,qogirn6pro-pcm-platform", },
	{ }
};
MODULE_DEVICE_TABLE(of, sprd_pcm_of_match);

static struct platform_driver sprd_pcm_driver = {
	.driver = {
		.name		= SPRD_PCM_DRIVER_NAME,
		.owner		= THIS_MODULE,
		.of_match_table = sprd_pcm_of_match,
	},
	.probe	= sprd_soc_platform_probe,
	.remove	= sprd_soc_platform_remove,
};

module_platform_driver(sprd_pcm_driver);

MODULE_DESCRIPTION("SPRD ASoC PCM DMA");
MODULE_AUTHOR("Jian chen <jian.chen@spreadtrum.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:sprd-audio");

EXPORT_SYMBOL(pm_dma);
EXPORT_SYMBOL(test_force_stop);
