====================ip_dvfs_init.987@00113de0====================
// signature: undefined ip_dvfs_init.987(void)

undefined8 ip_dvfs_init_987(long param_1)

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
    printk(&DAT_00104bcc);
    uVar5 = 0xffffffea;
  }
  else {
    lVar6 = 0;
    do {
      puVar1 = (uint *)((long)&DAT_00104208 + lVar6);
      puVar2 = (uint *)((long)&DAT_00104220 + lVar6);
      plVar4 = (long *)((long)&DAT_00104200 + lVar6);
      puVar3 = (uint *)((long)&DAT_00104210 + lVar6);
      lVar6 = lVar6 + 0x38;
      *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
    } while (lVar6 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00104394;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_0010420c;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffef;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffff7f;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x40;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x134;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xfffffff7;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1c8) = *(uint *)(g_mmreg_map + 0x1c8) & 0xfffffff8 | 5;
    *(uint *)(g_mmreg_map + 0x1cc) = *(uint *)(g_mmreg_map + 0x1cc) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 8;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar7 = 1;
    puVar7[4] = (ulong)DAT_00104324;
    printk(&DAT_00104a19,1,0,5,0,1);
    uVar5 = 0;
  }
  return uVar5;
}


