// get_dvfs_enable_show @ 00109a6c
// signature: undefined get_dvfs_enable_show(void)
// calling convention: unknown


long get_dvfs_enable_show(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  ulong uVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  uVar2 = get_display_power_status();
  if ((uVar2 & 1) == 0) {
    printk(&DAT_0010291a);
    lVar3 = -0x16;
  }
  else {
    iVar1 = sprintf(param_3,"%d\n",(ulong)*(uint *)(lVar3 + 0x474));
    lVar3 = (long)iVar1;
  }
  return lVar3;
}


