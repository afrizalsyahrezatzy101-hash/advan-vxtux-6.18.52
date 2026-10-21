// get_vsp_idle_freq.718 @ 00113248
// signature: undefined get_vsp_idle_freq.718(void)
// calling convention: unknown


undefined4 get_vsp_idle_freq_718(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x7a8);
  if (uVar1 < 5) {
    return *(undefined4 *)(&DAT_0010573c + (ulong)uVar1 * 0x18);
  }
  return 0;
}


