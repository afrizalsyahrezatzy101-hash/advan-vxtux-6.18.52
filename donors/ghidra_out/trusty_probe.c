// trusty_probe @ 00101dd8
// signature: undefined trusty_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int trusty_probe(long param_1)

{
  long lVar1;
  long *plVar2;
  undefined1 *puVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  long lVar7;
  undefined8 uVar8;
  long lVar9;
  undefined8 *in_x18;
  ulong uVar10;
  ulong uVar11;
  undefined8 unaff_x30;
  undefined8 local_80;
  undefined8 uStack_78;
  ulong local_70;
  undefined8 local_68;
  undefined8 local_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  lVar1 = param_1 + 0x10;
  local_58 = ___stack_chk_guard;
  if (*(long *)(param_1 + 0x310) == 0) {
    _dev_err(lVar1,"of_node required\n");
    iVar6 = -0x16;
    goto LAB_00101e30;
  }
  lVar7 = kmem_cache_alloc_trace(_DAT_00106228,0xdc0,0x90);
  if (lVar7 == 0) {
    iVar6 = -0xc;
    goto LAB_00101e30;
  }
  *(long *)(lVar7 + 0x60) = lVar1;
  *(undefined4 *)(lVar7 + 0x88) = 0;
  *(long *)(lVar7 + 0x78) = lVar7 + 0x78;
  *(long *)(lVar7 + 0x80) = lVar7 + 0x78;
  __mutex_init(lVar7,"&s->smc_lock",0x100000);
  *(undefined4 *)(lVar7 + 0x20) = 0;
  *(undefined8 *)(lVar7 + 0x28) = 0;
  *(undefined4 *)(lVar7 + 0x30) = 0;
  __init_waitqueue_head(lVar7 + 0x38,"&x->wait",0x100000);
  *(long *)(param_1 + 0xa8) = lVar7;
  local_80 = 0;
  uStack_78 = 0xbc00000a;
  local_70 = 0xffffffff;
  local_68 = 0;
  local_60 = 0;
  uVar4 = __smc(&local_80);
  if ((int)uVar4 < 1) {
LAB_00101f94:
    _dev_err(lVar1,"failed to get version: %d\n",uVar4);
  }
  else {
    uVar8 = __kmalloc((long)(int)(uVar4 + 1),0xcc0);
    uVar10 = 0;
    uVar11 = (ulong)uVar4;
    *(undefined8 *)(lVar7 + 0x50) = uVar8;
    do {
      local_80 = 0;
      uStack_78 = 0xbc00000a;
      local_68 = 0;
      local_60 = 0;
      local_70 = uVar10;
      uVar4 = __smc(&local_80);
      if ((int)uVar4 < 0) goto LAB_00101f88;
      *(char *)(*(long *)(lVar7 + 0x50) + uVar10) = (char)uVar4;
      uVar10 = uVar10 + 1;
    } while (uVar11 != uVar10);
    *(undefined1 *)(*(long *)(lVar7 + 0x50) + uVar11) = 0;
    _dev_info(lVar1,"trusty version: %s\n",*(undefined8 *)(lVar7 + 0x50));
    uVar4 = device_create_file(lVar1,&dev_attr_trusty_version);
    if (uVar4 != 0) {
LAB_00101f88:
      kfree(*(undefined8 *)(lVar7 + 0x50));
      *(undefined8 *)(lVar7 + 0x50) = 0;
      goto LAB_00101f94;
    }
  }
  local_80 = 0;
  uStack_78 = 0xbc00000b;
  local_70 = 3;
  local_68 = 0;
  local_60 = 0;
  uVar5 = __smc(&local_80);
  uVar4 = 0;
  if (uVar5 != 0xffffffff) {
    uVar4 = uVar5;
  }
  if (uVar4 < 4) {
    _dev_info(lVar1,"selected api version: %u (requested %u)\n",uVar4,3);
    *(uint *)(lVar7 + 0x58) = uVar4;
    lVar9 = alloc_workqueue("trusty-nop-wq",0x20,0);
    *(long *)(lVar7 + 0x68) = lVar9;
    if (lVar9 == 0) {
      _dev_err(lVar1,"Failed create trusty-nop-wq\n");
      iVar6 = -0x13;
    }
    else {
      lVar9 = __alloc_percpu(0x38,8);
      *(long *)(lVar7 + 0x70) = lVar9;
      if (lVar9 == 0) {
        _dev_err(lVar1,"Failed to allocate works\n");
        iVar6 = -0xc;
      }
      else {
        puVar3 = (undefined1 *)0x1034c0;
        if (1 < *(uint *)(lVar7 + 0x58)) {
          puVar3 = &LAB_001034c4;
        }
        uVar10 = cpumask_next(0xffffffff,&__cpu_possible_mask);
        if ((uint)uVar10 < _nr_cpu_ids) {
          do {
            if (0x1f < (uint)uVar10) goto LAB_001021e8;
            plVar2 = (long *)(*(long *)(&__per_cpu_offset + (uVar10 & 0xffffffff) * 8) +
                             *(long *)(lVar7 + 0x70));
            *plVar2 = lVar7;
            plVar2[1] = 0xfffffffe0;
            plVar2[2] = (long)(plVar2 + 2);
            plVar2[3] = (long)(plVar2 + 2);
            plVar2[4] = (long)puVar3;
            uVar10 = cpumask_next(uVar10,&__cpu_possible_mask);
          } while ((uint)uVar10 < _nr_cpu_ids);
        }
        iVar6 = of_platform_populate(*(undefined8 *)(param_1 + 0x310),0,0,lVar1);
        if (-1 < iVar6) {
          iVar6 = 0;
          goto LAB_00101e30;
        }
        _dev_err(lVar1,"Failed to add children: %d\n",iVar6);
        uVar4 = cpumask_next(0xffffffff,&__cpu_possible_mask);
        if (uVar4 < _nr_cpu_ids) {
          do {
            if (0x1f < uVar4) {
LAB_001021e8:
                    /* WARNING: Subroutine does not return */
              __ubsan_handle_cfi_check_fail_abort(0x12);
            }
            flush_work(*(long *)(&__per_cpu_offset + (ulong)uVar4 * 8) + *(long *)(lVar7 + 0x70) + 8
                      );
            uVar4 = cpumask_next((ulong)uVar4,&__cpu_possible_mask);
          } while (uVar4 < _nr_cpu_ids);
        }
        free_percpu(*(undefined8 *)(lVar7 + 0x70));
      }
      destroy_workqueue(*(undefined8 *)(lVar7 + 0x68));
    }
  }
  else {
    _dev_err(lVar1,"unsupported api version %u > %u\n",uVar4,3);
    iVar6 = -0x16;
  }
  if (*(long *)(lVar7 + 0x50) != 0) {
    device_remove_file(lVar1,&dev_attr_trusty_version);
    kfree(*(undefined8 *)(lVar7 + 0x50));
  }
  device_for_each_child(lVar1,0,&LAB_001034b8);
  kfree(lVar7);
LAB_00101e30:
  if (___stack_chk_guard != local_58) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar6;
}


