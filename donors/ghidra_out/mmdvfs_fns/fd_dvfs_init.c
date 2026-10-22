====================fd_dvfs_init@00109f14====================
// signature: undefined fd_dvfs_init(void)

int fd_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&fd_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = devfreq_add_governor(&fd_dvfs_gov);
    if (iVar1 == 0) {
      iVar1 = __platform_driver_register(&fd_dvfs_driver,&__this_module);
      if (iVar1 == 0) {
        return 0;
      }
      devfreq_remove_governor(&fd_dvfs_gov);
      return iVar1;
    }
    puVar2 = &DAT_00106638;
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"fd_dvfs_init",iVar1);
  return iVar1;
}


