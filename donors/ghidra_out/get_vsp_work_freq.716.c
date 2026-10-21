// get_vsp_work_freq.716 @ 001130d8
// signature: undefined get_vsp_work_freq.716(void)
// calling convention: unknown


undefined4 get_vsp_work_freq_716(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x7a4);
  if (uVar1 < 5) {
    return *(undefined4 *)(&DAT_0010573c + (ulong)uVar1 * 0x18);
  }
  return 0;
}


