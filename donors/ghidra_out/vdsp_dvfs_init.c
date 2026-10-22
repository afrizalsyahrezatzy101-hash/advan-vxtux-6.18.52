// vdsp_dvfs_init @ 00114598
// signature: undefined vdsp_dvfs_init(void)
// calling convention: unknown


undefined8 vdsp_dvfs_init(long param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  long lVar5;
  undefined4 uVar6;
  undefined8 *in_x18;
  undefined8 uVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  uVar2 = DAT_0010167c | DAT_00101684 << 3;
  uVar4 = DAT_00101678 << 5;
  *(uint *)(lVar5 + 0x54) = DAT_00101668 | DAT_00101670 << 3 | DAT_00101664 << 5;
  *(uint *)(lVar5 + 0x58) = uVar2 | uVar4;
  uVar2 = DAT_001016a4 | DAT_001016ac << 3;
  uVar4 = DAT_001016a0 << 5;
  *(uint *)(lVar5 + 0x5c) = DAT_00101690 | DAT_00101698 << 3 | DAT_0010168c << 5;
  *(uint *)(lVar5 + 0x60) = uVar2 | uVar4;
  *(uint *)(lVar5 + 100) = DAT_001016b8 | DAT_001016c0 << 3 | DAT_001016b4 << 5;
  *(uint *)(lVar5 + 0x68) = DAT_001016cc | DAT_001016d4 << 3 | DAT_001016c8 << 5;
  iVar1 = *(int *)(param_1 + 0x3ec);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar5 + 0x130) = *(uint *)(lVar5 + 0x130) & 0xfffff | iVar1 << 0x14;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x3f0);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  *(uint *)(lVar5 + 0x134) = *(uint *)(lVar5 + 0x134) & 0xfffffffe | (uint)(iVar1 != 0);
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x3f4);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar4 = *(uint *)(lVar5 + 0x134);
  *(uint *)(lVar5 + 0x134) = uVar4 & 0xfffffffc | uVar4 & 1 | (uint)(iVar1 != 0) << 1;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x3f8);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar4 = *(uint *)(lVar5 + 0x38);
  *(uint *)(lVar5 + 0x38) = uVar4 & 0xfffffff8 | uVar4 & 3 | (uint)(iVar1 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  iVar1 = *(int *)(param_1 + 0x404);
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  uVar4 = 2;
  if (iVar1 != 5) {
    uVar4 = (uint)(iVar1 == 4);
  }
  uVar7 = (&clk_pll)[uVar4];
  iVar3 = clk_prepare(uVar7);
  if (iVar3 == 0) {
    iVar3 = clk_enable(uVar7);
    if (iVar3 != 0) {
      clk_unprepare(uVar7);
      goto LAB_001147e0;
    }
    *(int *)(lVar5 + 0x114) = iVar1;
  }
  else {
LAB_001147e0:
    printk(&DAT_001024c3,"set_vdsp_work_index",iVar3,uVar4);
  }
  iVar3 = map_table_801;
  iVar1 = *(int *)(param_1 + 0x404);
  if (map_table_801 == iVar1) {
    lVar5 = 0;
LAB_00114814:
    uVar6 = (&DAT_0010166c)[lVar5 * 5];
  }
  else {
    if (DAT_00101674 == iVar1) {
      lVar5 = 1;
      goto LAB_00114814;
    }
    if (DAT_00101688 == iVar1) {
      lVar5 = 2;
      goto LAB_00114814;
    }
    if (DAT_0010169c == iVar1) {
      lVar5 = 3;
      goto LAB_00114814;
    }
    if (DAT_001016b0 == iVar1) {
      lVar5 = 4;
      goto LAB_00114814;
    }
    if (DAT_001016c4 == iVar1) {
      lVar5 = 5;
      goto LAB_00114814;
    }
    uVar6 = 0;
  }
  *(undefined4 *)(param_1 + 0x3e0) = uVar6;
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x118) =
       *(undefined4 *)(param_1 + 0x408);
  iVar1 = *(int *)(param_1 + 0x408);
  if (iVar3 == iVar1) {
    lVar5 = 0;
  }
  else if (DAT_00101674 == iVar1) {
    lVar5 = 1;
  }
  else if (DAT_00101688 == iVar1) {
    lVar5 = 2;
  }
  else if (DAT_0010169c == iVar1) {
    lVar5 = 3;
  }
  else if (DAT_001016b0 == iVar1) {
    lVar5 = 4;
  }
  else {
    if (DAT_001016c4 != iVar1) {
      uVar6 = 0;
      goto LAB_001148c4;
    }
    lVar5 = 5;
  }
  uVar6 = (&DAT_0010166c)[lVar5 * 5];
LAB_001148c4:
  iVar1 = *(int *)(param_1 + 0x400);
  *(undefined4 *)(param_1 + 0x3e4) = uVar6;
  lVar5 = *(long *)(*(long *)(param_1 + 0x488) + 0x3d8);
  mutex_lock(*(long *)(param_1 + 0x488) + 0x3f0);
  uVar4 = *(uint *)(lVar5 + 8);
  *(uint *)(lVar5 + 8) = uVar4 & 0xfffffff8 | uVar4 & 3 | (uint)(iVar1 != 0) << 2;
  mutex_unlock(*(long *)(param_1 + 0x488) + 0x3f0);
  return 0;
}


