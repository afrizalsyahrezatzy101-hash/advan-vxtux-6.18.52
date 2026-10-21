// vser_test_works @ 001028e4
// signature: undefined vser_test_works(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void vser_test_works(void)

{
  long *plVar1;
  long lVar2;
  long lVar3;
  uint uVar4;
  long lVar5;
  long *plVar6;
  long lVar7;
  int iVar8;
  int iVar9;
  long lVar10;
  undefined8 uVar11;
  ulong uVar12;
  long lVar13;
  uint uVar14;
  undefined8 *in_x18;
  undefined8 *puVar15;
  int iVar16;
  long *plVar17;
  long *plVar18;
  undefined8 unaff_x30;
  int local_ac;
  int local_9c;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  lVar7 = _vser_dev;
  local_68 = ___stack_chk_guard;
  local_9c = *(int *)(_vser_dev + 0x180);
  puVar15 = *(undefined8 **)(_vser_dev + 0x168);
  if (puVar15 == (undefined8 *)0x0) goto LAB_00103108;
  lVar13 = _vser_dev + 0x138;
  local_ac = 0;
  uVar4 = *(uint *)(_vser_dev + 0x174) & 3;
  plVar1 = (long *)(_vser_dev + 0x118);
  lVar2 = _vser_dev + 0xe0;
  lVar3 = _vser_dev + 0x150;
LAB_00102970:
  do {
    do {
      if (*(uint *)(lVar7 + 0x174) == 0) goto code_r0x00102f14;
    } while ((*(uint *)(lVar7 + 0x174) >> 7 & 1) == 0);
    if (uVar4 == 1) {
      uVar11 = _raw_spin_lock_irqsave(lVar2);
      plVar17 = (long *)*plVar1;
      if (plVar1 == plVar17) {
        _raw_spin_unlock_irqrestore(lVar2,uVar11);
        iVar8 = *(int *)(lVar7 + 0x100);
      }
      else {
        plVar18 = plVar17 + -8;
        uVar12 = __list_del_entry_valid(plVar17);
        if ((uVar12 & 1) != 0) {
          lVar10 = *plVar17;
          plVar6 = (long *)plVar17[1];
          *(long **)(lVar10 + 8) = plVar6;
          *plVar6 = lVar10;
        }
        *plVar17 = -0x2152ffffffffff00;
        plVar17[1] = -0x2152fffffffffede;
        tx_req_count = tx_req_count + -1;
        _raw_spin_unlock_irqrestore(lVar2,uVar11);
        if (plVar18 != (long *)0x0) {
LAB_00102b74:
          if (local_9c == 0) {
            uVar11 = *(undefined8 *)(lVar7 + 0xe8);
            kfree(*plVar18);
            usb_ep_free_request(uVar11,plVar18);
            local_ac = local_ac + 1;
            local_9c = 0;
            if (local_ac == 4) {
              uVar11 = *(undefined8 *)(lVar7 + 0xf0);
              kfree(*puVar15);
              usb_ep_free_request(uVar11,puVar15);
              goto code_r0x00102f14;
            }
          }
          else {
            iVar8 = *(int *)(lVar7 + 0x178);
            if (local_9c <= *(int *)(lVar7 + 0x178)) {
              iVar8 = local_9c;
            }
            *(int *)(plVar18 + 1) = iVar8;
            iVar8 = usb_ep_queue(*(undefined8 *)(lVar7 + 0xe8),plVar18,0xa20);
            if (iVar8 < 0) {
              *(undefined4 *)(lVar7 + 0x100) = 1;
              goto code_r0x00102f14;
            }
            local_9c = local_9c - (int)plVar18[1];
          }
          goto LAB_00102970;
        }
        iVar8 = *(int *)(lVar7 + 0x100);
      }
      if (iVar8 == 0) {
        local_78 = 0;
        uStack_70 = 0;
        local_88 = 0;
        uStack_80 = 0;
        local_90 = 0;
        init_wait_entry(&local_90,0);
        while( true ) {
          lVar10 = prepare_to_wait_event(lVar3,&local_90,1);
          uVar11 = _raw_spin_lock_irqsave(lVar2);
          plVar17 = (long *)*plVar1;
          if (plVar1 != plVar17) break;
          _raw_spin_unlock_irqrestore(lVar2,uVar11);
          iVar8 = *(int *)(lVar7 + 0x100);
joined_r0x00102b38:
          if (iVar8 != 0) {
            plVar18 = (long *)0x0;
            goto LAB_00102b64;
          }
          if (lVar10 != 0) goto LAB_00102970;
          schedule();
        }
        plVar18 = plVar17 + -8;
        uVar12 = __list_del_entry_valid(plVar17);
        if ((uVar12 & 1) != 0) {
          lVar5 = *plVar17;
          plVar6 = (long *)plVar17[1];
          *(long **)(lVar5 + 8) = plVar6;
          *plVar6 = lVar5;
        }
        *plVar17 = -0x2152ffffffffff00;
        plVar17[1] = -0x2152fffffffffede;
        tx_req_count = tx_req_count + -1;
        _raw_spin_unlock_irqrestore(lVar2,uVar11);
        if (plVar18 == (long *)0x0) {
          iVar8 = *(int *)(lVar7 + 0x100);
          goto joined_r0x00102b38;
        }
LAB_00102b64:
        finish_wait(lVar3,&local_90);
        if (plVar18 != (long *)0x0) goto LAB_00102b74;
      }
      goto LAB_00102970;
    }
    if (uVar4 != 2) {
      if (uVar4 == 3) {
        iVar8 = *(int *)(lVar7 + 0x17c);
        if (local_9c <= *(int *)(lVar7 + 0x17c)) {
          iVar8 = local_9c;
        }
        *(int *)(puVar15 + 1) = iVar8;
        uVar14 = (uint)*(ushort *)(*(long *)(lVar7 + 0xf0) + 0x2e);
        *(uint *)(puVar15 + 1) = (iVar8 + uVar14) - 1 & -uVar14;
        *(undefined4 *)(lVar7 + 0x170) = 0;
        iVar8 = usb_ep_queue(*(undefined8 *)(lVar7 + 0xf0),puVar15,0xa20);
        if (iVar8 < 0) {
LAB_00102ef0:
          *(undefined4 *)(lVar7 + 0xfc) = 1;
          goto code_r0x00102f14;
        }
        if (*(int *)(lVar7 + 0x170) == 0) {
          local_78 = 0;
          uStack_70 = 0;
          local_88 = 0;
          uStack_80 = 0;
          local_90 = 0;
          init_wait_entry(&local_90,0);
          while (lVar10 = prepare_to_wait_event(lVar13,&local_90,1), *(int *)(lVar7 + 0x170) == 0) {
            if (lVar10 != 0) {
              if (-1 < (int)lVar10) goto LAB_00102c78;
              goto LAB_00102ef0;
            }
            schedule();
          }
          finish_wait(lVar13,&local_90);
        }
LAB_00102c78:
        if (*(int *)(lVar7 + 0x174) == 0) goto code_r0x00102f14;
        iVar8 = *(int *)(puVar15 + 0xb);
        do {
          uVar11 = _raw_spin_lock_irqsave(lVar2);
          plVar17 = (long *)*plVar1;
          if (plVar1 == plVar17) {
            _raw_spin_unlock_irqrestore(lVar2,uVar11);
LAB_00102d38:
            if (*(int *)(lVar7 + 0x100) == 0) {
              local_78 = 0;
              uStack_70 = 0;
              local_88 = 0;
              uStack_80 = 0;
              local_90 = 0;
              init_wait_entry(&local_90,0);
              while( true ) {
                lVar10 = prepare_to_wait_event(lVar3,&local_90,1);
                uVar11 = _raw_spin_lock_irqsave(lVar2);
                plVar17 = (long *)*plVar1;
                if (plVar1 != plVar17) break;
                _raw_spin_unlock_irqrestore(lVar2,uVar11);
                iVar16 = *(int *)(lVar7 + 0x100);
joined_r0x00102dd4:
                if (iVar16 != 0) {
                  plVar18 = (long *)0x0;
                  goto LAB_00102e2c;
                }
                if (lVar10 != 0) goto LAB_00102c94;
                schedule();
              }
              plVar18 = plVar17 + -8;
              uVar12 = __list_del_entry_valid(plVar17);
              if ((uVar12 & 1) != 0) {
                lVar5 = *plVar17;
                plVar6 = (long *)plVar17[1];
                *(long **)(lVar5 + 8) = plVar6;
                *plVar6 = lVar5;
              }
              *plVar17 = -0x2152ffffffffff00;
              plVar17[1] = -0x2152fffffffffede;
              tx_req_count = tx_req_count + -1;
              _raw_spin_unlock_irqrestore(lVar2,uVar11);
              if (plVar18 == (long *)0x0) {
                iVar16 = *(int *)(lVar7 + 0x100);
                goto joined_r0x00102dd4;
              }
LAB_00102e2c:
              finish_wait(lVar3,&local_90);
              if (plVar18 != (long *)0x0) goto LAB_00102d00;
            }
          }
          else {
            plVar18 = plVar17 + -8;
            uVar12 = __list_del_entry_valid(plVar17);
            if ((uVar12 & 1) != 0) {
              lVar10 = *plVar17;
              plVar6 = (long *)plVar17[1];
              *(long **)(lVar10 + 8) = plVar6;
              *plVar6 = lVar10;
            }
            *plVar17 = -0x2152ffffffffff00;
            plVar17[1] = -0x2152fffffffffede;
            tx_req_count = tx_req_count + -1;
            _raw_spin_unlock_irqrestore(lVar2,uVar11);
            if (plVar18 == (long *)0x0) goto LAB_00102d38;
LAB_00102d00:
            if (*(int *)(lVar7 + 0x178) < iVar8) {
              memcpy((void *)*plVar18,(void *)*puVar15,(long)*(int *)(lVar7 + 0x178));
              iVar16 = iVar8 - *(int *)(lVar7 + 0x178);
              iVar8 = *(int *)(lVar7 + 0x178);
            }
            else {
              memcpy((void *)*plVar18,(void *)*puVar15,(long)iVar8);
              iVar16 = 0;
            }
            *(int *)(plVar18 + 1) = iVar8;
            iVar9 = usb_ep_queue(*(undefined8 *)(lVar7 + 0xe8),plVar18,0xa20);
            iVar8 = iVar16;
            if (iVar9 < 0) {
              *(undefined4 *)(lVar7 + 0x100) = 1;
              break;
            }
          }
LAB_00102c94:
        } while (0 < iVar8);
        local_9c = local_9c - *(int *)(puVar15 + 0xb);
        if (local_9c == 0) {
          uVar11 = *(undefined8 *)(lVar7 + 0xf0);
          kfree(*puVar15);
          usb_ep_free_request(uVar11,puVar15);
          iVar8 = 0;
          goto LAB_00102f7c;
        }
      }
      goto LAB_00102970;
    }
    uVar14 = *(uint *)(lVar7 + 0x17c);
    if (local_9c <= (int)uVar14) {
      *(int *)(puVar15 + 1) = local_9c;
      uVar14 = (uint)*(ushort *)(*(long *)(lVar7 + 0xf0) + 0x2e);
      uVar14 = (local_9c + uVar14) - 1 & -uVar14;
    }
    *(uint *)(puVar15 + 1) = uVar14;
    *(undefined4 *)(lVar7 + 0x170) = 0;
    iVar8 = usb_ep_queue(*(undefined8 *)(lVar7 + 0xf0),puVar15,0xa20);
    if (iVar8 < 0) goto LAB_00102ef0;
    if (*(int *)(lVar7 + 0x170) == 0) {
      local_78 = 0;
      uStack_70 = 0;
      local_88 = 0;
      uStack_80 = 0;
      local_90 = 0;
      init_wait_entry(&local_90,0);
      while (lVar10 = prepare_to_wait_event(lVar13,&local_90,1), *(int *)(lVar7 + 0x170) == 0) {
        if (lVar10 != 0) {
          if (-1 < (int)lVar10) goto LAB_00102c50;
          goto LAB_00102ef0;
        }
        schedule();
      }
      finish_wait(lVar13,&local_90);
    }
LAB_00102c50:
    local_9c = local_9c - *(int *)(puVar15 + 0xb);
  } while (local_9c != 0);
  vser_free_all_request(lVar7);
  goto code_r0x00102f14;
LAB_00102f7c:
  do {
    uVar11 = _raw_spin_lock_irqsave(lVar2);
    plVar17 = *(long **)(lVar7 + 0x118);
    if (plVar1 == plVar17) {
      _raw_spin_unlock_irqrestore(lVar2,uVar11);
LAB_00102ff0:
      if (*(int *)(lVar7 + 0x100) == 0) {
        local_78 = 0;
        uStack_70 = 0;
        local_88 = 0;
        uStack_80 = 0;
        local_90 = 0;
        init_wait_entry(&local_90,0);
        while( true ) {
          lVar13 = prepare_to_wait_event(lVar3,&local_90,1);
          uVar11 = _raw_spin_lock_irqsave(lVar2);
          plVar17 = *(long **)(lVar7 + 0x118);
          if (plVar1 != plVar17) break;
          _raw_spin_unlock_irqrestore(lVar2,uVar11);
          iVar16 = *(int *)(lVar7 + 0x100);
joined_r0x0010308c:
          if (iVar16 != 0) {
            plVar18 = (long *)0x0;
            goto LAB_001030b8;
          }
          if (lVar13 != 0) goto LAB_00102f74;
          schedule();
        }
        plVar18 = plVar17 + -8;
        uVar12 = __list_del_entry_valid(plVar17);
        if ((uVar12 & 1) != 0) {
          lVar10 = *plVar17;
          plVar6 = (long *)plVar17[1];
          *(long **)(lVar10 + 8) = plVar6;
          *plVar6 = lVar10;
        }
        *plVar17 = -0x2152ffffffffff00;
        plVar17[1] = -0x2152fffffffffede;
        tx_req_count = tx_req_count + -1;
        _raw_spin_unlock_irqrestore(lVar2,uVar11);
        if (plVar18 == (long *)0x0) {
          iVar16 = *(int *)(lVar7 + 0x100);
          goto joined_r0x0010308c;
        }
LAB_001030b8:
        finish_wait(lVar3,&local_90);
        if (plVar18 != (long *)0x0) goto LAB_00102f58;
      }
    }
    else {
      plVar18 = plVar17 + -8;
      uVar12 = __list_del_entry_valid(plVar17);
      if ((uVar12 & 1) != 0) {
        lVar13 = *plVar17;
        plVar6 = (long *)plVar17[1];
        *(long **)(lVar13 + 8) = plVar6;
        *plVar6 = lVar13;
      }
      *plVar17 = -0x2152ffffffffff00;
      plVar17[1] = -0x2152fffffffffede;
      tx_req_count = tx_req_count + -1;
      _raw_spin_unlock_irqrestore(lVar2,uVar11);
      if (plVar18 == (long *)0x0) goto LAB_00102ff0;
LAB_00102f58:
      uVar11 = *(undefined8 *)(lVar7 + 0xe8);
      iVar8 = iVar8 + 1;
      kfree(*plVar18);
      usb_ep_free_request(uVar11,plVar18);
    }
LAB_00102f74:
  } while (iVar8 < 4);
code_r0x00102f14:
  *(undefined8 *)(lVar7 + 0x178) = 0x400000004000;
  vser_alloc_all_request(lVar7);
  *(undefined4 *)(lVar7 + 0x174) = 0;
LAB_00103108:
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}


