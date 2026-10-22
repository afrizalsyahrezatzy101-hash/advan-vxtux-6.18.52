====================set_mmsys_sw_dvfs_en@00117198====================
// signature: undefined set_mmsys_sw_dvfs_en(void)

undefined8 set_mmsys_sw_dvfs_en(undefined8 param_1,int param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mutex_lock(&mmsys_glob_reg_lock);
  regmap_update_bits_base(DAT_00100c20,0x150,4,(ulong)(param_2 != 0) << 2,0,0,0);
  mutex_unlock(&mmsys_glob_reg_lock);
  printk(&DAT_0010480c,"set_mmsys_sw_dvfs_en");
  return 0;
}


