// trusty_console_show @ 001014cc
// signature: undefined trusty_console_show(void)
// calling convention: unknown


long trusty_console_show(long param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = trusty_std_call32(*(undefined8 *)(*(long *)(param_1 + 0x98) + 8),0x35000001,0,0,0);
  if (iVar1 == -1) {
    printk(&DAT_00100467);
    lVar2 = -0xe;
  }
  else {
    iVar1 = scnprintf(param_3,0x1000,&DAT_001003a7,iVar1);
    lVar2 = (long)iVar1;
  }
  return lVar2;
}


