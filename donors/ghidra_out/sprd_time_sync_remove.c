// sprd_time_sync_remove @ 00101508
// signature: undefined sprd_time_sync_remove(void)
// calling convention: unknown


undefined8 sprd_time_sync_remove(long param_1)

{
  undefined8 *in_x18;
  long lVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(param_1 + 0xa8);
  device_destroy(sprd_time_sync_class,*(uint *)(lVar1 + 0xc) | *(int *)(lVar1 + 8) << 0x14);
  cdev_del(lVar1 + 0x10);
  unregister_chrdev_region(*(uint *)(lVar1 + 0xc) | *(int *)(lVar1 + 8) << 0x14,1);
  *(undefined8 *)(param_1 + 0xa8) = 0;
  class_destroy(sprd_time_sync_class);
  return 0;
}


