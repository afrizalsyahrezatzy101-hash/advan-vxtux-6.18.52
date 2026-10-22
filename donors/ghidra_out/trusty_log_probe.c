// trusty_log_probe @ 00101030
// signature: undefined trusty_log_probe(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong trusty_log_probe(long param_1)

{
  long lVar1;
  int iVar2;
  uint uVar3;
  long *plVar4;
  undefined8 *puVar5;
  ulong uVar6;
  ulong uVar7;
  undefined8 *in_x18;
  long lVar8;
  ulong uVar9;
  long *plVar10;
  undefined8 unaff_x30;
  long local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  local_70 = 0;
  iVar2 = trusty_std_call32(*(undefined8 *)(param_1 + 0x70),0x33000000,1,0,0);
  if (iVar2 == -1) {
    printk(&DAT_001003b1);
  }
  else if (iVar2 < 0) {
    printk(&DAT_001002e9);
  }
  else {
    if (iVar2 == 1) {
      plVar4 = (long *)kmem_cache_alloc_trace(___stack_chk_guard,0xdc0,0x170);
      if (plVar4 == (long *)0x0) {
        uVar9 = 0xfffffff4;
      }
      else {
        *(undefined4 *)(plVar4 + 2) = 0;
        lVar8 = *(long *)(param_1 + 0x70);
        lVar1 = param_1 + 0x10;
        *(undefined4 *)(plVar4 + 4) = 0;
        *plVar4 = lVar1;
        plVar4[1] = lVar8;
        puVar5 = (undefined8 *)trusty_log_alloc_pages(4,&local_70);
        if (puVar5 < (undefined8 *)0xfffffffffffff001) {
          uVar6 = trusty_log_register_buf(lVar8,puVar5,4);
          lVar8 = local_70;
          if (uVar6 < 0xfffffffffffff001) {
            *(undefined4 *)(plVar4 + 7) = 4;
            plVar4[5] = uVar6;
            plVar4[6] = (long)puVar5;
            plVar10 = plVar4 + 8;
            *plVar10 = (long)&LAB_0010214c;
            *(undefined4 *)(plVar4 + 4) = 0;
            plVar4[3] = local_70;
            *(undefined4 *)((long)plVar4 + 0x3c) = 1;
            uVar3 = trusty_call_notifier_register(plVar4[1],plVar10);
            if ((int)uVar3 < 0) {
              _dev_err(lVar1,"failed to register trusty call notifier\n");
            }
            else {
              plVar4[0xb] = (long)&LAB_00102150;
              uVar3 = atomic_notifier_chain_register(&panic_notifier_list);
              if (-1 < (int)uVar3) {
                *(long **)(param_1 + 0xa8) = plVar4;
                iVar2 = sysfs_create_group(lVar1,&trusty_log_attr_group);
                if (iVar2 == 0) {
                  uVar9 = 0;
                }
                else {
                  _dev_err(lVar1,"Failed to create debug files\n");
                  uVar9 = 0;
                }
                goto LAB_001011c8;
              }
              _dev_err(lVar1,"failed to register panic notifier\n");
              trusty_call_notifier_unregister(plVar4[1],plVar10);
            }
            uVar9 = (ulong)uVar3;
            if ((uVar6 ^ 0xffffff8000000000) < (-1L << (_vabits_actual - 1U & 0x3f)) + 0x8000000000U
               ) {
              uVar7 = _memstart_addr + (uVar6 & 0x7fffffffff);
            }
            else {
              uVar7 = uVar6 - _kimage_voffset;
            }
            iVar2 = trusty_std_call32(plVar4[1],0x33000002,uVar7,uVar7 >> 0x20,0);
            if (iVar2 == 0) {
              free_pages(uVar6);
            }
            else {
              printk(&DAT_001004ec,iVar2);
            }
          }
          else {
            _dev_err(lVar1,"register failed(%ld)\n",uVar6);
            uVar9 = uVar6 & 0xffffffff;
            lVar8 = local_70;
          }
          if (lVar8 != 0) {
            vm_unmap_ram(lVar8,4);
          }
          __free_pages(puVar5[3],0);
          __free_pages(puVar5[2],0);
          __free_pages(puVar5[1],0);
          __free_pages(*puVar5,0);
          kfree(puVar5);
        }
        else {
          uVar9 = (ulong)puVar5 & 0xffffffff;
        }
        kfree(plVar4);
      }
      goto LAB_001011c8;
    }
    printk(&DAT_0010041f,iVar2,1);
  }
  uVar9 = 0xfffffffa;
LAB_001011c8:
  if (___stack_chk_guard == local_68) {
    return uVar9;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


