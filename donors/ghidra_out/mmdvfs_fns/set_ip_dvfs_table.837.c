====================set_ip_dvfs_table.837@00112474====================
// signature: undefined set_ip_dvfs_table.837(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_837(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_001043c0 = param_2[1];
  _DAT_001043c8 = param_2[2];
  _mtx_dvfs_config_table = *param_2;
  _DAT_001043e0 = param_2[5];
  PTR_DAT_001043e8 = (undefined *)param_2[6];
  _DAT_001043d0 = param_2[3];
  _DAT_001043d8 = param_2[4];
  return 0;
}


