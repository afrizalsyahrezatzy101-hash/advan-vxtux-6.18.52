// vsp_dvfs_gov_event_handler @ 00108f8c
// signature: undefined vsp_dvfs_gov_event_handler(void)
// calling convention: unknown


undefined8 vsp_dvfs_gov_event_handler(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001031ec);
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group_77);
    }
  }
  else if (param_2 == 1) {
    uVar1 = sysfs_create_group(param_1 + 0x30,&dev_attr_group_77);
    return uVar1;
  }
  return 0;
}


