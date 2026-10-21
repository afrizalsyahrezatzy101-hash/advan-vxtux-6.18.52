====================isp_dvfs_exit@0010c06c====================
// signature: undefined isp_dvfs_exit(void)

void isp_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&isp_dvfs_driver);
  iVar1 = devfreq_remove_governor(&isp_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"isp_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&isp_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"isp_dvfs_exit",iVar1);
  }
  return;
}


