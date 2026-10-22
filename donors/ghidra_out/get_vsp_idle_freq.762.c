// get_vsp_idle_freq.762 @ 00113dd8
// signature: undefined get_vsp_idle_freq.762(void)
// calling convention: unknown


undefined4 get_vsp_idle_freq_762(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x7a8);
  if (uVar1 < 4) {
    return *(undefined4 *)(&DAT_0010584c + (ulong)uVar1 * 0x18);
  }
  return 0;
}


