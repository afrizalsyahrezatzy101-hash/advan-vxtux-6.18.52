// set_vsp_freq_upd_en_byp.766 @ 00114010
// signature: undefined set_vsp_freq_upd_en_byp.766(void)
// calling convention: unknown


void set_vsp_freq_upd_en_byp_766(long param_1,int param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar2 + 0x48);
  *(uint *)(lVar2 + 0x48) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(param_2 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


