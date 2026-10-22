// locked_nop_work_func @ 00102348
// signature: undefined locked_nop_work_func(void)
// calling convention: unknown


void locked_nop_work_func(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + -8);
  iVar1 = trusty_std_call32(*(undefined8 *)(lVar2 + 0x60),0x3c000001,0,0,0);
  if (iVar1 != 0) {
    _dev_err(*(undefined8 *)(lVar2 + 0x60),"%s: SMC_SC_LOCKED_NOP failed %d","locked_nop_work_func",
             iVar1);
  }
  return;
}


