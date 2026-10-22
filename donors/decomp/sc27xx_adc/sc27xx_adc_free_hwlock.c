// signature: undefined sc27xx_adc_free_hwlock(void)

void sc27xx_adc_free_hwlock(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  hwspin_lock_free();
  return;
}



