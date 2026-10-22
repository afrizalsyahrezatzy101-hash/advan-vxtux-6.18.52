// ce_aes_ccm_final @ 00101104
// signature: undefined ce_aes_ccm_final(void)
// calling convention: unknown


void ce_aes_ccm_final(undefined1 (*param_1) [16],undefined1 (*param_2) [16],undefined8 *param_3,
                     int param_4)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined1 auVar4 [16];
  undefined1 auVar5 [16];
  undefined1 auVar6 [16];
  undefined8 *puVar7;
  undefined1 (*pauVar8) [16];
  int iVar9;
  undefined1 auVar10 [16];
  undefined1 auVar11 [16];
  undefined4 uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  undefined4 uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  undefined4 uVar19;
  undefined4 uVar20;
  undefined4 uVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  
  uVar12 = *(undefined4 *)param_3;
  uVar13 = *(undefined4 *)((long)param_3 + 4);
  puVar7 = param_3 + 1;
  uVar14 = *(undefined4 *)puVar7;
  uVar15 = *(undefined4 *)((long)param_3 + 0xc);
  pauVar8 = (undefined1 (*) [16])(param_3 + 2);
  auVar10 = *param_1;
  iVar9 = param_4 + -2;
  auVar11 = *param_2;
  if (-1 < param_4 + -0xc) {
    if (param_4 != 0xc) goto LAB_00101154;
    uVar22 = (undefined4)*puVar7;
    uVar23 = (undefined4)((ulong)*puVar7 >> 0x20);
    uVar20 = (undefined4)*param_3;
    uVar21 = (undefined4)((ulong)*param_3 >> 0x20);
    while( true ) {
      uVar12 = *(undefined4 *)*pauVar8;
      uVar13 = *(undefined4 *)((long)*pauVar8 + 4);
      uVar14 = *(undefined4 *)((long)*pauVar8 + 8);
      uVar15 = *(undefined4 *)((long)*pauVar8 + 0xc);
      pauVar8 = pauVar8 + 1;
      auVar5._4_4_ = uVar21;
      auVar5._0_4_ = uVar20;
      auVar5._8_4_ = uVar22;
      auVar5._12_4_ = uVar23;
      auVar10 = NEON_aese(auVar10,auVar5);
      auVar10 = NEON_aesmc(auVar10,auVar10);
      auVar6._4_4_ = uVar21;
      auVar6._0_4_ = uVar20;
      auVar6._8_4_ = uVar22;
      auVar6._12_4_ = uVar23;
      auVar11 = NEON_aese(auVar11,auVar6);
      auVar11 = NEON_aesmc(auVar11,auVar11);
LAB_00101154:
      uVar16 = *(undefined4 *)*pauVar8;
      uVar17 = *(undefined4 *)((long)*pauVar8 + 4);
      uVar18 = *(undefined4 *)((long)*pauVar8 + 8);
      uVar19 = *(undefined4 *)((long)*pauVar8 + 0xc);
      auVar6 = *pauVar8;
      auVar5 = *pauVar8;
      pauVar8 = pauVar8 + 1;
      iVar9 = iVar9 + -3;
      auVar1._4_4_ = uVar13;
      auVar1._0_4_ = uVar12;
      auVar1._8_4_ = uVar14;
      auVar1._12_4_ = uVar15;
      auVar10 = NEON_aese(auVar10,auVar1);
      auVar10 = NEON_aesmc(auVar10,auVar10);
      auVar2._4_4_ = uVar13;
      auVar2._0_4_ = uVar12;
      auVar2._8_4_ = uVar14;
      auVar2._12_4_ = uVar15;
      auVar11 = NEON_aese(auVar11,auVar2);
      auVar11 = NEON_aesmc(auVar11,auVar11);
      if (iVar9 < 0) break;
LAB_0010112c:
      uVar20 = *(undefined4 *)*pauVar8;
      uVar21 = *(undefined4 *)((long)*pauVar8 + 4);
      uVar22 = *(undefined4 *)((long)*pauVar8 + 8);
      uVar23 = *(undefined4 *)((long)*pauVar8 + 0xc);
      pauVar8 = pauVar8 + 1;
      auVar3._4_4_ = uVar17;
      auVar3._0_4_ = uVar16;
      auVar3._8_4_ = uVar18;
      auVar3._12_4_ = uVar19;
      auVar10 = NEON_aese(auVar10,auVar3);
      auVar10 = NEON_aesmc(auVar10,auVar10);
      auVar4._4_4_ = uVar17;
      auVar4._0_4_ = uVar16;
      auVar4._8_4_ = uVar18;
      auVar4._12_4_ = uVar19;
      auVar11 = NEON_aese(auVar11,auVar4);
      auVar11 = NEON_aesmc(auVar11,auVar11);
    }
    auVar10 = NEON_aese(auVar10,auVar5);
    auVar11 = NEON_aese(auVar11,auVar6);
    (*param_1)[0] = auVar10[0] ^ auVar11[0];
    (*param_1)[1] = auVar10[1] ^ auVar11[1];
    (*param_1)[2] = auVar10[2] ^ auVar11[2];
    (*param_1)[3] = auVar10[3] ^ auVar11[3];
    (*param_1)[4] = auVar10[4] ^ auVar11[4];
    (*param_1)[5] = auVar10[5] ^ auVar11[5];
    (*param_1)[6] = auVar10[6] ^ auVar11[6];
    (*param_1)[7] = auVar10[7] ^ auVar11[7];
    (*param_1)[8] = auVar10[8] ^ auVar11[8];
    (*param_1)[9] = auVar10[9] ^ auVar11[9];
    (*param_1)[10] = auVar10[10] ^ auVar11[10];
    (*param_1)[0xb] = auVar10[0xb] ^ auVar11[0xb];
    (*param_1)[0xc] = auVar10[0xc] ^ auVar11[0xc];
    (*param_1)[0xd] = auVar10[0xd] ^ auVar11[0xd];
    (*param_1)[0xe] = auVar10[0xe] ^ auVar11[0xe];
    (*param_1)[0xf] = auVar10[0xf] ^ auVar11[0xf];
    return;
  }
  uVar18 = (undefined4)*puVar7;
  uVar19 = (undefined4)((ulong)*puVar7 >> 0x20);
  uVar16 = (undefined4)*param_3;
  uVar17 = (undefined4)((ulong)*param_3 >> 0x20);
  goto LAB_0010112c;
}


