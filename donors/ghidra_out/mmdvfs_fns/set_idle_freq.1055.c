====================set_idle_freq.1055@00115424====================
// signature: undefined set_idle_freq.1055(void)

undefined8 set_idle_freq_1055(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_0010394c < param_2) {
    if (DAT_00103984 < param_2) {
      if (DAT_001039bc < param_2) {
        if (DAT_001039f4 < param_2) {
          if (DAT_00103a2c < param_2) {
            if (DAT_00103a64 < param_2) {
              uVar1 = 6;
              if (DAT_00103a9c < param_2) {
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
  *(uint *)(g_mmreg_map + 0x1d4) = *(uint *)(g_mmreg_map + 0x1d4) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}


