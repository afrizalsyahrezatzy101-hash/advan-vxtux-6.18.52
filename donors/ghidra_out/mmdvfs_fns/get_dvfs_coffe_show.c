====================get_dvfs_coffe_show@0010db2c====================
// signature: undefined get_dvfs_coffe_show(void)

long get_dvfs_coffe_show(long param_1,undefined8 param_2,char *param_3)

{
  undefined8 uVar1;
  undefined8 uVar2;
  int iVar3;
  int iVar4;
  undefined8 *in_x18;
  long lVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  mutex_lock(param_1 + -0x20);
  uVar2 = s_IP_dvfs_coffe_show_0010624c._8_8_;
  uVar1 = s_IP_dvfs_coffe_show_0010624c._0_8_;
  param_3[0x10] = 'o';
  param_3[0x11] = 'w';
  param_3[0x12] = '\n';
  param_3[0x13] = '\0';
  *(undefined8 *)param_3 = uVar1;
  *(undefined8 *)(param_3 + 8) = uVar2;
  iVar3 = sprintf(param_3 + 0x13,"%d\n",(ulong)*(uint *)(lVar5 + 0x11c));
  iVar4 = sprintf(param_3 + (iVar3 + 0x13),"%d\n",(ulong)*(uint *)(lVar5 + 0x110));
  iVar4 = iVar4 + iVar3 + 0x13;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x114));
  iVar3 = iVar3 + iVar4;
  iVar4 = sprintf(param_3 + iVar3,"%d\n",(ulong)*(uint *)(lVar5 + 0x10c));
  iVar4 = iVar4 + iVar3;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x108));
  iVar3 = iVar3 + iVar4;
  iVar4 = sprintf(param_3 + iVar3,"%d\n",(ulong)*(uint *)(lVar5 + 0x124));
  iVar4 = iVar4 + iVar3;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x124));
  iVar3 = iVar3 + iVar4;
  iVar4 = sprintf(param_3 + iVar3,"%d\n",(ulong)*(uint *)(lVar5 + 0x120));
  iVar4 = iVar4 + iVar3;
  iVar3 = sprintf(param_3 + iVar4,"%d\n",(ulong)*(uint *)(lVar5 + 0x118));
  mutex_unlock(param_1 + -0x20);
  return (long)(iVar3 + iVar4);
}


