====================set_ip_dvfs_swtrig_en.845@001128a4====================
// signature: undefined set_ip_dvfs_swtrig_en.845(void)

void set_ip_dvfs_swtrig_en_845(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  mutex_unlock(*(undefined4 *)(g_mmreg_map + 0xc),&mmsys_glob_reg_lock);
  return;
}


