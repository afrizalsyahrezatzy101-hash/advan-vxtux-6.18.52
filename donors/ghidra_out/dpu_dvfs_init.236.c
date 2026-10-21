// dpu_dvfs_init.236 @ 0010c1bc
// signature: undefined dpu_dvfs_init.236(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 dpu_dvfs_init_236(long param_1)

{
  uint uVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
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
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  uVar1 = DAT_00101488 | DAT_00101484 << 3;
  *(uint *)(lVar3 + 0x94) = (uint)DAT_00101470 | map_table_255._4_4_ << 3;
  *(uint *)(lVar3 + 0x98) = uVar1;
  *(uint *)(lVar3 + 0x9c) = DAT_001014a0 | DAT_0010149c << 3;
  *(uint *)(lVar3 + 0x100) = DAT_001014b8 | DAT_001014b4 << 3;
  *(uint *)(lVar3 + 0x104) = DAT_001014d0 | DAT_001014cc << 3;
  iVar2 = *(int *)(param_1 + 0x3ec);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar3 + 0x130) = *(uint *)(lVar3 + 0x130) & 0x3ff | iVar2 << 10;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x3f0);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = *(uint *)(lVar3 + 0x134);
  *(uint *)(lVar3 + 0x134) = uVar1 & 0xffffffe0 | uVar1 & 0xf | (uint)(iVar2 != 0) << 4;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x3f4);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar1 = *(uint *)(lVar3 + 0x134);
  *(uint *)(lVar3 + 0x134) = uVar1 & 0xffffffc0 | uVar1 & 0x1f | (uint)(iVar2 != 0) << 5;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar2 = *(int *)(param_1 + 0x3f8);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar3 + 0x38) = *(uint *)(lVar3 + 0x38) & 0xfffffffe | (uint)(iVar2 != 0);
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124) =
       *(undefined4 *)(param_1 + 0x404);
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x128) =
       *(undefined4 *)(param_1 + 0x408);
  strlcpy(&local_78,&DAT_00102c75,0x40);
  iVar2 = bcmp(&local_78,"UMS512-AB",9);
  *(uint *)(param_1 + 0x400) = (uint)(iVar2 == 0);
  lVar3 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar3 + 8) = *(uint *)(lVar3 + 8) & 0xfffffffe | (uint)(iVar2 == 0);
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  if (___stack_chk_guard == local_38) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


