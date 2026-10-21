// dpu_dvfs_init.334 @ 0010df6c
// signature: undefined dpu_dvfs_init.334(void)
// calling convention: unknown


undefined8 dpu_dvfs_init_334(long param_1)

{
  int iVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001034f0,"dpu_dvfs_init");
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  uVar2 = DAT_001015f0 | DAT_001015ec << 3;
  *(uint *)(lVar3 + 0x94) = DAT_001015d8 | DAT_001015d4 << 3;
  *(uint *)(lVar3 + 0x98) = uVar2;
  *(uint *)(lVar3 + 0x9c) = DAT_00101608 | DAT_00101604 << 3;
  *(uint *)(lVar3 + 0x100) = DAT_00101620 | DAT_0010161c << 3;
  *(uint *)(lVar3 + 0x104) = DAT_00101638 | DAT_00101634 << 3;
  *(uint *)(lVar3 + 0x108) = DAT_00101650 | DAT_0010164c << 3;
  iVar1 = *(int *)(param_1 + 0x3ec);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar3 + 0x130) = *(uint *)(lVar3 + 0x130) & 0x3ff | iVar1 << 10;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x3f0);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar2 = *(uint *)(lVar3 + 0x134);
  *(uint *)(lVar3 + 0x134) = uVar2 & 0xffffffe0 | uVar2 & 0xf | (uint)(iVar1 != 0) << 4;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x3f4);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar2 = *(uint *)(lVar3 + 0x134);
  *(uint *)(lVar3 + 0x134) = uVar2 & 0xffffffc0 | uVar2 & 0x1f | (uint)(iVar1 != 0) << 5;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x3f8);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar3 + 0x38) = *(uint *)(lVar3 + 0x38) & 0xfffffffe | (uint)(iVar1 != 0);
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124) =
       *(undefined4 *)(param_1 + 0x404);
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x128) =
       *(undefined4 *)(param_1 + 0x408);
  iVar1 = *(int *)(param_1 + 0x400);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar3 + 8) = *(uint *)(lVar3 + 8) & 0xfffffffe | (uint)(iVar1 != 0);
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return 0;
}


