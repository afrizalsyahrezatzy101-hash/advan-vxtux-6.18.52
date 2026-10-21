====================set_ip_dvfs_table.709@00110378====================
// signature: undefined set_ip_dvfs_table.709(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 set_ip_dvfs_table_709(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  DAT_00104040 = param_2[1];
  DAT_00104048 = param_2[2];
  isp_dvfs_config_table = *param_2;
  DAT_00104060 = param_2[5];
  PTR_DAT_00104068 = (undefined *)param_2[6];
  _DAT_00104050 = param_2[3];
  DAT_00104058 = param_2[4];
  return 0;
}


