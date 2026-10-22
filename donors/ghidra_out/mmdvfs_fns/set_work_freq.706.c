====================set_work_freq.706@00110160====================
// signature: undefined set_work_freq.706(void)

undefined8 set_work_freq_706(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00104048._4_4_ < param_2) {
    if (DAT_00104084 < param_2) {
      if (DAT_001040bc < param_2) {
        if (DAT_001040f4 < param_2) {
          if (DAT_0010412c < param_2) {
            if (DAT_00104164 < param_2) {
              uVar1 = 6;
              if (DAT_0010419c < param_2) {
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
  *(uint *)(g_mmreg_map + 0x1c0) = *(uint *)(g_mmreg_map + 0x1c0) & 0xfffffff8 | uVar1;
  printk(&DAT_00105d2a,"set_work_freq",param_2);
  return 0;
}


