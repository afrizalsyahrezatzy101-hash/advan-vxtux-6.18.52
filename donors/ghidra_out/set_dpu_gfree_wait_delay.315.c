// set_dpu_gfree_wait_delay.315 @ 0010dc78
// signature: undefined set_dpu_gfree_wait_delay.315(void)
// calling convention: unknown


void set_dpu_gfree_wait_delay_315(long param_1,int param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar1 + 0x130) = *(uint *)(lVar1 + 0x130) & 0x3ff | param_2 << 10;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


