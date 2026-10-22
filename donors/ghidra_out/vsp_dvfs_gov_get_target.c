// vsp_dvfs_gov_get_target @ 00108ed8
// signature: undefined vsp_dvfs_gov_get_target(void)
// calling convention: unknown


undefined8 vsp_dvfs_gov_get_target(long param_1,ulong *param_2)

{
  long lVar1;
  long lVar2;
  ulong uVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  lVar1 = 0x3d0;
  if (*(int *)(lVar2 + 0x480) != 0) {
    lVar1 = 0x3d4;
  }
  uVar3 = (ulong)*(uint *)(lVar2 + lVar1);
  uVar4 = *(ulong *)(param_1 + 0x4d0);
  if (uVar3 <= uVar4 || uVar4 == 0) {
    uVar4 = uVar3;
  }
  uVar3 = *(ulong *)(param_1 + 0x4c8);
  if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
    uVar3 = uVar4;
  }
  *param_2 = uVar3;
  return 0;
}


