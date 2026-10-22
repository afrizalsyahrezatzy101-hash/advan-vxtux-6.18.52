// sprd_mbox_debug_open @ 00102aa8
// signature: undefined sprd_mbox_debug_open(void)
// calling convention: unknown


void sprd_mbox_debug_open(long param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  single_open(param_2,*(undefined8 *)(*(long *)(*(long *)(param_1 + 0x280) + 0xe0) + 0x38));
  return;
}


