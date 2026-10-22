// get_vsp_idle_freq @ 00110508
// signature: undefined get_vsp_idle_freq(void)
// calling convention: unknown


undefined4 get_vsp_idle_freq(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x120);
  if (uVar1 < 4) {
    return *(undefined4 *)(&DAT_00105434 + (ulong)uVar1 * 0x18);
  }
  return 0;
}


