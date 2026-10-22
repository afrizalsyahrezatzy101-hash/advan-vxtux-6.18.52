====================set_ip_freq_upd_en_byp.786@001115dc====================
// signature: undefined set_ip_freq_upd_en_byp.786(void)

void set_ip_freq_upd_en_byp_786(int param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  if (param_1 == 0) {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffe;
  }
  else {
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) | 1;
  }
  mutex_unlock(&mmsys_glob_reg_lock);
  return;
}


