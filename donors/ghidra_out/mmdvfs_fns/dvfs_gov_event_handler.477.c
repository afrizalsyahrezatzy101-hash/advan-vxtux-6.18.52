====================dvfs_gov_event_handler.477@0010bfb8====================
// signature: undefined dvfs_gov_event_handler.477(void)

undefined8 dvfs_gov_event_handler_477(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  long lVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104dc3);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_480);
      sysfs_remove_group(param_1 + 0x30,&coeff_attr_group_481);
    }
  }
  else if (param_2 == 1) {
    lVar2 = *(long *)(*(long *)(param_1 + 0x90) + 0x98);
    uVar1 = get_ip_dvfs_ops("ISP_DVFS_OPS");
    *(undefined8 *)(lVar2 + 0x60) = uVar1;
    sysfs_create_group(param_1 + 0x30,&dev_attr_group_480);
    uVar1 = sysfs_create_group(param_1 + 0x30,&coeff_attr_group_481);
    return uVar1;
  }
  return 0;
}


