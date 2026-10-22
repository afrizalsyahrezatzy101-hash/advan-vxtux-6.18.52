====================set_ip_dvfs_swtrig_en.924@00113910====================
// signature: undefined set_ip_dvfs_swtrig_en.924(void)

void set_ip_dvfs_swtrig_en_924(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


