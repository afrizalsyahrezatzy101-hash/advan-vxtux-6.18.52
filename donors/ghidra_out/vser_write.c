// vser_write @ 00103680
// signature: undefined vser_write(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong vser_write(long param_1,long param_2,ulong param_3)

{
  int *piVar1;
  long lVar2;
  long *plVar3;
  long *plVar4;
  char cVar5;
  bool bVar6;
  code *pcVar7;
  int iVar8;
  long lVar9;
  undefined8 uVar10;
  ulong uVar11;
  ulong uVar12;
  undefined8 *in_x18;
  ulong uVar13;
  undefined8 *puVar14;
  long *plVar15;
  long lVar16;
  long lVar17;
  long *plVar18;
  long lVar19;
  undefined8 unaff_x30;
  long local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 uStack_80;
  undefined8 local_78;
  undefined8 uStack_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  if (s_in_bypass_mode == '\0') {
    lVar17 = *(long *)(param_1 + 200);
    if (*(int *)(lVar17 + 0x174) == 0) {
      piVar1 = (int *)(lVar17 + 0x108);
      Hint_Prefetch(piVar1,2,0,1);
      do {
        iVar8 = *piVar1;
        cVar5 = '\x01';
        bVar6 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar6) {
          *piVar1 = iVar8 + 1;
          cVar5 = ExclusiveMonitorsStatus();
        }
      } while (cVar5 != '\0');
      DataMemoryBarrier(2,3);
      if (iVar8 + 1 == 1) {
        lVar2 = lVar17 + 0x150;
        do {
          if ((*(int *)(lVar17 + 0xf8) != 0) || (*(int *)(lVar17 + 0x100) != 0)) {
            if (param_3 == 0) {
              uVar11 = 0;
              goto LAB_00103a90;
            }
            plVar3 = (long *)(lVar17 + 0x118);
            lVar9 = lVar17 + 0xe0;
            uVar13 = param_3;
            local_98 = param_2;
            goto LAB_001037f4;
          }
          if (*(int *)(lVar17 + 0x100) == 0) {
            local_78 = 0;
            uStack_70 = 0;
            local_88 = 0;
            uStack_80 = 0;
            local_90 = 0;
            init_wait_entry(&local_90,0);
            while ((lVar9 = prepare_to_wait_event(lVar2,&local_90,1), *(int *)(lVar17 + 0xf8) == 0
                   && (*(int *)(lVar17 + 0x100) == 0))) {
              if (lVar9 != 0) goto LAB_0010374c;
              schedule();
            }
            finish_wait(lVar2,&local_90);
            lVar9 = 0;
          }
          else {
            lVar9 = 0;
          }
LAB_0010374c:
        } while (-1 < (int)lVar9);
        Hint_Prefetch(piVar1,2,0,1);
        do {
          cVar5 = '\x01';
          bVar6 = (bool)ExclusiveMonitorPass(piVar1,0x10);
          if (bVar6) {
            *piVar1 = *piVar1 + -1;
            cVar5 = ExclusiveMonitorsStatus();
          }
        } while (cVar5 != '\0');
        param_3 = (ulong)(int)lVar9;
        goto LAB_00103ad8;
      }
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar5 = '\x01';
        bVar6 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar6) {
          *piVar1 = *piVar1 + -1;
          cVar5 = ExclusiveMonitorsStatus();
        }
      } while (cVar5 != '\0');
    }
    param_3 = 0xfffffffffffffff0;
  }
  else {
    msleep(100);
  }
  goto LAB_00103ad8;
LAB_001037f4:
  if (*(int *)(lVar17 + 0x100) != 0) {
    uVar11 = 0xfffffffb;
    goto LAB_00103a90;
  }
  uVar10 = _raw_spin_lock_irqsave(lVar9);
  plVar15 = (long *)*plVar3;
  if (plVar3 == plVar15) {
    _raw_spin_unlock_irqrestore(lVar9,uVar10);
LAB_001038e4:
    if (*(int *)(lVar17 + 0x100) == 0) {
      local_78 = 0;
      uStack_70 = 0;
      local_88 = 0;
      uStack_80 = 0;
      local_90 = 0;
      init_wait_entry(&local_90,0);
      do {
        uVar11 = prepare_to_wait_event(lVar2,&local_90,1);
        uVar10 = _raw_spin_lock_irqsave(lVar9);
        plVar15 = (long *)*plVar3;
        if (plVar3 != plVar15) {
          plVar18 = plVar15 + -8;
          uVar12 = __list_del_entry_valid(plVar15);
          if ((uVar12 & 1) != 0) {
            lVar16 = *plVar15;
            plVar4 = (long *)plVar15[1];
            *(long **)(lVar16 + 8) = plVar4;
            *plVar4 = lVar16;
          }
          *plVar15 = -0x2152ffffffffff00;
          plVar15[1] = -0x2152fffffffffede;
          tx_req_count = tx_req_count + -1;
          _raw_spin_unlock_irqrestore(lVar9,uVar10);
          if (plVar18 == (long *)0x0) {
            iVar8 = *(int *)(lVar17 + 0x100);
            goto joined_r0x00103980;
          }
LAB_001039ac:
          finish_wait(lVar2,&local_90);
          if (plVar18 != (long *)0x0) goto LAB_00103870;
          break;
        }
        _raw_spin_unlock_irqrestore(lVar9,uVar10);
        iVar8 = *(int *)(lVar17 + 0x100);
joined_r0x00103980:
        if (iVar8 != 0) {
          plVar18 = (long *)0x0;
          goto LAB_001039ac;
        }
        if (uVar11 != 0) goto LAB_001037ec;
        schedule();
      } while( true );
    }
    goto LAB_001037f0;
  }
  plVar18 = plVar15 + -8;
  uVar11 = __list_del_entry_valid(plVar15);
  if ((uVar11 & 1) != 0) {
    lVar16 = *plVar15;
    plVar4 = (long *)plVar15[1];
    *(long **)(lVar16 + 8) = plVar4;
    *plVar4 = lVar16;
  }
  *plVar15 = -0x2152ffffffffff00;
  plVar15[1] = -0x2152fffffffffede;
  tx_req_count = tx_req_count + -1;
  _raw_spin_unlock_irqrestore(lVar9,uVar10);
  if (plVar18 == (long *)0x0) goto LAB_001038e4;
LAB_00103870:
  iVar8 = *(int *)(lVar17 + 0x178);
  if (uVar13 <= (ulong)(long)iVar8) {
    iVar8 = (int)uVar13;
  }
  if (iVar8 < 0) {
                    /* WARNING: Does not return */
    pcVar7 = (code *)SoftwareBreakpoint(0x800,0x103a3c);
    (*pcVar7)();
  }
  lVar16 = *plVar18;
  lVar19 = (long)iVar8;
  __check_object_size(lVar16,lVar19,0);
  lVar16 = _copy_from_user(lVar16,local_98,lVar19);
  if (lVar16 != 0) {
    uVar11 = 0xfffffff2;
LAB_00103a40:
    uVar10 = _raw_spin_lock_irqsave(lVar9);
    puVar14 = *(undefined8 **)(lVar17 + 0x120);
    plVar15 = plVar18 + 8;
    uVar13 = __list_add_valid(plVar15,puVar14,plVar3);
    if ((uVar13 & 1) != 0) {
      *(long **)(lVar17 + 0x120) = plVar15;
      plVar18[8] = (long)plVar3;
      plVar18[9] = (long)puVar14;
      *puVar14 = plVar15;
    }
    tx_req_count = tx_req_count + 1;
    _raw_spin_unlock_irqrestore(lVar9,uVar10);
    goto LAB_00103a90;
  }
  *(int *)(plVar18 + 1) = iVar8;
  iVar8 = usb_ep_queue(*(undefined8 *)(lVar17 + 0xe8),plVar18,0xa20);
  if (iVar8 < 0) {
    *(undefined4 *)(lVar17 + 0x100) = 1;
    uVar11 = 0xfffffffb;
    goto LAB_00103a40;
  }
  uVar13 = uVar13 - lVar19;
  local_98 = local_98 + lVar19;
LAB_001037f0:
  if (uVar13 == 0) goto LAB_00103a1c;
  goto LAB_001037f4;
LAB_001037ec:
  if ((int)uVar11 < 0) goto LAB_00103a90;
  goto LAB_001037f0;
LAB_00103a1c:
  uVar11 = param_3 & 0xffffffff;
LAB_00103a90:
  Hint_Prefetch(piVar1,2,0,1);
  do {
    cVar5 = '\x01';
    bVar6 = (bool)ExclusiveMonitorPass(piVar1,0x10);
    if (bVar6) {
      *piVar1 = *piVar1 + -1;
      cVar5 = ExclusiveMonitorsStatus();
    }
  } while (cVar5 != '\0');
  param_3 = (ulong)(int)uVar11;
LAB_00103ad8:
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return param_3;
}


