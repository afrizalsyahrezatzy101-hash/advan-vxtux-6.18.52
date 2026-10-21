// vser_pass_user_write @ 00101030
// signature: undefined vser_pass_user_write(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void vser_pass_user_write(long param_1,long param_2)

{
  int *piVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  long *plVar5;
  char cVar6;
  bool bVar7;
  int iVar8;
  undefined8 uVar9;
  ulong uVar10;
  long lVar11;
  long lVar12;
  undefined8 *in_x18;
  undefined8 *puVar13;
  long *plVar14;
  long *plVar15;
  long lVar16;
  undefined8 unaff_x30;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  lVar12 = _vser_dev;
  local_68 = ___stack_chk_guard;
  if ((_vser_dev == 0) || (*(int *)(_vser_dev + 0xf8) == 0)) {
    lVar12 = -0x13;
  }
  else if (*(char *)(*(long *)(_vser_dev + 0xe8) + 0x2d) == '\0') {
    printk(&DAT_001007bb);
    lVar12 = -0x13;
  }
  else {
    piVar1 = (int *)(_vser_dev + 0x108);
    Hint_Prefetch(piVar1,2,0,1);
    do {
      iVar8 = *piVar1;
      cVar6 = '\x01';
      bVar7 = (bool)ExclusiveMonitorPass(piVar1,0x10);
      if (bVar7) {
        *piVar1 = iVar8 + 1;
        cVar6 = ExclusiveMonitorsStatus();
      }
    } while (cVar6 != '\0');
    DataMemoryBarrier(2,3);
    if (iVar8 + 1 == 1) {
      if (param_2 == 0) {
        lVar11 = 0;
      }
      else {
        plVar2 = (long *)(lVar12 + 0x128);
        lVar3 = lVar12 + 0xe0;
        lVar16 = param_2;
        do {
          while( true ) {
            uVar9 = _raw_spin_lock_irqsave(lVar3);
            plVar15 = (long *)*plVar2;
            if (plVar2 != plVar15) break;
            _raw_spin_unlock_irqrestore(lVar3,uVar9);
LAB_00101144:
            local_78 = 0;
            uStack_70 = 0;
            local_88 = 0;
            uStack_80 = 0;
            local_90 = 0;
            init_wait_entry(&local_90,0);
            while( true ) {
              lVar11 = prepare_to_wait_event(lVar12 + 0x150,&local_90,1);
              uVar9 = _raw_spin_lock_irqsave(lVar3);
              plVar15 = (long *)*plVar2;
              if (plVar2 == plVar15) {
                _raw_spin_unlock_irqrestore(lVar3,uVar9);
              }
              else {
                plVar14 = plVar15 + -8;
                uVar10 = __list_del_entry_valid(plVar15);
                if ((uVar10 & 1) != 0) {
                  lVar4 = *plVar15;
                  plVar5 = (long *)plVar15[1];
                  *(long **)(lVar4 + 8) = plVar5;
                  *plVar5 = lVar4;
                }
                *plVar15 = -0x2152ffffffffff00;
                plVar15[1] = -0x2152fffffffffede;
                tx_req_count = tx_req_count + -1;
                _raw_spin_unlock_irqrestore(lVar3,uVar9);
                if (plVar14 != (long *)0x0) {
                  finish_wait(lVar12 + 0x150,&local_90);
                  goto LAB_001011f4;
                }
              }
              if (lVar11 != 0) break;
              schedule();
            }
            if (((int)lVar11 < 0) || (lVar11 = param_2, lVar16 == 0)) goto LAB_0010129c;
          }
          plVar14 = plVar15 + -8;
          uVar10 = __list_del_entry_valid(plVar15);
          if ((uVar10 & 1) != 0) {
            lVar11 = *plVar15;
            plVar5 = (long *)plVar15[1];
            *(long **)(lVar11 + 8) = plVar5;
            *plVar5 = lVar11;
          }
          *plVar15 = -0x2152ffffffffff00;
          plVar15[1] = -0x2152fffffffffede;
          tx_req_count = tx_req_count + -1;
          _raw_spin_unlock_irqrestore(lVar3,uVar9);
          if (plVar14 == (long *)0x0) goto LAB_00101144;
LAB_001011f4:
          *(int *)(plVar14 + 1) = (int)lVar16;
          *plVar14 = param_1;
          iVar8 = usb_ep_queue(*(undefined8 *)(lVar12 + 0xe8),plVar14,0xa20);
          if (iVar8 < 0) {
            *(undefined4 *)(lVar12 + 0x100) = 1;
            uVar9 = _raw_spin_lock_irqsave(lVar3);
            puVar13 = *(undefined8 **)(lVar12 + 0x130);
            plVar15 = plVar14 + 8;
            uVar10 = __list_add_valid(plVar15,puVar13,plVar2);
            if ((uVar10 & 1) != 0) {
              *(long **)(lVar12 + 0x130) = plVar15;
              plVar14[8] = (long)plVar2;
              plVar14[9] = (long)puVar13;
              *puVar13 = plVar15;
            }
            tx_req_count = tx_req_count + 1;
            _raw_spin_unlock_irqrestore(lVar3,uVar9);
            lVar11 = 0xfffffffb;
            break;
          }
          lVar16 = lVar16 - (int)lVar16;
          lVar11 = param_2;
        } while (lVar16 != 0);
      }
LAB_0010129c:
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar6 = '\x01';
        bVar7 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar7) {
          *piVar1 = *piVar1 + -1;
          cVar6 = ExclusiveMonitorsStatus();
        }
      } while (cVar6 != '\0');
      lVar12 = (long)(int)lVar11;
    }
    else {
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar6 = '\x01';
        bVar7 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar7) {
          *piVar1 = *piVar1 + -1;
          cVar6 = ExclusiveMonitorsStatus();
        }
      } while (cVar6 != '\0');
      lVar12 = -0x10;
    }
  }
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail(lVar12);
  }
  return;
}


