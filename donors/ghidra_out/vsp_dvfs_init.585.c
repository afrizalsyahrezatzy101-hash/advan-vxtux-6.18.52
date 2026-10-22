// vsp_dvfs_init.585 @ 00110c0c
// signature: undefined vsp_dvfs_init.585(void)
// calling convention: unknown


undefined8 vsp_dvfs_init_585(long param_1)

{
  int iVar1;
  uint uVar2;
  long lVar3;
  uint uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  *(undefined4 *)(lVar3 + 0x74) = 0;
  *(undefined4 *)(lVar3 + 0x78) = 1;
  *(undefined4 *)(lVar3 + 0x7c) = 6;
  uVar4 = *(uint *)(param_1 + 0x460);
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  *(uint *)(lVar3 + 0x130) = *(uint *)(lVar3 + 0x130) | uVar4 & 0x3ff;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x464);
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar4 = *(uint *)(lVar3 + 0x134);
  *(uint *)(lVar3 + 0x134) = uVar4 & 0xfffffff8 | uVar4 & 3 | (uint)(iVar1 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x468);
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar4 = *(uint *)(lVar3 + 0x134);
  *(uint *)(lVar3 + 0x134) = uVar4 & 0xfffffff0 | uVar4 & 7 | (uint)(iVar1 != 0) << 3;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x46c);
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar4 = *(uint *)(lVar3 + 0x38);
  *(uint *)(lVar3 + 0x38) = uVar4 & 0xfffffffc | uVar4 & 1 | (uint)(iVar1 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar2 = 2;
  uVar4 = uVar2;
  if (*(int *)(param_1 + 0x3d0) != 384000000) {
    uVar4 = (uint)(*(int *)(param_1 + 0x3d0) == 0x124f8000);
  }
  *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x11c) = uVar4;
  if (*(int *)(param_1 + 0x3d4) != 384000000) {
    uVar2 = (uint)(*(int *)(param_1 + 0x3d4) == 0x124f8000);
  }
  *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x120) = uVar2;
  iVar1 = *(int *)(param_1 + 0x474);
  lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar4 = *(uint *)(lVar3 + 8);
  *(uint *)(lVar3 + 8) = uVar4 & 0xfffffffc | uVar4 & 1 | (uint)(iVar1 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return 0;
}


