// set_dpu_work_freq.242 @ 0010c468
// signature: undefined set_dpu_work_freq.242(void)
// calling convention: unknown


void set_dpu_work_freq_242(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_00101470._4_4_ == param_2) {
    uVar1 = 0;
  }
  else if (DAT_0010148c == param_2) {
    uVar1 = 1;
  }
  else if (DAT_001014a4 == param_2) {
    uVar1 = 2;
  }
  else if (DAT_001014bc == param_2) {
    uVar1 = 3;
  }
  else {
    if (DAT_001014d4 != param_2) {
      return;
    }
    uVar1 = 4;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x124) = uVar1;
  return;
}


