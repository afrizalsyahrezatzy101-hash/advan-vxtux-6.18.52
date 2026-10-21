====================set_ip_dvfs_table.1057@00115614====================
// signature: undefined set_ip_dvfs_table.1057(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_1057(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103940 = param_2[1];
  _DAT_00103948 = param_2[2];
  cpp_dvfs_config_table = *param_2;
  _DAT_00103960 = param_2[5];
  PTR_DAT_00103968 = (undefined *)param_2[6];
  _DAT_00103950 = param_2[3];
  DAT_00103958 = param_2[4];
  return 0;
}


