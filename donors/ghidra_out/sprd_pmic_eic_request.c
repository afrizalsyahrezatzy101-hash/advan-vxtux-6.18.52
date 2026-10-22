// sprd_pmic_eic_request @ 001014b0
// signature: undefined sprd_pmic_eic_request(void)
// calling convention: unknown


undefined8 sprd_pmic_eic_request(undefined8 param_1,uint param_2)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = gpiochip_get_data();
  iVar1 = 1 << (ulong)(param_2 & 0xf);
  regmap_update_bits_base
            (*(undefined8 *)(lVar2 + 0x310),*(int *)(lVar2 + 0x318) + 4,iVar1,iVar1,0,0,0);
  return 0;
}


