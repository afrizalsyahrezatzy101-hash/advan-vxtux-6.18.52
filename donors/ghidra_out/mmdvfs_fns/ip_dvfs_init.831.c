====================ip_dvfs_init.831@00111bdc====================
// signature: undefined ip_dvfs_init.831(void)

undefined8 ip_dvfs_init_831(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 *puVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar2 == (undefined8 *)0x0) {
    printk(&DAT_00104a03);
    uVar1 = 0xffffffea;
  }
  else {
    *(uint *)(DAT_001043c0 + g_mmreg_map) = DAT_001043d0 & 7 | (DAT_001043c8 & 7) << 3;
    *(uint *)(DAT_001043f8 + g_mmreg_map) = DAT_00104408 & 7 | (DAT_00104400 & 7) << 3;
    *(uint *)(DAT_00104430 + g_mmreg_map) = DAT_00104440 & 7 | (DAT_00104438 & 7) << 3;
    *(uint *)(DAT_00104468 + g_mmreg_map) = DAT_00104478 & 7 | (DAT_00104470 & 7) << 3;
    *(uint *)(DAT_001044a0 + g_mmreg_map) = DAT_001044b0 & 7 | (DAT_001044a8 & 7) << 3;
    *(uint *)(DAT_001044d8 + g_mmreg_map) = DAT_001044e8 & 7 | (DAT_001044e0 & 7) << 3;
    *(uint *)(DAT_00104510 + g_mmreg_map) = DAT_00104520 & 7 | (DAT_00104518 & 7) << 3;
    *(uint *)(DAT_00104548 + g_mmreg_map) = DAT_00104558 & 7 | (DAT_00104550 & 7) << 3;
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00104554;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_001043cc;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffd;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xffffffdf;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 0x10;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x13400000;
    mutex_lock(&mmsys_glob_reg_lock);
    mutex_unlock(*(undefined4 *)(g_mmreg_map + 0xc),&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x1d8) = *(uint *)(g_mmreg_map + 0x1d8) & 0xfffffff8 | 3;
    *(uint *)(g_mmreg_map + 0x1dc) = *(uint *)(g_mmreg_map + 0x1dc) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 1;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar2 = 1;
    puVar2[4] = (ulong)DAT_00104474;
    printk(&DAT_001067b7,1,0,3,0,1);
    uVar1 = 0;
  }
  return uVar1;
}


