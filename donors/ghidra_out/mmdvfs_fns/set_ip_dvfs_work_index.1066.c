====================set_ip_dvfs_work_index.1066@00115adc====================
// signature: undefined set_ip_dvfs_work_index.1066(void)

void set_ip_dvfs_work_index_1066(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1d0) = *(uint *)(g_mmreg_map + 0x1d0) & 0xfffffff8 | param_2;
  return;
}


