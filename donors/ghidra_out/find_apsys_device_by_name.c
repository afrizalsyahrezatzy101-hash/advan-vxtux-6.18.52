// find_apsys_device_by_name @ 00106180
// signature: undefined find_apsys_device_by_name(void)
// calling convention: unknown


long find_apsys_device_by_name(undefined8 param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = of_find_node_by_name(0,param_1);
  if (lVar1 == 0) {
    printk(&DAT_0010240e,param_1);
  }
  else {
    lVar1 = of_find_device_by_node();
    if (lVar1 == 0) {
      printk(&DAT_00102ffb,param_1);
    }
    else {
      lVar1 = *(long *)(lVar1 + 0xa8);
      if (lVar1 != 0) {
        printk(&DAT_00103651,param_1,*(undefined8 *)(lVar1 + 0x3d8));
        return lVar1;
      }
    }
    printk(&DAT_00102d61);
    dump_stack();
  }
  return 0;
}


