// signature: undefined sc27xx_adc_disable(void)

void sc27xx_adc_disable(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  regmap_update_bits_base
            (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(*(long *)(param_1 + 0xb0) + 8),0x60,0,0
             ,0,0);
  regmap_update_bits_base
            (*(undefined8 *)(param_1 + 0x18),*(undefined4 *)(*(long *)(param_1 + 0xb0) + 4),0x20,0,0
             ,0,0);
  return;
}



