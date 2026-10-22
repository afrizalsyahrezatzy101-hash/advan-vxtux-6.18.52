// get_gsp_idle_freq @ 00117804
// signature: undefined get_gsp_idle_freq(void)
// calling convention: unknown


undefined4 get_gsp_idle_freq(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7b0);
  if (map_table_930 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_00101a68 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101a80 == iVar1) {
    lVar2 = 2;
  }
  else {
    if (DAT_00101a98 != iVar1) {
      return 0;
    }
    lVar2 = 3;
  }
  return *(undefined4 *)(&DAT_00101a5c + lVar2 * 0x18);
}


