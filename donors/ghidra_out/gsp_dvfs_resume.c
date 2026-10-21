// gsp_dvfs_resume @ 00116ecc
// signature: undefined gsp_dvfs_resume(void)
// calling convention: unknown


undefined8 gsp_dvfs_resume(long param_1)

{
  long lVar1;
  code *pcVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(param_1 + 0x98);
  printk(&DAT_001034f0,"gsp_dvfs_resume");
  lVar1 = *(long *)(lVar3 + 0x480);
  if ((lVar1 != 0) && (pcVar2 = *(code **)(lVar1 + 8), pcVar2 != (code *)0x0)) {
    if (2 < ((ulong)(pcVar2 + -0x1181b8) >> 2 | (long)pcVar2 << 0x3e)) {
      __cfi_slowpath(0x56267983479e1e0a,pcVar2);
    }
    (*pcVar2)(lVar3);
  }
  return 0;
}


