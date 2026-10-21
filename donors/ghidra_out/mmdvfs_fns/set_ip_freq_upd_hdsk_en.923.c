====================set_ip_freq_upd_hdsk_en.923@00113878====================
// signature: undefined set_ip_freq_upd_hdsk_en.923(void)

void set_ip_freq_upd_hdsk_en_923(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


