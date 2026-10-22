// trusty_irq_enable_pending_irqs @ 001020a8
// signature: undefined trusty_irq_enable_pending_irqs(void)
// calling convention: unknown


void trusty_irq_enable_pending_irqs(undefined8 param_1,long *param_2,uint param_3)

{
  long *plVar1;
  long lVar2;
  undefined8 *in_x18;
  long *plVar3;
  long lVar4;
  long lVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((*param_2 != 0) && (lVar4 = *param_2 + -8, lVar4 != 0)) {
    param_2 = param_2 + 1;
    do {
      plVar3 = (long *)(lVar4 + 8);
      lVar5 = *plVar3;
      if ((param_3 & 1) == 0) {
        enable_irq(*(undefined4 *)(lVar4 + 0x18));
      }
      else {
        enable_percpu_irq(*(undefined4 *)(lVar4 + 0x18),0);
      }
      lVar2 = *(long *)(lVar4 + 8);
      plVar1 = *(long **)(lVar4 + 0x10);
      *plVar1 = lVar2;
      if (lVar2 != 0) {
        *(long **)(lVar2 + 8) = plVar1;
      }
      *(undefined8 *)(lVar4 + 8) = 0xdead000000000100;
      *(undefined1 **)(lVar4 + 0x10) = &DAT_dead000000000122;
      lVar2 = *param_2;
      *(long *)(lVar4 + 8) = lVar2;
      if (lVar2 != 0) {
        *(long **)(lVar2 + 8) = plVar3;
      }
      *param_2 = (long)plVar3;
      *(long **)(lVar4 + 0x10) = param_2;
      lVar4 = 0;
      if (lVar5 != 0) {
        lVar4 = lVar5 + -8;
      }
    } while (lVar4 != 0);
  }
  return;
}


