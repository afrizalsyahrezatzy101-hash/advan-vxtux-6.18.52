// get_dpu_idle_freq @ 0010bb78
// signature: undefined get_dpu_idle_freq(void)
// calling convention: unknown


undefined4 get_dpu_idle_freq(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7c0);
  if (map_table == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_001013c0 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_001013d8 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_001013f0 == iVar1) {
    lVar2 = 3;
  }
  else if (DAT_00101408 == iVar1) {
    lVar2 = 4;
  }
  else if (DAT_00101420 == iVar1) {
    lVar2 = 5;
  }
  else if (DAT_00101438 == iVar1) {
    lVar2 = 6;
  }
  else {
    if (DAT_00101450 != iVar1) {
      return 0;
    }
    lVar2 = 7;
  }
  return (&DAT_001013b4)[lVar2 * 6];
}


