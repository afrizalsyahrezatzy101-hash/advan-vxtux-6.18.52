// vsp_dvfs_notify_callback @ 00109fe8
// signature: undefined vsp_dvfs_notify_callback(void)
// calling convention: unknown


undefined8 vsp_dvfs_notify_callback(long param_1,undefined8 param_2,undefined4 *param_3)

{
  undefined4 uVar1;
  int iVar2;
  undefined *puVar3;
  undefined8 *in_x18;
  undefined8 uVar4;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar1 = *param_3;
  mutex_lock(*(long *)(param_1 + -0xb0) + 0x10);
  if (*(int *)(param_1 + -0x14) == 0) {
    puVar3 = &DAT_00102252;
  }
  else {
    *(undefined4 *)(param_1 + -0xb8) = uVar1;
    *(undefined4 *)(param_1 + -8) = 0;
    iVar2 = update_devfreq(*(undefined8 *)(param_1 + -0xb0));
    if (iVar2 == 0) {
      uVar4 = 1;
      goto LAB_0010a02c;
    }
    puVar3 = &DAT_00103307;
  }
  printk(puVar3);
  uVar4 = 0;
LAB_0010a02c:
  mutex_unlock(*(long *)(param_1 + -0xb0) + 0x10);
  return uVar4;
}


