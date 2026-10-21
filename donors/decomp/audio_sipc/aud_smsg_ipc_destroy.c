// signature: undefined aud_smsg_ipc_destroy(void)

undefined8 aud_smsg_ipc_destroy(byte param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 2) {
    (&aud_smsg_ipcs)[param_1] = 0;
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



