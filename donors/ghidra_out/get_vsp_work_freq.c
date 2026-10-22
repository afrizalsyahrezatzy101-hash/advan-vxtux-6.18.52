// get_vsp_work_freq @ 001103bc
// signature: undefined get_vsp_work_freq(void)
// calling convention: unknown


undefined4 get_vsp_work_freq(long param_1)

{
  uint uVar1;
  
  uVar1 = *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x11c);
  if (uVar1 < 4) {
    return *(undefined4 *)(&DAT_00105434 + (ulong)uVar1 * 0x18);
  }
  return 0;
}


