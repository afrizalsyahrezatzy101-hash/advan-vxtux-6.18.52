// mmc_wp_remove @ 001098ac
// signature: undefined mmc_wp_remove(void)
// calling convention: unknown


void mmc_wp_remove(long param_1)

{
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if ((*(int *)(param_1 + 0x3b0) == 0) && (wp_dat != '\0')) {
    put_device(&DAT_00100210);
    unregister_chrdev_region(DAT_00100640,0x100);
    if (wp_dat == '\0') {
      bus_unregister(&wp_bus_type);
      wp_bus_type = 0;
    }
    wp_dat = '\0';
    DAT_00100650 = 0;
  }
  return;
}


