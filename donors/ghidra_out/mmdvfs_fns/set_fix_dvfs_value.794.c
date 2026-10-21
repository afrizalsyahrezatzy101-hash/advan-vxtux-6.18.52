====================set_fix_dvfs_value.794@00111a1c====================
// signature: undefined set_fix_dvfs_value.794(void)

undefined8 set_fix_dvfs_value_794(undefined8 param_1,uint param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,param_2 >> 0x1f | (param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  return 0;
}


