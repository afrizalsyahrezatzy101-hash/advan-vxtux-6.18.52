// dpu_dvfs_init.270 @ 0010cbe0
// signature: undefined dpu_dvfs_init.270(void)
// calling convention: unknown


undefined8 dpu_dvfs_init_270(long param_1)

{
  int iVar1;
  uint uVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001034f0,"dpu_dvfs_init");
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  uVar2 = DAT_00101500 | DAT_001014fc << 3;
  *(uint *)(lVar3 + 0x94) = DAT_001014e8 | DAT_001014e4 << 3;
  *(uint *)(lVar3 + 0x98) = uVar2;
  *(uint *)(lVar3 + 0x9c) = DAT_00101518 | DAT_00101514 << 3;
  *(uint *)(lVar3 + 0x100) = DAT_00101530 | DAT_0010152c << 3;
  *(uint *)(lVar3 + 0x104) = DAT_00101548 | DAT_00101544 << 3;
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


