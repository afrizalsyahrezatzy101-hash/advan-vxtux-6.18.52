====================set_ip_dvfs_idle_index.927@00113a58====================
// signature: undefined set_ip_dvfs_idle_index.927(void)

void set_ip_dvfs_idle_index_927(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1e4) = *(uint *)(g_mmreg_map + 0x1e4) & 0xfffffff8 | param_2;
  return;
}


