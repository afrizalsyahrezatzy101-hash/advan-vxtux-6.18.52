// mbox_pm_notifier @ 00102ad8
// signature: undefined mbox_pm_notifier(void)
// calling convention: unknown


undefined8 mbox_pm_notifier(undefined8 param_1,long param_2)

{
  ulong uVar1;
  code *pcVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_2 == 5) {
    pcVar2 = *(code **)(mbox_phy_ops + 0x30);
    if (pcVar2 == __typeid__ZTSFbvE_global_addr) {
      uVar1 = __typeid__ZTSFbvE_global_addr();
      pcVar2 = mbox_wakeup_flag_callback_r1;
    }
    else {
      __cfi_slowpath(0x741b966c574d47b1,pcVar2);
      uVar1 = (*pcVar2)();
      pcVar2 = mbox_wakeup_flag_callback_r1;
    }
    mbox_wakeup_flag_callback_r1 = pcVar2;
    if ((uVar1 & 1) != 0) {
      if (pcVar2 == (code *)0x0) {
        printk(&DAT_00100953);
      }
      else {
        if (pcVar2 != __typeid__ZTSFvbE_global_addr) {
          __cfi_slowpath(0x301714697a011648,pcVar2);
        }
        (*pcVar2)(1);
      }
    }
  }
  return 1;
}


