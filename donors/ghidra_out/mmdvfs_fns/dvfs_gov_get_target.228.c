====================dvfs_gov_get_target.228@0010a524====================
// signature: undefined dvfs_gov_get_target.228(void)

undefined8 dvfs_gov_get_target_228(long param_1,ulong *param_2)

{
  long lVar1;
  ulong uVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar3 = *(long **)(*(long *)(param_1 + 0x90) + 0x98);
  if (*plVar3 == 0) {
    printk(&DAT_0010673b,"dvfs_gov_get_target");
    uVar2 = *param_2;
  }
  else {
    lVar1 = 0x74;
    if ((int)plVar3[0x5d] != 0) {
      lVar1 = 0x78;
    }
    uVar4 = *(ulong *)(param_1 + 0x4d0);
    if (*(uint *)((long)plVar3 + lVar1) <= uVar4 || uVar4 == 0) {
      uVar4 = (ulong)*(uint *)((long)plVar3 + lVar1);
    }
    uVar2 = *(ulong *)(param_1 + 0x4c8);
    if (*(ulong *)(param_1 + 0x4c8) <= uVar4) {
      uVar2 = uVar4;
    }
    *param_2 = uVar2;
  }
  printk(&DAT_001048f9,"dvfs_gov_get_target",uVar2);
  return 0;
}


