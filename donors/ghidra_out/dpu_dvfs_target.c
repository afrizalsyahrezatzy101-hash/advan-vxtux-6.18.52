// dpu_dvfs_target @ 00108c44
// signature: undefined dpu_dvfs_target(void)
// calling convention: unknown


ulong dpu_dvfs_target(long param_1,ulong *param_2,undefined4 param_3)

{
  uint uVar1;
  ulong uVar2;
  long lVar3;
  undefined8 *in_x18;
  long lVar4;
  code *pcVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = *(long *)(param_1 + 0x98);
  uVar2 = devfreq_recommended_opp(param_1,param_2,param_3);
  if (uVar2 < 0xfffffffffffff001) {
    uVar1 = dev_pm_opp_get_freq(uVar2);
    dev_pm_opp_put(uVar2);
    lVar3 = *(long *)(lVar4 + 0x480);
    if (*(int *)(lVar4 + 1000) == 0) {
      if ((lVar3 != 0) && (pcVar5 = *(code **)(lVar3 + 0x38), pcVar5 != (code *)0x0)) {
        if (0xe < ((ulong)(pcVar5 + -0x117c84) >> 2 | (long)pcVar5 << 0x3e)) {
          __cfi_slowpath(0x8e0c05c20dd39ba0,pcVar5);
        }
        (*pcVar5)(lVar4,uVar1);
      }
    }
    else if ((lVar3 != 0) && (pcVar5 = *(code **)(lVar3 + 0x48), pcVar5 != (code *)0x0)) {
      if (0xe < ((ulong)(pcVar5 + -0x117c84) >> 2 | (long)pcVar5 << 0x3e)) {
        __cfi_slowpath(0x8e0c05c20dd39ba0,pcVar5);
      }
      (*pcVar5)(lVar4,uVar1);
    }
    uVar2 = 0;
    *param_2 = (ulong)uVar1;
  }
  else {
    _dev_err(param_1,"failed to find opp for %lu KHz\n",*param_2);
  }
  return uVar2 & 0xffffffff;
}


