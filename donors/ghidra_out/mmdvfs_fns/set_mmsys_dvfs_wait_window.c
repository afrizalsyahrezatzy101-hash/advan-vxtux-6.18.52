====================set_mmsys_dvfs_wait_window@00117364====================
// signature: undefined set_mmsys_dvfs_wait_window(void)

undefined8 set_mmsys_dvfs_wait_window(undefined8 param_1,undefined4 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(undefined4 *)(g_mmreg_map + 4) = param_2;
  printk(&DAT_0010480c,"set_mmsys_dvfs_wait_window");
  return 0;
}


