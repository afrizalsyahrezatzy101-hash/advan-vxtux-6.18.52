====================set_ip_dvfs_table.781@00111374====================
// signature: undefined set_ip_dvfs_table.781(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_781(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00103b00 = param_2[1];
  DAT_00103b08 = param_2[2];
  dcamaxi_dvfs_config_table = *param_2;
  DAT_00103b20 = param_2[5];
  PTR_DAT_00103b28 = (undefined *)param_2[6];
  _DAT_00103b10 = param_2[3];
  DAT_00103b18 = param_2[4];
  return 0;
}


