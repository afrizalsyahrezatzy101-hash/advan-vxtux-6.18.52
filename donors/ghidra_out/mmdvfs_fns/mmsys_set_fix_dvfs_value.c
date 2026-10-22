====================mmsys_set_fix_dvfs_value@0010c118====================
// signature: undefined mmsys_set_fix_dvfs_value(void)

undefined8 mmsys_set_fix_dvfs_value(uint param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base(DAT_00100c20,0,1,param_1 >> 0x1f | (param_1 & 7) << 1,0,0,0);
  printk(&DAT_0010480c,"mmsys_set_fix_dvfs_value");
  return 0;
}


