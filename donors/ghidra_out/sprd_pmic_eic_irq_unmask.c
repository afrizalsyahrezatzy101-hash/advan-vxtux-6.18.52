// sprd_pmic_eic_irq_unmask @ 00101704
// signature: undefined sprd_pmic_eic_irq_unmask(void)
// calling convention: unknown


void sprd_pmic_eic_irq_unmask(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = gpiochip_get_data(*(undefined8 *)(param_1 + 0x30));
  if ((ulong)*(uint *)(param_1 + 8) < 0x10) {
    *(undefined2 *)(lVar1 + (ulong)*(uint *)(param_1 + 8) * 3 + 0x31d) = 0x101;
    return;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}


