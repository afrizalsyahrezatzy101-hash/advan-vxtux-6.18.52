// set_vsp_freq_upd_delay_en.723 @ 001134ec
// signature: undefined set_vsp_freq_upd_delay_en.723(void)
// calling convention: unknown


void set_vsp_freq_upd_delay_en_723(long param_1,int param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar2 + 0x9ac);
  *(uint *)(lVar2 + 0x9ac) = uVar1 & 0xffffffc0 | uVar1 & 0x1f | (uint)(param_2 != 0) << 5;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


