// dpu_gov_get_target @ 001079d4
// signature: undefined dpu_gov_get_target(void)
// calling convention: unknown


undefined8 dpu_gov_get_target(long param_1,ulong *param_2)

{
  uint uVar1;
  long lVar2;
  uint uVar3;
  long lVar4;
  ulong uVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  lVar2 = 0x3e0;
  if (*(int *)(lVar4 + 1000) != 0) {
    lVar2 = 0x3e4;
  }
  uVar5 = *(ulong *)(param_1 + 0x4d0);
  uVar3 = *(uint *)(lVar4 + lVar2);
  if ((uVar5 == 0) || (uVar3 <= uVar5)) {
    uVar1 = (uint)*(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= (ulong)uVar3) {
      uVar1 = uVar3;
    }
    uVar5 = (ulong)uVar1;
  }
  *param_2 = uVar5 & 0xffffffff;
  return 0;
}


