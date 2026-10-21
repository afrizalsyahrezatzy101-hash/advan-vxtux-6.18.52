// apsys_dvfs_hold_en @ 0010eb44
// signature: undefined apsys_dvfs_hold_en(void)
// calling convention: unknown


void apsys_dvfs_hold_en(long param_1,undefined4 param_2)

{
  **(undefined4 **)(param_1 + 0x3d8) = param_2;
  return;
}


