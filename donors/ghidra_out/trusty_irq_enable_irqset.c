// trusty_irq_enable_irqset @ 001021c4
// signature: undefined trusty_irq_enable_irqset(void)
// calling convention: unknown


void trusty_irq_enable_irqset(undefined8 *param_1,long param_2)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(long *)(param_2 + 8) != 0) {
    lVar3 = *(long *)(param_2 + 8) + -8;
    while (lVar3 != 0) {
      if (*(char *)(lVar3 + 0x1d) == '\0') {
        enable_percpu_irq(*(undefined4 *)(lVar3 + 0x18),0);
        *(undefined1 *)(lVar3 + 0x1d) = 1;
      }
      else {
        lVar2 = tpidr_el1;
        _dev_warn(*param_1,"%s: percpu irq %d already enabled, cpu %d\n","trusty_irq_enable_irqset",
                  *(undefined4 *)(lVar3 + 0x18),*(undefined4 *)(&cpu_number + lVar2));
      }
      plVar1 = (long *)(lVar3 + 8);
      lVar3 = 0;
      if (*plVar1 != 0) {
        lVar3 = *plVar1 + -8;
      }
    }
  }
  return;
}


