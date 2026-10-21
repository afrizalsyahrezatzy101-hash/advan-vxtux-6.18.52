// get_display_power_status @ 00106030
// signature: undefined get_display_power_status(void)
// calling convention: unknown


undefined8 get_display_power_status(void)

{
  long lVar1;
  undefined *puVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = of_find_node_by_name(0,&DAT_00102b3b);
  if (lVar1 == 0) {
    puVar2 = &DAT_00102624;
  }
  else {
    lVar1 = of_find_device_by_node();
    if (lVar1 != 0) {
      if (*(char *)(*(long *)(*(long *)(*(long *)(lVar1 + 0xa8) + 0x3a8) + 0x670) + 9) != '\0') {
        platform_device_put(lVar1);
        return 1;
      }
      printk(&DAT_0010244c);
      platform_device_put(lVar1);
      return 0;
    }
    puVar2 = &DAT_00101c96;
  }
  printk(puVar2);
  return 0;
}


