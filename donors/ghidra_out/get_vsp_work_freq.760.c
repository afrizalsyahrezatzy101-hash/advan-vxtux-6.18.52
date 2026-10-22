// get_vsp_work_freq.760 @ 00113c8c
// signature: undefined get_vsp_work_freq.760(void)
// calling convention: unknown


undefined4 get_vsp_work_freq_760(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x79c);
  if (uVar1 < 4) {
    return *(undefined4 *)(&DAT_0010584c + (ulong)uVar1 * 0x18);
  }
  return 0;
}


