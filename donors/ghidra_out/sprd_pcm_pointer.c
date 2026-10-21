// sprd_pcm_pointer @ 001042ac
// signature: undefined sprd_pcm_pointer(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long sprd_pcm_pointer(long param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  undefined4 uVar6;
  undefined8 *in_x18;
  long lVar7;
  long *plVar8;
  long lVar9;
  code *pcVar10;
  long lVar11;
  uint uVar12;
  ulong uVar13;
  int *piVar14;
  undefined8 unaff_x30;
  undefined8 local_78;
  int local_70;
  long local_68;
  
  *in_x18 = unaff_x30;
  local_68 = ___stack_chk_guard;
  lVar11 = *(long *)(param_1 + 0x140);
  piVar14 = *(int **)(lVar11 + 0x150);
  iVar1 = piVar14[0x23];
  iVar5 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
  iVar2 = piVar14[0x1a];
  if (((iVar5 == 0xb) || (iVar5 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
    _raw_spin_lock(pm_dma + 0x28);
  }
  plVar8 = *(long **)(piVar14 + 4);
  if (plVar8 == (long *)0x0) {
    uVar13 = 0xffffffff;
  }
  else {
    iVar5 = piVar14[0x18];
    local_70 = 0;
    local_78 = 0;
    pcVar10 = *(code **)(*plVar8 + 0x120);
    __cfi_slowpath(0x3c61e5e5db80f04c,pcVar10);
    (*pcVar10)(plVar8,iVar5,&local_78);
    iVar5 = local_70;
    lVar9 = (long)local_70;
    if (init_pm_dma___key_3 != 0) {
      uVar6 = audio_addr_ap2dsp(iVar2,local_70,1);
      printk(&DAT_001008eb,lVar9,uVar6);
    }
    iVar5 = audio_addr_ap2dsp(iVar2,iVar5,1);
    uVar3 = iVar5 - (int)*(undefined8 *)(lVar11 + 0x2c0);
    uVar13 = (ulong)uVar3;
    if (init_pm_dma___key_3 != 0) {
      printk(&DAT_001011a3,(long)(int)uVar3);
    }
  }
  plVar8 = *(long **)(piVar14 + 6);
  if (plVar8 == (long *)0x0) goto LAB_001044d0;
  iVar5 = piVar14[0x19];
  local_70 = 0;
  local_78 = 0;
  pcVar10 = *(code **)(*plVar8 + 0x120);
  __cfi_slowpath(0x3c61e5e5db80f04c,pcVar10);
  (*pcVar10)(plVar8,iVar5,&local_78);
  iVar5 = local_70;
  lVar9 = (long)local_70;
  if (init_pm_dma___key_3 != 0) {
    uVar6 = audio_addr_ap2dsp(iVar2,local_70,1);
    printk(&DAT_00100b79,lVar9,uVar6);
  }
  iVar5 = audio_addr_ap2dsp(iVar2,iVar5,1);
  uVar3 = (iVar5 - (int)*(undefined8 *)(lVar11 + 0x2c0)) - *piVar14;
  uVar12 = (uint)uVar13;
  if (init_pm_dma___key_3 != 0) {
    printk(&DAT_00100414,(long)(int)uVar3);
  }
  uVar4 = uVar3;
  if (uVar12 != 0xffffffff) {
    if (piVar14[0x21] == 0) {
      if ((int)uVar12 < piVar14[0x1f]) {
        iVar5 = piVar14[0x22];
        piVar14[0x21] = 1;
        goto joined_r0x00104464;
      }
      if (piVar14[0x22] != 0) goto LAB_001044b8;
      if ((int)uVar3 < piVar14[0x20]) {
        piVar14[0x22] = 1;
        goto LAB_001044b8;
      }
      piVar14[0x1f] = uVar12;
LAB_00104488:
      piVar14[0x20] = uVar3;
      if ((int)uVar3 <= (int)uVar12) {
        uVar12 = uVar3;
      }
    }
    else {
      iVar5 = piVar14[0x22];
joined_r0x00104464:
      if (iVar5 != 0) {
LAB_0010447c:
        piVar14[0x21] = 0;
        piVar14[0x22] = 0;
        piVar14[0x1f] = uVar12;
        goto LAB_00104488;
      }
      if ((int)uVar3 < piVar14[0x20]) {
        piVar14[0x22] = 1;
        goto LAB_0010447c;
      }
LAB_001044b8:
      piVar14[0x1f] = uVar12;
      piVar14[0x20] = uVar3;
      if ((int)uVar12 <= (int)uVar3) {
        uVar12 = uVar3;
      }
    }
    uVar4 = uVar12 << (iVar1 == 0);
  }
  uVar13 = (ulong)uVar4;
  if (init_pm_dma___key_3 != 0) {
    printk(&DAT_00100c80,(long)(int)uVar3);
  }
LAB_001044d0:
  iVar1 = *(int *)(*(long *)(*(long *)(param_1 + 0x10) + 0x5f8) + 8);
  if (((iVar1 == 0xb) || (iVar1 == 0)) && (*(int *)(param_1 + 0x3c) == 0)) {
    _raw_spin_unlock(pm_dma + 0x28);
  }
  lVar9 = 0;
  if ((ulong)*(uint *)(lVar11 + 0x98) != 0) {
    lVar9 = (long)(-(uVar13 >> 0x1f) & 0xfffffff800000000 | uVar13 << 3) /
            (long)(ulong)*(uint *)(lVar11 + 0x98);
  }
  if (init_pm_dma___key_3 == 0) {
    lVar7 = 0;
    if (lVar9 != *(long *)(lVar11 + 0x80)) {
      lVar7 = lVar9;
    }
  }
  else {
    printk(&DAT_00101003,lVar9);
    lVar7 = 0;
    if (lVar9 != *(long *)(lVar11 + 0x80)) {
      lVar7 = lVar9;
    }
    if (init_pm_dma___key_3 != 0) {
      printk(&DAT_00100fe9,lVar7);
    }
  }
  if (___stack_chk_guard != local_68) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return lVar7;
}


