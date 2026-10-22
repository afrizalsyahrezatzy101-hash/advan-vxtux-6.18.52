// get_dpu_work_freq.341 @ 0010e25c
// signature: undefined get_dpu_work_freq.341(void)
// calling convention: unknown


undefined4 get_dpu_work_freq_341(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124);
  if (map_table_353 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_001015e8 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101600 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_00101618 == iVar1) {
    lVar2 = 3;
  }
  else if (DAT_00101630 == iVar1) {
    lVar2 = 4;
  }
  else {
    if (DAT_00101648 != iVar1) {
      return 0;
    }
    lVar2 = 5;
  }
  return (&DAT_001015dc)[lVar2 * 6];
}


