// trusty_enqueue_nop @ 00101b8c
// signature: undefined trusty_enqueue_nop(void)
// calling convention: unknown


void trusty_enqueue_nop(long param_1,long *param_2)

{
  long lVar1;
  long lVar2;
  code *pcVar3;
  undefined8 uVar4;
  ulong uVar5;
  long lVar6;
  undefined8 *in_x18;
  undefined8 *puVar7;
  long lVar8;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar8 = *(long *)(param_1 + 0x98);
  lVar2 = sp_el0;
  *(int *)(lVar2 + 0x18) = *(int *)(lVar2 + 0x18) + 1;
  lVar6 = *(long *)(lVar8 + 0x70);
  lVar1 = tpidr_el1;
  if (param_2 != (long *)0x0) {
    if (*(uint *)(lVar8 + 0x58) < 3) {
                    /* WARNING: Does not return */
      pcVar3 = (code *)SoftwareBreakpoint(0x800,0x101c78);
      (*pcVar3)();
    }
    uVar4 = _raw_spin_lock_irqsave(lVar8 + 0x88);
    if (param_2 == (long *)*param_2) {
      puVar7 = *(undefined8 **)(lVar8 + 0x80);
      uVar5 = __list_add_valid(param_2,puVar7,lVar8 + 0x78);
      if ((uVar5 & 1) != 0) {
        *(long **)(lVar8 + 0x80) = param_2;
        *param_2 = lVar8 + 0x78;
        param_2[1] = (long)puVar7;
        *puVar7 = param_2;
      }
    }
    _raw_spin_unlock_irqrestore(lVar8 + 0x88,uVar4);
  }
  queue_work_on(0x20,*(undefined8 *)(lVar8 + 0x68),lVar1 + lVar6 + 8);
  lVar1 = *(long *)(lVar2 + 0x18) + -1;
  *(int *)(lVar2 + 0x18) = (int)lVar1;
  if ((lVar1 == 0) || (*(long *)(lVar2 + 0x18) == 0)) {
    preempt_schedule();
  }
  return;
}


