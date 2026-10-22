// init_module @ 001021bc
// signature: undefined init_module(void)
// calling convention: unknown


void init_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  __platform_driver_register(&trusty_tui_driver,&__this_module);
  return;
}


