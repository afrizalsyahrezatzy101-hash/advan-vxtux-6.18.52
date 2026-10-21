// vser_function_bind @ 00101afc
// signature: undefined vser_function_bind(void)
// calling convention: unknown


undefined8 vser_function_bind(long param_1,long param_2)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  *(undefined8 *)(param_2 + 0xd8) = *(undefined8 *)(param_1 + 0x30);
  uVar1 = usb_interface_id();
  if (-1 < (int)uVar1) {
    DAT_001005ba = (undefined1)uVar1;
    uVar1 = vser_create_bulk_endpoints(param_2);
    if ((int)uVar1 == 0) {
      DAT_001005cc = DAT_001005c3;
      DAT_001005de = DAT_001005d5;
      DAT_001005e7 = DAT_001005c3;
      DAT_001005f0 = DAT_001005d5;
      uVar1 = 0;
    }
  }
  return uVar1;
}


