// set_dpu_dvfs_swtrig_en @ 0010c048
// signature: undefined set_dpu_dvfs_swtrig_en(void)
// calling convention: unknown


void set_dpu_dvfs_swtrig_en(long param_1,uint param_2)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar1 + 0x1c) = *(uint *)(lVar1 + 0x1c) & 0xfffffffe | param_2 & 1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


