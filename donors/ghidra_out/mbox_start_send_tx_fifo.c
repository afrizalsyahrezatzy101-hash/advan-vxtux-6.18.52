// mbox_start_send_tx_fifo @ 00102178
// signature: undefined mbox_start_send_tx_fifo(void)
// calling convention: unknown


void mbox_start_send_tx_fifo(undefined1 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  mbox_tx_mask = param_1;
  __wake_up(&deliver_thread_wait,3,1,0);
  return;
}


