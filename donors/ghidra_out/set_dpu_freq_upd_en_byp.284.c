// set_dpu_freq_upd_en_byp.284 @ 0010d318
// signature: undefined set_dpu_freq_upd_en_byp.284(void)
// calling convention: unknown


void set_dpu_freq_upd_en_byp_284(long param_1,uint param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar1 + 0x38) = *(uint *)(lVar1 + 0x38) & 0xfffffffe | param_2 & 1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


