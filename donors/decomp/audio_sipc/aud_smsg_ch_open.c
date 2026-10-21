// signature: undefined aud_smsg_ch_open(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 aud_smsg_ch_open(byte param_1,ushort param_2)

{
  long lVar1;
  int *piVar2;
  long lVar3;
  char cVar4;
  bool bVar5;
  undefined8 uVar6;
  long lVar7;
  undefined8 *in_x18;
  long lVar8;
  ulong uVar9;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (1 < param_1) {
                    /* WARNING: Subroutine does not return */
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
  if (param_2 < 0x11) {
    lVar8 = (&aud_smsg_ipcs)[param_1];
    if (lVar8 == 0) {
      printk(&DAT_00101a91,"aud_smsg_ch_open");
      uVar6 = 0xfffffdfb;
    }
    else {
      uVar9 = (ulong)param_2;
      lVar1 = lVar8 + uVar9 * 0x20 + 0x78;
      mutex_lock(lVar1);
      if (*(char *)(lVar8 + uVar9 + 0x3a8) != '\x02') {
        lVar7 = kmem_cache_alloc_trace(_DAT_00109198,0xdc0,0x1440);
        if (lVar7 == 0) {
          mutex_unlock(lVar1);
          return 0xfffffff4;
        }
        lVar3 = lVar8 + uVar9 * 4;
        piVar2 = (int *)(lVar3 + 800);
        *(undefined4 *)(lVar3 + 800) = 1;
        __init_waitqueue_head(lVar7,"&(ch->rxwait)",&audio_sipc_driver);
        __mutex_init(lVar7 + 0x18,"&(ch->rxlock)",&audio_sipc_driver);
        *(long *)(lVar8 + uVar9 * 8 + 0x298) = lVar7;
        *(undefined1 *)(lVar8 + uVar9 + 0x3a8) = 2;
        Hint_Prefetch(piVar2,2,0,1);
        do {
          cVar4 = '\x01';
          bVar5 = (bool)ExclusiveMonitorPass(piVar2,0x10);
          if (bVar5) {
            *piVar2 = *piVar2 + -1;
            cVar4 = ExclusiveMonitorsStatus();
          }
        } while (cVar4 != '\0');
      }
      piVar2 = (int *)(lVar8 + uVar9 * 4 + 0x364);
      Hint_Prefetch(piVar2,2,0,1);
      do {
        cVar4 = '\x01';
        bVar5 = (bool)ExclusiveMonitorPass(piVar2,0x10);
        if (bVar5) {
          *piVar2 = *piVar2 + 1;
          cVar4 = ExclusiveMonitorsStatus();
        }
      } while (cVar4 != '\0');
      mutex_unlock(lVar1);
      uVar6 = 0;
    }
  }
  else {
    uVar6 = 0xffffffff;
  }
  return uVar6;
}



