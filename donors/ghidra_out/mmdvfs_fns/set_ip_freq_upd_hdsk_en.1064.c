====================set_ip_freq_upd_hdsk_en.1064@001159ac====================
// signature: undefined set_ip_freq_upd_hdsk_en.1064(void)

void set_ip_freq_upd_hdsk_en_1064(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffeff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x100;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


