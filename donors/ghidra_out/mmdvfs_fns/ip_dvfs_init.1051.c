====================ip_dvfs_init.1051@00114e64====================
// signature: undefined ip_dvfs_init.1051(void)

undefined8 ip_dvfs_init_1051(long param_1)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *puVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar7 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar7 == (undefined8 *)0x0) {
    printk(&DAT_001066ab);
    uVar5 = 0xffffffea;
  }
  else {
    lVar6 = 0;
    do {
      puVar1 = (uint *)((long)&DAT_00103948 + lVar6);
      puVar2 = (uint *)((long)&DAT_00103960 + lVar6);
      plVar4 = (long *)((long)&DAT_00103940 + lVar6);
      puVar3 = (uint *)((long)&DAT_00103950 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00103ad4;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_0010394c;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffff7;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffdff;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x100;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x4d000;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffffb;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1d0) = *(uint *)(g_mmreg_map + 0x1d0) & 0xfffffff8 | 2;
    lVar6 = 0;
    *(uint *)(g_mmreg_map + 0x1d4) = *(uint *)(g_mmreg_map + 0x1d4) & 0xfffffff8;
    do {
      puVar1 = (uint *)((long)&DAT_00103948 + lVar6);
      puVar2 = (uint *)((long)&DAT_00103960 + lVar6);
      plVar4 = (long *)((long)&DAT_00103940 + lVar6);
      puVar3 = (uint *)((long)&DAT_00103950 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 4;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar7 = 1;
    puVar7[4] = (ulong)DAT_001039bc;
    printk(&DAT_00105893,1,0,2,0,1);
    uVar5 = 0;
  }
  return uVar5;
}


