====================set_ip_dvfs_swtrig_en.1065@00115a44====================
// signature: undefined set_ip_dvfs_swtrig_en.1065(void)

void set_ip_dvfs_swtrig_en_1065(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffb;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 4;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


