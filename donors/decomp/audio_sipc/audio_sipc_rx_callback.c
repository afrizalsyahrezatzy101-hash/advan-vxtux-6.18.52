// signature: undefined audio_sipc_rx_callback(void)

void audio_sipc_rx_callback(undefined8 param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  aud_smsg_irq_handler(param_1,DAT_001009a0);
  return;
}



