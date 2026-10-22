// vdsp_dvfs_get_cur_freq @ 0010b4a8
// signature: undefined vdsp_dvfs_get_cur_freq(void)
// calling convention: unknown


undefined8 vdsp_dvfs_get_cur_freq(long param_1,ulong *param_2)

{
  long lVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0x98);
  printk(&DAT_00101afd);
  lVar1 = 0x3e0;
  if (*(int *)(lVar2 + 1000) != 0) {
    lVar1 = 0x3e4;
  }
  *param_2 = (ulong)*(uint *)(lVar2 + lVar1);
  return 0;
}


