// trusty_dequeue_nop @ 00101c7c
// signature: undefined trusty_dequeue_nop(void)
// calling convention: unknown


void trusty_dequeue_nop(long param_1,long *param_2)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  code *pcVar4;
  undefined8 uVar5;
  ulong uVar6;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_2 != (long *)0x0) {
    *in_x18 = unaff_x30;
    lVar1 = *(long *)(param_1 + 0x98) + 0x88;
    uVar5 = _raw_spin_lock_irqsave(lVar1);
    if (param_2 != (long *)*param_2) {
      uVar6 = __list_del_entry_valid(param_2);
      if ((uVar6 & 1) != 0) {
        lVar2 = *param_2;
        plVar3 = (long *)param_2[1];
        *(long **)(lVar2 + 8) = plVar3;
        *plVar3 = lVar2;
      }
      *param_2 = (long)param_2;
      param_2[1] = (long)param_2;
    }
    _raw_spin_unlock_irqrestore(lVar1,uVar5);
    return;
  }
                    /* WARNING: Does not return */
  pcVar4 = (code *)SoftwareBreakpoint(0x800,0x101cfc);
  (*pcVar4)();
}


