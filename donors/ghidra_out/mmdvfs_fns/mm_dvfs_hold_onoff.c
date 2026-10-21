====================mm_dvfs_hold_onoff@0011610c====================
// signature: undefined mm_dvfs_hold_onoff(void)

void mm_dvfs_hold_onoff(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001048e4,"mm_dvfs_hold_onoff",param_1);
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *g_mmreg_map = *g_mmreg_map & 0xfffffffe;
  }
  else {
    *g_mmreg_map = *g_mmreg_map | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


