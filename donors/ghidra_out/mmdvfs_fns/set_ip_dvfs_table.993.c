====================set_ip_dvfs_table.993@00114560====================
// signature: undefined set_ip_dvfs_table.993(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_993(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00104200 = param_2[1];
  _DAT_00104208 = param_2[2];
  jpg_dvfs_config_table = *param_2;
  _DAT_00104220 = param_2[5];
  PTR_DAT_00104228 = (undefined *)param_2[6];
  _DAT_00104210 = param_2[3];
  DAT_00104218 = param_2[4];
  return 0;
}


