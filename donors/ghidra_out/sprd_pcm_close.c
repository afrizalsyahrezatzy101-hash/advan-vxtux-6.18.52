// sprd_pcm_close @ 00102e9c
// signature: undefined sprd_pcm_close(void)
// calling convention: unknown


undefined8 sprd_pcm_close(long param_1)

{
  char *pcVar1;
  int *piVar2;
  undefined8 uVar3;
  long lVar4;
  int iVar5;
  undefined4 uVar6;
  undefined *puVar7;
  undefined *puVar8;
  undefined8 *in_x18;
  long lVar9;
  undefined8 uVar10;
  undefined8 *puVar11;
  long lVar12;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  piVar2 = pm_dma;
  puVar11 = *(undefined8 **)(param_1 + 0x10);
  lVar12 = *(long *)(param_1 + 0x140);
  lVar9 = *(long *)(lVar12 + 0x150);
  iVar5 = *(int *)(((undefined8 *)puVar11[0xbf])[3] + 8);
  if (iVar5 == 0x124) {
    puVar7 = &DAT_0010150d;
  }
  else if (iVar5 == 0x129) {
    puVar7 = &DAT_00101128;
  }
  else {
    puVar8 = *(undefined **)puVar11[0xbf];
    puVar7 = &DAT_00100602;
    if (puVar8 != (undefined *)0x0) {
      puVar7 = puVar8;
    }
  }
  uVar10 = *puVar11;
  pcVar1 = "Playback";
  if (*(int *)(param_1 + 0x3c) != 0) {
    pcVar1 = "Captrue";
  }
  printk(&DAT_00101145,"sprd_pcm_close",puVar7,pcVar1);
  if (lVar9 == 0) {
    return 0xffffffea;
  }
  mutex_lock(piVar2 + 0x14);
  if (((*(int *)(puVar11[0xbf] + 8) == 0xb) || (*(int *)(puVar11[0xbf] + 8) == 0)) &&
     (*(int *)(param_1 + 0x3c) == 0)) {
    piVar2[2] = 0;
    piVar2[3] = 0;
  }
  else {
    iVar5 = *piVar2;
    *piVar2 = iVar5 + -1;
    if (iVar5 + -1 < 0) {
      printk(&DAT_00100de3,"sprd_pcm_close");
      *piVar2 = 0;
    }
  }
  if (*(char *)(lVar9 + 0xa0) != '\0') {
    agdsp_access_disable();
    *(undefined1 *)(lVar9 + 0xa0) = 0;
  }
  mutex_unlock(piVar2 + 0x14);
  if ((*(uint *)(puVar11[0xbf] + 8) < 0x15) &&
     ((1 << (ulong)(*(uint *)(puVar11[0xbf] + 8) & 0x1f) & 0x1880a0U) != 0)) goto LAB_00102ff0;
  devm_kfree(uVar10,*(undefined8 *)(lVar9 + 0x28));
  devm_kfree(uVar10,*(undefined8 *)(lVar9 + 0x20));
  if (*(long *)(lVar9 + 0x40) != 0) {
    audio_mem_unmap();
    *(undefined8 *)(lVar9 + 0x40) = 0;
  }
  if (((*(int *)(puVar11[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
     (test_normal_cap_use_iram != 0)) {
    lVar4 = *(long *)(lVar9 + 0x30);
    if (lVar4 != 0) {
      uVar3 = 0x11;
      iVar5 = 0;
LAB_00103094:
      audio_mem_free(uVar3,lVar4,iVar5);
      *(undefined8 *)(lVar9 + 0x30) = 0;
    }
  }
  else {
    lVar4 = *(long *)(lVar9 + 0x30);
    if (lVar4 != 0) {
      uVar3 = 5;
      iVar5 = *(int *)(lVar12 + 0x1a4) * 0x78;
      goto LAB_00103094;
    }
  }
  if (*(long *)(lVar9 + 0x48) != 0) {
    audio_mem_unmap();
    *(undefined8 *)(lVar9 + 0x48) = 0;
  }
  if (((*(int *)(puVar11[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
     (test_normal_cap_use_iram != 0)) {
    lVar4 = *(long *)(lVar9 + 0x38);
    if (lVar4 != 0) {
      uVar3 = 0x12;
      iVar5 = 0;
LAB_001030fc:
      audio_mem_free(uVar3,lVar4,iVar5);
      *(undefined8 *)(lVar9 + 0x38) = 0;
    }
  }
  else {
    lVar4 = *(long *)(lVar9 + 0x38);
    if (lVar4 != 0) {
      uVar3 = 5;
      iVar5 = *(int *)(lVar12 + 0x1a4) * 0x78;
      goto LAB_001030fc;
    }
  }
  if (*(long *)(param_1 + 0x110) != 0) {
    audio_mem_unmap();
    *(undefined8 *)(param_1 + 0x110) = 0;
  }
  if (((*(int *)(puVar11[0xbf] + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
     (test_normal_cap_use_iram != 0)) {
    lVar12 = *(long *)(param_1 + 0x118);
    if (lVar12 == 0) goto LAB_00102ff0;
    uVar3 = 0x13;
    uVar6 = 0;
  }
  else {
    lVar12 = *(long *)(param_1 + 0x118);
    if (lVar12 == 0) goto LAB_00102ff0;
    uVar6 = *(undefined4 *)(param_1 + 0x120);
    uVar3 = 5;
  }
  audio_mem_free(uVar3,lVar12,uVar6);
  *(undefined8 *)(param_1 + 0x118) = 0;
LAB_00102ff0:
  devm_kfree(uVar10,lVar9);
  return 0;
}


