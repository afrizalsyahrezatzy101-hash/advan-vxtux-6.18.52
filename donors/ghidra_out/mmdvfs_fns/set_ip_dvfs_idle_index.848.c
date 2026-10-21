====================set_ip_dvfs_idle_index.848@001129c0====================
// signature: undefined set_ip_dvfs_idle_index.848(void)

void set_ip_dvfs_idle_index_848(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1dc) = *(uint *)(g_mmreg_map + 0x1dc) & 0xfffffff8 | param_2;
  return;
}


