/*
 * The per-IP parameters, read out of the stock blob.
 *
 * sw30_mask	bits to clear in 0x30 (the software DVFS gate)
 * upd220_clear	bits to clear in 0x220 (the update control)
 * upd220_set	bits to set in 0x220
 * delay		the value written to 0x214 (the update delay)
 * hold_clear	bits to clear in 0x0c (the voltage hold), 0 = leave alone
 * work_idx_reg	the work index register, and its default value
 * idle_idx_reg	the idle index register, and its default value
 * gate8		the bits set in 0x8 to clock-gate this IP's registers
 */
struct ip_dvfs_params {
	u32 sw30_mask;
	u32 upd220_clear;
	u32 upd220_set;
	u32 delay;
	u32 hold_clear;
	u32 work_idx_reg;
	u32 idle_idx_reg;
	u32 work_idx_def;
	u32 idle_idx_def;
	u32 gate8;
};

#define IP_PARAMS(ip)							\
	static const struct ip_dvfs_params ip##_params = {		\
		.sw30_mask	= ip##_SW30_MASK,			\
		.upd220_clear	= ip##_UPD220_CLEAR,		\
		.upd220_set	= ip##_UPD220_SET,			\
		.delay		= ip##_DELAY,			\
		.hold_clear	= ip##_HOLD_CLEAR,		\
		.work_idx_reg	= ip##_WORK_IDX_REG,		\
		.idle_idx_reg	= ip##_IDLE_IDX_REG,		\
		.work_idx_def	= ip##_WORK_IDX_DEF,		\
		.idle_idx_def	= ip##_IDLE_IDX_DEF,		\
		.gate8		= ip##_GATE8,			\
	}

#define fd_SW30_MASK		0xffffffbfU
#define fd_UPD220_CLEAR		0xffffdfffU
#define fd_UPD220_SET		0x00001000U
#define fd_DELAY		0x00000134U
#define fd_HOLD_CLEAR		0xffffffbfU
#define fd_WORK_IDX_REG		0x1f0U
#define fd_IDLE_IDX_REG		0x1f4U
#define fd_WORK_IDX_DEF		2U
#define fd_IDLE_IDX_DEF		0U
#define fd_GATE8		0x40U

#define isp_SW30_MASK		0xfffffffbU
#define isp_UPD220_CLEAR	0xfffff7ffU
#define isp_UPD220_SET		0x00000400U
#define isp_DELAY		0x13400000U
#define isp_HOLD_CLEAR		0xfffffffdU
#define isp_WORK_IDX_REG	0x1c0U
#define isp_IDLE_IDX_REG	0x1c4U
#define isp_WORK_IDX_DEF	4U
#define isp_IDLE_IDX_DEF	0U
#define isp_GATE8		0x02U

#define dcam_axi_SW30_MASK	0xfffffffeU
#define dcam_axi_UPD220_CLEAR	0xfffffff7U
#define dcam_axi_UPD220_SET	0x00000004U
#define dcam_axi_DELAY		0x0004d000U
#define dcam_axi_HOLD_CLEAR	0U
#define dcam_axi_WORK_IDX_REG	0x1e8U
#define dcam_axi_IDLE_IDX_REG	0x1ecU
#define dcam_axi_WORK_IDX_DEF	2U
#define dcam_axi_IDLE_IDX_DEF	0U
#define dcam_axi_GATE8		0x80U

#define dcam_if_SW30_MASK	0xffffffdfU
#define dcam_if_UPD220_CLEAR	0xfffffffdU
#define dcam_if_UPD220_SET	0x00000001U
#define dcam_if_DELAY		0x00000134U
#define dcam_if_HOLD_CLEAR	0xffffffdfU
#define dcam_if_WORK_IDX_REG	0x1e0U
#define dcam_if_IDLE_IDX_REG	0x1e4U
#define dcam_if_WORK_IDX_DEF	7U
#define dcam_if_IDLE_IDX_DEF	5U
#define dcam_if_GATE8		0x20U

#define jpg_SW30_MASK		0xffffffefU
#define jpg_UPD220_CLEAR	0xffffff7fU
#define jpg_UPD220_SET		0x00000040U
#define jpg_DELAY		0x00000134U
#define jpg_HOLD_CLEAR		0xfffffff7U
#define jpg_WORK_IDX_REG	0x1c8U
#define jpg_IDLE_IDX_REG	0x1ccU
#define jpg_WORK_IDX_DEF	5U
#define jpg_IDLE_IDX_DEF	0U
#define jpg_GATE8		0x08U

#define cpp_SW30_MASK		0xfffffff7U
#define cpp_UPD220_CLEAR	0xfffffdffU
#define cpp_UPD220_SET		0x00000100U
#define cpp_DELAY		0x0004d000U
#define cpp_HOLD_CLEAR		0xfffffffbU
#define cpp_WORK_IDX_REG	0x1d0U
#define cpp_IDLE_IDX_REG	0x1d4U
#define cpp_WORK_IDX_DEF	2U
#define cpp_IDLE_IDX_DEF	0U
#define cpp_GATE8		0x04U

#define mtx_SW30_MASK		0xfffffffdU
#define mtx_UPD220_CLEAR	0xffffffdfU
#define mtx_UPD220_SET		0x00000010U
#define mtx_DELAY		0x13400000U
#define mtx_HOLD_CLEAR		0U
#define mtx_WORK_IDX_REG	0x1d8U
#define mtx_IDLE_IDX_REG	0x1dcU
#define mtx_WORK_IDX_DEF	3U
#define mtx_IDLE_IDX_DEF	0U
#define mtx_GATE8		0x01U

IP_PARAMS(fd);
IP_PARAMS(isp);
IP_PARAMS(dcam_axi);
IP_PARAMS(dcam_if);
IP_PARAMS(jpg);
IP_PARAMS(cpp);
IP_PARAMS(mtx);

/*
 * The bit layout of one map register, per IP.  The stock driver packs
 * the three fields of a map entry differently for every IP.
 */
#define IP_PACK(ip, expr)						\
	static void ip##_map_apply(struct ip_dvfs_map_cfg *table,	\
				    struct ip_dvfs_map_cfg *cfg)		\
	{								\
		u32 idx = cfg->map_index;				\
									\
		(void)table;						\
		mm_write(cfg->reg_add, expr);				\
	}

/* fd, jpg and cpp: volt in 7:5, axi in 4:2, sub-level in 1:0 */
#define PACK_V5A2S2(t, i)	(((t)[i].volt_level & 0x7) << 5 |	\
				 ((t)[i].axi_index & 0x7) << 2 |	\
				 (t)[i].fdiv_num & 0x3)

/* isp: volt in 8:6, axi in 5:3, sub-level in 2:0 */
#define PACK_V6A3S3(t, i)	(((t)[i].volt_level & 0x7) << 6 |	\
				 ((t)[i].axi_index & 0x7) << 3 |	\
				 (t)[i].fdiv_num & 0x7)

/* dcam_axi: sub-level in 1:0, axi in 4:2 */
#define PACK_S2A2(t, i)		(((t)[i].fdiv_num & 0x3) |		\
				 ((t)[i].axi_index & 0x7) << 2)

/* mtx: sub-level in 2:0, volt in 5:3 */
#define PACK_S3V3(t, i)		(((t)[i].fdiv_num & 0x7) |		\
				 ((t)[i].volt_level & 0x7) << 3)

/* dcam_if: volt in 14:11, axi in 10:7, sub-level in 6:3, level in 2:0 */
#define PACK_DCAM_IF(t, i)	(((t)[i].volt_level & 0xf) << 11 |	\
				 ((t)[i].axi_index & 0xf) << 7 |	\
				 ((t)[i].fdiv_num & 0xf) << 3 |	\
				 (t)[i].map_index & 0x7)

IP_PACK(fd, PACK_V5A2S2(table, idx));
IP_PACK(isp, PACK_V6A3S3(table, idx));
IP_PACK(dcam_axi, PACK_S2A2(table, idx));
IP_PACK(dcam_if, PACK_DCAM_IF(table, idx));
IP_PACK(jpg, PACK_V5A2S2(table, idx));
IP_PACK(cpp, PACK_V5A2S2(table, idx));
IP_PACK(mtx, PACK_S3V3(table, idx));

