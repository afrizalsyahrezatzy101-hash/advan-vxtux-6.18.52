// trusty_irq_probe @ 00101148
// signature: undefined trusty_irq_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int trusty_irq_probe(long param_1)

{
  long lVar1;
  int iVar2;
  int iVar3;
  long *plVar4;
  undefined8 uVar5;
  long lVar6;
  undefined8 *in_x18;
  long *plVar7;
  long *plVar8;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar4 = (long *)kmem_cache_alloc_trace(_DAT_00105160,0xdc0,0x60);
  if (plVar4 == (long *)0x0) {
    iVar2 = -0xc;
  }
  else {
    lVar1 = param_1 + 0x10;
    *plVar4 = lVar1;
    lVar6 = *(long *)(param_1 + 0x70);
    plVar7 = plVar4 + 4;
    *(undefined4 *)plVar7 = 0;
    plVar4[1] = lVar6;
    lVar6 = of_find_compatible_node(0,0,"arm,gic-v3");
    if (lVar6 == 0) {
      *(undefined4 *)(plVar4 + 0xb) = 2;
    }
    else {
      *(undefined4 *)(plVar4 + 0xb) = 3;
    }
    lVar6 = __alloc_percpu(0x10,8);
    plVar4[5] = lVar6;
    if (lVar6 == 0) {
      iVar2 = -0xc;
    }
    else {
      *(long **)(param_1 + 0xa8) = plVar4;
      plVar8 = plVar4 + 6;
      *plVar8 = (long)&LAB_0010312c;
      iVar2 = trusty_call_notifier_register(plVar4[1],plVar8);
      if (iVar2 == 0) {
        if (((int)plVar4[0xb] == 3) && (iVar2 = trusty_irq_init_per_cpu_irq(plVar4,0xd), iVar2 != 0)
           ) {
          _dev_warn(*plVar4,"failed to initialize irq %d, irq will be ignored\n",0xd);
        }
        iVar2 = 0;
        do {
          while( true ) {
            iVar2 = trusty_fast_call32(plVar4[1],0xbc000003,iVar2,1,0);
            if (iVar2 < 0) goto LAB_0010128c;
            iVar3 = trusty_irq_init_per_cpu_irq(plVar4,iVar2);
            if (iVar3 != 0) break;
            iVar2 = iVar2 + 1;
            if (iVar2 < 0) goto LAB_0010128c;
          }
          _dev_warn(*plVar4,"failed to initialize irq %d, irq will be ignored\n",iVar2);
          iVar2 = iVar2 + 1;
        } while (-1 < iVar2);
LAB_0010128c:
        iVar2 = 0;
        do {
          while( true ) {
            iVar2 = trusty_fast_call32(plVar4[1],0xbc000003,iVar2,0,0);
            if (iVar2 < 0) goto LAB_001012ec;
            iVar3 = trusty_irq_init_normal_irq(plVar4,iVar2);
            if (iVar3 != 0) break;
            iVar2 = iVar2 + 1;
            if (iVar2 < 0) goto LAB_001012ec;
          }
          _dev_warn(*plVar4,"failed to initialize irq %d, irq will be ignored\n",iVar2);
          iVar2 = iVar2 + 1;
        } while (-1 < iVar2);
LAB_001012ec:
        iVar2 = __cpuhp_state_add_instance(trusty_irq_cpuhp_slot,plVar4 + 9,1);
        if (-1 < iVar2) {
          return 0;
        }
        _dev_err(lVar1,"cpuhp_state_add_instance failed %d\n",iVar2);
        uVar5 = _raw_spin_lock_irqsave(plVar7);
        trusty_irq_disable_irqset(plVar4,plVar4 + 2);
        _raw_spin_unlock_irqrestore(plVar7,uVar5);
        trusty_irq_free_irqs(plVar4);
        trusty_call_notifier_unregister(plVar4[1],plVar8);
      }
      else {
        _dev_err(lVar1,"failed to register trusty call notifier\n");
      }
      free_percpu(plVar4[5]);
    }
    kfree(plVar4);
  }
  return iVar2;
}


