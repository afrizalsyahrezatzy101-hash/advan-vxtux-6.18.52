// trusty_call_notifier_unregister @ 00101d00
// signature: undefined trusty_call_notifier_unregister(void)
// calling convention: unknown


void trusty_call_notifier_unregister(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  atomic_notifier_chain_unregister(*(long *)(param_1 + 0x98) + 0x20);
  return;
}


