// set_dpu_work_freq.276 @ 0010ce28
// signature: undefined set_dpu_work_freq.276(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_dpu_work_freq_276(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (_DAT_001014ec == param_2) {
    uVar1 = 0;
  }
  else if (DAT_00101504 == param_2) {
    uVar1 = 1;
  }
  else if (DAT_0010151c == param_2) {
    uVar1 = 2;
  }
  else if (DAT_00101534 == param_2) {
    uVar1 = 3;
  }
  else {
    if (DAT_0010154c != param_2) {
      return;
    }
    uVar1 = 4;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124) = uVar1;
  return;
}


