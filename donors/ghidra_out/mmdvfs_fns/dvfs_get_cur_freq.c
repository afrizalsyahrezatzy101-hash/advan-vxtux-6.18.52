====================dvfs_get_cur_freq@0010cc90====================
// signature: undefined dvfs_get_cur_freq(void)

undefined8 dvfs_get_cur_freq(long param_1,long *param_2)

{
  undefined8 uVar1;
  long lVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (**(long **)(param_1 + 0x98) == 0) {
    printk(&DAT_00105763);
    uVar1 = 0xffffffea;
  }
  else {
    lVar2 = (*(long **)(param_1 + 0x98))[4];
    *param_2 = lVar2;
    printk(&DAT_00104ae7,lVar2);
    uVar1 = 0;
  }
  return uVar1;
}


