====================mmsys_init@0011618c====================
// signature: undefined mmsys_init(void)

undefined8 mmsys_init(long param_1)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
  printk(&DAT_00104ebe,0,0);
  *(undefined4 *)(lVar1 + 0x40) = 1;
  mutex_lock(&mmsys_glob_reg_lock);
  *DAT_00100c28 = *DAT_00100c28 | 0x80;
  mutex_unlock(&mmsys_glob_reg_lock);
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x18,0x100000,0,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x150,4,0,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  mutex_lock(&mmsys_glob_reg_lock);
  *(uint *)(g_mmreg_map + 0x34) = *(uint *)(g_mmreg_map + 0x34) | 2;
  mutex_unlock(&mmsys_glob_reg_lock);
  return 0;
}


