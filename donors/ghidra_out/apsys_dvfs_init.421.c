// apsys_dvfs_init.421 @ 0010f05c
// signature: undefined apsys_dvfs_init.421(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void apsys_dvfs_init_421(long param_1)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  local_2c = 0;
  regmap_read(regmap_aon_base,0,&local_2c);
  if ((local_2c >> 0x15 & 1) == 0) {
    printk(&DAT_00103627,"apsys_dvfs_init");
  }
  else {
    **(undefined4 **)(param_1 + 0x3d8) = *(undefined4 *)(param_1 + 0x3b4);
    uVar1 = *(uint *)(*(long *)(param_1 + 0x3d8) + 0x50);
    *(uint *)(*(long *)(param_1 + 0x3d8) + 0x50) =
         uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(*(int *)(param_1 + 0x3c0) != 0) << 1;
    *(uint *)(*(long *)(param_1 + 0x3d8) + 0x50) =
         *(uint *)(*(long *)(param_1 + 0x3d8) + 0x50) & 0xfffffffe |
         (uint)(*(int *)(param_1 + 0x3c4) != 0);
    *(undefined4 *)(*(long *)(param_1 + 0x3d8) + 4) = *(undefined4 *)(param_1 + 0x3b8);
    *(undefined4 *)(*(long *)(param_1 + 0x3d8) + 0x24) = *(undefined4 *)(param_1 + 0x3bc);
  }
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


