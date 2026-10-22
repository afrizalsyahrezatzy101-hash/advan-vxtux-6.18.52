====================set_work_freq.778@00111040====================
// signature: undefined set_work_freq.778(void)

undefined8 set_work_freq_778(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103b08._4_4_ < param_2) {
    if (DAT_00103b44 < param_2) {
      if (DAT_00103b7c < param_2) {
        if (DAT_00103bb4 < param_2) {
          if (DAT_00103bec < param_2) {
            if (DAT_00103c24 < param_2) {
              uVar1 = 6;
              if (DAT_00103c5c < param_2) {
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
  *(uint *)(g_mmreg_map + 0x1e8) = *(uint *)(g_mmreg_map + 0x1e8) & 0xfffffff8 | uVar1;
  printk(&DAT_001055b1,"set_work_freq",param_2);
  return 0;
}


