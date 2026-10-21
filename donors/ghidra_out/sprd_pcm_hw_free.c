// sprd_pcm_hw_free @ 00103cc4
// signature: undefined sprd_pcm_hw_free(void)
// calling convention: unknown


undefined8 sprd_pcm_hw_free(long param_1)

{
  char *pcVar1;
  undefined *puVar2;
  int iVar3;
  undefined8 *puVar4;
  long lVar5;
  undefined8 *in_x18;
  long lVar6;
  long lVar7;
  long lVar8;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar4 = *(undefined8 **)(*(long *)(param_1 + 0x10) + 0x5f8);
  lVar8 = *(long *)(*(long *)(param_1 + 0x140) + 0x150);
  lVar6 = *(long *)(lVar8 + 8);
  if (*(int *)(puVar4[3] + 8) == 0x124) {
    puVar2 = &DAT_0010150d;
  }
  else if (*(int *)(puVar4[3] + 8) == 0x129) {
    puVar2 = &DAT_00101128;
  }
  else {
    puVar2 = &DAT_00100602;
    if ((undefined *)*puVar4 != (undefined *)0x0) {
      puVar2 = (undefined *)*puVar4;
    }
  }
  pcVar1 = "Playback";
  if (*(int *)(param_1 + 0x3c) != 0) {
    pcVar1 = "Captrue";
  }
  printk(&DAT_001003b9,"sprd_pcm_hw_free",puVar2,*(undefined4 *)(puVar4 + 1),pcVar1);
  lVar5 = *(long *)(param_1 + 0x140);
  *(undefined8 *)(lVar5 + 0x2b8) = 0;
  *(undefined8 *)(lVar5 + 0x2c0) = 0;
  *(undefined8 *)(lVar5 + 0x2c8) = 0;
  *(undefined8 *)(lVar5 + 0x2d0) = 0;
  if (lVar6 != 0) {
    iVar3 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
      mutex_lock(pm_dma + 0x30);
    }
    iVar3 = *(int *)(lVar8 + 0x78);
    if (0 < iVar3) {
      lVar6 = 0;
      do {
        if (lVar6 == 2) {
                    /* WARNING: Subroutine does not return */
          __ubsan_handle_cfi_check_fail_abort(0x12);
        }
        lVar5 = lVar8 + lVar6 * 8;
        lVar7 = *(long *)(lVar5 + 0x10);
        if (lVar7 != 0) {
          printk(&DAT_00100e2a,"sprd_pcm_hw_free",*(undefined4 *)(lVar7 + 0x10));
          iVar3 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
          if (((iVar3 == 0xb) || (iVar3 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
            _raw_spin_lock(pm_dma + 0x28);
          }
          *(undefined8 *)(lVar5 + 0x10) = 0;
          *(undefined8 *)(lVar8 + lVar6 * 8 + 0x50) = 0;
          *(undefined4 *)(lVar8 + lVar6 * 4 + 0x60) = 0;
          iVar3 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
          if (((iVar3 == 0xb) || (iVar3 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
            _raw_spin_unlock(pm_dma + 0x28);
          }
          dma_release_channel(lVar7);
          iVar3 = *(int *)(lVar8 + 0x78);
        }
        lVar6 = lVar6 + 1;
      } while (lVar6 < iVar3);
    }
    *(undefined8 *)(lVar8 + 8) = 0;
    iVar3 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
      mutex_unlock(pm_dma + 0x30);
    }
  }
  return 0;
}


