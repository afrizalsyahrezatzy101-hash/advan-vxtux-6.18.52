// sprd_mbox_startup @ 00102e14
// signature: undefined sprd_mbox_startup(void)
// calling convention: unknown


undefined8 sprd_mbox_startup(long *param_1)

{
  code *pcVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *param_1;
  _dev_info(*(undefined8 *)(lVar2 + 0x88),"startup chan-%ld\n",param_1[0x1e]);
  pcVar1 = (code *)**(undefined8 **)(lVar2 + 0xe0);
  if (1 < ((ulong)(pcVar1 + -0x104364) >> 2 | (long)pcVar1 << 0x3e)) {
    __cfi_slowpath(0xed5c207f8272b5da,pcVar1);
  }
  (*pcVar1)(param_1);
  return 0;
}


