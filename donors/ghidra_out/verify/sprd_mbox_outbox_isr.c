// sprd_mbox_outbox_isr @ 0010275c
// signature: undefined sprd_mbox_outbox_isr(void)
// calling convention: unknown


undefined4 sprd_mbox_outbox_isr(undefined4 param_1,long param_2)

{
  undefined4 uVar1;
  code *pcVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  _raw_spin_lock(&outbox_lock);
  pcVar2 = *(code **)(*(long *)(param_2 + 0xe0) + 0x28);
  if (3 < ((ulong)(pcVar2 + -0x104384) >> 2 | (long)pcVar2 << 0x3e)) {
    __cfi_slowpath(0xc6783f1d911d68d7,pcVar2);
  }
  uVar1 = (*pcVar2)(param_1,param_2);
  _raw_spin_unlock(&outbox_lock);
  return uVar1;
}


