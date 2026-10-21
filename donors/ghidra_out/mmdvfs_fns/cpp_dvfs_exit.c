====================cpp_dvfs_exit@00109e98====================
// signature: undefined cpp_dvfs_exit(void)

void cpp_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&cpp_dvfs_driver);
  iVar1 = devfreq_remove_governor(&cpp_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"cpp_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&cpp_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"cpp_dvfs_exit",iVar1);
  }
  return;
}


