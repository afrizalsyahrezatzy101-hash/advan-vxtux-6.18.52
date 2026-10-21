====================set_ip_freq_upd_delay_en@0010f754====================
// signature: undefined set_ip_freq_upd_delay_en(void)

void set_ip_freq_upd_delay_en(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffdfff;
  }
  else {
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x2000;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


