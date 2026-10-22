====================mmsys_dvfs_power@00109a18====================
// signature: undefined mmsys_dvfs_power(void)

undefined8 mmsys_dvfs_power(undefined8 param_1,long param_2)

{
  undefined4 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00104c1d,"mmsys_dvfs_power",param_2);
  uVar1 = 1;
  if (param_2 != 1) {
    uVar1 = 2;
  }
  mmsys_notify_call_chain(uVar1);
  printk(&DAT_0010616e,"mmsys_dvfs_power");
  return 0;
}


