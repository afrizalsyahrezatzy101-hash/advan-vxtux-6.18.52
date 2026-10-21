// vsp_dvfs_target @ 0010a06c
// signature: undefined vsp_dvfs_target(void)
// calling convention: unknown


ulong vsp_dvfs_target(long param_1,undefined8 *param_2,undefined4 param_3)

{
  undefined4 uVar1;
  ulong uVar2;
  int iVar3;
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
    iVar3 = *(int *)(lVar4 + 0x480);
    if (iVar3 == 0) {
      *(undefined4 *)(lVar4 + 0x3d0) = uVar1;
    }
    else {
      *(undefined4 *)(lVar4 + 0x3d4) = uVar1;
    }
    pcVar5 = *(code **)(*(long *)(lVar4 + 1000) + 0x90);
    if (5 < ((ulong)(pcVar5 + -0x117f5c) >> 2 | (long)pcVar5 << 0x3e)) {
      __cfi_slowpath(0x43374dcd9e82cff8,pcVar5);
      iVar3 = *(int *)(lVar4 + 0x480);
    }
    (*pcVar5)(lVar4,uVar1,iVar3);
    uVar2 = 0;
  }
  else {
    _dev_err(param_1,"Failed to find opp for %lu KHz\n",*param_2);
  }
  return uVar2 & 0xffffffff;
}


