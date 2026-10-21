====================set_ip_freq_upd_en_byp.921@00113748====================
// signature: undefined set_ip_freq_upd_en_byp.921(void)

void set_ip_freq_upd_en_byp_921(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffdf;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 0x20;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


