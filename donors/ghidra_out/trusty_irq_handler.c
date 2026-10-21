// trusty_irq_handler @ 00101ee4
// signature: undefined trusty_irq_handler(void)
// calling convention: unknown


undefined8 trusty_irq_handler(undefined4 param_1,undefined8 *param_2)

{
  long lVar1;
  long *plVar2;
  long *plVar3;
  undefined8 *in_x18;
  undefined8 *puVar4;
  long *plVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar4 = (undefined8 *)*param_2;
  lVar1 = irq_get_irq_data();
  if (*(int *)(puVar4 + 0xb) == 2) {
    if (*(char *)((long)param_2 + 0x1c) == '\0') {
      disable_irq_nosync(param_1);
      plVar5 = puVar4 + 2;
    }
    else {
      disable_percpu_irq(param_1);
      lVar1 = tpidr_el1;
      plVar5 = (long *)(lVar1 + puVar4[5]);
    }
    _raw_spin_lock(puVar4 + 4);
    if (*(char *)((long)param_2 + 0x1d) != '\0') {
      plVar2 = param_2 + 1;
      lVar1 = *plVar2;
      plVar3 = (long *)param_2[2];
      *plVar3 = lVar1;
      if (lVar1 != 0) {
        *(long **)(lVar1 + 8) = plVar3;
      }
      param_2[1] = 0xdead000000000100;
      param_2[2] = &DAT_dead000000000122;
      lVar1 = *plVar5;
      param_2[1] = lVar1;
      if (lVar1 != 0) {
        *(long **)(lVar1 + 8) = plVar2;
      }
      *plVar5 = (long)plVar2;
      param_2[2] = plVar5;
    }
    _raw_spin_unlock(puVar4 + 4);
  }
  else {
    if (*(int *)(puVar4 + 0xb) != 3) {
      _dev_err(*puVar4,"unexcepted gic version: %d\n");
      return 0;
    }
    if (*(long *)(lVar1 + 8) != 0xd) {
      _dev_err(*puVar4,"unexcepted irq: %ld\n");
      return 0;
    }
  }
  trusty_enqueue_nop(puVar4[1],0);
  return 1;
}


