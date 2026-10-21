// set_vsp_gfree_wait_delay.765 @ 00113fb4
// signature: undefined set_vsp_gfree_wait_delay.765(void)
// calling convention: unknown


void set_vsp_gfree_wait_delay_765(long param_1,uint param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  *(uint *)(lVar1 + 0x948) = *(uint *)(lVar1 + 0x948) | (param_2 & 0x3ff) << 10;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


