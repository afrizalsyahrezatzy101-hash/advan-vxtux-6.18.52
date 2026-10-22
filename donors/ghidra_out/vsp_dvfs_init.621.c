// vsp_dvfs_init.621 @ 0011150c
// signature: undefined vsp_dvfs_init.621(void)
// calling convention: unknown


undefined8 vsp_dvfs_init_621(long param_1)

{
  int iVar1;
  long lVar2;
  uint uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  *(undefined4 *)(lVar2 + 0x74) = 0;
  *(undefined4 *)(lVar2 + 0x78) = 1;
  *(undefined4 *)(lVar2 + 0x7c) = 6;
  uVar3 = *(uint *)(param_1 + 0x460);
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  *(uint *)(lVar2 + 0x130) = *(uint *)(lVar2 + 0x130) | uVar3 & 0x3ff;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x464);
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar3 = *(uint *)(lVar2 + 0x134);
  *(uint *)(lVar2 + 0x134) = uVar3 & 0xfffffff8 | uVar3 & 3 | (uint)(iVar1 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x468);
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar3 = *(uint *)(lVar2 + 0x134);
  *(uint *)(lVar2 + 0x134) = uVar3 & 0xfffffff0 | uVar3 & 7 | (uint)(iVar1 != 0) << 3;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x46c);
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar3 = *(uint *)(lVar2 + 0x38);
  *(uint *)(lVar2 + 0x38) = uVar3 & 0xfffffffc | uVar3 & 1 | (uint)(iVar1 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar3 = 2;
  if (*(int *)(param_1 + 0x3d0) != 384000000) {
    uVar3 = (uint)(*(int *)(param_1 + 0x3d0) == 0x124f8000);
  }
  *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x11c) = uVar3;
  uVar3 = 2;
  if (*(int *)(param_1 + 0x3d4) != 384000000) {
    uVar3 = (uint)(*(int *)(param_1 + 0x3d4) == 0x124f8000);
  }
  *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x120) = uVar3;
  iVar1 = *(int *)(param_1 + 0x474);
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar3 = *(uint *)(lVar2 + 8);
  *(uint *)(lVar2 + 8) = uVar3 & 0xfffffffc | uVar3 & 1 | (uint)(iVar1 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return 0;
}


