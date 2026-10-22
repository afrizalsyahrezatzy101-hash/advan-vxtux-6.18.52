// set_vsp_dvfs_swtrig_en.769 @ 00114124
// signature: undefined set_vsp_dvfs_swtrig_en.769(void)
// calling convention: unknown


void set_vsp_dvfs_swtrig_en_769(long param_1,int param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  *(uint *)(lVar1 + 0x1c) = *(uint *)(lVar1 + 0x1c) & 0xfffffffe | (uint)(param_2 != 0);
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


