// signature: undefined aud_smsg_ch_close(void)

undefined8 aud_smsg_ch_close(byte param_1,ushort param_2)

{
  long lVar1;
  long lVar2;
  int *piVar3;
  long *plVar4;
  int iVar5;
  char cVar6;
  bool bVar7;
  undefined8 uVar8;
  undefined8 *in_x18;
  long lVar9;
  long lVar10;
  uint uVar11;
  ulong uVar12;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_2 < 0x11) {
    if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
      __ubsan_handle_cfi_check_fail_abort(0x12);
    }
    lVar10 = (&aud_smsg_ipcs)[param_1];
    if (lVar10 != 0) {
      lVar1 = lVar10 + (ulong)param_2 * 8;
      lVar9 = *(long *)(lVar1 + 0x298);
      if (lVar9 != 0) {
        uVar12 = (ulong)param_2;
        lVar2 = lVar10 + uVar12 * 0x20 + 0x78;
        mutex_lock(lVar2);
        piVar3 = (int *)(lVar10 + uVar12 * 4 + 0x364);
        Hint_Prefetch(piVar3,2,0,1);
        do {
          iVar5 = *piVar3;
          cVar6 = '\x01';
          bVar7 = (bool)ExclusiveMonitorPass(piVar3,0x10);
          if (bVar7) {
            *piVar3 = iVar5 + -1;
            cVar6 = ExclusiveMonitorsStatus();
          }
        } while (cVar6 != '\0');
        DataMemoryBarrier(2,3);
        if (iVar5 + -1 == 0) {
          plVar4 = (long *)(lVar1 + 0x298);
          *(undefined1 *)(lVar10 + uVar12 + 0x3a8) = 3;
          if (*plVar4 != 0) {
            lVar1 = lVar10 + uVar12 * 4;
            if (*(int *)(lVar1 + 800) != 0) {
              uVar11 = 0;
              do {
                printk(&DAT_001021d4,param_2);
                __wake_up(lVar9,3,1,0);
                msleep(1000);
                if (0x62 < uVar11) break;
                uVar11 = uVar11 + 1;
              } while (*(int *)(lVar1 + 800) != 0);
            }
            kfree(lVar9);
            *plVar4 = 0;
          }
          *(undefined1 *)(lVar10 + uVar12 + 0x3a8) = 0;
        }
        mutex_unlock(lVar2);
      }
    }
    uVar8 = 0;
  }
  else {
    uVar8 = 0xffffffff;
  }
  return uVar8;
}



