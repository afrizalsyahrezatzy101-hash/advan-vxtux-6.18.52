// trusty_tui_remove @ 00101110
// signature: undefined trusty_tui_remove(void)
// calling convention: unknown


undefined8 trusty_tui_remove(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  misc_deregister(&tui_dev);
  return 0;
}


