// set_dpu_idle_freq.310 @ 0010d918
// signature: undefined set_dpu_idle_freq.310(void)
// calling convention: unknown


void set_dpu_idle_freq_310(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_00101560._4_4_ == param_2) {
    uVar1 = 0;
  }
  else if (DAT_0010157c == param_2) {
    uVar1 = 1;
  }
  else if (DAT_00101594 == param_2) {
    uVar1 = 2;
  }
  else if (DAT_001015ac == param_2) {
    uVar1 = 3;
  }
  else {
    if (DAT_001015c4 != param_2) {
      return;
    }
    uVar1 = 4;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x128) = uVar1;
  return;
}


