// vser_function_disable @ 00101f58
// signature: undefined vser_function_disable(void)
// calling convention: unknown


void vser_function_disable(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(undefined8 *)(param_1 + 0xf8) = 0x100000000;
  *(undefined4 *)(param_1 + 0x100) = 1;
  usb_ep_disable(*(undefined8 *)(param_1 + 0xe8));
  usb_ep_disable(*(undefined8 *)(param_1 + 0xf0));
  __wake_up(param_1 + 0x138,3,1,0);
  __wake_up(param_1 + 0x150,3,1,0);
  return;
}


