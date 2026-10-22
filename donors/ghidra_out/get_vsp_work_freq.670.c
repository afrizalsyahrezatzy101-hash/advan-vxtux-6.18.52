// get_vsp_work_freq.670 @ 00112584
// signature: undefined get_vsp_work_freq.670(void)
// calling convention: unknown


undefined4 get_vsp_work_freq_670(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x11c);
  if (uVar1 < 3) {
    return *(undefined4 *)(&DAT_0010565c + (ulong)uVar1 * 0x18);
  }
  return 0;
}


