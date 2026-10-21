// mmc_hsq_resume @ 001011d4
// signature: undefined mmc_hsq_resume(void)
// calling convention: unknown


undefined4 mmc_hsq_resume(long param_1)

{
  undefined8 *in_x18;
  undefined4 uVar1;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0x658);
  _raw_spin_lock_irq(lVar2 + 0x30);
  if (*(char *)(lVar2 + 0x178) == '\0') {
    uVar1 = 0;
    *(undefined1 *)(lVar2 + 0x178) = 1;
  }
  else {
    uVar1 = 0xfffffff0;
  }
  _raw_spin_unlock_irq(lVar2 + 0x30);
  return uVar1;
}


