====================set_ip_freq_upd_en_byp.1062@0011587c====================
// signature: undefined set_ip_freq_upd_en_byp.1062(void)

void set_ip_freq_upd_en_byp_1062(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffff7;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 8;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


