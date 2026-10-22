// cleanup_module @ 00102338
// signature: undefined cleanup_module(void)
// calling convention: unknown


void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&trusty_tui_driver);
  return;
}


