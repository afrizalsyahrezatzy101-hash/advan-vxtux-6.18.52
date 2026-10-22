// signature: undefined sc27xx_adc_pm_suspend(void)

undefined8 sc27xx_adc_pm_suspend(long param_1)

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
                       *(undefined4 *)(lVar2 + 0x684),0,0,0,0);
    if (iVar1 == 0) {
      *(undefined1 *)(lVar2 + 0x689) = 1;
    }
    else {
      _dev_err(*(undefined8 *)(lVar2 + 0x5c8),"clean clk26m_sinout_pmic failed\n");
    }
    mutex_unlock(*(long *)(lVar2 + 0x5c0) + 1000);
  }
  return 0;
}



