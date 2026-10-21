====================mmsys_register_notifier@0010c520====================
// signature: undefined mmsys_register_notifier(void)

undefined8 mmsys_register_notifier(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    uVar1 = blocking_notifier_chain_register(&mmsys_notifier_list,param_1);
    return uVar1;
  }
  return 0xffffffea;
}


