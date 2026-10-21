// sdhci_sprd_irq_request_swcq @ 00107b78
// signature: undefined sdhci_sprd_irq_request_swcq(void)
// calling convention: unknown


undefined8 sdhci_sprd_irq_request_swcq(long param_1)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (((*(uint *)(*(long *)(param_1 + 0x40) + 0x3f4) ^ 0xffffffff) & 0x280000) == 0) {
    free_irq(*(undefined4 *)(param_1 + 0x10),param_1);
    lVar2 = *(long *)(*(long *)(param_1 + 0x40) + 0x78);
    if (lVar2 == 0) {
      lVar2 = *(long *)(*(long *)(param_1 + 0x40) + 8);
    }
    uVar1 = request_threaded_irq
                      (*(undefined4 *)(param_1 + 0x10),&LAB_0010e47c,&LAB_0010e480,0x80,lVar2,
                       param_1);
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}


