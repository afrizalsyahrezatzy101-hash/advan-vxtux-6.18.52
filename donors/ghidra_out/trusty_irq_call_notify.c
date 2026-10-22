// trusty_irq_call_notify @ 00101898
// signature: undefined trusty_irq_call_notify(void)
// calling convention: unknown


bool trusty_irq_call_notify(long param_1,long param_2)

{
  ulong uVar1;
  long lVar2;
  code *pcVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = daif;
  if ((uVar1 & 0x80) == 0) {
                    /* WARNING: Does not return */
    pcVar3 = (code *)SoftwareBreakpoint(0x800,0x101928);
    (*pcVar3)();
  }
  if (param_2 == 0) {
    _raw_spin_lock(param_1 + -0x10);
    trusty_irq_enable_pending_irqs(param_1 + -0x30,param_1 + -0x20,0);
    _raw_spin_unlock(param_1 + -0x10);
    lVar2 = tpidr_el1;
    trusty_irq_enable_pending_irqs(param_1 + -0x30,lVar2 + *(long *)(param_1 + -8),1);
  }
  return param_2 == 0;
}


