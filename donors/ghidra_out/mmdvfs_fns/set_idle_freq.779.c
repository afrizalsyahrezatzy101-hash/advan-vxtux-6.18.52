====================set_idle_freq.779@00111184====================
// signature: undefined set_idle_freq.779(void)

undefined8 set_idle_freq_779(undefined8 param_1,ulong param_2)

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
  *(uint *)(g_mmreg_map + 0x1ec) = *(uint *)(g_mmreg_map + 0x1ec) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}


