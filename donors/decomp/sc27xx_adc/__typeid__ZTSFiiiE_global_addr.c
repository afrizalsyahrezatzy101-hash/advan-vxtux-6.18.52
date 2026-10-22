// signature: undefined __typeid__ZTSFiiiE_global_addr(void)

undefined4 __typeid__ZTSFiiiE_global_addr(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  
  uVar1 = 0x10001;
  switch(param_1) {
  case 0xb:
    goto switchD_00103744_caseD_b;
  default:
    if (param_2 - 1U < 3) {
      uVar1 = *(undefined4 *)(&DAT_00102068 + (long)(int)(param_2 - 1U) * 4);
    }
switchD_00103744_caseD_b:
    return uVar1;
  case 0xe:
    uVar1 = 0x440384;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  case 0xf:
    uVar1 = 0x10003;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  case 0x15:
  case 0x16:
  case 0x17:
    uVar1 = 0x30008;
    if (param_2 != 0) {
      uVar1 = 0x10001;
    }
    return uVar1;
  }
}



