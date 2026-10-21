/*
 * The per-IP callback template.
 *
 * Every IP gets its own copies of these, differing only in the register
 * window they touch, which comes from <ip>_params.  The names are
 * generated with token pasting so the eight variants stay identical.
 */

#define IP_CALLBACKS(ip)							\
static int ip##_dvfs_init(struct ip_dvfs_ops *ops)			\
{									\
	u32 i;								\
									\
	if (!ops)							\
		return -EINVAL;						\
									\
	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++)			\
		ip##_map_apply(ip##_dvfs_config_table,			\
			       &ip##_dvfs_config_table[i]);		\
									\
	mm_update(0x30, ip##_params.sw30_mask, 0);			\
	mm_update(0x220, ip##_params.upd220_clear,			\
		 ip##_params.upd220_set);				\
	mm_write(0x214, ip##_params.delay);				\
	mm_update(0x0c, ip##_params.hold_clear, 0);			\
	mm_write(ip##_params.work_idx_reg,				\
		 (mm_read(ip##_params.work_idx_reg) & ~0x7U) |	\
		 ip##_params.work_idx_def);				\
	mm_write(ip##_params.idle_idx_reg,				\
		 mm_read(ip##_params.idle_idx_reg) & ~0x7U);		\
	mm_update(0x08, 0, ip##_params.gate8);				\
									\
	return 0;							\
}									\
									\
static int ip##_get_dvfs_table(struct ip_dvfs_ops *ops,		\
			       struct ip_dvfs_map_cfg *table,		\
			       int count)				\
{									\
	int i;								\
									\
	if (!ops || !table)						\
		return -EINVAL;						\
									\
	for (i = 0; i < count && i < DVFS_FREQ_LEVEL_NUM; i++)		\
		table[i] = ip##_dvfs_config_table[i];			\
									\
	return 0;							\
}									\
									\
static int ip##_set_dvfs_table(struct ip_dvfs_ops *ops,		\
			       struct ip_dvfs_map_cfg *cfg)		\
{									\
	int i;								\
									\
	if (!ops || !cfg)						\
		return -EINVAL;						\
									\
	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++)			\
		ip##_dvfs_config_table[i].clk_level =			\
			cfg[i].clk_level;					\
									\
	return 0;							\
}									\
									\
static int ip##_get_index_from_table(struct ip_dvfs_ops *ops,	\
				      u64 freq, u32 *index)		\
{									\
	u32 i, best = 0;							\
									\
	if (!ops || !index)						\
		return -EINVAL;						\
									\
	for (i = 0; i < DVFS_FREQ_LEVEL_NUM; i++) {			\
		if (freq > ip##_dvfs_config_table[i].clk_level)		\
			best = i;						\
	}								\
	*index = best;							\
	return 0;							\
}									\
									\
static int ip##_set_work_index(struct ip_dvfs_ops *ops, u32 index)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_write(ip##_params.work_idx_reg,				\
		 (mm_read(ip##_params.work_idx_reg) & ~0x7U) |	\
		 (index & 0x7U));					\
	return 0;							\
}									\
									\
static int ip##_get_work_index(struct ip_dvfs_ops *ops, u32 *index)	\
{									\
	if (!ops || !index)						\
		return -EINVAL;						\
									\
	*index = mm_read(ip##_params.work_idx_reg) & 0x7U;		\
	return 0;							\
}									\
									\
static int ip##_set_idle_index(struct ip_dvfs_ops *ops, u32 index)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_write(ip##_params.idle_idx_reg,				\
		 (mm_read(ip##_params.idle_idx_reg) & ~0x7U) |	\
		 (index & 0x7U));					\
	return 0;							\
}									\
									\
static int ip##_set_work_freq(struct ip_dvfs_ops *ops, u64 freq)	\
{									\
	u32 index;							\
	int ret;								\
									\
	if (!ops)							\
		return -EINVAL;						\
									\
	ret = ip##_get_index_from_table(ops, freq, &index);		\
	if (ret)							\
		return ret;						\
									\
	return ip##_set_work_index(ops, index);			\
}									\
									\
static int ip##_set_idle_freq(struct ip_dvfs_ops *ops, u64 freq)	\
{									\
	return ip##_set_work_freq(ops, freq);				\
}									\
									\
static int ip##_set_hw_dvfs_en(struct ip_dvfs_ops *ops, int en)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x30, en ? 0 : ip##_params.sw30_mask,			\
		 en ? ip##_params.sw30_mask : 0);				\
	return 0;							\
}									\
									\
static int ip##_set_auto_tune_en(struct ip_dvfs_ops *ops, u32 en)	\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x30, en ? 0 : ip##_params.sw30_mask,			\
		 en ? ip##_params.sw30_mask : 0);				\
	return 0;							\
}									\
									\
static int ip##_power_on(struct ip_dvfs_ops *ops)			\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x08, 0, ip##_params.gate8);				\
	return 0;							\
}									\
									\
static int ip##_power_off(struct ip_dvfs_ops *ops)			\
{									\
	if (!ops)							\
		return -EINVAL;						\
									\
	mm_update(0x08, ip##_params.gate8, 0);				\
	return 0;							\
}									\
									\
static int ip##_set_gfree_wait_delay(struct ip_dvfs_ops *ops, u32 d)\
{									\
	(void)ops;							\
	mm_write(0x214, d);						\
	return 0;							\
}									\
static int ip##_set_freq_upd_en_byp(struct ip_dvfs_ops *ops, int en)\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(12) : 0, en ? 0 : BIT(12));		\
	return 0;							\
}									\
static int ip##_set_freq_upd_delay_en(struct ip_dvfs_ops *ops, int en)\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(11) : 0, en ? 0 : BIT(11));		\
	return 0;							\
}									\
static int ip##_set_freq_upd_hdsk_en(struct ip_dvfs_ops *ops, int en)\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(7) : 0, en ? 0 : BIT(7));		\
	return 0;							\
}									\
static int ip##_set_sw_trig_en(struct ip_dvfs_ops *ops, int en)	\
{									\
	(void)ops;							\
	mm_update(0x220, en ? BIT(4) : 0, en ? 0 : BIT(4));		\
	return 0;							\
}									\
static int ip##_get_ip_status(struct ip_dvfs_ops *ops,			\
			       struct ip_dvfs_status *status)		\
{									\
	if (!ops || !status)						\
		return -EINVAL;						\
									\
	memset(status, 0, sizeof(*status));				\
	return 0;							\
}									\
static int ip##_update_target_freq(struct ip_dvfs_ops *ops,		\
				    u64 freq, u32 index, int is_fix)	\
{									\
	int ret;								\
									\
	(void)freq;							\
	(void)is_fix;							\
	if (!ops)							\
		return -EINVAL;						\
									\
	ret = ip##_get_index_from_table(ops, freq, &index);		\
	if (ret)							\
		return ret;						\
									\
	return ip##_set_work_index(ops, index);			\
}									\
static int ip##_set_fix_dvfs_value(struct ip_dvfs_ops *ops, u32 v)	\
{									\
	(void)ops;							\
	mmsys_set_fix_dvfs_value(v);					\
	return 0;							\
}									\
static int ip##_top_current_volt(struct ip_dvfs_ops *ops, u32 *volt)\
{									\
	if (!ops || !volt)						\
		return -EINVAL;						\
									\
	*volt = top_mm_dvfs_current_volt();				\
	return 0;							\
}									\
static int ip##_mm_current_volt(struct ip_dvfs_ops *ops, u32 *volt)	\
{									\
	if (!ops || !volt)						\
		return -EINVAL;						\
									\
	*volt = mm_dvfs_read_current_volt();				\
	return 0;							\
}

IP_CALLBACKS(fd);
IP_CALLBACKS(isp);
IP_CALLBACKS(dcam_axi);
IP_CALLBACKS(dcam_if);
IP_CALLBACKS(jpg);
IP_CALLBACKS(cpp);
IP_CALLBACKS(mtx);

/*
 * The per-IP operations tables.
 */
#define IP_OPS(ip)							\
struct ip_dvfs_ops ip##_dvfs_ops = {				\
	.name			= #ip "_DVFS_OPS",			\
	.ver			= 1,				\
	.dvfs_init		= ip##_dvfs_init,		\
	.set_hw_dvfs_en		= ip##_set_hw_dvfs_en,		\
	.set_auto_tune_en	= ip##_set_auto_tune_en,	\
	.set_work_freq		= ip##_set_work_freq,		\
	.set_idle_freq		= ip##_set_idle_freq,		\
	.get_dvfs_table		= ip##_get_dvfs_table,		\
	.set_dvfs_table		= ip##_set_dvfs_table,		\
	.get_ip_status		= ip##_get_ip_status,		\
	.power_on		= ip##_power_on,		\
	.power_off		= ip##_power_off,		\
	.set_gfree_wait_delay	= ip##_set_gfree_wait_delay,	\
	.set_freq_upd_en_byp	= ip##_set_freq_upd_en_byp,	\
	.set_freq_upd_delay_en	= ip##_set_freq_upd_delay_en,	\
	.set_freq_upd_hdsk_en	= ip##_set_freq_upd_hdsk_en,	\
	.set_ip_dvfs_swtrig_en	= ip##_set_sw_trig_en,		\
	.set_ip_dvfs_work_index	= ip##_set_work_index,		\
	.get_ip_dvfs_work_index	= ip##_get_work_index,		\
	.set_ip_dvfs_idle_index	= ip##_set_idle_index,		\
	.get_ip_index_from_table	= ip##_get_index_from_table,	\
	.get_ip_index_from_table2	= ip##_get_index_from_table,	\
	.updata_target_freq	= ip##_update_target_freq,	\
	.set_fix_dvfs_value	= ip##_set_fix_dvfs_value,	\
	.get_top_current_volt	= ip##_top_current_volt,	\
	.get_mm_current_volt	= ip##_mm_current_volt,	\
}

IP_OPS(fd);
IP_OPS(isp);
IP_OPS(dcam_axi);
IP_OPS(dcam_if);
IP_OPS(jpg);
IP_OPS(cpp);
IP_OPS(mtx);
