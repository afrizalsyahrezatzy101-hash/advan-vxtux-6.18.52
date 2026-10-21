// set_dpu_work_freq.340 @ 0010e1c0
// signature: undefined set_dpu_work_freq.340(void)
// calling convention: unknown


void set_dpu_work_freq_340(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_001015dc == param_2) {
    uVar1 = 0;
  }
  else if (DAT_001015f4 == param_2) {
    uVar1 = 1;
  }
  else if (DAT_0010160c == param_2) {
    uVar1 = 2;
  }
  else if (DAT_00101624 == param_2) {
    uVar1 = 3;
  }
  else if (DAT_0010163c == param_2) {
    uVar1 = 4;
  }
  else {
    if (DAT_00101654 != param_2) {
      return;
    }
    uVar1 = 5;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124) = uVar1;
  return;
}


