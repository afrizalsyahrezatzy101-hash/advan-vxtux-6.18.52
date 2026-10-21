// dpu_gov_event_handler @ 00107a70
// signature: undefined dpu_gov_event_handler(void)
// calling convention: unknown


undefined8 dpu_gov_event_handler(long param_1,int param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_2 == 2) {
    if (*(long *)(param_1 + 0x60) != 0) {
      sysfs_remove_group(param_1 + 0x30,&dev_attr_group);
    }
  }
  else if (param_2 == 1) {
    uVar1 = sysfs_create_group(param_1 + 0x30,&dev_attr_group);
    return uVar1;
  }
  return 0;
}


