// sprd_mbox_flush @ 00102d7c
// signature: undefined sprd_mbox_flush(void)
// calling convention: unknown


void sprd_mbox_flush(long *param_1,undefined8 param_2)

{
  code *pcVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *param_1;
  _dev_info(*(undefined8 *)(lVar2 + 0x88),"flush chan%ld\n",param_1[0x1e]);
  pcVar1 = *(code **)(*(long *)(lVar2 + 0xe0) + 0x10);
  if (1 < ((ulong)(pcVar1 + -0x104374) >> 2 | (long)pcVar1 << 0x3e)) {
    __cfi_slowpath(0xb7f6835fab7e2fce,pcVar1);
  }
  (*pcVar1)(param_1,param_2);
  return;
}


