// signature: undefined sc27xx_adc_pm_resume(void)

undefined8 sc27xx_adc_pm_resume(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0x98);
  if (*(char *)(lVar2 + 0x688) != '\0') {
    mutex_lock(*(long *)(lVar2 + 0x5c0) + 1000);
    iVar1 = regmap_update_bits_base
                      (*(undefined8 *)(lVar2 + 0x678),*(undefined4 *)(lVar2 + 0x680),
                       *(undefined4 *)(lVar2 + 0x684),*(undefined4 *)(lVar2 + 0x684),0,0,0);
    if (iVar1 == 0) {
      *(undefined1 *)(lVar2 + 0x689) = 0;
    }
    else {
      _dev_err(param_1,"failed to set the UMP9620 ADC clk26m bit8 on IP\n");
    }
    mutex_unlock(*(long *)(lVar2 + 0x5c0) + 1000);
  }
  return 0;
}



