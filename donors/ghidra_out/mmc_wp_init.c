// mmc_wp_init @ 001090f0
// signature: undefined mmc_wp_init(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int mmc_wp_init(long param_1)

{
  long *plVar1;
  int iVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  char local_88 [64];
  long local_48;
  
  *in_x18 = unaff_x30;
  iVar2 = 0;
  local_48 = ___stack_chk_guard;
  local_88[0x30] = '\0';
  local_88[0x31] = '\0';
  local_88[0x32] = '\0';
  local_88[0x33] = '\0';
  local_88[0x34] = '\0';
  local_88[0x35] = '\0';
  local_88[0x36] = '\0';
  local_88[0x37] = '\0';
  local_88[0x38] = '\0';
  local_88[0x39] = '\0';
  local_88[0x3a] = '\0';
  local_88[0x3b] = '\0';
  local_88[0x3c] = '\0';
  local_88[0x3d] = '\0';
  local_88[0x3e] = '\0';
  local_88[0x3f] = '\0';
  local_88[0x20] = '\0';
  local_88[0x21] = '\0';
  local_88[0x22] = '\0';
  local_88[0x23] = '\0';
  local_88[0x24] = '\0';
  local_88[0x25] = '\0';
  local_88[0x26] = '\0';
  local_88[0x27] = '\0';
  local_88[0x28] = '\0';
  local_88[0x29] = '\0';
  local_88[0x2a] = '\0';
  local_88[0x2b] = '\0';
  local_88[0x2c] = '\0';
  local_88[0x2d] = '\0';
  local_88[0x2e] = '\0';
  local_88[0x2f] = '\0';
  local_88[0x10] = '\0';
  local_88[0x11] = '\0';
  local_88[0x12] = '\0';
  local_88[0x13] = '\0';
  local_88[0x14] = '\0';
  local_88[0x15] = '\0';
  local_88[0x16] = '\0';
  local_88[0x17] = '\0';
  local_88[0x18] = '\0';
  local_88[0x19] = '\0';
  local_88[0x1a] = '\0';
  local_88[0x1b] = '\0';
  local_88[0x1c] = '\0';
  local_88[0x1d] = '\0';
  local_88[0x1e] = '\0';
  local_88[0x1f] = '\0';
  local_88[0] = '\0';
  local_88[1] = '\0';
  local_88[2] = '\0';
  local_88[3] = '\0';
  local_88[4] = '\0';
  local_88[5] = '\0';
  local_88[6] = '\0';
  local_88[7] = '\0';
  local_88[8] = '\0';
  local_88[9] = '\0';
  local_88[10] = '\0';
  local_88[0xb] = '\0';
  local_88[0xc] = '\0';
  local_88[0xd] = '\0';
  local_88[0xe] = '\0';
  local_88[0xf] = '\0';
  if ((*(int *)(param_1 + 0x3b0) == 0) && (wp_dat == '\0')) {
    if (wp_bus_type == (char *)0x0) {
      wp_bus_type = "mmc_wp";
      iVar2 = bus_register();
      if (iVar2 < 0) {
        lVar3 = *(long *)(param_1 + 0x78);
        if (lVar3 == 0) {
          lVar3 = *(long *)(param_1 + 8);
        }
        printk(&DAT_0010104d,lVar3);
        wp_bus_type = (char *)0x0;
        goto LAB_00109140;
      }
    }
    lVar3 = *(long *)(param_1 + 0x78);
    plVar1 = (long *)(param_1 + 8);
    if (lVar3 == 0) {
      lVar3 = *plVar1;
    }
    snprintf(local_88,0x40,"%s_wp",lVar3);
    iVar2 = alloc_chrdev_region(&DAT_00100640,0,0x100,local_88);
    if (iVar2 < 0) {
      lVar3 = *(long *)(param_1 + 0x78);
      if (lVar3 == 0) {
        lVar3 = *plVar1;
      }
      printk(&DAT_00101d49,lVar3);
    }
    else {
      DAT_00100290 = &wp_bus_type;
      DAT_00100520 = DAT_00100640 & 0xfff00000 | 1;
      DAT_00100270 = plVar1;
      DAT_00100280 = local_88;
      device_initialize(&DAT_00100210);
      DAT_001002a8 = &wp_dat;
      cdev_init(&DAT_001005b8,&wp_fileops);
      DAT_00100618 = &__this_module;
      iVar2 = cdev_device_add(&DAT_001005b8,&DAT_00100210);
      if (iVar2 == 0) {
        lVar3 = *(long *)(param_1 + 0x78);
        if (lVar3 == 0) {
          lVar3 = *plVar1;
        }
        DAT_00100648 = param_1;
        printk(&DAT_00101edf,lVar3,DAT_00100640 >> 0x14);
        iVar2 = 0;
        wp_dat = '\x01';
        DAT_00100650 = 1;
        goto LAB_00109140;
      }
      lVar3 = *(long *)(param_1 + 0x78);
      if (lVar3 == 0) {
        lVar3 = *plVar1;
      }
      printk(&DAT_001010b3,lVar3);
      put_device(&DAT_00100210);
      unregister_chrdev_region(DAT_00100640,0x100);
    }
    if (wp_dat == '\0') {
      bus_unregister(&wp_bus_type);
      wp_bus_type = (char *)0x0;
    }
  }
LAB_00109140:
  if (___stack_chk_guard == local_48) {
    return iVar2;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


