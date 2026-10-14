/* SPDX-License-Identifier: GPL-2.0 */
/*
 * mali_kbase_platform.h - varian platform "devicetree".
 *
 * Latar (temuan 2026-09-25, tools/re): pohon ini membawa integrasi Huawei di
 * berkas inti kbase (mali_kbase_defs.h:1212/1860 memuat
 * `struct kbase_gpu_device_data gpu_dev_data;` dan `..._ctx_data gpu_ctx_data;`
 * — komentar "MALI_HISI_INTEGRATION"), sementara varian hisilicon
 * menyediakan struct itu lewat platform/hisilicon/mali_kbase_platform.h yang
 * menarik pula header vendor Huawei (dsm/, platform_include/) yang tidak ada
 * di kernel 6.18 umum. Varian devicetree (default Kconfig MALI_PLATFORM_NAME,
 * platform/Kconfig pohon ini) tidak punya berkas ini sama sekali, sehingga
 * MALI_MIDGARD tidak bisa di-compile apa adanya.
 *
 * Shim ini mendefinisikan HANYA anggota yang benar-benar dipakai berkas inti
 * (audit `gpu_dev_data.<anggota>` / `gpu_ctx_data.<anggota>` 2026-09-25,
 * luar platform/hisilicon). Versi Huawei penuh: lihat platform/hisilicon/.
 * Fungsi `kbase_platform_get_device()` tetap dideklarasikan karena
 * mali_kbase_softjobs.c:611 memanggilnya; implementasinya milik platform
 * (devicetree menyediakan lewat kbase_platform_funcs_conf / binding).
 */
#ifndef _MALI_KBASE_PLATFORM_DEVICETREE_H_
#define _MALI_KBASE_PLATFORM_DEVICETREE_H_

#include <linux/atomic.h>
#include <linux/hrtimer.h>
#include <linux/kernel.h>
#include <linux/types.h>

struct kbase_device;
struct kbase_context;
struct kbase_ipa_context;
struct devfreq;
struct proc_dir_entry;

/* dsm_report_* tidak ada tanpa CONFIG_HUAWEI_DSM; berkas inti memanggilnya
 * tanpa penjaga (mali_kbase_jd.c:1501, mali_kbase_pm_driver.c:2796). */
#define dsm_report_enable(count, threshold, factory_mode) (0)
#define gpu_dsm_report(kbdev, dsm_no, fmt, args...) do {} while (0)

/* Anggota dipakai inti (audit 2026-09-25; luar platform/hisilicon):
 *   callbacks          pm_metrics.c:158, mali_kbase_js.c:3517
 *   devfreq            mmu/mali_kbase_mmu_hw_direct.c:141
 *   error_num          jm_hw.c, js_backend.c, jd.c:1499, pm_driver.c:2766,
 *                      core_linux.c:3988-4030
 *   factory_mode       pm_driver.c, mali_kbase_jd.c, mmu/backend/mmu_jm
 *   game_pid           mali_kbase_sync_file.c
 *   gpu_fpga_exist     pm_policy.c
 *   lb_enabled         mali_kbase_mem_linux.c
 *   proc_gpu_memory_dentry mali_kbase_gpu_memory_debugfs.c
 *   regulator_refcount pm_backend.c, pm_driver.c:1828
 *   runtime_pm_delay_ms mali_kbase_core_linux.c:2617
 *   shader_present_lo_cfg gpuprops_backend.c
 *   signaled_seqno     mali_kbase_sync_file.c
 *   sys_cache_capacity mali_kbase_gpuprops.c, mali_kbase_mem.h:697
 *   thro_enable        pm_metrics.c, core_linux.c:4097
 *   unhandled_page_fault_count mmu/backend/mali_kbase_mmu_jm.c
 *   fence_wait_timer / timer_started (ctx)  context.c:182, softjobs.c:1830
 *   force_l2_flush     backend/gpu/mali_kbase_jm_hw.c
 */
struct kbase_gpu_callbacks {
	void (*cl_boost_init)(void *dev);
	void (*cl_boost_update_utilization)(void *dev, void *atom,
			u64 microseconds_spent);
};

struct kbase_gpu_device_data {
	struct kbase_gpu_callbacks *callbacks;
	struct devfreq *devfreq;

	/* error counters (core_linux.c debugfs + jd/pm_driver) */
	u32 page_fault;
	u32 gpu_fault;
	u32 soft_reset;
	u32 hard_reset;
	u64 ts;

	bool factory_mode;
	pid_t game_pid;
	u32 gpu_fpga_exist;
	u32 lb_enabled;
	struct proc_dir_entry *proc_gpu_memory_dentry;
	atomic_t regulator_refcount;
	atomic_t runtime_pm_delay_ms;
	u32 shader_present_lo_cfg;
	u64 signaled_seqno;
	u32 sys_cache_capacity;
	atomic_t thro_enable;
	u32 unhandled_page_fault_count;
};

struct kbase_gpu_ctx_data {
	struct hrtimer fence_wait_timer;
	int timer_started;
};

struct kbase_device *kbase_platform_get_device(void);

#endif /* _MALI_KBASE_PLATFORM_DEVICETREE_H_ */
