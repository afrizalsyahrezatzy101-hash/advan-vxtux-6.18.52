// trusty_log_call_notify @ 00101c64
// signature: undefined trusty_log_call_notify(void)
// calling convention: unknown


undefined8 trusty_log_call_notify(long param_1,long param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_2 != 0) {
    *in_x18 = unaff_x30;
    uVar1 = _raw_spin_lock_irqsave(param_1 + -0x30);
    trusty_dump_logs(param_1 + -0x40,param_2);
    _raw_spin_unlock_irqrestore(param_1 + -0x30,uVar1);
    return 1;
  }
  return 0;
}


