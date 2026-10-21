====================fd_dvfs_exit@0010a288====================
// signature: undefined fd_dvfs_exit(void)

void fd_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&fd_dvfs_driver);
  iVar1 = devfreq_remove_governor(&fd_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"fd_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&fd_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"fd_dvfs_exit",iVar1);
  }
  return;
}


