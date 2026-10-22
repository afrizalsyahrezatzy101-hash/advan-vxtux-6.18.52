// gsp_dvfs_init @ 001173b8
// signature: undefined gsp_dvfs_init(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void gsp_dvfs_init(long param_1)

{
  uint uVar1;
  int iVar2;
  undefined8 uVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_7c;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_48 = 0;
  uStack_40 = 0;
  local_58 = 0;
  uStack_50 = 0;
  local_68 = 0;
  uStack_60 = 0;
  local_78 = 0;
  uStack_70 = 0;
  printk(&DAT_001034f0,"gsp_dvfs_init");
  local_7c = 0;
  regmap_read(regmap_aon_base,0,&local_7c);
  if ((local_7c >> 0x15 & 1) == 0) {
    printk(&DAT_00103627,"gsp_dvfs_init");
    uVar3 = 0xffffffff;
  }
  else {
    lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    *(uint *)(lVar4 + 0x1b4) = DAT_00101a58 | DAT_00101a54 << 2;
    *(uint *)(lVar4 + 0x1b8) = DAT_00101a70 | DAT_00101a6c << 2;
    *(uint *)(lVar4 + 0x1bc) = DAT_00101a88 | DAT_00101a84 << 2;
    *(uint *)(lVar4 + 0x1c0) = DAT_00101aa0 | DAT_00101a9c << 2;
    uVar1 = *(uint *)(param_1 + 0x3ec);
    lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    *(uint *)(lVar4 + 0x944) = *(uint *)(lVar4 + 0x944) & 0x3ff | uVar1;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x3f0);
    lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x9ac);
    *(uint *)(lVar4 + 0x9ac) = uVar1 & 0xffffff80 | uVar1 & 0x3f | (uint)(iVar2 != 0) << 6;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x3f4);
    lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x9ac);
    *(uint *)(lVar4 + 0x9ac) = uVar1 & 0xffffff00 | uVar1 & 0x7f | (uint)(iVar2 != 0) << 7;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    iVar2 = *(int *)(param_1 + 0x3f8);
    lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x48);
    *(uint *)(lVar4 + 0x48) = uVar1 & 0xfffffff0 | uVar1 & 7 | (uint)(iVar2 != 0) << 3;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7ac) =
         *(undefined4 *)(param_1 + 0x404);
    *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7b0) =
         *(undefined4 *)(param_1 + 0x408);
    strlcpy(&local_78,&DAT_00102c75,0x40);
    iVar2 = bcmp(&local_78,"UMS512-AB",9);
    *(uint *)(param_1 + 0x400) = (uint)(iVar2 == 0);
    lVar4 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar1 = *(uint *)(lVar4 + 0x14);
    *(uint *)(lVar4 + 0x14) = uVar1 & 0xfffffff8 | uVar1 & 3 | (uint)(iVar2 == 0) << 2;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar3 = 0;
  }
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}


