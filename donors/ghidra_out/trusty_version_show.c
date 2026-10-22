// trusty_version_show @ 00102310
// signature: undefined trusty_version_show(void)
// calling convention: unknown


long trusty_version_show(long param_1,undefined8 param_2,undefined8 param_3)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = scnprintf(param_3,0x1000,&DAT_0010088a,*(undefined8 *)(*(long *)(param_1 + 0x98) + 0x50));
  return (long)iVar1;
}


