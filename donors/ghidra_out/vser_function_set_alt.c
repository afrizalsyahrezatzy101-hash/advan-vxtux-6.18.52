// vser_function_set_alt @ 00101cdc
// signature: undefined vser_function_set_alt(void)
// calling convention: unknown


int vser_function_set_alt(long param_1)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 *puVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar2 = *(undefined8 **)(*(long *)(param_1 + 0x30) + 0x30);
  iVar1 = config_ep_by_speed(*puVar2,param_1,*(undefined8 *)(param_1 + 0xe8));
  if (iVar1 == 0) {
    iVar1 = usb_ep_enable(*(undefined8 *)(param_1 + 0xe8));
    if (iVar1 == 0) {
      iVar1 = config_ep_by_speed(*puVar2,param_1,*(undefined8 *)(param_1 + 0xf0));
      if (iVar1 == 0) {
        iVar1 = usb_ep_enable(*(undefined8 *)(param_1 + 0xf0));
        if (iVar1 != 0) {
          usb_ep_disable(*(undefined8 *)(param_1 + 0xe8));
        }
      }
    }
  }
  return iVar1;
}


