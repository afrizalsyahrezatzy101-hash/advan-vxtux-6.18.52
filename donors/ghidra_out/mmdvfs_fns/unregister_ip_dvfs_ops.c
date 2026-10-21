====================unregister_ip_dvfs_ops@0010c438====================
// signature: undefined unregister_ip_dvfs_ops(void)

ulong unregister_ip_dvfs_ops(long *param_1)

{
  long lVar1;
  long *plVar2;
  undefined *puVar3;
  undefined8 *in_x18;
  ulong uVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_1 == (long *)0x0) {
    uVar4 = 0xffffffea;
    puVar3 = &DAT_00105966;
  }
  else {
    printk(&DAT_0010488c,"unregister_ip_dvfs_ops");
    uVar4 = get_ip_dvfs_ops(param_1 + 2);
    if (uVar4 < 0xfffffffffffff001) {
      mutex_lock(&ip_dvfsops_list_lock);
      uVar4 = __list_del_entry_valid(param_1);
      if ((uVar4 & 1) != 0) {
        lVar1 = *param_1;
        plVar2 = (long *)param_1[1];
        *(long **)(lVar1 + 8) = plVar2;
        *plVar2 = lVar1;
      }
      *param_1 = -0x2152ffffffffff00;
      param_1[1] = -0x2152fffffffffede;
      mutex_unlock(&ip_dvfsops_list_lock);
      uVar4 = 0;
    }
    else {
      printk(&DAT_00104ac5,"unregister_ip_dvfs_ops",param_1 + 2);
    }
    puVar3 = &DAT_001051bb;
  }
  printk(puVar3,"unregister_ip_dvfs_ops");
  return uVar4 & 0xffffffff;
}


