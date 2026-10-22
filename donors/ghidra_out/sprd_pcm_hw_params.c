// sprd_pcm_hw_params @ 00103168
// signature: undefined sprd_pcm_hw_params(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sprd_pcm_hw_params(long param_1,byte *param_2)

{
  size_t __n;
  uint uVar1;
  int iVar2;
  int iVar3;
  long lVar4;
  undefined *puVar5;
  uint uVar6;
  undefined4 uVar7;
  long lVar8;
  long lVar9;
  long *plVar10;
  undefined8 uVar11;
  long lVar12;
  long *plVar13;
  undefined8 *in_x18;
  long lVar14;
  ulong *puVar15;
  uint uVar16;
  long lVar17;
  long lVar18;
  code *pcVar19;
  int *piVar20;
  long lVar21;
  uint uVar22;
  ulong uVar23;
  undefined4 *puVar24;
  long lVar25;
  void *__s;
  uint *puVar26;
  long *plVar27;
  ulong uVar28;
  undefined8 unaff_x30;
  undefined8 local_a0;
  long local_98 [7];
  
  *in_x18 = unaff_x30;
  local_98[6] = ___stack_chk_guard;
  plVar27 = (long *)(param_1 + 0x10);
  lVar21 = *plVar27;
  lVar8 = *(long *)(param_1 + 0x140);
  piVar20 = *(int **)(lVar8 + 0x150);
  puVar26 = (uint *)(param_1 + 0x3c);
  uVar22 = *puVar26;
  if (1 < uVar22) {
LAB_00103c98:
                    /* WARNING: Subroutine does not return */
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
  plVar10 = (long *)(lVar21 + (ulong)uVar22 * 0x2a0 + 0x98);
  for (plVar13 = (long *)*plVar10; plVar10 != plVar13; plVar13 = (long *)*plVar13) {
    if (plVar13[-2] == lVar21) {
      lVar17 = plVar13[-3];
      goto LAB_001031fc;
    }
  }
  lVar17 = 0;
LAB_001031fc:
  local_98[4] = 0;
  local_98[5] = 0;
  local_98[2] = 0;
  local_98[3] = 0;
  uVar28 = (ulong)*(uint *)(param_2 + 0x17c);
  uVar1 = *(uint *)(param_2 + 0x14c);
  local_98[0] = 0;
  local_98[1] = 0;
  printk(&DAT_001013e1,"sprd_pcm_hw_params",*(undefined4 *)(*(long *)(lVar21 + 0x5f8) + 8));
  if ((lVar17 == 0) || (lVar9 = *(long *)(lVar17 + 0x5f8), *(int *)(lVar9 + 8) != 0x125)) {
    lVar9 = *(long *)(lVar21 + 0x5f8);
  }
  lVar14 = 0x40;
  if (*puVar26 != 0) {
    lVar14 = 0x48;
  }
  lVar9 = *(long *)(lVar9 + lVar14);
  if (lVar9 == 0) {
    printk(&DAT_001004b4);
    piVar20[2] = 0;
    piVar20[3] = 0;
    lVar21 = *(long *)(param_1 + 0x140);
    if (param_1 + 0x100 == 0) {
      *(undefined8 *)(lVar21 + 0x2d0) = 0;
      *(undefined8 *)(lVar21 + 0x2b8) = 0;
      *(undefined8 *)(lVar21 + 0x2c0) = 0;
      uVar11 = 0;
    }
    else {
      *(long *)(lVar21 + 0x2d0) = param_1 + 0x100;
      *(undefined8 *)(lVar21 + 0x2b8) = *(undefined8 *)(param_1 + 0x110);
      *(undefined8 *)(lVar21 + 0x2c0) = *(undefined8 *)(param_1 + 0x118);
      uVar11 = *(undefined8 *)(param_1 + 0x120);
    }
    *(undefined8 *)(lVar21 + 0x2c8) = uVar11;
    iVar3 = 0;
    *(ulong *)(lVar8 + 0x2c8) = uVar28;
    goto LAB_00103c24;
  }
  uVar16 = *(uint *)(param_2 + 0x11c);
  if (*(int *)(lVar9 + 0x98) == 1) {
    uVar16 = 1;
  }
  uVar23 = (ulong)uVar16;
  printk(&DAT_00100758,uVar23,uVar28,uVar1);
  iVar3 = *(int *)(*(long *)(*(long *)(lVar21 + 0x5f8) + 0x18) + 8);
  if (((iVar3 == 0x129) || (iVar3 == 0x124)) ||
     ((lVar17 != 0 && (*(int *)(*(long *)(lVar17 + 0x5f8) + 8) == 0x125)))) {
    uVar16 = piVar20[0x1e];
    uVar23 = (ulong)uVar16;
  }
  else {
    if (uVar16 == 2) {
      uVar6 = (uint)(*(int *)(lVar8 + 0x58) == 3 || *(int *)(lVar8 + 0x58) == 0);
    }
    else {
      uVar6 = 0;
    }
    piVar20[0x23] = uVar6;
    sprd_pcm_dma_step_config(lVar9,uVar22 == 0);
  }
  if (2 < (int)uVar16) {
    printk(&DAT_0010044d,uVar23,2);
    iVar3 = -0x16;
    goto LAB_00103c24;
  }
  uVar22 = (uint)uVar23;
  if ((int)uVar22 < 1) {
    printk(&DAT_00101030,uVar23);
    iVar3 = -0x16;
    goto LAB_00103c24;
  }
  if (*(long *)(piVar20 + 2) == 0) {
    *(long *)(piVar20 + 2) = lVar9;
    lVar14 = *(long *)(param_1 + 0x140);
    lVar17 = snd_soc_rtdcom_lookup(*(undefined8 *)(param_1 + 0x10),"sprd-pcm-dma");
    lVar25 = *(long *)(lVar14 + 0x150);
    lVar14 = *(long *)(lVar25 + 8);
    if (lVar14 != 0) {
      lVar18 = 0;
      do {
        uVar16 = (uint)lVar18;
        plVar13 = (long *)(lVar14 + (long)(int)uVar16 * 8 + 0xa0);
        if (*plVar13 == 0) {
          printk(&DAT_0010082d);
LAB_00103a84:
          if (uVar16 < 2) {
            printk(&DAT_00100487,*(undefined4 *)(lVar14 + lVar18 * 4 + 8));
            iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
            if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
              mutex_lock(pm_dma + 0x30);
            }
            uVar28 = 0;
            lVar8 = lVar25 + 0x60;
            do {
              lVar21 = lVar8 + uVar28 * 8;
              lVar17 = *(long *)(lVar21 + -0x50);
              if (lVar17 != 0) {
                iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
                if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
                  _raw_spin_lock(pm_dma + 0x28);
                }
                *(undefined8 *)(lVar21 + -0x50) = 0;
                *(undefined8 *)(lVar8 + uVar28 * 8 + -0x10) = 0;
                *(undefined4 *)(lVar8 + uVar28 * 4) = 0;
                *(undefined8 *)(lVar25 + 8) = 0;
                iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
                if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
                  _raw_spin_unlock(pm_dma + 0x28);
                }
                dma_release_channel(lVar17);
              }
              uVar28 = uVar28 + 1;
            } while (uVar23 != uVar28);
            iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
            if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
              mutex_unlock(pm_dma + 0x30);
            }
            iVar3 = -0x13;
            goto LAB_00103c08;
          }
          goto LAB_00103c98;
        }
        plVar10 = (long *)dma_request_slave_channel(*(undefined8 *)(lVar17 + 0x18));
        if ((plVar10 == (long *)0x0) || ((long *)0xfffffffffffff000 < plVar10)) {
          printk(&DAT_00100a9a,*plVar13,plVar10);
          goto LAB_00103a84;
        }
        iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
        if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
          mutex_lock(pm_dma + 0x30);
          iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
        }
        if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
          _raw_spin_lock(pm_dma + 0x28);
        }
        puVar15 = (ulong *)(lVar25 + (long)(int)uVar16 * 8 + 0x10);
        *puVar15 = (ulong)plVar10;
        lVar12 = *plVar27;
        local_a0 = 0;
        lVar4 = *(long *)(*(long *)(*plVar10 + 0x60) + 0x300);
        if ((lVar4 == 0) ||
           (iVar3 = of_property_read_string(lVar4,"compatible",&local_a0), iVar3 != 0)) {
          uVar7 = 0xffffffff;
        }
        else if (((*(int *)(*(long *)(lVar12 + 0x5f8) + 8) == 0) && (*puVar26 == 1)) &&
                (test_normal_cap_use_iram != 0)) {
          uVar7 = 0x13;
        }
        else {
          uVar7 = 5;
        }
        *(undefined4 *)(lVar25 + 0x68) = uVar7;
        printk(&DAT_00100e8d,*(undefined4 *)(*puVar15 + 0x10),
               *(undefined4 *)(*(long *)(lVar25 + 8) + (long)(int)uVar16 * 4 + 8));
        iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
        if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
          _raw_spin_unlock(pm_dma + 0x28);
          iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
        }
        if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
          mutex_unlock(pm_dma + 0x30);
        }
        lVar18 = lVar18 + 1;
      } while (uVar22 != (uint)lVar18);
      goto LAB_00103610;
    }
    printk(&DAT_00100e62,"sprd_pcm_request_dma_channel");
    iVar3 = -0x16;
LAB_00103c08:
    printk(&DAT_00101496);
  }
  else {
LAB_00103610:
    lVar17 = *(long *)(param_1 + 0x140);
    if (param_1 + 0x100 == 0) {
      *(undefined8 *)(lVar17 + 0x2d0) = 0;
      *(undefined8 *)(lVar17 + 0x2b8) = 0;
      *(undefined8 *)(lVar17 + 0x2c0) = 0;
      uVar11 = 0;
    }
    else {
      *(long *)(lVar17 + 0x2d0) = param_1 + 0x100;
      *(undefined8 *)(lVar17 + 0x2b8) = *(undefined8 *)(param_1 + 0x110);
      *(undefined8 *)(lVar17 + 0x2c0) = *(undefined8 *)(param_1 + 0x118);
      uVar11 = *(undefined8 *)(param_1 + 0x120);
    }
    *(undefined8 *)(lVar17 + 0x2c8) = uVar11;
    iVar3 = 0;
    if ((long)(int)uVar22 != 0) {
      iVar3 = (int)(uVar28 / (ulong)(long)(int)uVar22);
    }
    *(ulong *)(lVar8 + 0x2c8) = uVar28;
    *piVar20 = iVar3;
    if (((*(int *)(lVar8 + 0x58) == 3) || (*(int *)(lVar8 + 0x58) == 0)) &&
       ((iVar3 = *(int *)(lVar9 + 0x74), iVar3 == 4 || (iVar3 == 2)))) {
      *piVar20 = iVar3;
    }
    iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
      mutex_lock(pm_dma + 0x30);
      iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
    }
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
      _raw_spin_lock(pm_dma + 0x28);
    }
    lVar17 = 0;
    uVar28 = 0;
    do {
      if (uVar28 == 2) goto LAB_00103c94;
      __n = (ulong)*(uint *)(lVar8 + 0x1a4) * 0x20 + 0x78;
      __s = (void *)(*(long *)(piVar20 + 8) + __n * uVar28);
      local_98[uVar28 + 4] = (long)__s;
      memset(__s,0,__n);
      plVar13 = (long *)(*(long *)(piVar20 + 10) + lVar17);
      local_98[uVar28 + 2] = (long)plVar13;
      *plVar13 = 0;
      plVar13[1] = 0;
      lVar9 = *(long *)(piVar20 + uVar28 * 2 + 4);
      *plVar13 = param_1;
      plVar13[1] = lVar9;
      local_98[uVar28] = *(long *)(lVar8 + 0x2c0) + (long)(*piVar20 * (int)uVar28);
      *(long *)((long)__s + 0x70) = (long)__s + 0x78;
      printk(&DAT_00101423,uVar28 & 0xffffffff);
      uVar28 = uVar28 + 1;
      lVar17 = lVar17 + 0x10;
    } while (uVar23 != uVar28);
    iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
      _raw_spin_unlock(pm_dma + 0x28);
      iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
    }
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
      mutex_unlock(pm_dma + 0x30);
    }
    uVar16 = 0;
    if (uVar22 != 0) {
      uVar16 = uVar1 / uVar22;
    }
    printk(&DAT_00101448,"sprd_pcm_hw_params",uVar16);
    iVar3 = sprd_pcm_config_dma(param_1,param_2,local_98 + 4,local_98);
    if (iVar3 != 0) goto LAB_00103c08;
    iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
    if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
      mutex_lock(pm_dma + 0x30);
    }
    iVar3 = *(int *)(*(long *)(lVar21 + 0x5f8) + 8);
    if (((iVar3 == 0xb) || (iVar3 == 0)) && ((*puVar26 == 0 && (*(long *)(piVar20 + 4) == 0)))) {
      printk(&DAT_00100ba4,"sprd_pcm_hw_params");
      iVar3 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
      if (((iVar3 == 0xb) || (iVar3 == 0)) && (*puVar26 == 0)) {
        mutex_unlock(pm_dma + 0x30);
      }
      iVar3 = 0;
      goto LAB_00103c08;
    }
    lVar8 = 0;
    do {
      if (lVar8 == 0x10) {
LAB_00103c94:
                    /* WARNING: Does not return */
        pcVar19 = (code *)SoftwareBreakpoint(1,0x103c98);
        (*pcVar19)();
      }
      plVar13 = *(long **)((long)piVar20 + lVar8 + 0x10);
      pcVar19 = *(code **)(*plVar13 + 0xf8);
      if (pcVar19 == (code *)0x0) {
        iVar3 = -0x26;
LAB_00103ba4:
        uVar7 = (undefined4)plVar13[2];
        puVar5 = &DAT_0010086d;
LAB_00103bd0:
        printk(puVar5,"sprd_pcm_hw_params",uVar7);
        iVar2 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
        if (((iVar2 == 0xb) || (iVar2 == 0)) && (*puVar26 == 0)) {
          mutex_unlock(pm_dma + 0x30);
        }
        goto LAB_00103c08;
      }
      puVar24 = *(undefined4 **)((long)local_98 + lVar8 + 0x20);
      __cfi_slowpath(0xa5b1eb43ed4fa99d,pcVar19);
      iVar3 = (*pcVar19)(plVar13,puVar24);
      plVar13 = *(long **)((long)piVar20 + lVar8 + 0x10);
      if (iVar3 < 0) goto LAB_00103ba4;
      pcVar19 = *(code **)(*plVar13 + 0xd8);
      __cfi_slowpath(0x3675599da7c39b65,pcVar19);
      lVar21 = (*pcVar19)(*(undefined8 *)((long)piVar20 + lVar8 + 0x10),
                          *(undefined8 *)(puVar24 + 0x1c),puVar24[0x1b],*puVar24,
                          *(undefined8 *)(puVar24 + 0x14),puVar24 + 0xe);
      if (lVar21 == 0) {
        puVar5 = &DAT_0010120f;
        uVar7 = *(undefined4 *)(*(long *)((long)piVar20 + lVar8 + 0x10) + 0x10);
        goto LAB_00103bd0;
      }
      iVar2 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
      if (((iVar2 == 0xb) || (iVar2 == 0)) && (*puVar26 == 0)) {
        _raw_spin_lock(pm_dma + 0x28);
      }
      *(long *)((long)piVar20 + lVar8 + 0x50) = lVar21;
      iVar2 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
      if (((iVar2 == 0xb) || (iVar2 == 0)) && (*puVar26 == 0)) {
        _raw_spin_unlock(pm_dma + 0x28);
      }
      if ((*param_2 >> 2 & 1) == 0) {
        printk(&DAT_001008a2,"sprd_pcm_hw_params",
               *(undefined4 *)(*(long *)((long)piVar20 + lVar8 + 0x10) + 0x10));
        *(undefined1 **)(*(long *)((long)piVar20 + lVar8 + 0x50) + 0x28) = &LAB_00106364;
        *(undefined8 *)(*(long *)((long)piVar20 + lVar8 + 0x50) + 0x38) =
             *(undefined8 *)((long)local_98 + lVar8 + 0x10);
        *(undefined8 *)(*(long *)((long)piVar20 + lVar8 + 0x50) + 0x30) = 0;
      }
      lVar8 = lVar8 + 8;
    } while (uVar23 * 8 - lVar8 != 0);
    iVar2 = *(int *)(*(long *)(*plVar27 + 0x5f8) + 8);
    if (((iVar2 == 0xb) || (iVar2 == 0)) && (*puVar26 == 0)) {
      mutex_unlock(pm_dma + 0x30);
    }
  }
  printk(&DAT_001011db,iVar3);
LAB_00103c24:
  if (___stack_chk_guard == local_98[6]) {
    return iVar3;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


