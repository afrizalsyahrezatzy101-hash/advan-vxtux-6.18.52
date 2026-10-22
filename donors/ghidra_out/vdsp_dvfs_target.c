// vdsp_dvfs_target @ 0010b31c
// signature: undefined vdsp_dvfs_target(void)
// calling convention: unknown


ulong vdsp_dvfs_target(long param_1,ulong *param_2,undefined4 param_3)

{
  uint uVar1;
  ulong uVar2;
  ulong uVar3;
  long lVar4;
  undefined8 *in_x18;
  long lVar5;
  code *pcVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = *(long *)(param_1 + 0x98);
  printk(&DAT_00102284);
  uVar2 = devfreq_recommended_opp(param_1,param_2,param_3);
  if (uVar2 < 0xfffffffffffff001) {
    uVar1 = dev_pm_opp_get_freq(uVar2);
    dev_pm_opp_put(uVar2);
    lVar4 = *(long *)(lVar5 + 0x480);
    if (*(int *)(lVar5 + 1000) == 0) {
      if ((lVar4 != 0) && (*(long *)(lVar4 + 0x40) != 0)) {
        *(uint *)(lVar5 + 0x3e0) = uVar1;
        pcVar6 = *(code **)(lVar4 + 0x40);
        if (1 < ((ulong)(pcVar6 + -0x117fa8) >> 2 | (long)pcVar6 << 0x3e)) {
          __cfi_slowpath(0x5b28ba0bd9034e37,pcVar6);
        }
        uVar3 = (*pcVar6)(lVar5,uVar1);
        uVar2 = uVar3 & 0xffffffff;
        printk(&DAT_0010220b,uVar1,uVar2);
        if ((int)uVar3 != 0) goto LAB_0010b3ec;
      }
    }
    else if ((lVar4 != 0) && (*(long *)(lVar4 + 0x50) != 0)) {
      *(uint *)(lVar5 + 0x3e4) = uVar1;
      pcVar6 = *(code **)(lVar4 + 0x50);
      if (3 < ((ulong)(pcVar6 + -0x117fc8) >> 2 | (long)pcVar6 << 0x3e)) {
        __cfi_slowpath(0x4f4b54674264424e,pcVar6);
      }
      (*pcVar6)(lVar5,uVar1);
      printk(&DAT_001029d6,uVar1);
    }
    uVar2 = 0;
    *param_2 = (ulong)uVar1;
  }
  else {
    _dev_err(param_1,"failed to find opp for %lu KHz\n",*param_2);
  }
LAB_0010b3ec:
  return uVar2 & 0xffffffff;
}


