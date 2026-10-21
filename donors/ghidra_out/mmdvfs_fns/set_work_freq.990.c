====================set_work_freq.990@0011420c====================
// signature: undefined set_work_freq.990(void)

undefined8 set_work_freq_990(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_0010420c < param_2) {
    if (DAT_00104244 < param_2) {
      if (DAT_0010427c < param_2) {
        if (DAT_001042b4 < param_2) {
          if (DAT_001042ec < param_2) {
            if (DAT_00104324 < param_2) {
              uVar1 = 6;
              if (DAT_0010435c < param_2) {
                uVar1 = 7;
              }
            }
            else {
              uVar1 = 5;
            }
          }
          else {
            uVar1 = 4;
          }
        }
        else {
          uVar1 = 3;
        }
      }
      else {
        uVar1 = 2;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 0;
  }
  *(uint *)(g_mmreg_map + 0x1c8) = *(uint *)(g_mmreg_map + 0x1c8) & 0xfffffff8 | uVar1;
  printk(&DAT_00105a0e,"set_work_freq",param_2);
  return 0;
}


