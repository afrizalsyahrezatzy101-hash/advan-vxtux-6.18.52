// trusty_log_panic_notify @ 00101ccc
// signature: undefined trusty_log_panic_notify(void)
// calling convention: unknown


undefined8 trusty_log_panic_notify(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = trusty_version_str_get(*(undefined8 *)(param_1 + -0x50));
  printk(&DAT_00100232,uVar1);
  trusty_dump_logs(param_1 + -0x58,2);
  return 1;
}


