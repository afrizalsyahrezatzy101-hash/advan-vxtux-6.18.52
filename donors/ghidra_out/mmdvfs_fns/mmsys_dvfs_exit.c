====================mmsys_dvfs_exit@00109a7c====================
// signature: undefined mmsys_dvfs_exit(void)

void mmsys_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&mmsys_dvfs_driver);
  iVar1 = devfreq_remove_governor(&mmsys_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"mmsys_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&mmsys_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"mmsys_dvfs_exit",iVar1);
  }
  iVar1 = sprd_mm_pw_notify_unregister(&dvfs_power_notifier);
  if (iVar1 != 0) {
    printk(&DAT_001060cb,"mmsys_dvfs_exit",iVar1);
  }
  return;
}


