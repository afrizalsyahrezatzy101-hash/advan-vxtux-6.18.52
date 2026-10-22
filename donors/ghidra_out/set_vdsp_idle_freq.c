// set_vdsp_idle_freq @ 00114c7c
// signature: undefined set_vdsp_idle_freq(void)
// calling convention: unknown


void set_vdsp_idle_freq(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_0010166c == param_2) {
    uVar1 = 0;
  }
  else if (DAT_00101680 == param_2) {
    uVar1 = 1;
  }
  else if (DAT_00101694 == param_2) {
    uVar1 = 2;
  }
  else if (DAT_001016a8 == param_2) {
    uVar1 = 3;
  }
  else if (DAT_001016bc == param_2) {
    uVar1 = 4;
  }
  else {
    if (DAT_001016d0 != param_2) {
      return;
    }
    uVar1 = 5;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x118) = uVar1;
  return;
}


