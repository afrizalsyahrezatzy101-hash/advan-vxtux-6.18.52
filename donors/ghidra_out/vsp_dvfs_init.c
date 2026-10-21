// vsp_dvfs_init @ 0010ff18
// signature: undefined vsp_dvfs_init(void)
// calling convention: unknown


undefined8 vsp_dvfs_init(long param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  *(undefined4 *)(lVar4 + 0x74) = 0;
  *(undefined4 *)(lVar4 + 0x78) = 1;
  *(undefined4 *)(lVar4 + 0x7c) = 6;
  uVar1 = *(uint *)(param_1 + 0x460);
  lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  *(uint *)(lVar4 + 0x130) = *(uint *)(lVar4 + 0x130) | uVar1 & 0x3ff;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x464);
  lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar4 + 0x134);
  *(uint *)(lVar4 + 0x134) = uVar1 & 0xfffffff8 | uVar1 & 3 | (uint)(iVar2 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x468);
  lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar4 + 0x134);
  *(uint *)(lVar4 + 0x134) = uVar1 & 0xfffffff0 | uVar1 & 7 | (uint)(iVar2 != 0) << 3;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x46c);
  lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar4 + 0x38);
  *(uint *)(lVar4 + 0x38) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(iVar2 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x3d0);
  if (iVar2 == 512000000) {
    uVar3 = 3;
  }
  else if (iVar2 == 384000000) {
    uVar3 = 2;
  }
  else if (iVar2 == 0x124f8000) {
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x11c) = uVar3;
  iVar2 = *(int *)(param_1 + 0x3d4);
  if (iVar2 == 512000000) {
    uVar3 = 3;
  }
  else if (iVar2 == 384000000) {
    uVar3 = 2;
  }
  else if (iVar2 == 0x124f8000) {
    uVar3 = 1;
  }
  else {
    uVar3 = 0;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x120) = uVar3;
  iVar2 = *(int *)(param_1 + 0x474);
  lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  uVar1 = *(uint *)(lVar4 + 8);
  *(uint *)(lVar4 + 8) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(iVar2 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  return 0;
}


