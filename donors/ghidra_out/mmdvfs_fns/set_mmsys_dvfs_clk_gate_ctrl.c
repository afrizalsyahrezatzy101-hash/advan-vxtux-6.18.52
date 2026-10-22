====================set_mmsys_dvfs_clk_gate_ctrl@001172c0====================
// signature: undefined set_mmsys_dvfs_clk_gate_ctrl(void)

undefined8 set_mmsys_dvfs_clk_gate_ctrl(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001048e4,"mm_dvfs_auto_gate_sel",param_2);
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 0x34) = *(uint *)(g_mmreg_map + 0x34) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 0x34) = *(uint *)(g_mmreg_map + 0x34) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  printk(&DAT_0010480c,"set_mmsys_dvfs_clk_gate_ctrl");
  return 0;
}


