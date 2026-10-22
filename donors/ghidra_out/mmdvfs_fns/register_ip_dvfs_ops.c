====================register_ip_dvfs_ops@0010c270====================
// signature: undefined register_ip_dvfs_ops(void)

undefined4 register_ip_dvfs_ops(undefined8 *param_1)

{
  ulong uVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined4 uVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 == (undefined8 *)0x0) {
    uVar3 = 0xffffffea;
    puVar2 = &DAT_00105966;
  }
  else {
    printk(&DAT_00105ee7,"register_ip_dvfs_ops");
    uVar1 = get_ip_dvfs_ops(param_1 + 2);
    if (uVar1 < 0xfffffffffffff001) {
      printk(&DAT_00105df5,"register_ip_dvfs_ops",uVar1 + 0x10);
      uVar3 = 0xffffffea;
    }
    else {
      mutex_lock(&ip_dvfsops_list_lock);
      puVar2 = ip_dvfs_ops_list;
      uVar1 = __list_add_valid(param_1,&ip_dvfs_ops_list,ip_dvfs_ops_list);
      if ((uVar1 & 1) != 0) {
        *(undefined8 **)(puVar2 + 8) = param_1;
        *param_1 = puVar2;
        param_1[1] = &ip_dvfs_ops_list;
        ip_dvfs_ops_list = (undefined *)param_1;
      }
      mutex_unlock(&ip_dvfsops_list_lock);
      uVar3 = 0;
    }
    puVar2 = &DAT_00105144;
  }
  printk(puVar2,"register_ip_dvfs_ops");
  return uVar3;
}


