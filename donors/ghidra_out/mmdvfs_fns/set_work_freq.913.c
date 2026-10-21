====================set_work_freq.913@00113198====================
// signature: undefined set_work_freq.913(void)

undefined8 set_work_freq_913(undefined8 param_1,ulong param_2)

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
  *(uint *)(g_mmreg_map + 0x1e0) = *(uint *)(g_mmreg_map + 0x1e0) & 0xfffffff8 | uVar1;
  printk(&DAT_00105f64,"set_work_freq",param_2);
  return 1;
}


