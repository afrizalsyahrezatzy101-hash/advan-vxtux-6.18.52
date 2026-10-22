====================set_ip_dvfs_idle_index@0010f9cc====================
// signature: undefined set_ip_dvfs_idle_index(void)

void set_ip_dvfs_idle_index(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 500) = *(uint *)(g_mmreg_map + 500) & 0xfffffff8 | param_2;
  return;
}


