// sprd_mbox_debug_show @ 00103e0c
// signature: undefined sprd_mbox_debug_show(void)
// calling convention: unknown


undefined8 sprd_mbox_debug_show(long param_1)

{
  undefined8 *in_x18;
  int iVar1;
  long lVar2;
  int iVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = *(long *)(param_1 + 0x78);
  iVar1 = 0x6e;
  do {
    seq_putc(param_1,0x2a);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  seq_putc(param_1,10);
  seq_printf(param_1,"CHAN_NUM : %d\n",*(undefined4 *)(lVar2 + 0x18));
  seq_printf(param_1,"INBOX_IRQ : %d\n",*(undefined4 *)(lVar2 + 0xbc));
  seq_printf(param_1,"OUTBOX_IRQ : %d\n",*(undefined4 *)(lVar2 + 0xc0));
  seq_printf(param_1,"OOB_IRQ : %d\n",*(undefined4 *)(lVar2 + 0xd0));
  seq_printf(param_1,"OOB_ID : %ld\n",*(undefined8 *)(lVar2 + 0xd8));
  seq_printf(param_1,"VERSION : 0x%x\n",*(undefined4 *)(lVar2 + 0xb8));
  seq_printf(param_1,"mbox_rx_fifo_cnt : %d\n",mbox_rx_fifo_cnt);
  seq_printf(param_1,"started_chan_msk : 0x%02x\n",g_started_chan_msk);
  iVar1 = 0;
  do {
    iVar3 = 0x6e;
    do {
      seq_putc(param_1,0x2a);
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    seq_putc(param_1,10);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    seq_printf(param_1,
               "[INBOX %d]\n\t id: 0x%08x | msg_low: 0x%08x | msg_high:  0x%08x\n\t fifo_reset: 0x%08x | fifo_status: 0x%08x | irq_status: 0x%08x | irq_mask: 0x%08x\n"
               ,iVar1);
    iVar3 = 100;
    do {
      seq_putc(param_1,0x2d);
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    seq_putc(param_1,10);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    DataMemoryBarrier(0,1);
    seq_printf(param_1,
               "[OUTBOX %d]\n\t id: 0x%08x | msg_low: 0x%08x | msg_high:  0x%08x\n\t fifo_reset: 0x%08x | fifo_status: 0x%08x | irq_status: 0x%08x | irq_mask: 0x%08x\n\t depth: 0x%08x\n"
               ,iVar1);
    iVar1 = iVar1 + 1;
  } while (iVar1 != 8);
  iVar1 = 0x6e;
  do {
    seq_putc(param_1,0x2a);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  seq_putc(param_1,10);
  return 0;
}


