====================set_ip_dvfs_table.916@001134ec====================
// signature: undefined set_ip_dvfs_table.916(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_916(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103cc0 = param_2[1];
  _DAT_00103cc8 = param_2[2];
  dcam_dvfs_config_table = *param_2;
  DAT_00103ce0 = param_2[5];
  PTR_DAT_00103ce8 = (undefined *)param_2[6];
  _DAT_00103cd0 = param_2[3];
  _DAT_00103cd8 = param_2[4];
  return 1;
}


