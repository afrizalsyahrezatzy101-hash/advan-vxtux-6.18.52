// trusty_tui_probe @ 00101090
// signature: undefined trusty_tui_probe(void)
// calling convention: unknown


int trusty_tui_probe(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = misc_register(&tui_dev);
  if (iVar1 != 0) {
    _dev_err(param_1 + 0x10,"can\'t register miscdev minor=%d (%d)\n",0xbb,iVar1);
  }
  return iVar1;
}


