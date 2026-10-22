====================get_ip_dvfs_ops@0010c358====================
// signature: undefined get_ip_dvfs_ops(void)

undefined ** get_ip_dvfs_ops(char *param_1)

{
  int iVar1;
  undefined8 *in_x18;
  undefined **ppuVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00105e1e,"get_ip_dvfs_ops");
  if ((param_1 == (char *)0x0) || ((char *)0xfffffffffffff000 < param_1)) {
    printk(&DAT_0010638d,"get_ip_dvfs_ops");
    ppuVar2 = (undefined **)0xffffffffffffffea;
  }
  else {
    mutex_lock(&ip_dvfsops_list_lock);
    for (ppuVar2 = (undefined **)ip_dvfs_ops_list; ppuVar2 != &ip_dvfs_ops_list;
        ppuVar2 = (undefined **)*ppuVar2) {
      iVar1 = strncmp((char *)(ppuVar2 + 2),param_1,0x10);
      if (iVar1 == 0) {
        printk(&DAT_00104979,"get_ip_dvfs_ops");
        mutex_unlock(&ip_dvfsops_list_lock);
        return ppuVar2;
      }
    }
    mutex_unlock(&ip_dvfsops_list_lock);
    ppuVar2 = (undefined **)0xffffffffffffffed;
  }
  return ppuVar2;
}


