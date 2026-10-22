====================get_dvfs_enable_show@0010cd8c====================
// signature: undefined get_dvfs_enable_show(void)

long get_dvfs_enable_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  long *plVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = *(long **)(*(long *)(param_1 + 0x60) + 0x98);
  if (*plVar4 == 0) {
    printk(&DAT_00105763);
    lVar3 = -0x16;
  }
  else {
    mutex_lock(param_1 + -0x20);
    uVar1 = s_undefined_00104b17._0_8_;
    if (plVar4 == (long *)0x0) {
      param_3[7] = 'e';
      param_3[8] = 'd';
      param_3[9] = '\n';
      param_3[10] = '\0';
      *(undefined8 *)param_3 = uVar1;
      lVar3 = 10;
    }
    else {
      iVar2 = sprintf(param_3,"%d\n",(ulong)*(uint *)((long)plVar4 + 0x6c));
      lVar3 = (long)iVar2;
    }
    mutex_unlock(param_1 + -0x20);
  }
  return lVar3;
}


