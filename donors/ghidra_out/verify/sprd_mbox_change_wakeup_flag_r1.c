// sprd_mbox_change_wakeup_flag_r1 @ 001033a4
// signature: undefined sprd_mbox_change_wakeup_flag_r1(void)
// calling convention: unknown


void sprd_mbox_change_wakeup_flag_r1(uint param_1)

{
  code *pcVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  pcVar1 = mbox_wakeup_flag_callback_r1;
  if (mbox_wakeup_flag_callback_r1 == (code *)0x0) {
    printk(&DAT_00100953);
  }
  else {
    if (mbox_wakeup_flag_callback_r1 != __typeid__ZTSFvbE_global_addr) {
      __cfi_slowpath(0x301714697a011648,mbox_wakeup_flag_callback_r1);
    }
    (*pcVar1)(param_1 & 1);
  }
  return;
}


