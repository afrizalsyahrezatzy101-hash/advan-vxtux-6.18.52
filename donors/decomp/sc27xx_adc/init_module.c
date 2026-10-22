// signature: undefined init_module(void)

void init_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  __platform_driver_register(&sc27xx_adc_driver,&__this_module);
  return;
}



