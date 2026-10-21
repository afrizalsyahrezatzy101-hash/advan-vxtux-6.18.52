// trusty_irq_remove @ 00101428
// signature: undefined trusty_irq_remove(void)
// calling convention: unknown


undefined4 trusty_irq_remove(long param_1)

{
  code *pcVar1;
  int iVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  long lVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = *(long *)(param_1 + 0xa8);
  iVar2 = __cpuhp_state_remove_instance(trusty_irq_cpuhp_slot,lVar4 + 0x48,1);
  if (iVar2 == 0) {
    uVar3 = _raw_spin_lock_irqsave(lVar4 + 0x20);
    trusty_irq_disable_irqset(lVar4,lVar4 + 0x10);
    _raw_spin_unlock_irqrestore(lVar4 + 0x20,uVar3);
    trusty_irq_free_irqs(lVar4);
    trusty_call_notifier_unregister(*(undefined8 *)(lVar4 + 8),lVar4 + 0x30);
    free_percpu(*(undefined8 *)(lVar4 + 0x28));
    kfree(lVar4);
    return 0;
  }
                    /* WARNING: Does not return */
  pcVar1 = (code *)SoftwareBreakpoint(0x800,0x1014c8);
  (*pcVar1)();
}


