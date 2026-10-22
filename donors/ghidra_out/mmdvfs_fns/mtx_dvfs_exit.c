====================mtx_dvfs_exit@001083a4====================
// signature: undefined mtx_dvfs_exit(void)

void mtx_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&mtx_dvfs_driver);
  iVar1 = devfreq_remove_governor(&mtx_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"mtx_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&mtx_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"mtx_dvfs_exit",iVar1);
  }
  return;
}


