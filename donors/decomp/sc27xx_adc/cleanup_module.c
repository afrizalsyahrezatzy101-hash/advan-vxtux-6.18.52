// signature: undefined cleanup_module(void)

void cleanup_module(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&sc27xx_adc_driver);
  return;
}



