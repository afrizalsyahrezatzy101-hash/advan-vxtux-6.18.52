// set_gsp_work_freq @ 001176a4
// signature: undefined set_gsp_work_freq(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void set_gsp_work_freq(long param_1,int param_2)

{
  undefined4 uVar1;
  
  if (_DAT_00101a5c == param_2) {
    uVar1 = 0;
  }
  else if (DAT_00101a74 == param_2) {
    uVar1 = 1;
  }
  else if (DAT_00101a8c == param_2) {
    uVar1 = 2;
  }
  else {
    if (DAT_00101aa4 != param_2) {
      return;
    }
    uVar1 = 3;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x488) + 0x3d8) + 0x7ac) = uVar1;
  return;
}


