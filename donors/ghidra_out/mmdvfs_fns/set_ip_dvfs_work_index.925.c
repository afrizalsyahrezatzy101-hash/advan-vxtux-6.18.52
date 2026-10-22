====================set_ip_dvfs_work_index.925@001139a8====================
// signature: undefined set_ip_dvfs_work_index.925(void)

void set_ip_dvfs_work_index_925(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1e0) = *(uint *)(g_mmreg_map + 0x1e0) & 0xfffffff8 | param_2;
  return;
}


