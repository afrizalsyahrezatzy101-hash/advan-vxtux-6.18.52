// get_vsp_dvfs_status.720 @ 00113300
// signature: undefined get_vsp_dvfs_status.720(void)
// calling convention: unknown


void get_vsp_dvfs_status_720(long param_1,undefined8 *param_2)

{
  ulong uVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = (ulong)(*(uint *)(lVar3 + 0x130) >> 8) & 7;
  if ((uint)uVar1 < 5) {
    puVar2 = (&PTR_s_0_55v_00105d30)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  *param_2 = puVar2;
  uVar1 = (ulong)(*(uint *)(lVar3 + 0x54) >> 8) & 0xf;
  if ((uint)uVar1 < 5) {
    puVar2 = (&PTR_s_0_55v_00105d30)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  param_2[5] = puVar2;
  uVar1 = (ulong)(*(uint *)(lVar3 + 0x54) >> 0xc) & 0xf;
  if ((uint)uVar1 < 5) {
    puVar2 = (&PTR_s_0_55v_00105d30)[uVar1];
  }
  else {
    printk(&DAT_0010238e);
    puVar2 = &DAT_001023b6;
  }
  param_2[4] = puVar2;
  param_2[0xb] = (&PTR_DAT_00105e68)[(ulong)*(uint *)(lVar3 + 0x6c) & 3];
  uVar1 = (ulong)*(uint *)(lVar3 + 0x70) & 7;
  if ((uint)uVar1 < 5) {
    puVar2 = (&PTR_DAT_00105d58)[uVar1];
  }
  else {
    printk(&DAT_001035ed);
    puVar2 = &DAT_001023b6;
  }
  param_2[10] = puVar2;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return;
}


