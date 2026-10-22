// dpu_dvfs_init @ 0010b618
// signature: undefined dpu_dvfs_init(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 dpu_dvfs_init(long param_1)

{
  int iVar1;
  uint uVar2;
  long lVar3;
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
  printk(&DAT_001034f0,"dpu_dvfs_init");
  local_7c = 0;
  regmap_read(regmap_aon_base,0,&local_7c);
  if ((local_7c >> 0x15 & 1) == 0) {
    printk(&DAT_00103627,"dpu_dvfs_init");
  }
  else {
    lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    *(uint *)(lVar3 + 500) = DAT_001013b0 | DAT_001013ac << 3;
    *(uint *)(lVar3 + 0x1f8) = DAT_001013c8 | DAT_001013c4 << 3;
    *(uint *)(lVar3 + 0x1fc) = DAT_001013e0 | DAT_001013dc << 3;
    *(uint *)(lVar3 + 0x200) = DAT_001013f8 | DAT_001013f4 << 3;
    *(uint *)(lVar3 + 0x204) = DAT_00101410 | DAT_0010140c << 3;
    uVar2 = DAT_00101428 | DAT_00101424 << 3;
    *(uint *)(lVar3 + 0x208) = uVar2;
    *(uint *)(lVar3 + 0x20c) = uVar2;
    *(uint *)(lVar3 + 0x210) = uVar2;
    iVar1 = *(int *)(param_1 + 0x3ec);
    lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    *(uint *)(lVar3 + 0x944) = *(uint *)(lVar3 + 0x944) & 0x3ff | iVar1 << 10;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    iVar1 = *(int *)(param_1 + 0x3f0);
    lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar2 = *(uint *)(lVar3 + 0x9ac);
    *(uint *)(lVar3 + 0x9ac) = uVar2 & 0xffffffe0 | uVar2 & 0xf | (uint)(iVar1 != 0) << 4;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    iVar1 = *(int *)(param_1 + 0x3f4);
    lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    uVar2 = *(uint *)(lVar3 + 0x9ac);
    *(uint *)(lVar3 + 0x9ac) = uVar2 & 0xffffffc0 | uVar2 & 0x1f | (uint)(iVar1 != 0) << 5;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    iVar1 = *(int *)(param_1 + 0x3f8);
    lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    *(uint *)(lVar3 + 0x48) = *(uint *)(lVar3 + 0x48) & 0xfffffffe | (uint)(iVar1 != 0);
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
    *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7bc) =
         *(undefined4 *)(param_1 + 0x404);
    *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7c0) =
         *(undefined4 *)(param_1 + 0x408);
    strlcpy(&local_78,&DAT_00102c75,0x40);
    *(undefined4 *)(param_1 + 0x400) = 1;
    lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
    mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
    *(uint *)(lVar3 + 0x14) = *(uint *)(lVar3 + 0x14) | 1;
    mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  }
  if (___stack_chk_guard == local_38) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


