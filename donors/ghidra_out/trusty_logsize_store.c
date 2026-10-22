// trusty_logsize_store @ 001017b0
// signature: undefined trusty_logsize_store(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong trusty_logsize_store(long param_1,undefined8 param_2,undefined8 param_3,ulong param_4)

{
  long lVar1;
  int iVar2;
  ulong uVar3;
  ulong uVar4;
  ulong uVar5;
  undefined8 *in_x18;
  ulong uVar6;
  ulong uVar7;
  long lVar8;
  long lVar9;
  long lVar10;
  undefined8 unaff_x30;
  ulong local_78;
  long local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  local_78 = 0;
  local_70 = 0;
  iVar2 = kstrtoull(param_3,0,&local_78);
  if (iVar2 != 0) {
    printk(&DAT_0010027e,param_3);
    uVar3 = 0xffffffffffffffea;
    goto LAB_00101918;
  }
  lVar10 = *(long *)(param_1 + 0x98);
  *(undefined4 *)(lVar10 + 0x3c) = 0;
  uVar6 = local_78 >> 0xc;
  uVar3 = trusty_log_alloc_pages(uVar6,&local_70);
  if (uVar3 < 0xfffffffffffff001) {
    uVar4 = trusty_log_register_buf(*(undefined8 *)(lVar10 + 8),uVar3,uVar6);
    if (uVar4 < 0xfffffffffffff001) {
      uVar7 = *(ulong *)(lVar10 + 0x28);
      if ((uVar7 ^ 0xffffff8000000000) < (-1L << (_vabits_actual - 1U & 0x3f)) + 0x8000000000U) {
        uVar5 = _memstart_addr + (uVar7 & 0x7fffffffff);
      }
      else {
        uVar5 = uVar7 - _kimage_voffset;
      }
      iVar2 = trusty_std_call32(*(undefined8 *)(lVar10 + 8),0x33000002,uVar5,uVar5 >> 0x20,0);
      if (iVar2 == 0) {
        free_pages(uVar7,0);
        lVar8 = *(long *)(lVar10 + 0x30);
        iVar2 = *(int *)(lVar10 + 0x38);
        lVar9 = (long)iVar2;
        if (*(long *)(lVar10 + 0x18) != 0) {
          vm_unmap_ram(*(long *)(lVar10 + 0x18),iVar2);
        }
        if (iVar2 != 0) {
          do {
            lVar1 = lVar9 * 8;
            lVar9 = lVar9 + -1;
            __free_pages(*(undefined8 *)(lVar8 + -8 + lVar1),0);
          } while (lVar9 != 0);
        }
        kfree(lVar8);
        *(int *)(lVar10 + 0x38) = (int)uVar6;
        *(ulong *)(lVar10 + 0x28) = uVar4;
        *(ulong *)(lVar10 + 0x30) = uVar3;
        *(undefined4 *)(lVar10 + 0x20) = 0;
        *(long *)(lVar10 + 0x18) = local_70;
        uVar3 = param_4;
        goto LAB_00101910;
      }
      printk(&DAT_001004ec,iVar2);
      printk(&DAT_0010036c,iVar2);
      uVar4 = (ulong)iVar2;
    }
    else {
      printk(&DAT_00100342,uVar4);
    }
    if (local_70 != 0) {
      vm_unmap_ram(local_70,uVar6 & 0xffffffff);
    }
    if (uVar6 != 0) {
      lVar8 = uVar6 << 3;
      do {
        __free_pages(*(undefined8 *)((uVar3 - 8) + lVar8),0);
        lVar8 = lVar8 + -8;
      } while (lVar8 != 0);
    }
    kfree(uVar3);
    uVar3 = uVar4;
  }
  else {
    printk(&DAT_0010029d,uVar3);
  }
LAB_00101910:
  *(undefined4 *)(lVar10 + 0x3c) = 1;
LAB_00101918:
  if (___stack_chk_guard == local_68) {
    return uVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


