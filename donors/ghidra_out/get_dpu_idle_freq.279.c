// get_dpu_idle_freq.279 @ 0010cfd0
// signature: undefined get_dpu_idle_freq.279(void)
// calling convention: unknown


undefined4 get_dpu_idle_freq_279(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x128);
  if (map_table_289 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_001014f8 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101510 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_00101528 == iVar1) {
    lVar2 = 3;
  }
  else {
    if (DAT_00101540 != iVar1) {
      return 0;
    }
    lVar2 = 4;
  }
  return *(undefined4 *)(&DAT_001014ec + lVar2 * 0x18);
}


