====================set_ip_dvfs_table@0010f454====================
// signature: undefined set_ip_dvfs_table(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103e80 = param_2[1];
  _DAT_00103e88 = param_2[2];
  _fd_dvfs_config_table = *param_2;
  _DAT_00103ea0 = param_2[5];
  PTR_DAT_00103ea8 = (undefined *)param_2[6];
  _DAT_00103e90 = param_2[3];
  _DAT_00103e98 = param_2[4];
  return 0;
}


