====================mmsys_dvfs_init@001094ac====================
// signature: undefined mmsys_dvfs_init(void)

int mmsys_dvfs_init(void)

{
  int iVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = register_ip_dvfs_ops(&mmsys_dvfs_ops);
  if (iVar1 == 0) {
    iVar1 = sprd_mm_pw_notify_register(&dvfs_power_notifier);
    if (iVar1 == 0) {
      iVar1 = devfreq_add_governor(&mmsys_dvfs_gov);
      if (iVar1 == 0) {
        iVar1 = __platform_driver_register(&mmsys_dvfs_driver,&__this_module);
        if (iVar1 == 0) {
          return 0;
        }
        devfreq_remove_governor(&mmsys_dvfs_gov);
        return iVar1;
      }
      puVar2 = &DAT_00106638;
    }
    else {
      puVar2 = &DAT_001065ed;
    }
  }
  else {
    puVar2 = &DAT_0010645e;
  }
  printk(puVar2,"mmsys_dvfs_init",iVar1);
  return iVar1;
}


