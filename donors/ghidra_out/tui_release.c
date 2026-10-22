// tui_release @ 001013b0
// signature: undefined tui_release(void)
// calling convention: unknown


undefined8 tui_release(undefined8 param_1,long param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(long *)(param_2 + 200) != 0) {
    kfree();
  }
  s_ts = 0;
  return 0;
}


