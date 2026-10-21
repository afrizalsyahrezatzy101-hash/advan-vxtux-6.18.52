// set_dpu_freq_upd_en_byp @ 0010bf20
// signature: undefined set_dpu_freq_upd_en_byp(void)
// calling convention: unknown


void set_dpu_freq_upd_en_byp(long param_1,uint param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar1 + 0x48) = *(uint *)(lVar1 + 0x48) & 0xfffffffe | param_2 & 1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


