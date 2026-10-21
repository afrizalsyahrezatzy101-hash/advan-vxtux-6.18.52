// get_vdsp_work_freq.823 @ 00115700
// signature: undefined get_vdsp_work_freq.823(void)
// calling convention: unknown


undefined4 get_vdsp_work_freq_823(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x114);
  if (map_table_834 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_001016f0 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101708 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_00101720 == iVar1) {
    lVar2 = 3;
  }
  else if (DAT_00101738 == iVar1) {
    lVar2 = 4;
  }
  else {
    if (DAT_00101750 != iVar1) {
      return 0;
    }
    lVar2 = 5;
  }
  return (&DAT_001016e4)[lVar2 * 6];
}


