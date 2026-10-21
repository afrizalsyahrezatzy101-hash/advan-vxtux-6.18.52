====================set_idle_freq@0010f248====================
// signature: undefined set_idle_freq(void)

undefined8 set_idle_freq(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103e8c < param_2) {
    if (DAT_00103ec4 < param_2) {
      if (DAT_00103efc < param_2) {
        if (DAT_00103f34 < param_2) {
          if (DAT_00103f6c < param_2) {
            if (DAT_00103fa4 < param_2) {
              uVar1 = 6;
              if (DAT_00103fdc < param_2) {
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
  *(uint *)(g_mmreg_map + 500) = *(uint *)(g_mmreg_map + 500) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}


