// signature: undefined aud_smsg_wakeup_all_ch(void)

undefined8 aud_smsg_wakeup_all_ch(long param_1)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = 0;
  do {
    if (*(char *)(param_1 + 0x298 + lVar1 + 0x110) == '\x02') {
      __wake_up(*(undefined8 *)(param_1 + 0x298 + lVar1 * 8),1,0,0);
    }
    lVar1 = lVar1 + 1;
  } while (lVar1 != 0x11);
  return 0;
}



