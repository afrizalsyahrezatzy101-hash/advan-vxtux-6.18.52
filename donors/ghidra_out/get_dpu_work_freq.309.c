// get_dpu_work_freq.309 @ 0010d878
// signature: undefined get_dpu_work_freq.309(void)
// calling convention: unknown


undefined4 get_dpu_work_freq_309(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124);
  if ((int)map_table_321 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_00101570 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101588 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_001015a0 == iVar1) {
    lVar2 = 3;
  }
  else {
    if (DAT_001015b8 != iVar1) {
      return 0;
    }
    lVar2 = 4;
  }
  return *(undefined4 *)((long)&DAT_00101560 + lVar2 * 0x18 + 4);
}


