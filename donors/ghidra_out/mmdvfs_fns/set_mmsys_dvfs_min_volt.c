====================set_mmsys_dvfs_min_volt@001173a4====================
// signature: undefined set_mmsys_dvfs_min_volt(void)

undefined8 set_mmsys_dvfs_min_volt(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(uint *)(g_mmreg_map + 0x10) = *(uint *)(g_mmreg_map + 0x10) | param_2 & 7;
  printk(&DAT_0010480c,"set_mmsys_dvfs_min_volt");
  return 0;
}


