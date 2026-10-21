// set_dpu_idle_freq @ 0010baac
// signature: undefined set_dpu_idle_freq(void)
// calling convention: unknown


void set_dpu_idle_freq(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (DAT_001013b4 == param_2) {
    uVar1 = 0;
  }
  else if (DAT_001013cc == param_2) {
    uVar1 = 1;
  }
  else if (DAT_001013e4 == param_2) {
    uVar1 = 2;
  }
  else if (DAT_001013fc == param_2) {
    uVar1 = 3;
  }
  else if (DAT_00101414 == param_2) {
    uVar1 = 4;
  }
  else if (DAT_0010142c == param_2) {
    uVar1 = 5;
  }
  else if (DAT_00101444 == param_2) {
    uVar1 = 6;
  }
  else {
    if (DAT_0010145c != param_2) {
      return;
    }
    uVar1 = 7;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7c0) = uVar1;
  return;
}


