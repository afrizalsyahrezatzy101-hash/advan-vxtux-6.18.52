// sprd_pcm_config_dma @ 00104784
// signature: undefined sprd_pcm_config_dma(void)
// calling convention: unknown


undefined8 sprd_pcm_config_dma(long param_1,uint *param_2,long param_3,long param_4)

{
  long lVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  ulong uVar5;
  undefined8 uVar6;
  ulong uVar7;
  ulong uVar8;
  int iVar9;
  ulong uVar10;
  undefined4 uVar11;
  long lVar12;
  long *plVar13;
  long lVar14;
  ushort *puVar15;
  uint uVar16;
  long *plVar17;
  long lVar18;
  int iVar19;
  undefined8 *in_x18;
  ulong uVar20;
  undefined4 *puVar21;
  long lVar22;
  undefined4 *puVar23;
  int iVar24;
  uint *puVar25;
  ulong uVar26;
  ulong uVar27;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar2 = *(uint *)(param_1 + 0x3c);
  if (1 < uVar2) {
                    /* WARNING: Subroutine does not return */
    __ubsan_handle_cfi_check_fail_abort(0x12);
  }
  lVar22 = *(long *)(param_1 + 0x10);
  lVar12 = *(long *)(*(long *)(param_1 + 0x140) + 0x150);
  plVar13 = (long *)(lVar22 + (ulong)uVar2 * 0x2a0 + 0x98);
  for (plVar17 = (long *)*plVar13; plVar13 != plVar17; plVar17 = (long *)*plVar17) {
    if (plVar17[-2] == lVar22) {
      lVar14 = plVar17[-3];
      if ((lVar14 == 0) || (lVar18 = *(long *)(lVar14 + 0x5f8), *(int *)(lVar18 + 8) != 0x125))
      goto LAB_00104818;
      goto LAB_0010481c;
    }
  }
  lVar14 = 0;
LAB_00104818:
  lVar18 = *(long *)(lVar22 + 0x5f8);
LAB_0010481c:
  lVar1 = 0x40;
  if (uVar2 != 0) {
    lVar1 = 0x48;
  }
  lVar18 = *(long *)(lVar18 + lVar1);
  if (lVar18 == 0) {
    printk(&DAT_00100d42,"sprd_pcm_config_dma");
    return 0xffffffea;
  }
  uVar16 = param_2[0x47];
  uVar20 = (ulong)param_2[0x5f];
  uVar3 = param_2[0x53];
  uVar4 = *param_2;
  iVar24 = *(int *)(*(long *)(*(long *)(lVar22 + 0x5f8) + 0x18) + 8);
  if (*(int *)(lVar18 + 0x98) == 1) {
    uVar16 = 1;
  }
  uVar10 = (ulong)uVar16;
  if (iVar24 == 0x129) {
    uVar10 = (ulong)*(uint *)(lVar12 + 0x78);
    if (uVar2 == 0) {
      puVar15 = (ushort *)&DAT_0000003a;
      goto LAB_001048b8;
    }
    puVar15 = (ushort *)&DAT_0000003c;
LAB_00104918:
    uVar16 = (uint)*puVar15;
LAB_0010491c:
    iVar24 = *(int *)(lVar18 + 0x74);
    uVar16 = uVar16 & 0xffff;
  }
  else {
    if (iVar24 == 0x124) {
      uVar10 = (ulong)*(uint *)(lVar12 + 0x78);
      if (uVar2 != 0) {
        puVar15 = (ushort *)&DAT_00000016;
        goto LAB_00104918;
      }
      puVar15 = (ushort *)&DAT_00000014;
LAB_001048b8:
      uVar16 = 0x20 - *puVar15;
      goto LAB_0010491c;
    }
    if ((lVar14 == 0) || (*(int *)(*(long *)(lVar14 + 0x5f8) + 8) != 0x125)) {
      iVar24 = *(int *)(lVar18 + 0x74);
      iVar19 = iVar24;
      if (iVar24 != 2) {
        iVar19 = 4;
      }
      iVar19 = iVar19 * *(int *)(lVar18 + 0x80);
      goto LAB_00104928;
    }
    uVar16 = *(uint *)(lVar18 + 0x80);
    iVar24 = *(int *)(lVar18 + 0x74);
    uVar10 = (ulong)*(uint *)(lVar12 + 0x78);
  }
  iVar19 = iVar24 * uVar16;
LAB_00104928:
  iVar9 = (int)uVar10;
  if (iVar9 < 3) {
    if (iVar9 < 1) {
      printk(&DAT_00101030);
      uVar6 = 0xffffffea;
    }
    else {
      uVar16 = 0;
      if (uVar3 != 0) {
        uVar16 = param_2[0x5f] / uVar3;
      }
      lVar14 = 0;
      puVar25 = (uint *)(lVar18 + 0x90);
      while( true ) {
        puVar21 = *(undefined4 **)(param_3 + lVar14 * 8);
        puVar21[6] = iVar24;
        puVar21[7] = *(undefined4 *)(lVar18 + 0x74);
        puVar21[8] = iVar19;
        puVar21[0xd] = puVar25[-0x22];
        if (uVar2 == 0) {
          puVar21[10] = *(undefined4 *)(lVar18 + 0x78);
          uVar7 = audio_addr_ap2dsp(5,*(undefined8 *)(param_4 + lVar14 * 8),0);
          *(ulong *)(puVar21 + 2) = uVar7 & 0xffffffff;
          uVar7 = (ulong)*puVar25;
          uVar11 = 1;
        }
        else {
          puVar21[0xb] = *(undefined4 *)(lVar18 + 0x7c);
          *(ulong *)(puVar21 + 2) = (ulong)*puVar25;
          if (((*(int *)(*(long *)(lVar22 + 0x5f8) + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
             (test_normal_cap_use_iram != 0)) {
            uVar6 = 0x13;
          }
          else {
            uVar6 = 5;
          }
          uVar7 = audio_addr_ap2dsp(uVar6,*(undefined8 *)(param_4 + lVar14 * 8),0);
          uVar11 = 2;
        }
        *puVar21 = uVar11;
        *(ulong *)(puVar21 + 4) = uVar7 & 0xffffffff;
        *(ulong *)(puVar21 + 0x14) = (ulong)uVar4 & 4 ^ 4;
        *(undefined8 *)(puVar21 + 0xe) = *(undefined8 *)(lVar12 + lVar14 * 8 + 0x40);
        if (((*(int *)(*(long *)(lVar22 + 0x5f8) + 8) == 0) && (*(int *)(param_1 + 0x3c) == 1)) &&
           (test_normal_cap_use_iram != 0)) {
          uVar6 = 0x11;
        }
        else {
          uVar6 = 5;
        }
        uVar7 = audio_addr_ap2dsp(uVar6,*(undefined4 *)(lVar12 + lVar14 * 8 + 0x30),0);
        *(ulong *)(puVar21 + 0x10) = uVar7 & 0xffffffff;
        printk(&DAT_0010092f,puVar21[6],puVar21[7],*(undefined8 *)(puVar21 + 0x14),puVar21[8],
               puVar21[0xd],puVar21[10],*(undefined8 *)(puVar21 + 0xe),uVar7 & 0xffffffff,
               *(undefined8 *)(puVar21 + 4),*(undefined8 *)(puVar21 + 2),(int)lVar14);
        if (uVar10 - 1 == lVar14) break;
        iVar24 = *(int *)(lVar18 + 0x74);
        lVar14 = lVar14 + 1;
        puVar25 = puVar25 + 1;
      }
      uVar26 = 0;
      uVar7 = (ulong)uVar3;
      do {
        uVar27 = 0;
        do {
          puVar23 = *(undefined4 **)(param_3 + uVar27 * 8);
          lVar14 = *(long *)(puVar23 + 0x1c) +
                   (-(uVar26 >> 0x1f) & 0xffffffe000000000 | uVar26 << 5);
          iVar24 = (int)uVar26;
          if (iVar24 == 0) {
            sg_init_table(lVar14,uVar16);
            puVar23[0x1b] = uVar16;
            puVar21 = puVar23;
          }
          uVar5 = 0;
          if ((long)iVar9 != 0) {
            uVar5 = uVar7 / (ulong)(long)iVar9;
          }
          *(int *)(lVar14 + 0x18) = (int)uVar5;
          if (((uVar2 == 0) || (*(int *)(*(long *)(lVar22 + 0x5f8) + 8) != 0)) ||
             ((*(int *)(param_1 + 0x3c) != 1 || (test_normal_cap_use_iram == 0)))) {
            uVar6 = 5;
          }
          else {
            uVar6 = 0x13;
          }
          lVar18 = uVar27 * 8;
          uVar8 = audio_addr_ap2dsp(uVar6,*(undefined8 *)(param_4 + lVar18),0);
          *(ulong *)(lVar14 + 0x10) = uVar8 & 0xffffffff;
          if (*(int *)(lVar12 + 0x8c) != 0) {
            uVar5 = uVar7;
          }
          *(ulong *)(param_4 + lVar18) = *(long *)(param_4 + lVar18) + uVar5;
          if (iVar24 == 0) {
            uVar6 = *(undefined8 *)(puVar21 + 0x14);
          }
          else {
            uVar6 = 0;
          }
          printk(&DAT_00100d6c,uVar7,uVar16,*(undefined8 *)(lVar14 + 0x10),uVar6,uVar27 & 0xffffffff
                 ,uVar26);
          uVar27 = uVar27 + 1;
        } while (uVar10 != uVar27);
        uVar27 = uVar20;
        if (uVar7 <= uVar20) {
          uVar27 = uVar7;
        }
        uVar20 = uVar20 - uVar27;
        uVar26 = (ulong)(iVar24 + 1);
        uVar7 = uVar27;
      } while (uVar20 != 0);
      printk(&DAT_00100dc6,uVar26);
      uVar6 = 0;
    }
  }
  else {
    printk(&DAT_0010044d,uVar10,2);
    uVar6 = 0xffffffea;
  }
  return uVar6;
}


