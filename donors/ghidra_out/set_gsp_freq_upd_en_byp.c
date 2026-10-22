// set_gsp_freq_upd_en_byp @ 00117a34
// signature: undefined set_gsp_freq_upd_en_byp(void)
// calling convention: unknown


void set_gsp_freq_upd_en_byp(long param_1,uint param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = 8;
  if ((param_2 & 1) == 0) {
    uVar1 = 0;
  }
  *(uint *)(lVar2 + 0x48) = *(uint *)(lVar2 + 0x48) & 0xfffffff7 | uVar1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


