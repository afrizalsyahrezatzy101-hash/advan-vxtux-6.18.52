// vdsp_gov_get_target @ 0010a218
// signature: undefined vdsp_gov_get_target(void)
// calling convention: unknown


undefined8 vdsp_gov_get_target(long param_1,ulong *param_2)

{
  uint uVar1;
  long lVar2;
  uint uVar3;
  ulong uVar4;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  printk(&DAT_00102587);
  lVar2 = 0x3e0;
  if (*(int *)(lVar5 + 1000) != 0) {
    lVar2 = 0x3e4;
  }
  uVar4 = *(ulong *)(param_1 + 0x4d0);
  uVar3 = *(uint *)(lVar5 + lVar2);
  if ((uVar4 == 0) || (uVar3 <= uVar4)) {
    uVar1 = (uint)*(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= (ulong)uVar3) {
      uVar1 = uVar3;
    }
    uVar4 = (ulong)uVar1;
  }
  *param_2 = uVar4 & 0xffffffff;
  return 0;
}


