// sprd_pcm_pm_notifier @ 00102140
// signature: undefined sprd_pcm_pm_notifier(void)
// calling convention: unknown


undefined8 sprd_pcm_pm_notifier(undefined8 param_1,long param_2)

{
  int *piVar1;
  long lVar2;
  int *piVar3;
  int *piVar4;
  undefined8 *in_x18;
  ulong uVar5;
  undefined8 uVar6;
  long lVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  piVar3 = pm_dma;
  if (param_2 == 4) {
    printk(&DAT_00100573,"sprd_pcm_pm_notifier");
  }
  else if (param_2 == 3) {
    printk(&DAT_00100edd,"sprd_pcm_pm_notifier");
    mutex_lock(piVar3 + 0x14);
    piVar4 = pm_dma;
    if (*piVar3 == 0) {
      lVar7 = *(long *)(piVar3 + 2);
      if (lVar7 == 0) {
        printk(&DAT_00100738,"pm_normal_dma_chan_release");
        lVar7 = *(long *)(piVar3 + 2);
      }
      else {
        piVar1 = pm_dma + 0xc;
        mutex_lock(piVar1);
        if (0 < *(int *)(lVar7 + 0x78)) {
          uVar5 = 0;
          do {
            if (uVar5 == 2) {
                    /* WARNING: Subroutine does not return */
              __ubsan_handle_cfi_check_fail_abort(0x12);
            }
            lVar2 = lVar7 + uVar5 * 8;
            if (*(long *)(lVar2 + 0x10) == 0) {
              printk(&DAT_00100fc4,"pm_normal_dma_chan_release",uVar5 & 0xffffffff);
            }
            else {
              printk(&DAT_00100e03,"pm_normal_dma_chan_release",
                     *(undefined4 *)(*(long *)(lVar2 + 0x10) + 0x10));
              uVar6 = *(undefined8 *)(lVar2 + 0x10);
              _raw_spin_lock(piVar4 + 10);
              *(undefined8 *)(lVar2 + 0x10) = 0;
              *(undefined8 *)(lVar2 + 0x50) = 0;
              *(undefined4 *)(lVar7 + uVar5 * 4 + 0x60) = 0;
              _raw_spin_unlock(piVar4 + 10);
              dma_release_channel(uVar6);
            }
            uVar5 = uVar5 + 1;
          } while ((long)uVar5 < (long)*(int *)(lVar7 + 0x78));
        }
        mutex_unlock(piVar1);
        lVar7 = *(long *)(piVar3 + 2);
      }
      if ((lVar7 != 0) && (*(char *)(lVar7 + 0xa0) != '\0')) {
        agdsp_access_disable();
        *(undefined1 *)(*(long *)(piVar3 + 2) + 0xa0) = 0;
      }
    }
    mutex_unlock(piVar3 + 0x14);
  }
  return 0;
}


