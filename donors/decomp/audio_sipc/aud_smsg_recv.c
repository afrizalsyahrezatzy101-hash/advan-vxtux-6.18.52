// signature: undefined aud_smsg_recv(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

ulong aud_smsg_recv(byte param_1,undefined1 *param_2,uint param_3)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  bool bVar4;
  int iVar5;
  undefined *puVar6;
  undefined8 uVar7;
  undefined8 uVar8;
  undefined1 *__dest;
  undefined1 *__src;
  size_t __n;
  ulong uVar9;
  uint uVar10;
  long lVar11;
  long lVar12;
  undefined8 *in_x18;
  long lVar13;
  long lVar14;
  ulong uVar15;
  uint *puVar16;
  undefined8 unaff_x30;
  undefined8 local_80;
  undefined8 local_78;
  ulong local_70;
  undefined8 local_68;
  undefined8 uStack_60;
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  if (1 < param_1) goto LAB_00106c9c;
  lVar13 = (&aud_smsg_ipcs)[param_1];
  if (lVar13 == 0) {
    printk(&DAT_0010191b,"aud_smsg_recv");
    uVar9 = 0xffffffed;
    goto LAB_00106bf0;
  }
  if (0x11 < (ulong)*(ushort *)(param_2 + 2)) goto LAB_00106c9c;
  uVar15 = (ulong)param_3;
  piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
  Hint_Prefetch(piVar1,2,0,1);
  do {
    cVar3 = '\x01';
    bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
    if (bVar4) {
      *piVar1 = *piVar1 + 1;
      cVar3 = ExclusiveMonitorsStatus();
    }
  } while (cVar3 != '\0');
  uVar9 = (ulong)*(ushort *)(param_2 + 2);
  if (0x10 < uVar9) goto LAB_00106c9c;
  lVar14 = *(long *)(lVar13 + uVar9 * 8 + 0x298);
  if (lVar14 == 0) {
    printk(&DAT_00101fd1,"aud_smsg_recv",uVar9);
    if ((ulong)*(ushort *)(param_2 + 2) < 0x12) {
      piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar4) {
          *piVar1 = *piVar1 + -1;
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
      uVar9 = 0xffffffed;
      goto LAB_00106bf0;
    }
    goto LAB_00106c9c;
  }
  if (param_3 == 0) {
    iVar5 = mutex_trylock(lVar14 + 0x18);
    if (iVar5 == 0) {
      if ((ulong)*(ushort *)(param_2 + 2) < 0x11) {
        printk(&DAT_00101942,"aud_smsg_recv",
               *(undefined4 *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800));
        if ((ulong)*(ushort *)(param_2 + 2) < 0x12) {
          piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
          Hint_Prefetch(piVar1,2,0,1);
          do {
            cVar3 = '\x01';
            bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
            if (bVar4) {
              *piVar1 = *piVar1 + -1;
              cVar3 = ExclusiveMonitorsStatus();
            }
          } while (cVar3 != '\0');
          uVar9 = 0xfffffff0;
          goto LAB_00106bf0;
        }
      }
      goto LAB_00106c9c;
    }
    if (*(int *)(lVar14 + 0x38) == *(int *)(lVar14 + 0x3c)) {
      printk(&DAT_001021a9,"aud_smsg_recv");
      uVar9 = 0xffffffc3;
    }
    else {
      uVar9 = 0;
LAB_00106a70:
      puVar16 = (uint *)(lVar14 + 0x3c);
      uVar15 = (ulong)(*puVar16 & 0xff);
      lVar11 = lVar14 + uVar15 * 0x14;
      __src = (undefined1 *)(lVar11 + 0x40);
      if ((((uint)__src ^ (uint)param_2) & 7) == 0) {
        if (((ulong)__src & 7) == 0) {
          __n = 0x14;
          __dest = param_2;
        }
        else {
          lVar12 = lVar14 + uVar15 * 0x14;
          lVar11 = 0;
          do {
            iVar5 = (int)lVar11;
            param_2[lVar11] = *(undefined1 *)(lVar12 + 0x40 + lVar11);
            lVar11 = lVar11 + 1;
          } while (((int)lVar12 + 0x41 + iVar5 & 7U) != 0);
          __src = (undefined1 *)(lVar14 + uVar15 * 0x14 + lVar11 + 0x40);
          __n = 0x14 - lVar11;
          __dest = param_2 + lVar11;
        }
        memcpy(__dest,__src,__n);
      }
      else {
        lVar12 = lVar14 + uVar15 * 0x14;
        *param_2 = *__src;
        param_2[1] = *(undefined1 *)(lVar11 + 0x41);
        param_2[2] = *(undefined1 *)(lVar12 + 0x42);
        param_2[3] = *(undefined1 *)(lVar12 + 0x43);
        param_2[4] = *(undefined1 *)(lVar12 + 0x44);
        param_2[5] = *(undefined1 *)(lVar12 + 0x45);
        param_2[6] = *(undefined1 *)(lVar12 + 0x46);
        param_2[7] = *(undefined1 *)(lVar12 + 0x47);
        param_2[8] = *(undefined1 *)(lVar12 + 0x48);
        param_2[9] = *(undefined1 *)(lVar12 + 0x49);
        param_2[10] = *(undefined1 *)(lVar12 + 0x4a);
        param_2[0xb] = *(undefined1 *)(lVar12 + 0x4b);
        param_2[0xc] = *(undefined1 *)(lVar12 + 0x4c);
        param_2[0xd] = *(undefined1 *)(lVar12 + 0x4d);
        param_2[0xe] = *(undefined1 *)(lVar12 + 0x4e);
        param_2[0xf] = *(undefined1 *)(lVar12 + 0x4f);
        param_2[0x10] = *(undefined1 *)(lVar12 + 0x50);
        param_2[0x11] = *(undefined1 *)(lVar12 + 0x51);
        param_2[0x12] = *(undefined1 *)(lVar12 + 0x52);
        param_2[0x13] = *(undefined1 *)(lVar12 + 0x53);
      }
      *puVar16 = *puVar16 + 1;
    }
  }
  else {
    mutex_lock();
    piVar1 = (int *)(lVar14 + 0x38);
    piVar2 = (int *)(lVar14 + 0x3c);
    if ((int)param_3 < 0) {
      if (*piVar1 == *piVar2) {
        uVar9 = (ulong)*(ushort *)(param_2 + 2);
        if (0x10 < uVar9) goto LAB_00106c9c;
        if (((*(char *)(lVar13 + uVar9 + 0x3a8) != '\x03') && (*(int *)(lVar13 + 0x3cc) != 0)) &&
           (*(char *)(lVar13 + uVar9 + 0x3b9) == '\0')) {
          local_68 = 0;
          uStack_60 = 0;
          local_78 = 0;
          local_70 = 0;
          local_80 = 0;
          init_wait_entry(&local_80,0);
          uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
          if (*piVar1 == *piVar2) {
            do {
              uVar15 = (ulong)*(ushort *)(param_2 + 2);
              if (0x10 < uVar15) goto LAB_00106c9c;
              if (((*(char *)(lVar13 + uVar15 + 0x3a8) == '\x03') || (*(int *)(lVar13 + 0x3cc) == 0)
                  ) || (*(char *)(lVar13 + uVar15 + 0x3b9) != '\0')) break;
              if (uVar9 != 0) {
                if (-1 < (int)uVar9) goto LAB_00106820;
                puVar6 = &DAT_00101337;
                goto LAB_001068ec;
              }
              schedule();
              uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
            } while (*piVar1 == *piVar2);
          }
          finish_wait(lVar14,&local_80);
        }
      }
      uVar9 = 0;
LAB_00106820:
      uVar15 = (ulong)*(ushort *)(param_2 + 2);
      if (0x10 < uVar15) goto LAB_00106c9c;
      if (*(char *)(lVar13 + uVar15 + 0x3a8) != '\x03') {
        if (*piVar1 != *piVar2) goto LAB_00106a70;
        uVar15 = (ulong)*(ushort *)(param_2 + 2);
      }
      printk(&DAT_00101258,"aud_smsg_recv",uVar15,*(undefined4 *)(lVar13 + 0x3cc));
      uVar10 = 0xffffffe0;
      if (*(int *)(lVar13 + 0x3cc) != 0) {
        uVar10 = 0xfffffffb;
      }
      uVar9 = (ulong)uVar10;
    }
    else {
      uVar9 = uVar15;
      if (*piVar1 == *piVar2) {
        if (0x10 < (ulong)*(ushort *)(param_2 + 2)) goto LAB_00106c9c;
        if ((*(char *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) + 0x3a8) != '\x03') &&
           (*(int *)(lVar13 + 0x3cc) != 0)) {
          local_68 = 0;
          uStack_60 = 0;
          local_78 = 0;
          local_70 = 0;
          local_80 = 0;
          init_wait_entry(&local_80,0);
          uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
          if (*piVar1 == *piVar2) {
            while( true ) {
              if (0x10 < (ulong)*(ushort *)(param_2 + 2)) goto LAB_00106c9c;
              if (*(char *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) + 0x3a8) == '\x03') break;
              if (*(int *)(lVar13 + 0x3cc) == 0 && uVar15 == 0) {
                uVar15 = 1;
              }
              if ((*(int *)(lVar13 + 0x3cc) == 0) || (uVar15 == 0)) goto LAB_001066cc;
              if (uVar9 != 0) goto LAB_001066e0;
              uVar15 = schedule_timeout(uVar15);
              uVar9 = prepare_to_wait_event(lVar14,&local_80,1);
              if (*piVar1 != *piVar2) break;
            }
          }
          if (uVar15 == 0) {
            uVar15 = 1;
          }
LAB_001066cc:
          finish_wait(lVar14,&local_80);
          uVar9 = uVar15;
        }
      }
LAB_001066e0:
      if ((int)uVar9 < 0) {
        puVar6 = &DAT_00101456;
LAB_001068ec:
        printk(puVar6,"aud_smsg_recv");
      }
      else if ((int)uVar9 == 0) {
        local_70 = local_70 & 0xffffffff00000000;
        local_80 = 0;
        local_78 = 0;
        printk(&DAT_00101ec5,"aud_smsg_recv");
        agdsp_access_dumpreg();
        printk(&DAT_001022d6);
        uVar7 = _raw_spin_lock_irqsave(lVar13 + 0x68);
        uVar8 = _raw_spin_lock_irqsave(lVar13 + 0x6c);
        *(undefined4 *)(lVar13 + 0x3cc) = 0;
        aud_smsg_dump_func(1);
        **(undefined4 **)(lVar13 + 0x28) = 0;
        **(undefined4 **)(lVar13 + 0x20) = 0;
        **(undefined4 **)(lVar13 + 0x48) = 0;
        **(undefined4 **)(lVar13 + 0x40) = 0;
        local_80 = 0x26000b0026;
        aud_smsg_ch_send(lVar13,&local_80);
        local_80._0_4_ = CONCAT22(2,(undefined2)local_80);
        aud_smsg_ch_send(lVar13,&local_80);
        _raw_spin_unlock_irqrestore(lVar13 + 0x6c,uVar8);
        _raw_spin_unlock_irqrestore(lVar13 + 0x68,uVar7);
        lVar11 = 0;
        do {
          if (*(char *)(lVar13 + 0x298 + lVar11 + 0x110) == '\x02') {
            __wake_up(*(undefined8 *)(lVar13 + 0x298 + lVar11 * 8),1,0,0);
          }
          lVar11 = lVar11 + 1;
        } while (lVar11 != 0x11);
        uVar9 = 0xffffffc2;
      }
      else {
        uVar15 = (ulong)*(ushort *)(param_2 + 2);
        if (0x10 < uVar15) goto LAB_00106c9c;
        if (*(char *)(lVar13 + uVar15 + 0x3a8) != '\x03') {
          if (*piVar1 != *piVar2) goto LAB_00106a70;
          uVar15 = (ulong)*(ushort *)(param_2 + 2);
        }
        printk(&DAT_00101700,"aud_smsg_recv",*(undefined4 *)(lVar13 + 0x3cc),uVar15);
        uVar9 = 0xfffffffb;
      }
    }
  }
  if ((ulong)*(ushort *)(param_2 + 2) < 0x11) {
    *(undefined1 *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) + 0x3b9) = 0;
    mutex_unlock(lVar14 + 0x18);
    if ((ulong)*(ushort *)(param_2 + 2) < 0x12) {
      piVar1 = (int *)(lVar13 + (ulong)*(ushort *)(param_2 + 2) * 4 + 800);
      Hint_Prefetch(piVar1,2,0,1);
      do {
        cVar3 = '\x01';
        bVar4 = (bool)ExclusiveMonitorPass(piVar1,0x10);
        if (bVar4) {
          *piVar1 = *piVar1 + -1;
          cVar3 = ExclusiveMonitorsStatus();
        }
      } while (cVar3 != '\0');
LAB_00106bf0:
      if (___stack_chk_guard == local_58) {
        return uVar9 & 0xffffffff;
      }
                    /* WARNING: Subroutine does not return */
      __stack_chk_fail();
    }
  }
LAB_00106c9c:
                    /* WARNING: Subroutine does not return */
  __ubsan_handle_cfi_check_fail_abort(0x12);
}



