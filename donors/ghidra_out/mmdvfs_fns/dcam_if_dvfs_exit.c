====================dcam_if_dvfs_exit@0010bc7c====================
// signature: undefined dcam_if_dvfs_exit(void)

void dcam_if_dvfs_exit(void)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  platform_driver_unregister(&dcam_if_dvfs_driver);
  iVar1 = devfreq_remove_governor(&dcam_if_dvfs_gov);
  if (iVar1 != 0) {
    printk(&DAT_00104cf7,"dcam_if_dvfs_exit",iVar1);
  }
  iVar1 = unregister_ip_dvfs_ops(&dcam_if_dvfs_ops);
  if (iVar1 != 0) {
    printk(&DAT_00104c76,"dcam_if_dvfs_exit",iVar1);
  }
  return;
}


