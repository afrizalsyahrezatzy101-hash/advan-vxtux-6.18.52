// sprd_pmic_eic_bus_lock @ 001017ec
// signature: undefined sprd_pmic_eic_bus_lock(void)
// calling convention: unknown


void sprd_pmic_eic_bus_lock(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = gpiochip_get_data(*(undefined8 *)(param_1 + 0x30));
  mutex_lock(lVar1 + 0x350);
  return;
}


