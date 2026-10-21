// gsp_dvfs_get_cur_freq @ 00117230
// signature: undefined gsp_dvfs_get_cur_freq(void)
// calling convention: unknown


undefined8 gsp_dvfs_get_cur_freq(long param_1,ulong *param_2)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = 0x3e0;
  if (*(int *)(*(long *)(param_1 + 0x98) + 1000) != 0) {
    lVar1 = 0x3e4;
  }
  *param_2 = (ulong)*(uint *)(*(long *)(param_1 + 0x98) + lVar1);
  return 0;
}


