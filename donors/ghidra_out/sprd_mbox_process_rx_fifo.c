// sprd_mbox_process_rx_fifo @ 00104228
// signature: undefined sprd_mbox_process_rx_fifo(void)
// calling convention: unknown


void sprd_mbox_process_rx_fifo(long param_1)

{
  ulong uVar1;
  undefined8 *in_x18;
  ulong uVar2;
  long lVar3;
  undefined *puVar4;
  int iVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = (ulong)mbox_rx_fifo_cnt;
  if (mbox_rx_fifo_cnt != 0) {
    lVar3 = *(long *)(param_1 + 0xf0);
    uVar2 = 0;
    iVar5 = 0;
    puVar4 = &DAT_001001e8;
    do {
      if (uVar2 == 0x40) {
        __ubsan_handle_cfi_check_fail_abort(0x12);
        init_module();
        return;
      }
      if (*(long *)(puVar4 + -8) == 8) {
LAB_00104288:
        iVar5 = iVar5 + 1;
      }
      else if ((*(long *)(puVar4 + -8) == lVar3) && (*(long *)(param_1 + 0x10) != 0)) {
        mbox_chan_received_data(param_1,puVar4);
        uVar1 = (ulong)mbox_rx_fifo_cnt;
        *(undefined8 *)(puVar4 + -8) = 8;
        goto LAB_00104288;
      }
      uVar2 = uVar2 + 1;
      puVar4 = puVar4 + 0x10;
    } while (uVar2 < uVar1);
    if (iVar5 != (int)uVar1) {
      return;
    }
  }
  mbox_rx_fifo_cnt = 0;
  return;
}


