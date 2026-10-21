// set_vdsp_gfree_wait_delay @ 001150c0
// signature: undefined set_vdsp_gfree_wait_delay(void)
// calling convention: unknown


void set_vdsp_gfree_wait_delay(long param_1,int param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar1 + 0x130) = *(uint *)(lVar1 + 0x130) & 0xfffff | param_2 << 0x14;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


