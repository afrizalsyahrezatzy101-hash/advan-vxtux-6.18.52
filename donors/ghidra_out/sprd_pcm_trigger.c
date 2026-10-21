// sprd_pcm_trigger @ 00103eec
// signature: undefined sprd_pcm_trigger(void)
// calling convention: unknown


undefined8 sprd_pcm_trigger(long param_1,uint param_2)

{
  char *pcVar1;
  uint uVar2;
  undefined4 uVar3;
  undefined *puVar4;
  int iVar5;
  undefined8 *puVar6;
  undefined8 *in_x18;
  long lVar7;
  long *plVar8;
  long lVar9;
  code *pcVar10;
  long lVar11;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar6 = *(undefined8 **)(*(long *)(param_1 + 0x10) + 0x5f8);
  lVar11 = *(long *)(*(long *)(param_1 + 0x140) + 0x150);
  lVar9 = *(long *)(lVar11 + 8);
  if (*(int *)(puVar6[3] + 8) == 0x124) {
    puVar4 = &DAT_0010150d;
  }
  else if (*(int *)(puVar6[3] + 8) == 0x129) {
    puVar4 = &DAT_00101128;
  }
  else {
    puVar4 = &DAT_00100602;
    if ((undefined *)*puVar6 != (undefined *)0x0) {
      puVar4 = (undefined *)*puVar6;
    }
  }
  pcVar1 = "Playback";
  if (*(int *)(param_1 + 0x3c) != 0) {
    pcVar1 = "Captrue";
  }
  printk(&DAT_00100598,"sprd_pcm_trigger",puVar4,*(undefined4 *)(puVar6 + 1),pcVar1,param_2);
  if (lVar9 == 0) {
    printk(&DAT_00100915);
    return 0;
  }
  if (6 < param_2) {
    return 0xffffffea;
  }
  uVar2 = 1 << (ulong)(param_2 & 0x1f);
  if ((uVar2 & 0x29) == 0) {
    if ((uVar2 & 0x52) == 0) {
      return 0xffffffea;
    }
    iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
    if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
      _raw_spin_lock(pm_dma + 0x28);
    }
    if (*(long *)(lVar11 + 0x50) == 0) {
      iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
      if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
        _raw_spin_unlock(pm_dma + 0x28);
      }
      puVar4 = &DAT_0010137a;
LAB_001040b8:
      printk(puVar4,"sprd_pcm_trigger",0xffffffc3,0xffffffc3);
      return 0xffffffc3;
    }
    iVar5 = *(int *)(lVar11 + 0x78);
    if (0 < iVar5) {
      lVar9 = 0;
      do {
        if (lVar9 == 2) goto LAB_001042a4;
        lVar7 = *(long *)(lVar11 + 0x50 + lVar9 * 8);
        if (lVar7 != 0) {
          pcVar10 = *(code **)(lVar7 + 0x18);
          __cfi_slowpath(0xc97b94b4f4a7b659,pcVar10);
          uVar3 = (*pcVar10)(lVar7);
          *(undefined4 *)(lVar11 + 0x50 + lVar9 * 4 + 0x10) = uVar3;
          iVar5 = *(int *)(lVar11 + 0x78);
        }
        lVar9 = lVar9 + 1;
      } while (lVar9 < iVar5);
      if (0 < iVar5) {
        lVar9 = 0;
        do {
          if (lVar9 == 2) {
LAB_001042a4:
                    /* WARNING: Subroutine does not return */
            __ubsan_handle_cfi_check_fail_abort(0x12);
          }
          plVar8 = *(long **)(lVar11 + 0x10 + lVar9 * 8);
          if (plVar8 != (long *)0x0) {
            pcVar10 = *(code **)(*plVar8 + 0x128);
            __cfi_slowpath(0x6d896a35912b7bb6,pcVar10);
            (*pcVar10)(plVar8);
            iVar5 = *(int *)(lVar11 + 0x78);
          }
          lVar9 = lVar9 + 1;
        } while (lVar9 < iVar5);
      }
    }
    printk(&DAT_00100a80);
    iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
    if ((iVar5 != 0xb) && (iVar5 != 0)) {
      return 0;
    }
  }
  else {
    iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
    if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
      _raw_spin_lock(pm_dma + 0x28);
    }
    printk(&DAT_0010112c);
    if (*(long *)(lVar11 + 0x10) == 0) {
      iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
      if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
        _raw_spin_unlock(pm_dma + 0x28);
      }
      puVar4 = &DAT_001013af;
      goto LAB_001040b8;
    }
    iVar5 = *(int *)(lVar11 + 0x78);
    if (0 < iVar5) {
      lVar9 = 0;
      do {
        if (lVar9 == 2) goto LAB_001042a4;
        plVar8 = *(long **)(lVar11 + 0x10 + lVar9 * 8);
        if ((plVar8 != (long *)0x0) &&
           (pcVar10 = *(code **)(*plVar8 + 0x100), pcVar10 != (code *)0x0)) {
          __cfi_slowpath(0x146502001565ea5c,pcVar10);
          (*pcVar10)(plVar8);
          iVar5 = *(int *)(lVar11 + 0x78);
        }
        lVar9 = lVar9 + 1;
      } while (lVar9 < iVar5);
    }
    *(undefined4 *)(lVar11 + 0x90) = 0;
    printk(&DAT_00100f05);
    iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
    if ((iVar5 != 0xb) && (iVar5 != 0)) {
      return 0;
    }
  }
  if (*(int *)(param_1 + 0x3c) == 0) {
    _raw_spin_unlock(pm_dma + 0x28);
  }
  return 0;
}


