// trusty_irq_init_per_cpu_irq @ 00101928
// signature: undefined trusty_irq_init_per_cpu_irq(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int trusty_irq_init_per_cpu_irq(undefined8 *param_1,int param_2)

{
  undefined8 *puVar1;
  long lVar2;
  ulong uVar3;
  long lVar4;
  long *plVar5;
  long lVar6;
  long *plVar7;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  while( true ) {
    param_2 = trusty_irq_create_irq_mapping(param_1,param_2);
    if (param_2 < 1) {
      _dev_err(*param_1,"trusty_irq_create_irq_mapping failed (%d)\n",param_2);
      return param_2;
    }
    lVar2 = __alloc_percpu(0x28,8);
    if (lVar2 == 0) {
      return -0xc;
    }
    uVar3 = cpumask_next(0xffffffff,&__cpu_possible_mask);
    if ((uint)uVar3 < _nr_cpu_ids) {
      do {
        if (0x1f < (uint)uVar3) goto LAB_00101b0c;
        lVar4 = *(long *)(&__per_cpu_offset + (uVar3 & 0xffffffff) * 8);
        lVar6 = param_1[5];
        puVar1 = (undefined8 *)(lVar4 + lVar2);
        *puVar1 = param_1;
        plVar5 = (long *)(lVar4 + lVar6 + 8);
        lVar4 = *plVar5;
        plVar7 = puVar1 + 1;
        *plVar7 = lVar4;
        if (lVar4 != 0) {
          *(long **)(lVar4 + 8) = plVar7;
        }
        *plVar5 = (long)plVar7;
        puVar1[2] = plVar5;
        *(int *)(puVar1 + 3) = param_2;
        *(undefined1 *)((long)puVar1 + 0x1c) = 1;
        puVar1[4] = lVar2;
        uVar3 = cpumask_next(uVar3,&__cpu_possible_mask);
      } while ((uint)uVar3 < _nr_cpu_ids);
    }
    param_2 = __request_percpu_irq(param_2,&LAB_00103130,0,"trusty",lVar2);
    if (param_2 == 0) {
      return 0;
    }
    _dev_err(*param_1,"request_percpu_irq failed %d\n",param_2);
    uVar3 = cpumask_next(0xffffffff,&__cpu_possible_mask);
    if (_nr_cpu_ids <= (uint)uVar3) break;
    param_1 = (undefined8 *)&__cpu_possible_mask;
    while ((uint)uVar3 < 0x20) {
      lVar4 = *(long *)(&__per_cpu_offset + (uVar3 & 0xffffffff) * 8) + lVar2;
      lVar6 = *(long *)(lVar4 + 8);
      plVar5 = *(long **)(lVar4 + 0x10);
      *plVar5 = lVar6;
      if (lVar6 != 0) {
        *(long **)(lVar6 + 8) = plVar5;
      }
      *(undefined8 *)(lVar4 + 8) = 0xdead000000000100;
      *(undefined1 **)(lVar4 + 0x10) = &DAT_dead000000000122;
      uVar3 = cpumask_next(uVar3,&__cpu_possible_mask);
      if (_nr_cpu_ids <= (uint)uVar3) goto LAB_00101b00;
    }
LAB_00101b0c:
    __ubsan_handle_cfi_check_fail_abort(0x12);
    __dynamic_dev_dbg(trusty_irq_init_per_cpu_irq___UNIQUE_ID_ddebug184,*param_1,"%s: irq %d\n",
                      "trusty_irq_init_per_cpu_irq",param_2);
  }
LAB_00101b00:
  free_percpu(lVar2);
  return param_2;
}


