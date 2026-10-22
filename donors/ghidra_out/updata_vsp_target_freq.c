// updata_vsp_target_freq @ 00110900
// signature: undefined updata_vsp_target_freq(void)
// calling convention: unknown


void updata_vsp_target_freq(long param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  if (param_3 == 0) {
    if (param_2 == 512000000) {
      uVar1 = 3;
    }
    else if (param_2 == 384000000) {
      uVar1 = 2;
    }
    else if (param_2 == 0x124f8000) {
      uVar1 = 1;
    }
    else {
      uVar1 = 0;
    }
    *(undefined4 *)(lVar2 + 0x11c) = uVar1;
  }
  else {
    if (param_2 == 512000000) {
      uVar1 = 3;
    }
    else if (param_2 == 384000000) {
      uVar1 = 2;
    }
    else if (param_2 == 0x124f8000) {
      uVar1 = 1;
    }
    else {
      uVar1 = 0;
    }
    *(undefined4 *)(lVar2 + 0x120) = uVar1;
  }
  return;
}


