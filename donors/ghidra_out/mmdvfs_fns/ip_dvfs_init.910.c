====================ip_dvfs_init.910@00112d48====================
// signature: undefined ip_dvfs_init.910(void)

undefined8 ip_dvfs_init_910(long param_1)

{
  long lVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 *puVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar3 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar3 == (undefined8 *)0x0) {
    printk(&DAT_00105665);
  }
  else {
    lVar2 = 0;
    do {
      lVar1 = lVar2 + 0x38;
      *(uint *)(*(long *)((long)&DAT_00103cc0 + lVar2) + g_mmreg_map) =
           *(uint *)(*(long *)((long)&DAT_00103cc0 + lVar2) + g_mmreg_map) & 0xfffe0000 |
           (*(uint *)((long)&DAT_00103cdc + lVar2) & 7) << 0xe |
           (*(uint *)((long)&DAT_00103cc8 + lVar2) & 7) << 0xb |
           (*(uint *)((long)&DAT_00103cd4 + lVar2) & 0xf) << 7 |
           (*(uint *)((long)&DAT_00103cd8 + lVar2) & 0xf) << 3 |
           *(uint *)((long)&DAT_00103cd0 + lVar2) & 7;
      lVar2 = lVar1;
    } while (lVar1 != 0x1c0);
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00103e54;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_00103ccc;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xffffffdf;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffffd;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 1;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x134;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0xc) = *(uint *)(g_mmreg_map + 0xc) & 0xffffffdf;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1e0) = *(uint *)(g_mmreg_map + 0x1e0) | 7;
    *(uint *)(g_mmreg_map + 0x1e4) = *(uint *)(g_mmreg_map + 0x1e4) & 0xfffffff8 | 5;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x20;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar3 = 1;
    puVar3[4] = (ulong)DAT_00103e54;
    printk(&DAT_00105b7f,1,0,7,5,1);
  }
  return 1;
}


