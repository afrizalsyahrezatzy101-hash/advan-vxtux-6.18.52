// trusty_logsize_show @ 00101770
// signature: undefined trusty_logsize_show(void)
// calling convention: unknown


long trusty_logsize_show(long param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = scnprintf(param_3,0x1000,"0x%x\n",*(int *)(*(long *)(param_1 + 0x98) + 0x38) << 0xc);
  return (long)iVar1;
}


