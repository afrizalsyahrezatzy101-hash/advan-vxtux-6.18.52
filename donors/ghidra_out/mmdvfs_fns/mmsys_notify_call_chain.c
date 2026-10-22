====================mmsys_notify_call_chain@0010c588====================
// signature: undefined mmsys_notify_call_chain(void)

undefined8 mmsys_notify_call_chain(int param_1)

{
  int iVar1;
  char cVar2;
  bool bVar3;
  undefined *puVar4;
  undefined8 *in_x18;
  undefined8 uVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010489d,"mmsys_notify_call_chain",param_1);
  printk(&DAT_001057b7,"mmsys_notify_call_chain",mmsys_notify_call_chain_pw_opened);
  if (param_1 == 2) {
    Hint_Prefetch(0x100c40,2,0,1);
    do {
      iVar1 = mmsys_notify_call_chain_pw_opened + -1;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x100c40,0x10);
      if (bVar3) {
        cVar2 = ExclusiveMonitorsStatus();
        mmsys_notify_call_chain_pw_opened = iVar1;
      }
    } while (cVar2 != '\0');
    DataMemoryBarrier(2,3);
    uVar5 = 2;
    if (iVar1 == 0) goto LAB_0010c6b4;
    puVar4 = &DAT_00105c42;
  }
  else {
    if (param_1 != 1) {
      return 0xffffffea;
    }
    Hint_Prefetch(0x100c40,2,0,1);
    do {
      iVar1 = mmsys_notify_call_chain_pw_opened + 1;
      cVar2 = '\x01';
      bVar3 = (bool)ExclusiveMonitorPass(0x100c40,0x10);
      if (bVar3) {
        cVar2 = ExclusiveMonitorsStatus();
        mmsys_notify_call_chain_pw_opened = iVar1;
      }
    } while (cVar2 != '\0');
    DataMemoryBarrier(2,3);
    if (1 < iVar1) {
      printk(&DAT_001057d3);
      Hint_Prefetch(0x100c40,2,0,1);
      do {
        cVar2 = '\x01';
        bVar3 = (bool)ExclusiveMonitorPass(0x100c40,0x10);
        if (bVar3) {
          cVar2 = ExclusiveMonitorsStatus();
          mmsys_notify_call_chain_pw_opened = mmsys_notify_call_chain_pw_opened + -1;
        }
      } while (cVar2 != '\0');
      return 0xffffffe8;
    }
    puVar4 = &DAT_00105748;
    uVar5 = 1;
  }
  printk(puVar4,"mmsys_notify_call_chain",mmsys_notify_call_chain_pw_opened);
LAB_0010c6b4:
  blocking_notifier_call_chain(&mmsys_notifier_list,uVar5,0);
  printk(&DAT_00105ff4,"mmsys_notify_call_chain",param_1);
  return 0;
}


