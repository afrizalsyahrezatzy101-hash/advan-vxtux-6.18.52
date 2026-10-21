====================set_ip_dvfs_idle_index.1004@00114adc====================
// signature: undefined set_ip_dvfs_idle_index.1004(void)

void set_ip_dvfs_idle_index_1004(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1cc) = *(uint *)(g_mmreg_map + 0x1cc) & 0xfffffff8 | param_2;
  return;
}


