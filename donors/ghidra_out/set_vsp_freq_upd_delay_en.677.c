// set_vsp_freq_upd_delay_en.677 @ 00112954
// signature: undefined set_vsp_freq_upd_delay_en.677(void)
// calling convention: unknown


void set_vsp_freq_upd_delay_en_677(long param_1,int param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar2 + 0x134);
  *(uint *)(lVar2 + 0x134) = uVar1 & 0xfffffff0 | uVar1 & 7 | (uint)(param_2 != 0) << 3;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


