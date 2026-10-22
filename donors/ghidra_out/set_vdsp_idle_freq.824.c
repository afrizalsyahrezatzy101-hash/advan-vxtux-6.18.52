// set_vdsp_idle_freq.824 @ 001157b8
// signature: undefined set_vdsp_idle_freq.824(void)
// calling convention: unknown


void set_vdsp_idle_freq_824(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_001016e4 == param_2) {
    uVar1 = 0;
  }
  else if (DAT_001016fc == param_2) {
    uVar1 = 1;
  }
  else if (DAT_00101714 == param_2) {
    uVar1 = 2;
  }
  else if (DAT_0010172c == param_2) {
    uVar1 = 3;
  }
  else if (DAT_00101744 == param_2) {
    uVar1 = 4;
  }
  else {
    if (DAT_0010175c != param_2) {
      return;
    }
    uVar1 = 5;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x118) = uVar1;
  return;
}


