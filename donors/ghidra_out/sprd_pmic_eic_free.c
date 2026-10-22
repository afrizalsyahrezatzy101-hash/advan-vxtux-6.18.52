// sprd_pmic_eic_free @ 00101508
// signature: undefined sprd_pmic_eic_free(void)
// calling convention: unknown


void sprd_pmic_eic_free(undefined8 param_1,uint param_2)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = gpiochip_get_data();
  regmap_update_bits_base
            (*(undefined8 *)(lVar1 + 0x310),*(int *)(lVar1 + 0x318) + 4,1 << (ulong)(param_2 & 0xf),
             0,0,0,0);
  return;
}


