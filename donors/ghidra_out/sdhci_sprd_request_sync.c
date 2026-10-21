// sdhci_sprd_request_sync @ 00103030
// signature: undefined sdhci_sprd_request_sync(void)
// calling convention: unknown


undefined4 sdhci_sprd_request_sync(long param_1,long *param_2)

{
  long lVar1;
  ulong uVar2;
  undefined4 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((*(uint *)(param_1 + 0x9f4) >> 3 & 1) == 0) {
    lVar1 = 8;
    if (*param_2 != 0) {
      lVar1 = (ulong)(*(uint *)(param_1 + 0x9f4) >> 7 & 1) << 3;
    }
    uVar2 = sdhci_sprd_send_command(param_1 + 0x740,*(undefined8 *)((long)param_2 + lVar1));
    uVar3 = 0;
    if ((uVar2 & 1) == 0) {
      uVar3 = 0xfffffff0;
    }
  }
  else {
    *(undefined4 *)(param_2[1] + 0x20) = 0xffffff85;
    __sdhci_sprd_finish_mrq(param_1 + 0x740);
    queue_work_on(0x20,*(undefined8 *)(param_1 + 0xad8),param_1 + 0xae0);
    uVar3 = 0;
  }
  return uVar3;
}


