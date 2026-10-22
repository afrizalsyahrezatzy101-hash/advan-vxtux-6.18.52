// vsp_dvfs_init.713 @ 00112d64
// signature: undefined vsp_dvfs_init.713(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 vsp_dvfs_init_713(long param_1)

{
  uint uVar1;
  int iVar2;
  long lVar3;
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
    lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    *(undefined4 *)(lVar3 + 0x194) = 0x10;
    *(undefined4 *)(lVar3 + 0x198) = 0x11;
    *(undefined4 *)(lVar3 + 0x19c) = 0x12;
    *(undefined4 *)(lVar3 + 0x1a0) = 0x1b;
    *(undefined4 *)(lVar3 + 0x1a4) = 0x24;
    uVar1 = *(uint *)(param_1 + 0x460);
    lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    *(uint *)(lVar3 + 0x948) = *(uint *)(lVar3 + 0x948) | (uVar1 & 0x3ff) << 0x14;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x464);
    lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar3 + 0x9ac);
    *(uint *)(lVar3 + 0x9ac) = uVar1 & 0xffffffe0 | uVar1 & 0xf | (uint)(iVar2 != 0) << 4;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x468);
    lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar3 + 0x9ac);
    *(uint *)(lVar3 + 0x9ac) = uVar1 & 0xffffffc0 | uVar1 & 0x1f | (uint)(iVar2 != 0) << 5;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x46c);
    lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar3 + 0x48);
    *(uint *)(lVar3 + 0x48) = uVar1 & 0xfffffff8 | uVar1 & 3 | (uint)(iVar2 != 0) << 2;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    set_vsp_work_freq_715(param_1,*(undefined4 *)(param_1 + 0x3d0));
    set_vsp_idle_freq_717(param_1,*(undefined4 *)(param_1 + 0x3d4));
    iVar2 = *(int *)(param_1 + 0x474);
    lVar3 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x4a8) + 0x3f0);
    uVar1 = *(uint *)(lVar3 + 0x14);
    *(uint *)(lVar3 + 0x14) = uVar1 & 0xfffffffc | uVar1 & 1 | (uint)(iVar2 != 0) << 1;
    mutex_unlock(*(long *)(param_1 + 0x4a8) + 0x3f0);
  }
  if (___stack_chk_guard == local_38) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


