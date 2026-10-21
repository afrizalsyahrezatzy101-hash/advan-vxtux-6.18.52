====================set_ip_dvfs_work_index.846@00112910====================
// signature: undefined set_ip_dvfs_work_index.846(void)

void set_ip_dvfs_work_index_846(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1d8) = *(uint *)(g_mmreg_map + 0x1d8) & 0xfffffff8 | param_2;
  return;
}


