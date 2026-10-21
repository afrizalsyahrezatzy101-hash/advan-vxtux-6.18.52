// sprd_pmic_eic_irq_set_type @ 00101758
// signature: undefined sprd_pmic_eic_irq_set_type(void)
// calling convention: unknown


undefined8 sprd_pmic_eic_irq_set_type(long param_1,int param_2)

{
  long lVar1;
  ulong uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = gpiochip_get_data(*(undefined8 *)(param_1 + 0x30));
  if (2 < param_2 - 1U) {
    uVar2 = (ulong)*(uint *)(param_1 + 8);
    if (param_2 == 8) {
      if (0xf < uVar2) goto LAB_001017e4;
      *(undefined1 *)(lVar1 + uVar2 * 3 + 0x31c) = 0;
    }
    else {
      if (param_2 != 4) {
        return 0xfffffdf4;
      }
      if (0xf < uVar2) {
LAB_001017e4:
                    /* WARNING: Subroutine does not return */
        __ubsan_handle_cfi_check_fail_abort(0x12);
      }
      *(undefined1 *)(lVar1 + uVar2 * 3 + 0x31c) = 1;
    }
  }
  return 0;
}


