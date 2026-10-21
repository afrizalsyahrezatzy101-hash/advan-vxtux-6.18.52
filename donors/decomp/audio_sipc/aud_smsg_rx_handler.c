// signature: undefined aud_smsg_rx_handler(void)

void aud_smsg_rx_handler(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  aud_smsg_irq_handler(param_1,DAT_001009a0);
  return;
}



