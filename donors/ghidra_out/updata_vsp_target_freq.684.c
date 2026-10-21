// updata_vsp_target_freq.684 @ 00112aa8
// signature: undefined updata_vsp_target_freq.684(void)
// calling convention: unknown


void updata_vsp_target_freq_684(long param_1,int param_2,int param_3)

{
  long lVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(*(long *)(param_1 + 0x4a8) + 0x3d8);
  if (param_3 == 0) {
    uVar2 = 2;
    if (param_2 != 384000000) {
      uVar2 = (uint)(param_2 == 0x124f8000);
    }
    *(uint *)(lVar1 + 0x11c) = uVar2;
  }
  else {
    uVar2 = 2;
    if (param_2 != 384000000) {
      uVar2 = (uint)(param_2 == 0x124f8000);
    }
    *(uint *)(lVar1 + 0x120) = uVar2;
  }
  return;
}


