// dpu_dvfs_notify_callback @ 00108b60
// signature: undefined dpu_dvfs_notify_callback(void)
// calling convention: unknown


undefined8 dpu_dvfs_notify_callback(long param_1,undefined8 param_2,uint *param_3)

{
  uint uVar1;
  long lVar2;
  undefined8 uVar3;
  code *pcVar4;
  undefined8 *in_x18;
  ulong uVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(int *)(param_1 + -0x3c8) == 0) {
    uVar3 = 0;
  }
  else {
    uVar1 = *param_3;
    uVar5 = (ulong)uVar1;
    if ((*(long *)(param_1 + 0xb8) != 0) &&
       (pcVar4 = *(code **)(*(long *)(param_1 + 0xb8) + 0x38), pcVar4 != (code *)0x0)) {
      if (0xe < ((ulong)(pcVar4 + -0x117c84) >> 2 | (long)pcVar4 << 0x3e)) {
        __cfi_slowpath(0x8e0c05c20dd39ba0,pcVar4);
      }
      (*pcVar4)((int *)(param_1 + -0x3c8),uVar5);
    }
    lVar2 = *(long *)(param_1 + -0x18);
    if (*(long *)(*(long *)(lVar2 + 0x3d8) + 0x30) != 0) {
      devfreq_update_status(lVar2,uVar5);
      lVar2 = *(long *)(param_1 + -0x18);
    }
    *(uint *)(param_1 + 0x18) = uVar1;
    *(undefined4 *)(param_1 + 0x20) = 0;
    *(ulong *)(lVar2 + 0x498) = uVar5;
    uVar3 = 1;
  }
  return uVar3;
}


