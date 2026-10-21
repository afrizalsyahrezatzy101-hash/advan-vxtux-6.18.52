====================set_ip_freq_upd_hdsk_en.716@00110710====================
// signature: undefined set_ip_freq_upd_hdsk_en.716(void)

void set_ip_freq_upd_hdsk_en_716(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffbff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x400;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


