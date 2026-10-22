====================set_idle_freq.835@00112264====================
// signature: undefined set_idle_freq.835(void)

undefined8 set_idle_freq_835(undefined8 param_1,ulong param_2)

{
  uint uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (DAT_001043cc < param_2) {
    if (DAT_00104404 < param_2) {
      if (DAT_0010443c < param_2) {
        if (DAT_00104474 < param_2) {
          if (DAT_001044ac < param_2) {
            if (DAT_001044e4 < param_2) {
              uVar1 = 6;
              if (DAT_0010451c < param_2) {
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
  *(uint *)(g_mmreg_map + 0x1dc) = *(uint *)(g_mmreg_map + 0x1dc) & 0xfffffff8 | uVar1;
  printk(&DAT_00105348,"set_idle_freq",param_2);
  return 0;
}


