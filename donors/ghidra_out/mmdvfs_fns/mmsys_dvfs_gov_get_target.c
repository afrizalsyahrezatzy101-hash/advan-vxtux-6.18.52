====================mmsys_dvfs_gov_get_target@0010b77c====================
// signature: undefined mmsys_dvfs_gov_get_target(void)

undefined8 mmsys_dvfs_gov_get_target(long param_1,ulong *param_2)

{
  ulong uVar1;
  ulong uVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  printk(&DAT_0010523e);
  if (*(int *)(lVar3 + 0x28) == 0) {
    uVar2 = *(ulong *)(param_1 + 0x4d0);
    if (*param_2 <= uVar2 || uVar2 == 0) {
      uVar2 = *param_2;
    }
    uVar1 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar2) {
      uVar1 = uVar2;
    }
    *param_2 = uVar1;
  }
  else {
    printk(&DAT_00104c2f,"mmsys_dvfs_gov_get_target");
    uVar1 = *param_2;
  }
  printk(&DAT_001045a4,"mmsys_dvfs_gov_get_target",uVar1);
  return 0;
}


