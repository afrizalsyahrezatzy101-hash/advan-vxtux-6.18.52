// mbox_pm_event @ 00103510
// signature: undefined mbox_pm_event(void)
// calling convention: unknown


undefined8 mbox_pm_event(undefined8 param_1,long param_2)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (g_mbox_nowakeup_base != 0) {
    if (param_2 == 4) {
      *(undefined4 *)(g_mbox_nowakeup_base + 0x1c) = g_irq_msk_backup;
    }
    else if (param_2 == 3) {
      g_irq_msk_backup = *(undefined4 *)(g_mbox_nowakeup_base + 0x1c);
      DataMemoryBarrier(0,1);
      *(undefined4 *)(g_mbox_nowakeup_base + 0x1c) = 0xffffffff;
    }
  }
  return 0;
}


