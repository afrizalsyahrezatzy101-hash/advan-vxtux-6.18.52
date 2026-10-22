// vdsp_dvfs_notify_callback @ 0010b270
// signature: undefined vdsp_dvfs_notify_callback(void)
// calling convention: unknown


undefined4 vdsp_dvfs_notify_callback(long param_1,undefined8 param_2,undefined4 *param_3)

{
  int iVar1;
  undefined8 *in_x18;
  undefined4 uVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = *param_3;
  mutex_lock(*(long *)(param_1 + -0x18) + 0x10);
  printk(&DAT_001027a5,"vdsp_dvfs_notify_callback");
  if (*(int *)(param_1 + -0x3c8) == 0) {
    printk(&DAT_001022b0);
    uVar2 = 0;
  }
  else {
    *(undefined4 *)(param_1 + 0x18) = uVar2;
    *(undefined4 *)(param_1 + 0x20) = 0;
    iVar1 = update_devfreq(*(undefined8 *)(param_1 + -0x18));
    if (iVar1 < 0) {
      printk(&DAT_00101c10);
    }
    printk(&DAT_00102c06,"vdsp_dvfs_notify_callback");
    uVar2 = 1;
  }
  mutex_unlock(*(long *)(param_1 + -0x18) + 0x10);
  return uVar2;
}


