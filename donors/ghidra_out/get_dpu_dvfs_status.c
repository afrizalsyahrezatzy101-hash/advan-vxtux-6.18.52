// get_dpu_dvfs_status @ 0010bd90
// signature: undefined get_dpu_dvfs_status(void)
// calling convention: unknown


void get_dpu_dvfs_status(long param_1,undefined8 *param_2)

{
  ulong uVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = (ulong)(*(uint *)(lVar3 + 0x54) >> 0xc) & 7;
  if ((uint)uVar1 < 3) {
    puVar2 = (&PTR_DAT_00105df0)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  *param_2 = puVar2;
  uVar1 = (ulong)(*(uint *)(lVar3 + 0x54) >> 6) & 7;
  if ((uint)uVar1 < 3) {
    puVar2 = (&PTR_DAT_00105df0)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  param_2[4] = puVar2;
  uVar1 = (ulong)(*(uint *)(lVar3 + 0x54) >> 3) & 7;
  if ((uint)uVar1 < 3) {
    puVar2 = (&PTR_DAT_00105df0)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  param_2[1] = puVar2;
  param_2[6] = &DAT_001023b6;
  param_2[10] = &DAT_001023b6;
  uVar1 = (ulong)*(uint *)(lVar3 + 0x7c) & 7;
  if ((uint)uVar1 < 6) {
    puVar2 = (&PTR_DAT_00105d00)[uVar1];
  }
  else {
    printk(&DAT_001035ed);
    puVar2 = &DAT_001023b6;
  }
  param_2[7] = puVar2;
  param_2[0xc] = &DAT_001023b6;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return;
}


