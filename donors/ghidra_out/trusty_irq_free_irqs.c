// trusty_irq_free_irqs @ 001016d8
// signature: undefined trusty_irq_free_irqs(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong trusty_irq_free_irqs(ulong param_1)

{
  bool bVar1;
  long lVar2;
  long *plVar3;
  code *pcVar4;
  ulong uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *extraout_x18;
  long lVar7;
  long lVar8;
  undefined8 unaff_x30;
  undefined1 auVar9 [16];
  
  *in_x18 = unaff_x30;
  uVar5 = param_1;
  if (*(long *)(param_1 + 0x18) != 0) {
    lVar6 = *(long *)(param_1 + 0x18) + -8;
    while (lVar6 != 0) {
      lVar7 = *(long *)(lVar6 + 8);
      free_irq(*(undefined4 *)(lVar6 + 0x18),lVar6);
      lVar8 = *(long *)(lVar6 + 8);
      plVar3 = *(long **)(lVar6 + 0x10);
      *plVar3 = lVar8;
      if (lVar8 != 0) {
        *(long **)(lVar8 + 8) = plVar3;
      }
      *(undefined8 *)(lVar6 + 8) = 0xdead000000000100;
      *(undefined1 **)(lVar6 + 0x10) = &DAT_dead000000000122;
      uVar5 = kfree(lVar6);
      lVar6 = 0;
      if (lVar7 != 0) {
        lVar6 = lVar7 + -8;
      }
    }
  }
  lVar6 = tpidr_el1;
  lVar6 = *(long *)(lVar6 + *(long *)(param_1 + 0x28) + 8);
  if (lVar6 != 0) {
    lVar6 = lVar6 + -8;
    while (lVar6 != 0) {
      lVar7 = *(long *)(lVar6 + 8);
      lVar8 = *(long *)(lVar6 + 0x20);
      free_percpu_irq(*(undefined4 *)(lVar6 + 0x18),lVar8);
      uVar5 = 0xffffffff;
      while( true ) {
        uVar5 = cpumask_next(uVar5,&__cpu_possible_mask);
        if (_nr_cpu_ids <= (uint)uVar5) break;
        if (0x1f < (uint)uVar5) {
          auVar9 = __ubsan_handle_cfi_check_fail_abort(0x12);
          lVar6 = auVar9._0_8_;
          *extraout_x18 = trusty_irq_call_notify;
          uVar5 = daif;
          if ((uVar5 & 0x80) == 0) {
                    /* WARNING: Does not return */
            pcVar4 = (code *)SoftwareBreakpoint(0x800,0x101928);
            (*pcVar4)();
          }
          bVar1 = auVar9._8_8_ == 0;
          if (bVar1) {
            _raw_spin_lock(lVar6 + -0x10);
            trusty_irq_enable_pending_irqs(lVar6 + -0x30,lVar6 + -0x20,0);
            _raw_spin_unlock(lVar6 + -0x10);
            lVar8 = tpidr_el1;
            trusty_irq_enable_pending_irqs(lVar6 + -0x30,lVar8 + *(long *)(lVar6 + -8),1);
          }
          return (ulong)bVar1;
        }
        lVar6 = *(long *)(&__per_cpu_offset + (uVar5 & 0xffffffff) * 8) + lVar8;
        lVar2 = *(long *)(lVar6 + 8);
        plVar3 = *(long **)(lVar6 + 0x10);
        *plVar3 = lVar2;
        if (lVar2 != 0) {
          *(long **)(lVar2 + 8) = plVar3;
        }
        *(undefined8 *)(lVar6 + 8) = 0xdead000000000100;
        *(undefined1 **)(lVar6 + 0x10) = &DAT_dead000000000122;
      }
      uVar5 = free_percpu(lVar8);
      lVar6 = 0;
      if (lVar7 != 0) {
        lVar6 = lVar7 + -8;
      }
    }
  }
  return uVar5;
}


