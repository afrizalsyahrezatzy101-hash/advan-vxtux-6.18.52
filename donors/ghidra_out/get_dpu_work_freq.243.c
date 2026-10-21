// get_dpu_work_freq.243 @ 0010c4ec
// signature: undefined get_dpu_work_freq.243(void)
// calling convention: unknown


undefined4 get_dpu_work_freq_243(long param_1)

{
  int iVar1;
  long lVar2;
  
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124);
  if ((int)map_table_255 == iVar1) {
    lVar2 = 0;
  }
  else if (DAT_00101480 == iVar1) {
    lVar2 = 1;
  }
  else if (DAT_00101498 == iVar1) {
    lVar2 = 2;
  }
  else if (DAT_001014b0 == iVar1) {
    lVar2 = 3;
  }
  else {
    if (DAT_001014c8 != iVar1) {
      return 0;
    }
    lVar2 = 4;
  }
  return *(undefined4 *)((long)&DAT_00101470 + lVar2 * 0x18 + 4);
}


