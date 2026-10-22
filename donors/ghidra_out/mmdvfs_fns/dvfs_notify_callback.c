====================dvfs_notify_callback@0010cce4====================
// signature: undefined dvfs_notify_callback(void)

undefined8 dvfs_notify_callback(long param_1,long param_2)

{
  code *pcVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (param_2 == 2) {
    if (*(long *)(param_1 + -0x2b0) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(*(long *)(param_1 + -0x2b0) + 0x90);
  }
  else {
    if (param_2 != 1) {
      return 0xffffffea;
    }
    if (*(long *)(param_1 + -0x2b0) == 0) {
      return 1;
    }
    pcVar1 = *(code **)(*(long *)(param_1 + -0x2b0) + 0x88);
  }
  if (pcVar1 != (code *)0x0) {
    if (0xf < ((ulong)(pcVar1 + -0x1177bc) >> 2 | (long)pcVar1 << 0x3e)) {
      __cfi_slowpath(0x24a34366d538acc6,pcVar1);
    }
    (*pcVar1)(*(undefined8 *)(param_1 + -0x2c0));
  }
  return 1;
}


