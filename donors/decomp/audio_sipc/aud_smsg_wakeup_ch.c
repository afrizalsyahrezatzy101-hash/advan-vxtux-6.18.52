// signature: undefined aud_smsg_wakeup_ch(void)

undefined8 aud_smsg_wakeup_ch(byte param_1,ulong param_2)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  ulong uVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 < 2) {
    lVar2 = (&aud_smsg_ipcs)[param_1];
    if (lVar2 == 0) {
      uVar1 = 0xffffffed;
    }
    else {
      uVar3 = 0;
      do {
        if (((param_2 & 0xff) == uVar3) && (*(char *)(lVar2 + 0x3a8 + uVar3) == '\x02')) {
          *(undefined1 *)(lVar2 + (param_2 & 0xff) + 0x3b9) = 1;
          __wake_up(*(undefined8 *)(lVar2 + (param_2 & 0xff) * 8 + 0x298),1,0,0);
        }
        uVar3 = uVar3 + 1;
      } while (uVar3 != 0x11);
      uVar1 = 0;
    }
    return uVar1;
  }
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



