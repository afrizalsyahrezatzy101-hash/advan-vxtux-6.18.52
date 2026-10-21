====================set_ip_dvfs_work_index.1002@00114a28====================
// signature: undefined set_ip_dvfs_work_index.1002(void)

void set_ip_dvfs_work_index_1002(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x1c8) = *(uint *)(g_mmreg_map + 0x1c8) & 0xfffffff8 | param_2;
  return;
}


