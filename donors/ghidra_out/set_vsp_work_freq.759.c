// set_vsp_work_freq.759 @ 00113b74
// signature: undefined set_vsp_work_freq.759(void)
// calling convention: unknown


void set_vsp_work_freq_759(long param_1,int param_2)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
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
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x79c) = uVar1;
  return;
}


