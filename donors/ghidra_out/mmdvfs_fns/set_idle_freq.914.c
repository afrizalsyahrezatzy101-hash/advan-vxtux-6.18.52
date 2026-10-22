====================set_idle_freq.914@001132dc====================
// signature: undefined set_idle_freq.914(void)

undefined8 set_idle_freq_914(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_00103ccc < param_2) {
    if (DAT_00103d04 < param_2) {
      if (DAT_00103d3c < param_2) {
        if (DAT_00103d74 < param_2) {
          if (DAT_00103dac < param_2) {
            if (DAT_00103de4 < param_2) {
              uVar1 = 6;
              if (DAT_00103e1c < param_2) {
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
  *(uint *)(g_mmreg_map + 0x1e4) = *(uint *)(g_mmreg_map + 0x1e4) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 1;
}


