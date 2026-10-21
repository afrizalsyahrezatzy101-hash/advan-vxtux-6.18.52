====================set_mmsys_dvfs_hold_en@00117240====================
// signature: undefined set_mmsys_dvfs_hold_en(void)

undefined8 set_mmsys_dvfs_hold_en(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001048e4,"set_mmsys_dvfs_hold_en",param_2);
  if (param_2 == 0) {
    *g_mmreg_map = *g_mmreg_map & 0xfffffffe;
  }
  else {
    *g_mmreg_map = *g_mmreg_map | 1;
  }
  printk(&DAT_0010480c,"set_mmsys_dvfs_hold_en");
  return 0;
}


