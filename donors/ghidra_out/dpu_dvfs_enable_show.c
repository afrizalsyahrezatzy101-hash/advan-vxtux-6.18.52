// dpu_dvfs_enable_show @ 00108664
// signature: undefined dpu_dvfs_enable_show(void)
// calling convention: unknown


long dpu_dvfs_enable_show(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = sprintf(param_3,"%d\n",(ulong)**(uint **)(*(long *)(param_1 + 0x60) + 0x98));
  return (long)iVar1;
}


