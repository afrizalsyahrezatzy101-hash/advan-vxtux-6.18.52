====================mmsys_unregister_notifier@0010c554====================
// signature: undefined mmsys_unregister_notifier(void)

undefined8 mmsys_unregister_notifier(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    uVar1 = blocking_notifier_chain_unregister(&mmsys_notifier_list,param_1);
    return uVar1;
  }
  return 0xffffffea;
}


