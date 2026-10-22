// vsp_hw_dvfs_en.622 @ 0011180c
// signature: undefined vsp_hw_dvfs_en.622(void)
// calling convention: unknown


void vsp_hw_dvfs_en_622(long param_1,int param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar2 + 8);
  *(uint *)(lVar2 + 8) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(param_2 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


