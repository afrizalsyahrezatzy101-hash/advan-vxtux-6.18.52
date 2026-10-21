// vsp_dvfs_init.757 @ 0011376c
// signature: undefined vsp_dvfs_init.757(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 vsp_dvfs_init_757(long param_1)

{
  uint uVar1;
  int iVar2;
  undefined4 uVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  regmap_read(regmap_aon_base,0,&local_3c);
  if ((local_3c >> 0x15 & 1) == 0) {
    printk(&DAT_00101f77,"vsp_dvfs_init");
  }
  else {
    lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    *(undefined4 *)(lVar4 + 0x174) = 8;
    *(undefined4 *)(lVar4 + 0x178) = 9;
    *(undefined4 *)(lVar4 + 0x17c) = 0xe;
    *(undefined4 *)(lVar4 + 0x180) = 0x13;
    uVar1 = *(uint *)(param_1 + 0x460);
    lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    *(uint *)(lVar4 + 0x948) = *(uint *)(lVar4 + 0x948) | (uVar1 & 0x3ff) << 10;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x464);
    lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x9ac);
    *(uint *)(lVar4 + 0x9ac) = uVar1 & 0xfffffff8 | uVar1 & 3 | (uint)(iVar2 != 0) << 2;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x468);
    lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x9ac);
    *(uint *)(lVar4 + 0x9ac) = uVar1 & 0xfffffff0 | uVar1 & 7 | (uint)(iVar2 != 0) << 3;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x46c);
    lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x48);
    *(uint *)(lVar4 + 0x48) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(iVar2 != 0) << 1;
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
    *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x79c) = uVar3;
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
    *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x7a0) = uVar3;
    iVar2 = *(int *)(param_1 + 0x474);
    lVar4 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x14);
    *(uint *)(lVar4 + 0x14) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(iVar2 != 0) << 1;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  }
  if (___stack_chk_guard == local_38) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


