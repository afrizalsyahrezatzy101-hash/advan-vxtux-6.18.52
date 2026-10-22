// notify_cancel_tui @ 00101030
// signature: undefined notify_cancel_tui(void)
// calling convention: unknown


void notify_cancel_tui(void)

{
  undefined4 *puVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (s_ts != (undefined4 *)0x0) {
    puVar1 = s_ts + 4;
    *s_ts = 1;
    __wake_up(puVar1,1,1,0);
  }
  return;
}


