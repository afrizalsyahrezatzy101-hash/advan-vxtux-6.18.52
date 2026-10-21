// signature: undefined __cfi_check_fail(void)

void __cfi_check_fail(byte *param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((param_1 != (byte *)0x0) && (4 < *param_1)) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(2);
}



