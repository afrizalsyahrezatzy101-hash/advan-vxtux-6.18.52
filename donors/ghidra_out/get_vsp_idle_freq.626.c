// get_vsp_idle_freq.626 @ 00111a7c
// signature: undefined get_vsp_idle_freq.626(void)
// calling convention: unknown


undefined4 get_vsp_idle_freq_626(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x120);
  if (uVar1 < 3) {
    return *(undefined4 *)(&DAT_0010565c + (ulong)uVar1 * 0x18);
  }
  return 0;
}


