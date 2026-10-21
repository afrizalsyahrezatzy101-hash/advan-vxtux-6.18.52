// mmc_wp_ioctl @ 0010932c
// signature: undefined mmc_wp_ioctl(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void mmc_wp_ioctl(long param_1,int param_2,undefined8 param_3)

{
  char cVar1;
  bool bVar2;
  int iVar3;
  ulong uVar4;
  long lVar5;
  undefined *puVar6;
  undefined8 *in_x18;
  undefined8 *extraout_x18;
  long lVar7;
  long lVar8;
  ulong uVar9;
  code *pcVar10;
  long lVar11;
  undefined8 unaff_x30;
  long local_460 [127];
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  if (param_2 != -0x3fb74d00) {
    lVar7 = -0x16;
    goto LAB_00109594;
  }
  lVar7 = *(long *)(param_1 + 200);
  lVar8 = *(long *)(lVar7 + 0x488);
  memset(local_460,0,0x3f8);
  if ((lVar8 == 0) || ((*(byte *)(lVar8 + 0x3bc) >> 4 & 1) != 0)) {
    iVar3 = -0x13;
  }
  else {
    uVar4 = kmem_cache_alloc_trace(_memcpy,0xcc0,0x60);
    if (uVar4 == 0) {
      iVar3 = -0xc;
LAB_001093e0:
      uVar4 = (ulong)iVar3;
    }
    else {
      lVar5 = _copy_from_user(uVar4,param_3);
      if (lVar5 != 0) {
        uVar9 = 0xfffffffffffffff2;
LAB_001093c0:
        iVar3 = (int)uVar9;
        kfree(uVar4);
        goto LAB_001093e0;
      }
      lVar5 = (ulong)*(uint *)(uVar4 + 0x28) * (ulong)*(uint *)(uVar4 + 0x24);
      *(long *)(uVar4 + 0x50) = lVar5;
      if (lVar5 == 0) {
        uVar9 = 0xffffffffffffffea;
        goto LAB_001093c0;
      }
      uVar9 = memdup_user(*(undefined8 *)(uVar4 + 0x40));
      *(ulong *)(uVar4 + 0x48) = uVar9;
      if (0xfffffffffffff000 < uVar9) goto LAB_001093c0;
    }
    if (uVar4 < 0xfffffffffffff001) {
      if (*(int *)(uVar4 + 8) == 0x53505244) {
        if (*(int *)(lVar7 + 0x3b0) != 0) {
LAB_001096b4:
          __ubsan_handle_cfi_check_fail_abort(0x12);
          *extraout_x18 = mmc_wp_ioctl_compat;
          mmc_wp_ioctl();
          return;
        }
        iVar3 = *(int *)(uVar4 + 0xc);
        if (iVar3 == DAT_00100650) {
          if (iVar3 == 0) {
            lVar8 = *(long *)(lVar7 + 0x78);
            if (lVar8 == 0) {
              lVar8 = *(long *)(lVar7 + 8);
            }
            puVar6 = &DAT_00101b22;
          }
          else {
            if (iVar3 != 1) {
              iVar3 = 0;
              goto LAB_00109580;
            }
            lVar8 = *(long *)(lVar7 + 0x78);
            if (lVar8 == 0) {
              lVar8 = *(long *)(lVar7 + 8);
            }
            puVar6 = &DAT_00100bdc;
          }
          printk(puVar6,lVar8);
          iVar3 = 0;
        }
        else {
          lVar11 = *(long *)(lVar8 + 0xa0);
          iVar3 = -1000;
          lVar5 = lVar11 + 0xd0;
          do {
            _raw_spin_lock_irq(lVar5);
            if ((*(char *)(lVar11 + 0xe6) == '\0') && (*(char *)(lVar11 + 0xe4) == '\0')) {
              *(undefined1 *)(lVar11 + 0xe4) = 1;
              _raw_spin_unlock_irq(lVar5);
              if ((*(char *)(lVar7 + 0x664) != '\0') || (*(char *)(lVar7 + 0x720) != '\0')) {
                pcVar10 = *(code **)(*(long *)(lVar7 + 0x650) + 0x28);
                __cfi_slowpath(0x5149616ef9bb8d20,pcVar10);
                iVar3 = (*pcVar10)(lVar7);
                if (iVar3 != 0) {
                  lVar8 = *(long *)(lVar7 + 0x78);
                  if (lVar8 == 0) {
                    lVar8 = *(long *)(lVar7 + 8);
                  }
                  printk(&DAT_00101dae,lVar8);
                  goto LAB_00109580;
                }
              }
              local_460[0] = lVar8;
              sdio_claim_host(local_460);
              cVar1 = *(char *)(lVar8 + 0x50a);
              if ((cVar1 == '\0') || (iVar3 = mmc_cmdq_disable(lVar8), iVar3 == 0)) {
                if (*(int *)(lVar7 + 0x3b0) != 0) goto LAB_001096b4;
                DAT_00100650 = *(int *)(uVar4 + 0xc);
                mmc_hw_reset(lVar7);
                if (cVar1 == '\0') {
                  iVar3 = 0;
                }
                else {
                  iVar3 = mmc_cmdq_enable(lVar8);
                  if (iVar3 != 0) {
                    lVar8 = *(long *)(lVar7 + 0x78);
                    if (lVar8 == 0) {
                      lVar8 = *(long *)(lVar7 + 8);
                    }
                    printk(&DAT_00101eff,lVar8,iVar3);
                    mmc_hw_reset(lVar7);
                  }
                }
              }
              else {
                lVar8 = *(long *)(lVar7 + 0x78);
                if (lVar8 == 0) {
                  lVar8 = *(long *)(lVar7 + 8);
                }
                printk(&DAT_0010122c,lVar8,iVar3);
              }
              _raw_spin_lock_irq(lVar5);
              *(undefined1 *)(lVar11 + 0xe4) = 0;
              _raw_spin_unlock_irq(lVar5);
              sdio_release_host(local_460);
              goto LAB_00109580;
            }
            _raw_spin_unlock_irq(lVar5);
            usleep_range(3000,5000);
            bVar2 = iVar3 != -1;
            iVar3 = iVar3 + 1;
          } while (bVar2);
          lVar8 = *(long *)(lVar7 + 0x78);
          if (lVar8 == 0) {
            lVar8 = *(long *)(lVar7 + 8);
          }
          printk(&DAT_001012ed,lVar8);
          iVar3 = -0x10;
        }
      }
      else {
        lVar8 = *(long *)(lVar7 + 0x78);
        if (lVar8 == 0) {
          lVar8 = *(long *)(lVar7 + 8);
        }
        printk(&DAT_00101928,lVar8);
        iVar3 = -0x16;
      }
LAB_00109580:
      kfree(*(undefined8 *)(uVar4 + 0x48));
      kfree(uVar4);
    }
    else {
      lVar8 = *(long *)(lVar7 + 0x78);
      if (lVar8 == 0) {
        lVar8 = *(long *)(lVar7 + 8);
      }
      printk(&DAT_00100ab7,lVar8);
      iVar3 = (int)uVar4;
    }
  }
  lVar7 = (long)iVar3;
LAB_00109594:
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar7);
  }
  return;
}


