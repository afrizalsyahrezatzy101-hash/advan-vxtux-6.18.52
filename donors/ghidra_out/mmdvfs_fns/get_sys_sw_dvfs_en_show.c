====================get_sys_sw_dvfs_en_show@0010b1b8====================
// signature: undefined get_sys_sw_dvfs_en_show(void)

long get_sys_sw_dvfs_en_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar1 = s_undefined_00104b17._0_8_;
  if (*(int *)(lVar3 + 0x28) == 0) {
    iVar2 = sprintf(param_3,"%d\n",0);
    lVar3 = (long)iVar2;
  }
  else {
    param_3[7] = 'e';
    param_3[8] = 'd';
    param_3[9] = '\n';
    param_3[10] = '\0';
    *(undefined8 *)param_3 = uVar1;
    lVar3 = 10;
  }
  mutex_unlock(param_1 + -0x20);
  return lVar3;
}


