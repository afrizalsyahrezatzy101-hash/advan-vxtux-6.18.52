// get_hw_dfs_show.188 @ 0010ac88
// signature: undefined get_hw_dfs_show.188(void)
// calling convention: unknown


long get_hw_dfs_show_188(long param_1,undefined8 param_2,char *param_3)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = sprintf(param_3,"%d\n",
                  (ulong)*(uint *)(*(long *)(*(long *)(param_1 + 0x60) + 0x98) + 0x400));
  return (long)iVar1;
}


