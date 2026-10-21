// set_vsp_idle_freq.717 @ 0011310c
// signature: undefined set_vsp_idle_freq.717(void)
// calling convention: unknown


void set_vsp_idle_freq_717(long param_1,int param_2)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = 0;
  if (param_2 < 512000000) {
    if (param_2 == 0x124f8000) {
      uVar1 = 1;
    }
    else if (param_2 == 384000000) {
      uVar1 = 2;
    }
  }
  else if (param_2 == 512000000) {
    uVar1 = 3;
  }
  else if (param_2 == 0x27d4af90) {
    uVar1 = 4;
  }
  *(undefined4 *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x7a8) = uVar1;
  return;
}


