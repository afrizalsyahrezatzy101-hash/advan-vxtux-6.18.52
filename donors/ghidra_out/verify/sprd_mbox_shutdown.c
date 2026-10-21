// sprd_mbox_shutdown @ 00102ea8
// signature: undefined sprd_mbox_shutdown(void)
// calling convention: unknown


void sprd_mbox_shutdown(long *param_1)

{
  code *pcVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *param_1;
  _dev_info(*(undefined8 *)(lVar2 + 0x88),"shutdown chan-%ld\n",param_1[0x1e]);
  pcVar1 = *(code **)(*(long *)(lVar2 + 0xe0) + 8);
  if (1 < ((ulong)(pcVar1 + -0x10436c) >> 2 | (long)pcVar1 << 0x3e)) {
    __cfi_slowpath(0xf769d37fdf578bba,pcVar1);
  }
  (*pcVar1)(param_1);
  return;
}


