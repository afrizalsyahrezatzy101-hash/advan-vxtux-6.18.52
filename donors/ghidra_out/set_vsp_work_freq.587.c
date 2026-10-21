// set_vsp_work_freq.587 @ 00110e58
// signature: undefined set_vsp_work_freq.587(void)
// calling convention: unknown


void set_vsp_work_freq_587(long param_1,int param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = 2;
  if (param_2 != 384000000) {
    uVar1 = (uint)(param_2 == 0x124f8000);
  }
  *(uint *)(*(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8) + 0x11c) = uVar1;
  return;
}


