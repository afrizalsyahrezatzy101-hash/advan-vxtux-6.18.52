// sprd_pcm_open @ 00102484
// signature: undefined sprd_pcm_open(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int sprd_pcm_open(long param_1)

{
  int *piVar1;
  char *pcVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  long lVar6;
  undefined8 uVar7;
  long lVar8;
  undefined *puVar9;
  uint uVar10;
  long lVar11;
  int *piVar12;
  uint uVar13;
  undefined8 uVar14;
  undefined *puVar15;
  long *plVar16;
  long lVar17;
  long *plVar18;
  long *plVar19;
  undefined8 *in_x18;
  long lVar20;
  long lVar21;
  long lVar22;
  undefined8 *puVar23;
  int iVar24;
  undefined8 unaff_x30;
  int local_74;
  ulong local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  piVar12 = pm_dma;
  local_68 = ___stack_chk_guard;
  uVar10 = *(uint *)(param_1 + 0x3c);
  if (1 < uVar10) goto LAB_00102bd4;
  puVar23 = *(undefined8 **)(param_1 + 0x10);
  lVar20 = *(long *)(param_1 + 0x140);
  for (plVar18 = (long *)puVar23[(ulong)uVar10 * 0x54 + 0x13];
      puVar23 + (ulong)uVar10 * 0x54 + 0x13 != plVar18; plVar18 = (long *)*plVar18) {
    if ((undefined8 *)plVar18[-2] == puVar23) {
      lVar21 = plVar18[-3];
      goto LAB_00102504;
    }
  }
  lVar21 = 0;
LAB_00102504:
  uVar14 = *puVar23;
  plVar18 = (long *)puVar23[0xbc];
  local_74 = 0;
  iVar24 = *(int *)(((undefined8 *)puVar23[0xbf])[3] + 8);
  if (iVar24 == 0x124) {
    puVar9 = &DAT_0010150d;
  }
  else if (iVar24 == 0x129) {
    puVar9 = &DAT_00101128;
  }
  else {
    puVar15 = *(undefined **)puVar23[0xbf];
    puVar9 = &DAT_00100602;
    if (puVar15 != (undefined *)0x0) {
      puVar9 = puVar15;
    }
  }
  pcVar2 = "Playback";
  if (uVar10 != 0) {
    pcVar2 = "Captrue";
  }
  printk(&DAT_00100503,"sprd_pcm_open",puVar9,pcVar2);
  iVar24 = *(int *)(*(long *)(puVar23[0xbf] + 0x18) + 8);
  if ((iVar24 == 0x129) || (iVar24 == 0x124)) {
    snd_soc_set_runtime_hwparams(param_1,&sprd_i2s_pcm_hardware);
    if (*(int *)(*(long *)(puVar23[0xbf] + 0x18) + 8) == 0x124) {
      if (*(int *)(param_1 + 0x3c) == 0) {
        uVar10 = 0x20 - _DAT_00000014;
      }
      else {
        uVar10 = (uint)_DAT_00000016;
      }
      uVar13 = (uint)(_DAT_00000010 >> 1);
    }
    else {
      if (*(int *)(param_1 + 0x3c) == 0) {
        uVar10 = 0x20 - _DAT_0000003a;
      }
      else {
        uVar10 = (uint)_DAT_0000003c;
      }
      uVar13 = (uint)DAT_00000038;
    }
    iVar24 = uVar10 << (ulong)(uVar13 & 3);
    lVar21 = 1;
  }
  else if ((lVar21 == 0) || (*(int *)(*(long *)(lVar21 + 0x5f8) + 8) != 0x125)) {
    snd_soc_set_runtime_hwparams(param_1,&sprd_pcm_hardware_v1);
    lVar21 = 2;
    iVar24 = 0x50;
  }
  else {
    snd_soc_set_runtime_hwparams(param_1,&sprd_i2s_pcm_hardware);
    lVar21 = 1;
    iVar24 = 0x300;
  }
  iVar5 = snd_pcm_hw_constraint_step(lVar20,0,0xe,(long)iVar24);
  if (((iVar5 != 0) || (iVar5 = snd_pcm_hw_constraint_step(lVar20,0,0x12,(long)iVar24), iVar5 != 0))
     || (iVar5 = snd_pcm_hw_constraint_integer(lVar20,0xf), iVar5 < 0)) goto LAB_00102714;
  lVar6 = devm_kmalloc(uVar14,0xa8,0xdc0);
  if (lVar6 == 0) {
    iVar5 = -0xc;
    goto LAB_00102714;
  }
  *(long *)(lVar20 + 0x150) = lVar6;
  piVar1 = piVar12 + 0x14;
  mutex_lock();
  uVar10 = *(uint *)(puVar23[0xbf] + 8);
  if ((uVar10 < 0x15) && ((1 << (ulong)(uVar10 & 0x1f) & 0x1880a0U) != 0)) {
    iVar5 = -0xc;
    if (uVar10 == 0xb) goto LAB_001027c0;
LAB_001027bc:
    if (uVar10 == 0) goto LAB_001027c0;
LAB_001027c8:
    *piVar12 = *piVar12 + 1;
  }
  else {
    iVar5 = agdsp_access_enable();
    if (iVar5 != 0) {
      mutex_unlock(piVar1);
      printk(&DAT_00100eae,"sprd_pcm_open",iVar5);
      goto LAB_00102714;
    }
    *(undefined1 *)(lVar6 + 0xa0) = 1;
    uVar10 = *(uint *)(puVar23[0xbf] + 8);
    if (uVar10 != 0xb) goto LAB_001027bc;
LAB_001027c0:
    if (*(int *)(param_1 + 0x3c) != 0) goto LAB_001027c8;
    *(long *)(piVar12 + 2) = lVar6;
  }
  mutex_unlock(piVar1);
  uVar10 = *(uint *)(puVar23[0xbf] + 8);
  if ((uVar10 < 0x15) && ((1 << (ulong)(uVar10 & 0x1f) & 0x1880a0U) != 0)) {
    iVar5 = 0;
    goto LAB_00102714;
  }
  local_74 = *(int *)(lVar20 + 0x1a4) * 0x78;
  if (((uVar10 == 0) && (*(int *)(param_1 + 0x3c) == 1)) && (test_normal_cap_use_iram != 0)) {
    uVar7 = 0x11;
  }
  else {
    uVar7 = 5;
  }
  uVar10 = audio_mem_alloc(uVar7,&local_74);
  *(ulong *)(lVar6 + 0x30) = (ulong)uVar10;
  if (uVar10 == 0) {
    printk(&DAT_00101164);
    iVar5 = -0xc;
  }
  else {
    lVar8 = audio_mem_vmap((ulong)uVar10,local_74,1);
    *(long *)(lVar6 + 0x40) = lVar8;
    if (lVar8 == 0) {
      puVar9 = &DAT_00100521;
    }
    else {
      __memset_io(lVar8,0,local_74);
      local_74 = *(int *)(lVar20 + 0x1a4) * 0x78;
      if (((*(int *)(puVar23[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
         (test_normal_cap_use_iram != 0)) {
        uVar7 = 0x12;
      }
      else {
        uVar7 = 5;
      }
      uVar10 = audio_mem_alloc(uVar7,&local_74);
      *(ulong *)(lVar6 + 0x38) = (ulong)uVar10;
      if (uVar10 == 0) {
        puVar9 = &DAT_0010128a;
      }
      else {
        lVar8 = audio_mem_vmap((ulong)uVar10,local_74,1);
        *(long *)(lVar6 + 0x48) = lVar8;
        if (lVar8 != 0) {
          __memset_io(*(undefined8 *)(lVar6 + 0x40),0,local_74);
          printk(&DAT_001012c9,*(undefined8 *)(lVar6 + 0x40),*(undefined8 *)(lVar6 + 0x30));
          printk(&DAT_00100cc0,*(undefined8 *)(lVar6 + 0x48),*(undefined8 *)(lVar6 + 0x38));
          printk(&DAT_00101071,(ulong)*(uint *)(lVar20 + 0x1a4) * 0x78);
          printk(&DAT_00100bfc,0x78,local_74);
          if (1 < *(uint *)(param_1 + 0x3c)) {
LAB_00102bd4:
                    /* WARNING: Subroutine does not return */
            __ubsan_handle_cfi_check_fail_abort(0x12);
          }
          lVar8 = plVar18[(ulong)*(uint *)(param_1 + 0x3c) * 0x7b + 0x1a];
          if (1 < *(uint *)(lVar8 + 0x3c)) goto LAB_00102bd4;
          lVar22 = *(long *)(lVar8 + 0x10);
          lVar11 = plVar18[0x114];
          plVar16 = (long *)(lVar22 + (ulong)*(uint *)(lVar8 + 0x3c) * 0x2a0 + 0x98);
          for (plVar19 = (long *)*plVar16; plVar16 != plVar19; plVar19 = (long *)*plVar19) {
            if (plVar19[-2] == lVar22) {
              lVar17 = plVar19[-3];
              goto LAB_00102be0;
            }
          }
          lVar17 = 0;
LAB_00102be0:
          local_70 = 0x20000;
          *(undefined4 *)(lVar8 + 0x100) = 2;
          *(undefined8 *)(lVar8 + 0x108) = *(undefined8 *)(*plVar18 + 0x5f8);
          iVar5 = *(int *)(*(long *)(*(long *)(lVar11 + 0x5f8) + 0x18) + 8);
          if (((iVar5 == 0x124) || (iVar5 == 0x129)) ||
             ((lVar17 != 0 && (*(int *)(*(long *)(lVar17 + 0x5f8) + 8) == 0x125)))) {
            local_70 = 0x10000;
          }
          else if ((test_normal_cap_use_iram == 0) && (test_ddr_buffer_size != 0)) {
            local_70 = (ulong)test_ddr_buffer_size;
          }
          printk(&DAT_001014b3,"sprd_pcm_preallocate_dma_ddr32_buffer",local_70);
          *(undefined8 *)(lVar8 + 0x128) = 0;
          if (((*(int *)(*(long *)(lVar22 + 0x5f8) + 8) == 0) && (*(int *)(lVar8 + 0x3c) == 1)) &&
             (test_normal_cap_use_iram != 0)) {
            uVar7 = 0x13;
          }
          else {
            uVar7 = 5;
          }
          uVar10 = audio_mem_alloc(uVar7,&local_70);
          *(ulong *)(lVar8 + 0x118) = (ulong)uVar10;
          if (uVar10 == 0) {
            printk(&DAT_00100c3d,"sprd_pcm_preallocate_dma_ddr32_buffer");
LAB_00102d58:
            iVar5 = -0xc;
          }
          else {
            lVar11 = audio_mem_vmap((ulong)uVar10,local_70,1);
            *(long *)(lVar8 + 0x110) = lVar11;
            if (lVar11 == 0) {
              printk(&DAT_00100f8e,"sprd_pcm_preallocate_dma_ddr32_buffer");
              if (((*(int *)(*(long *)(lVar22 + 0x5f8) + 8) == 0) && (*(int *)(lVar8 + 0x3c) == 1))
                 && (test_normal_cap_use_iram != 0)) {
                uVar4 = *(undefined4 *)(lVar8 + 0x118);
                uVar7 = 0x13;
                uVar3 = 0;
              }
              else {
                uVar4 = *(undefined4 *)(lVar8 + 0x118);
                uVar7 = 5;
                uVar3 = (undefined4)local_70;
              }
              audio_mem_free(uVar7,uVar4,uVar3);
              goto LAB_00102d58;
            }
            printk(&DAT_001010ba,"sprd_pcm_preallocate_dma_ddr32_buffer",lVar11,
                   *(undefined4 *)(lVar8 + 0x118),local_70);
            if (*(long *)(lVar8 + 0x110) == 0) goto LAB_00102d58;
            iVar5 = 0;
            *(ulong *)(lVar8 + 0x120) = local_70;
          }
          lVar8 = devm_kmalloc(uVar14,((ulong)*(uint *)(lVar20 + 0x1a4) * 0x20 + 0x78) * lVar21,
                               0xdc0);
          *(long *)(lVar6 + 0x20) = lVar8;
          if (lVar8 != 0) {
            lVar8 = devm_kmalloc(uVar14,lVar21 << 4,0xdc0);
            *(long *)(lVar6 + 0x28) = lVar8;
            if (lVar8 != 0) {
              lVar20 = *(long *)(param_1 + 0x10);
              iVar5 = *(int *)(*(long *)(lVar20 + 0x5f8) + 8);
              if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
                mutex_lock(pm_dma + 0xc);
                lVar20 = *(long *)(param_1 + 0x10);
                iVar5 = *(int *)(*(long *)(lVar20 + 0x5f8) + 8);
              }
              if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
                _raw_spin_lock(pm_dma + 10);
                lVar20 = *(long *)(param_1 + 0x10);
              }
              piVar12 = pm_dma;
              *(undefined8 *)(lVar6 + 0x10) = 0;
              *(undefined8 *)(lVar6 + 0x18) = 0;
              *(undefined8 *)(lVar6 + 0x50) = 0;
              *(undefined8 *)(lVar6 + 0x58) = 0;
              *(undefined8 *)(lVar6 + 0x60) = 0;
              iVar5 = *(int *)(*(long *)(lVar20 + 0x5f8) + 8);
              if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
                _raw_spin_unlock(piVar12 + 10);
                iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
                piVar12 = pm_dma;
              }
              if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
                mutex_unlock(piVar12 + 0xc);
              }
              iVar5 = 0;
              *(int *)(lVar6 + 0x74) = iVar24;
              *(int *)(lVar6 + 0x78) = (int)lVar21;
              goto LAB_00102714;
            }
          }
          goto LAB_00102990;
        }
        puVar9 = &DAT_00100d03;
      }
    }
    printk(puVar9);
  }
LAB_00102990:
  printk(&DAT_0010070c);
  mutex_lock(piVar1);
  if (*(char *)(lVar6 + 0xa0) != '\0') {
    agdsp_access_disable();
    *(undefined1 *)(lVar6 + 0xa0) = 0;
  }
  mutex_unlock(piVar1);
  if (*(long *)(lVar6 + 0x20) != 0) {
    devm_kfree(uVar14);
    *(undefined8 *)(lVar6 + 0x20) = 0;
  }
  if (*(long *)(lVar6 + 0x40) != 0) {
    audio_mem_unmap();
    *(undefined8 *)(lVar6 + 0x40) = 0;
  }
  if (((*(int *)(puVar23[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
     (test_normal_cap_use_iram != 0)) {
    lVar21 = *(long *)(lVar6 + 0x30);
    if (lVar21 != 0) {
      uVar7 = 0x11;
      iVar24 = 0;
LAB_00102a04:
      audio_mem_free(uVar7,lVar21,iVar24);
      *(undefined8 *)(lVar6 + 0x30) = 0;
    }
  }
  else {
    lVar21 = *(long *)(lVar6 + 0x30);
    if (lVar21 != 0) {
      uVar7 = 5;
      iVar24 = *(int *)(lVar20 + 0x1a4) * 0x78;
      goto LAB_00102a04;
    }
  }
  if (*(long *)(lVar6 + 0x48) != 0) {
    audio_mem_unmap();
    *(undefined8 *)(lVar6 + 0x48) = 0;
  }
  if (((*(int *)(puVar23[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
     (test_normal_cap_use_iram != 0)) {
    lVar21 = *(long *)(lVar6 + 0x38);
    if (lVar21 != 0) {
      uVar7 = 0x12;
      iVar24 = 0;
LAB_00102a40:
      audio_mem_free(uVar7,lVar21,iVar24);
      *(undefined8 *)(lVar6 + 0x38) = 0;
    }
  }
  else {
    lVar21 = *(long *)(lVar6 + 0x38);
    if (lVar21 != 0) {
      uVar7 = 5;
      iVar24 = *(int *)(lVar20 + 0x1a4) * 0x78;
      goto LAB_00102a40;
    }
  }
  if (*(long *)(param_1 + 0x110) != 0) {
    audio_mem_unmap();
    *(undefined8 *)(param_1 + 0x110) = 0;
  }
  if (((*(int *)(puVar23[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
     (test_normal_cap_use_iram != 0)) {
    lVar20 = *(long *)(param_1 + 0x118);
    if (lVar20 != 0) {
      uVar7 = 0x13;
      uVar3 = 0;
LAB_00102a74:
      audio_mem_free(uVar7,lVar20,uVar3);
      *(undefined8 *)(param_1 + 0x118) = 0;
    }
  }
  else {
    lVar20 = *(long *)(param_1 + 0x118);
    if (lVar20 != 0) {
      uVar3 = *(undefined4 *)(param_1 + 0x120);
      uVar7 = 5;
      goto LAB_00102a74;
    }
  }
  devm_kfree(uVar14,lVar6);
LAB_00102714:
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return iVar5;
}


