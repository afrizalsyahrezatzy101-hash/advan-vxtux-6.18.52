====================set_ip_freq_upd_en_byp.714@001105e0====================
// signature: undefined set_ip_freq_upd_en_byp.714(void)

void set_ip_freq_upd_en_byp_714(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffb;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 4;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


