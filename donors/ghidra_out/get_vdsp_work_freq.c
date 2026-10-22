// get_vdsp_work_freq @ 00114bc4
// signature: undefined get_vdsp_work_freq(void)
// calling convention: unknown


undefined4 get_vdsp_work_freq(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x114);
  if (map_table_801 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_00101674 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101688 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_0010169c == iVar1) {
    lVar2 = 3;
  }
  else if (DAT_001016b0 == iVar1) {
    lVar2 = 4;
  }
  else {
    if (DAT_001016c4 != iVar1) {
      return 0;
    }
    lVar2 = 5;
  }
  return (&DAT_0010166c)[lVar2 * 5];
}


