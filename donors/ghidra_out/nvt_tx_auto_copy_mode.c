// nvt_tx_auto_copy_mode @ 0012573c
// signature: undefined nvt_tx_auto_copy_mode(void)
// calling convention: unknown


void nvt_tx_auto_copy_mode(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  nvt_write_addr(*(undefined4 *)(*(long *)(ts + 0xf8) + 0x84),0x69);
  printk(&DAT_00123910,"NVT-ts","nvt_tx_auto_copy_mode",0x1d9);
  return;
}


