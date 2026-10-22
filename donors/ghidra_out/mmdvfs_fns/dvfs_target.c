====================dvfs_target@0010caa0====================
// signature: undefined dvfs_target(void)

ulong dvfs_target(long param_1,long *param_2,undefined4 param_3)

{
  ulong uVar1;
  long lVar2;
  long lVar3;
  undefined8 *in_x18;
  code *pcVar4;
  long *plVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  plVar5 = *(long **)(param_1 + 0x98);
  printk(&DAT_00106012,*param_2,plVar5[4]);
  if (*plVar5 == 0) {
    printk(&DAT_00105763);
    uVar1 = 0xffffffea;
  }
  else {
    uVar1 = devfreq_recommended_opp(param_1,param_2,param_3);
    if (uVar1 < 0xfffffffffffff001) {
      lVar2 = dev_pm_opp_get_freq(uVar1);
      lVar3 = dev_pm_opp_get_voltage(uVar1);
      dev_pm_opp_put(uVar1);
      if (plVar5[4] != *param_2) {
        mutex_lock(plVar5 + 0x5e);
        pcVar4 = *(code **)(plVar5[0xc] + 0xe8);
        if (6 < ((ulong)(pcVar4 + -0x1176b8) >> 2 | (long)pcVar4 << 0x3e)) {
          __cfi_slowpath(0xf11ee00dae052e85,pcVar4);
        }
        (*pcVar4)(plVar5[10],lVar3,lVar2,0);
        plVar5[4] = lVar2;
        plVar5[6] = lVar3;
        mutex_unlock(plVar5 + 0x5e);
      }
      uVar1 = 0;
    }
    else {
      _dev_err(param_1,"Failed to find opp for %lu KHz\n",*param_2);
    }
  }
  return uVar1 & 0xffffffff;
}


