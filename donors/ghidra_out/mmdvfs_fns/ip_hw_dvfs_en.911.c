====================ip_hw_dvfs_en.911@001130b8====================
// signature: undefined ip_hw_dvfs_en.911(void)

undefined8 ip_hw_dvfs_en_911(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_2 == 0) {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return 1;
}


