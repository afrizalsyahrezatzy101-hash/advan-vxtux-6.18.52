// trusty_irq_init_normal_irq @ 00101b3c
// signature: undefined trusty_irq_init_normal_irq(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int trusty_irq_init_normal_irq(undefined8 *param_1,undefined4 param_2)

{
  undefined8 *puVar1;
  int iVar2;
  long *plVar3;
  undefined8 uVar4;
  long *plVar5;
  long *plVar6;
  long lVar7;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar2 = trusty_irq_create_irq_mapping(param_1,param_2);
  if (iVar2 < 0) {
    _dev_err(*param_1,"trusty_irq_create_irq_mapping failed (%d)\n",iVar2);
  }
  else {
    plVar3 = (long *)kmem_cache_alloc_trace(_DAT_00105160,0xdc0,0x28);
    if (plVar3 == (long *)0x0) {
      iVar2 = -0xc;
    }
    else {
      puVar1 = param_1 + 4;
      *plVar3 = (long)param_1;
      *(int *)(plVar3 + 3) = iVar2;
      *(undefined1 *)((long)plVar3 + 0x1d) = 1;
      uVar4 = _raw_spin_lock_irqsave(puVar1);
      plVar5 = param_1 + 3;
      lVar7 = *plVar5;
      plVar6 = plVar3 + 1;
      *plVar6 = lVar7;
      if (lVar7 != 0) {
        *(long **)(lVar7 + 8) = plVar6;
      }
      *plVar5 = (long)plVar6;
      plVar3[2] = (long)plVar5;
      _raw_spin_unlock_irqrestore(puVar1,uVar4);
      iVar2 = request_threaded_irq(iVar2,&LAB_00103130,0,0x10000,"trusty",plVar3);
      if (iVar2 != 0) {
        _dev_err(*param_1,"request_irq failed %d\n",iVar2);
        uVar4 = _raw_spin_lock_irqsave(puVar1);
        lVar7 = plVar3[1];
        plVar5 = (long *)plVar3[2];
        *plVar5 = lVar7;
        if (lVar7 != 0) {
          *(long **)(lVar7 + 8) = plVar5;
        }
        plVar3[1] = -0x2152ffffffffff00;
        plVar3[2] = (long)&DAT_dead000000000122;
        _raw_spin_unlock_irqrestore(puVar1,uVar4);
        kfree(plVar3);
      }
    }
  }
  return iVar2;
}


