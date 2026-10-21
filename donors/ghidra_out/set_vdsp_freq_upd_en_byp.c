// set_vdsp_freq_upd_en_byp @ 00115118
// signature: undefined set_vdsp_freq_upd_en_byp(void)
// calling convention: unknown


void set_vdsp_freq_upd_en_byp(long param_1,uint param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = 4;
  if ((param_2 & 1) == 0) {
    uVar1 = 0;
  }
  *(uint *)(lVar2 + 0x38) = *(uint *)(lVar2 + 0x38) & 0xfffffffb | uVar1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


