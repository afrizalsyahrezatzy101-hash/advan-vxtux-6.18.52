// sprd_time_sync_fn @ 00101030
// signature: undefined sprd_time_sync_fn(void)
// calling convention: unknown


undefined8 sprd_time_sync_fn(void)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  sprd_send_ap_time();
  return 1;
}


