// trusty_irq_disable_irqset @ 001014f0
// signature: undefined trusty_irq_disable_irqset(void)
// calling convention: unknown


void trusty_irq_disable_irqset(undefined8 *param_1,long *param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  undefined8 *in_x18;
  long *plVar4;
  long *plVar5;
  long lVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar5 = param_2 + 1;
  if (*plVar5 != 0) {
    lVar2 = *plVar5 + -8;
    while (lVar2 != 0) {
      if (*(char *)(lVar2 + 0x1d) == '\0') {
        lVar6 = tpidr_el1;
        _dev_warn(*param_1,"irq %d already disabled, percpu %d, cpu %d\n",
                  *(undefined4 *)(lVar2 + 0x18),*(undefined1 *)(lVar2 + 0x1c),
                  *(undefined4 *)(&cpu_number + lVar6));
      }
      else {
        *(undefined1 *)(lVar2 + 0x1d) = 0;
        if (*(char *)(lVar2 + 0x1c) == '\0') {
          disable_irq_nosync(*(undefined4 *)(lVar2 + 0x18));
        }
        else {
          disable_percpu_irq();
        }
      }
      plVar4 = (long *)(lVar2 + 8);
      lVar2 = 0;
      if (*plVar4 != 0) {
        lVar2 = *plVar4 + -8;
      }
    }
  }
  if (*param_2 != 0) {
    lVar2 = *param_2 + -8;
    while (lVar2 != 0) {
      plVar4 = (long *)(lVar2 + 8);
      lVar6 = *plVar4;
      if (*(char *)(lVar2 + 0x1d) == '\0') {
        lVar3 = tpidr_el1;
        _dev_warn(*param_1,"pending irq %d already disabled, percpu %d, cpu %d\n",
                  *(undefined4 *)(lVar2 + 0x18),*(undefined1 *)(lVar2 + 0x1c),
                  *(undefined4 *)(&cpu_number + lVar3));
      }
      lVar3 = *(long *)(lVar2 + 8);
      plVar1 = *(long **)(lVar2 + 0x10);
      *(undefined1 *)(lVar2 + 0x1d) = 0;
      *plVar1 = lVar3;
      if (lVar3 != 0) {
        *(long **)(lVar3 + 8) = plVar1;
      }
      *(undefined8 *)(lVar2 + 8) = 0xdead000000000100;
      *(undefined1 **)(lVar2 + 0x10) = &DAT_dead000000000122;
      lVar3 = *plVar5;
      *(long *)(lVar2 + 8) = lVar3;
      if (lVar3 != 0) {
        *(long **)(lVar3 + 8) = plVar4;
      }
      *plVar5 = (long)plVar4;
      *(long **)(lVar2 + 0x10) = plVar5;
      lVar2 = 0;
      if (lVar6 != 0) {
        lVar2 = lVar6 + -8;
      }
    }
  }
  return;
}


