// _sdhci_request_atomic @ 00107a2c
// signature: undefined _sdhci_request_atomic(void)
// calling convention: unknown


undefined4 _sdhci_request_atomic(long param_1,long *param_2)

{
  long lVar1;
  undefined8 uVar2;
  ulong uVar3;
  undefined4 uVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = _raw_spin_lock_irqsave(param_1 + 0x9f0);
  if ((*(uint *)(param_1 + 0x9f4) >> 3 & 1) == 0) {
    lVar1 = 8;
    if (*param_2 != 0) {
      lVar1 = (ulong)(*(uint *)(param_1 + 0x9f4) >> 7 & 1) << 3;
    }
    uVar3 = sdhci_sprd_send_command(param_1 + 0x740,*(undefined8 *)((long)param_2 + lVar1));
    uVar4 = 0;
    if ((uVar3 & 1) == 0) {
      uVar4 = 0xfffffff0;
    }
  }
  else {
    *(undefined4 *)(param_2[1] + 0x20) = 0xffffff85;
    __sdhci_sprd_finish_mrq(param_1 + 0x740,param_2);
    queue_work_on(0x20,*(undefined8 *)(param_1 + 0xad8),param_1 + 0xae0);
    uVar4 = 0;
  }
  _raw_spin_unlock_irqrestore(param_1 + 0x9f0,uVar2);
  return uVar4;
}


