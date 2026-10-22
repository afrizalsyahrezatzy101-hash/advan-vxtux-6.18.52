====================set_ip_dvfs_idle_index.720@001108f0====================
// signature: undefined set_ip_dvfs_idle_index.720(void)

void set_ip_dvfs_idle_index_720(undefined8 param_1,uint param_2)

{
  *(uint *)(g_mmreg_map + 0x1c4) = *(uint *)(g_mmreg_map + 0x1c4) & 0xfffffff8 | param_2;
  return;
}


