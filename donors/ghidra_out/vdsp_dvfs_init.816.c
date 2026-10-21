// vdsp_dvfs_init.816 @ 001153b4
// signature: undefined vdsp_dvfs_init.816(void)
// calling convention: unknown


undefined8 vdsp_dvfs_init_816(long param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  long lVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001034f0,"vdsp_dvfs_init");
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  uVar4 = DAT_001016f8 | DAT_00101700 << 3;
  uVar1 = DAT_00101704 << 5;
  uVar2 = DAT_001016f4 << 7;
  *(uint *)(lVar5 + 0x54) = DAT_001016e0 | DAT_001016e8 << 3 | DAT_001016ec << 5 | DAT_001016dc << 7
  ;
  *(uint *)(lVar5 + 0x58) = uVar4 | uVar1 | uVar2;
  *(uint *)(lVar5 + 0x5c) = DAT_00101710 | DAT_00101718 << 3 | DAT_0010171c << 5 | DAT_0010170c << 7
  ;
  *(uint *)(lVar5 + 0x60) = DAT_00101728 | DAT_00101730 << 3 | DAT_00101734 << 5 | DAT_00101724 << 7
  ;
  *(uint *)(lVar5 + 100) = DAT_00101740 | DAT_00101748 << 3 | DAT_0010174c << 5 | DAT_0010173c << 7;
  *(uint *)(lVar5 + 0x68) = DAT_00101758 | DAT_00101760 << 3 | DAT_00101764 << 5 | DAT_00101754 << 7
  ;
  iVar3 = *(int *)(param_1 + 0x3ec);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar5 + 0x130) = *(uint *)(lVar5 + 0x130) & 0xfffff | iVar3 << 0x14;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar3 = *(int *)(param_1 + 0x3f0);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar5 + 0x134) = *(uint *)(lVar5 + 0x134) & 0xfffffffe | (uint)(iVar3 != 0);
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar3 = *(int *)(param_1 + 0x3f4);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = *(uint *)(lVar5 + 0x134);
  *(uint *)(lVar5 + 0x134) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(iVar3 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar3 = *(int *)(param_1 + 0x3f8);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = *(uint *)(lVar5 + 0x38);
  *(uint *)(lVar5 + 0x38) = uVar1 & 0xfffffff8 | uVar1 & 3 | (uint)(iVar3 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x114) =
       *(undefined4 *)(param_1 + 0x404);
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x118) =
       *(undefined4 *)(param_1 + 0x408);
  iVar3 = *(int *)(param_1 + 0x400);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = *(uint *)(lVar5 + 8);
  *(uint *)(lVar5 + 8) = uVar1 & 0xfffffff8 | uVar1 & 3 | (uint)(iVar3 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return 0;
}


