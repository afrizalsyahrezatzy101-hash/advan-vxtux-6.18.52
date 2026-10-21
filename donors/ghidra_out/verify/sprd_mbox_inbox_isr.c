// sprd_mbox_inbox_isr @ 001026ec
// signature: undefined sprd_mbox_inbox_isr(void)
// calling convention: unknown


void sprd_mbox_inbox_isr(ulong param_1,long param_2)

{
  code *pcVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  pcVar1 = *(code **)(*(long *)(param_2 + 0xe0) + 0x20);
  if (3 < ((ulong)(pcVar1 + -0x104384) >> 2 | (long)pcVar1 << 0x3e)) {
    __cfi_slowpath(0xc6783f1d911d68d7,pcVar1);
    param_1 = param_1 & 0xffffffff;
  }
  (*pcVar1)(param_1,param_2);
  return;
}


