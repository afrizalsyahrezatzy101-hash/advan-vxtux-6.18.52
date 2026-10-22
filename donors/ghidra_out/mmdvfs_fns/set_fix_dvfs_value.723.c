====================set_fix_dvfs_value.723@00110a68====================
// signature: undefined set_fix_dvfs_value.723(void)

undefined8 set_fix_dvfs_value_723(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,(uint)param_2 >> 0x1f | ((uint)param_2 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  printk(&DAT_001061da,"set_fix_dvfs_value",param_2);
  return 0;
}


