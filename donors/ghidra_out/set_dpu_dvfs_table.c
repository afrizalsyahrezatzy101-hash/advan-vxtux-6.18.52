// set_dpu_dvfs_table @ 0010bd74
// signature: undefined set_dpu_dvfs_table(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dpu_dvfs_table(undefined8 *param_1)

{
  _DAT_001013b0 = param_1[1];
  DAT_001013b8 = param_1[2];
  _map_table = *param_1;
  return;
}


