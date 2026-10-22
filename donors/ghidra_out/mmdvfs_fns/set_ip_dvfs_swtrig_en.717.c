====================set_ip_dvfs_swtrig_en.717@001107a8====================
// signature: undefined set_ip_dvfs_swtrig_en.717(void)

void set_ip_dvfs_swtrig_en_717(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffd;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 2;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


