====================ip_dvfs_init.775@00110c28====================
// signature: undefined ip_dvfs_init.775(void)

undefined8 ip_dvfs_init_775(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 *puVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar2 == (undefined8 *)0x0) {
    printk(&DAT_00105b65);
    uVar1 = 0xffffffea;
  }
  else {
    *(uint *)(DAT_00103b00 + g_mmreg_map) = DAT_00103b10 & 3 | ((uint)DAT_00103b08 & 7) << 2;
    *(uint *)(DAT_00103b38 + g_mmreg_map) = DAT_00103b48 & 3 | (DAT_00103b40 & 7) << 2;
    *(uint *)(DAT_00103b70 + g_mmreg_map) = DAT_00103b80 & 3 | (DAT_00103b78 & 7) << 2;
    *(uint *)(DAT_00103ba8 + g_mmreg_map) = DAT_00103bb8 & 3 | (DAT_00103bb0 & 7) << 2;
    *(uint *)(DAT_00103be0 + g_mmreg_map) = DAT_00103bf0 & 3 | (DAT_00103be8 & 7) << 2;
    *(uint *)(DAT_00103c18 + g_mmreg_map) = DAT_00103c28 & 3 | (DAT_00103c20 & 7) << 2;
    *(uint *)(DAT_00103c50 + g_mmreg_map) = DAT_00103c60 & 3 | (DAT_00103c58 & 7) << 2;
    *(uint *)(DAT_00103c88 + g_mmreg_map) = DAT_00103c98 & 3 | (DAT_00103c90 & 7) << 2;
    *(ulong *)(param_1 + 0x4d0) = (ulong)DAT_00103c94;
    *(ulong *)(param_1 + 0x4c8) = (ulong)DAT_00103b08._4_4_;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x30) = *(uint *)(g_mmreg_map + 0x30) & 0xfffffffe;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) & 0xfffffff7;
    mutex_unlock(&mmsys_glob_reg_lock);
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 0x220) = *(uint *)(g_mmreg_map + 0x220) | 4;
    mutex_unlock(&mmsys_glob_reg_lock);
    *(undefined4 *)(g_mmreg_map + 0x214) = 0x4d000;
    *(uint *)(g_mmreg_map + 0x1e8) = *(uint *)(g_mmreg_map + 0x1e8) & 0xfffffff8 | 2;
    *(uint *)(g_mmreg_map + 0x1ec) = *(uint *)(g_mmreg_map + 0x1ec) & 0xfffffff8;
    mutex_lock(&mmsys_glob_reg_lock);
    *(uint *)(g_mmreg_map + 8) = *(uint *)(g_mmreg_map + 8) | 0x80;
    mutex_unlock(&mmsys_glob_reg_lock);
    *puVar2 = 1;
    puVar2[4] = (ulong)DAT_00103b7c;
    printk(&DAT_00106531,1,0,2,0,1);
    uVar1 = 0;
  }
  return uVar1;
}


