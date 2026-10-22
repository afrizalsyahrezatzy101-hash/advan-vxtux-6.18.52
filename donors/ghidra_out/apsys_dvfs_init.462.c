// apsys_dvfs_init.462 @ 0010f510
// signature: undefined apsys_dvfs_init.462(void)
// calling convention: unknown


void apsys_dvfs_init_462(long param_1)

{
  uint uVar1;
  
  **(undefined4 **)(param_1 + 0x3d8) = *(undefined4 *)(param_1 + 0x3b4);
  uVar1 = *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c);
  *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) =
       uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(*(int *)(param_1 + 0x3c0) != 0) << 1;
  *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) =
       *(uint *)(*(long *)(param_1 + 0x3d8) + 0x3c) & 0xfffffffe |
       (uint)(*(int *)(param_1 + 0x3c4) != 0);
  *(undefined4 *)(*(long *)(param_1 + 0x3d8) + 4) = *(undefined4 *)(param_1 + 0x3b8);
  *(undefined4 *)(*(long *)(param_1 + 0x3d8) + 0x10) = *(undefined4 *)(param_1 + 0x3bc);
  return;
}


