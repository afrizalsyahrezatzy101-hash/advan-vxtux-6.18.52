// ce_aes_ccm_auth_data @ 00101000
// signature: undefined ce_aes_ccm_auth_data(void)
// calling convention: unknown


void ce_aes_ccm_auth_data
               (undefined1 (*param_1) [16],byte *param_2,int param_3,int *param_4,
               undefined1 (*param_5) [16],int param_6)

{
  undefined1 auVar1 [16];
  undefined1 auVar2 [16];
  undefined1 auVar3 [16];
  undefined8 uVar4;
  undefined8 uVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte *pbVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte *pbVar11;
  byte *pbVar12;
  byte *pbVar13;
  byte *pbVar14;
  byte *pbVar15;
  byte *pbVar16;
  byte *pbVar17;
  byte *pbVar18;
  byte *pbVar19;
  undefined8 *puVar20;
  byte *pbVar21;
  int iVar22;
  undefined1 (*pauVar23) [16];
  int iVar24;
  int iVar25;
  int iVar26;
  undefined1 auVar27 [16];
  undefined1 auVar28 [16];
  undefined4 uVar29;
  undefined4 uVar30;
  undefined4 uVar31;
  undefined4 uVar32;
  undefined4 uVar33;
  undefined4 uVar34;
  undefined4 uVar35;
  undefined4 uVar36;
  byte bVar37;
  byte bVar38;
  byte bVar39;
  byte bVar40;
  byte bVar41;
  byte bVar42;
  byte bVar43;
  byte bVar44;
  byte bVar45;
  byte bVar46;
  byte bVar47;
  byte bVar48;
  byte bVar49;
  byte bVar50;
  byte bVar51;
  byte bVar52;
  
  iVar25 = *param_4;
  auVar27 = *param_1;
  if (iVar25 != 0) {
    auVar28 = ZEXT216(0);
    pbVar21 = param_2;
    iVar24 = iVar25 + -0x10;
    do {
      param_2 = pbVar21 + 1;
      param_3 = param_3 + -1;
      iVar25 = iVar24 + 1;
      auVar28[0] = *pbVar21;
      auVar28 = NEON_ext(auVar28,auVar28,1,1);
      bVar37 = auVar27[1];
      bVar38 = auVar27[2];
      bVar39 = auVar27[3];
      bVar40 = auVar27[4];
      bVar41 = auVar27[5];
      bVar42 = auVar27[6];
      bVar43 = auVar27[7];
      bVar44 = auVar27[8];
      bVar45 = auVar27[9];
      bVar46 = auVar27[10];
      bVar47 = auVar27[0xb];
      bVar48 = auVar27[0xc];
      bVar49 = auVar27[0xd];
      bVar50 = auVar27[0xe];
      bVar51 = auVar27[0xf];
      if (param_3 == 0) {
        iVar26 = iVar25;
        if (iVar25 != 0) {
          iVar26 = iVar24 + 0x11;
          do {
            auVar28 = NEON_ext(auVar28,auVar28,1,1);
            iVar25 = iVar25 + 1;
          } while (iVar25 != 0);
        }
        (*param_1)[0] = auVar27[0] ^ auVar28[0];
        (*param_1)[1] = bVar37 ^ auVar28[1];
        (*param_1)[2] = bVar38 ^ auVar28[2];
        (*param_1)[3] = bVar39 ^ auVar28[3];
        (*param_1)[4] = bVar40 ^ auVar28[4];
        (*param_1)[5] = bVar41 ^ auVar28[5];
        (*param_1)[6] = bVar42 ^ auVar28[6];
        (*param_1)[7] = bVar43 ^ auVar28[7];
        (*param_1)[8] = bVar44 ^ auVar28[8];
        (*param_1)[9] = bVar45 ^ auVar28[9];
        (*param_1)[10] = bVar46 ^ auVar28[10];
        (*param_1)[0xb] = bVar47 ^ auVar28[0xb];
        (*param_1)[0xc] = bVar48 ^ auVar28[0xc];
        (*param_1)[0xd] = bVar49 ^ auVar28[0xd];
        (*param_1)[0xe] = bVar50 ^ auVar28[0xe];
        (*param_1)[0xf] = bVar51 ^ auVar28[0xf];
        goto LAB_001010fc;
      }
      pbVar21 = param_2;
      iVar24 = iVar25;
    } while (iVar25 != 0);
    auVar27[0] = auVar27[0] ^ auVar28[0];
    auVar27[1] = bVar37 ^ auVar28[1];
    auVar27[2] = bVar38 ^ auVar28[2];
    auVar27[3] = bVar39 ^ auVar28[3];
    auVar27[4] = bVar40 ^ auVar28[4];
    auVar27[5] = bVar41 ^ auVar28[5];
    auVar27[6] = bVar42 ^ auVar28[6];
    auVar27[7] = bVar43 ^ auVar28[7];
    auVar27[8] = bVar44 ^ auVar28[8];
    auVar27[9] = bVar45 ^ auVar28[9];
    auVar27[10] = bVar46 ^ auVar28[10];
    auVar27[0xb] = bVar47 ^ auVar28[0xb];
    auVar27[0xc] = bVar48 ^ auVar28[0xc];
    auVar27[0xd] = bVar49 ^ auVar28[0xd];
    auVar27[0xe] = bVar50 ^ auVar28[0xe];
    auVar27[0xf] = bVar51 ^ auVar28[0xf];
  }
  do {
    iVar22 = param_3;
    uVar29 = *(undefined4 *)*param_5;
    uVar30 = *(undefined4 *)(*param_5 + 4);
    uVar4 = *(undefined8 *)*param_5;
    puVar20 = (undefined8 *)(*param_5 + 8);
    uVar31 = *(undefined4 *)puVar20;
    uVar32 = *(undefined4 *)(*param_5 + 0xc);
    uVar5 = *puVar20;
    Hint_Prefetch(param_2,0,0,1);
    pauVar23 = param_5 + 1;
    iVar24 = param_6 + -2;
    if (param_6 + -0xc < 0) {
      uVar35 = (undefined4)*puVar20;
      uVar36 = (undefined4)((ulong)*puVar20 >> 0x20);
      uVar33 = (undefined4)*(undefined8 *)*param_5;
      uVar34 = (undefined4)((ulong)*(undefined8 *)*param_5 >> 0x20);
      uVar29 = *(undefined4 *)*pauVar23;
      bVar37 = (byte)uVar29;
      bVar38 = (byte)((uint)uVar29 >> 8);
      bVar39 = (byte)((uint)uVar29 >> 0x10);
      bVar40 = (byte)((uint)uVar29 >> 0x18);
      uVar29 = *(undefined4 *)(param_5[1] + 4);
      bVar41 = (byte)uVar29;
      bVar42 = (byte)((uint)uVar29 >> 8);
      bVar43 = (byte)((uint)uVar29 >> 0x10);
      bVar44 = (byte)((uint)uVar29 >> 0x18);
      uVar29 = *(undefined4 *)(param_5[1] + 8);
      bVar45 = (byte)uVar29;
      bVar46 = (byte)((uint)uVar29 >> 8);
      bVar47 = (byte)((uint)uVar29 >> 0x10);
      bVar48 = (byte)((uint)uVar29 >> 0x18);
      uVar29 = *(undefined4 *)(param_5[1] + 0xc);
      bVar49 = (byte)uVar29;
      bVar50 = (byte)((uint)uVar29 >> 8);
      bVar51 = (byte)((uint)uVar29 >> 0x10);
      bVar52 = (byte)((uint)uVar29 >> 0x18);
      pauVar23 = param_5;
      goto LAB_00101060;
    }
    if (param_6 != 0xc) goto LAB_00101074;
    bVar45 = (byte)uVar5;
    bVar46 = (byte)((ulong)uVar5 >> 8);
    bVar47 = (byte)((ulong)uVar5 >> 0x10);
    bVar48 = (byte)((ulong)uVar5 >> 0x18);
    bVar49 = (byte)((ulong)uVar5 >> 0x20);
    bVar50 = (byte)((ulong)uVar5 >> 0x28);
    bVar51 = (byte)((ulong)uVar5 >> 0x30);
    bVar52 = (byte)((ulong)uVar5 >> 0x38);
    bVar37 = (byte)uVar4;
    bVar38 = (byte)((ulong)uVar4 >> 8);
    bVar39 = (byte)((ulong)uVar4 >> 0x10);
    bVar40 = (byte)((ulong)uVar4 >> 0x18);
    bVar41 = (byte)((ulong)uVar4 >> 0x20);
    bVar42 = (byte)((ulong)uVar4 >> 0x28);
    bVar43 = (byte)((ulong)uVar4 >> 0x30);
    bVar44 = (byte)((ulong)uVar4 >> 0x38);
    while( true ) {
      uVar29 = *(undefined4 *)*pauVar23;
      uVar30 = *(undefined4 *)(*pauVar23 + 4);
      uVar31 = *(undefined4 *)(*pauVar23 + 8);
      uVar32 = *(undefined4 *)(*pauVar23 + 0xc);
      pauVar23 = pauVar23 + 1;
      auVar3[1] = bVar38;
      auVar3[0] = bVar37;
      auVar3[2] = bVar39;
      auVar3[3] = bVar40;
      auVar3[4] = bVar41;
      auVar3[5] = bVar42;
      auVar3[6] = bVar43;
      auVar3[7] = bVar44;
      auVar3[8] = bVar45;
      auVar3[9] = bVar46;
      auVar3[10] = bVar47;
      auVar3[0xb] = bVar48;
      auVar3[0xc] = bVar49;
      auVar3[0xd] = bVar50;
      auVar3[0xe] = bVar51;
      auVar3[0xf] = bVar52;
      auVar27 = NEON_aese(auVar27,auVar3);
      auVar27 = NEON_aesmc(auVar27,auVar27);
LAB_00101074:
      uVar33 = *(undefined4 *)*pauVar23;
      uVar34 = *(undefined4 *)(*pauVar23 + 4);
      uVar35 = *(undefined4 *)(*pauVar23 + 8);
      uVar36 = *(undefined4 *)(*pauVar23 + 0xc);
      iVar24 = iVar24 + -3;
      auVar1._4_4_ = uVar30;
      auVar1._0_4_ = uVar29;
      auVar1._8_4_ = uVar31;
      auVar1._12_4_ = uVar32;
      auVar27 = NEON_aese(auVar27,auVar1);
      auVar27 = NEON_aesmc(auVar27,auVar27);
      uVar29 = *(undefined4 *)pauVar23[1];
      bVar37 = (byte)uVar29;
      bVar38 = (byte)((uint)uVar29 >> 8);
      bVar39 = (byte)((uint)uVar29 >> 0x10);
      bVar40 = (byte)((uint)uVar29 >> 0x18);
      uVar29 = *(undefined4 *)(pauVar23[1] + 4);
      bVar41 = (byte)uVar29;
      bVar42 = (byte)((uint)uVar29 >> 8);
      bVar43 = (byte)((uint)uVar29 >> 0x10);
      bVar44 = (byte)((uint)uVar29 >> 0x18);
      uVar29 = *(undefined4 *)(pauVar23[1] + 8);
      bVar45 = (byte)uVar29;
      bVar46 = (byte)((uint)uVar29 >> 8);
      bVar47 = (byte)((uint)uVar29 >> 0x10);
      bVar48 = (byte)((uint)uVar29 >> 0x18);
      uVar29 = *(undefined4 *)(pauVar23[1] + 0xc);
      bVar49 = (byte)uVar29;
      bVar50 = (byte)((uint)uVar29 >> 8);
      bVar51 = (byte)((uint)uVar29 >> 0x10);
      bVar52 = (byte)((uint)uVar29 >> 0x18);
      if (iVar24 < 0) break;
LAB_00101060:
      pauVar23 = pauVar23 + 2;
      auVar2._4_4_ = uVar34;
      auVar2._0_4_ = uVar33;
      auVar2._8_4_ = uVar35;
      auVar2._12_4_ = uVar36;
      auVar27 = NEON_aese(auVar27,auVar2);
      auVar27 = NEON_aesmc(auVar27,auVar27);
    }
    auVar28 = NEON_aese(auVar27,*pauVar23);
    param_3 = iVar22 + -0x10;
    auVar27[0] = auVar28[0] ^ bVar37;
    auVar27[1] = auVar28[1] ^ bVar38;
    auVar27[2] = auVar28[2] ^ bVar39;
    auVar27[3] = auVar28[3] ^ bVar40;
    auVar27[4] = auVar28[4] ^ bVar41;
    auVar27[5] = auVar28[5] ^ bVar42;
    auVar27[6] = auVar28[6] ^ bVar43;
    auVar27[7] = auVar28[7] ^ bVar44;
    auVar27[8] = auVar28[8] ^ bVar45;
    auVar27[9] = auVar28[9] ^ bVar46;
    auVar27[10] = auVar28[10] ^ bVar47;
    auVar27[0xb] = auVar28[0xb] ^ bVar48;
    auVar27[0xc] = auVar28[0xc] ^ bVar49;
    auVar27[0xd] = auVar28[0xd] ^ bVar50;
    auVar27[0xe] = auVar28[0xe] ^ bVar51;
    auVar27[0xf] = auVar28[0xf] ^ bVar52;
    if (param_3 < 0) break;
    bVar37 = *param_2;
    pbVar21 = param_2 + 1;
    pbVar6 = param_2 + 2;
    pbVar7 = param_2 + 3;
    pbVar8 = param_2 + 4;
    pbVar9 = param_2 + 5;
    pbVar10 = param_2 + 6;
    pbVar11 = param_2 + 7;
    pbVar12 = param_2 + 8;
    pbVar13 = param_2 + 9;
    pbVar14 = param_2 + 10;
    pbVar15 = param_2 + 0xb;
    pbVar16 = param_2 + 0xc;
    pbVar17 = param_2 + 0xd;
    pbVar18 = param_2 + 0xe;
    pbVar19 = param_2 + 0xf;
    param_2 = param_2 + 0x10;
    auVar27[0] = auVar27[0] ^ bVar37;
    auVar27[1] = auVar27[1] ^ *pbVar21;
    auVar27[2] = auVar27[2] ^ *pbVar6;
    auVar27[3] = auVar27[3] ^ *pbVar7;
    auVar27[4] = auVar27[4] ^ *pbVar8;
    auVar27[5] = auVar27[5] ^ *pbVar9;
    auVar27[6] = auVar27[6] ^ *pbVar10;
    auVar27[7] = auVar27[7] ^ *pbVar11;
    auVar27[8] = auVar27[8] ^ *pbVar12;
    auVar27[9] = auVar27[9] ^ *pbVar13;
    auVar27[10] = auVar27[10] ^ *pbVar14;
    auVar27[0xb] = auVar27[0xb] ^ *pbVar15;
    auVar27[0xc] = auVar27[0xc] ^ *pbVar16;
    auVar27[0xd] = auVar27[0xd] ^ *pbVar17;
    auVar27[0xe] = auVar27[0xe] ^ *pbVar18;
    auVar27[0xf] = auVar27[0xf] ^ *pbVar19;
  } while (param_3 != 0);
  (*param_1)[0] = auVar27[0];
  (*param_1)[1] = auVar27[1];
  (*param_1)[2] = auVar27[2];
  (*param_1)[3] = auVar27[3];
  (*param_1)[4] = auVar27[4];
  (*param_1)[5] = auVar27[5];
  (*param_1)[6] = auVar27[6];
  (*param_1)[7] = auVar27[7];
  (*param_1)[8] = auVar27[8];
  (*param_1)[9] = auVar27[9];
  (*param_1)[10] = auVar27[10];
  (*param_1)[0xb] = auVar27[0xb];
  (*param_1)[0xc] = auVar27[0xc];
  (*param_1)[0xd] = auVar27[0xd];
  (*param_1)[0xe] = auVar27[0xe];
  (*param_1)[0xf] = auVar27[0xf];
  iVar26 = iVar25;
  if ((param_3 != 0) && (iVar25 = iVar22, iVar22 != 0)) {
    while( true ) {
      (*param_1)[0] = auVar27[0] ^ *param_2;
      iVar26 = iVar22;
      if (iVar25 + -1 == 0) break;
      auVar27 = NEON_ext(auVar27,auVar27,1,1);
      param_1 = (undefined1 (*) [16])(*param_1 + 1);
      param_2 = param_2 + 1;
      iVar25 = iVar25 + -1;
    }
  }
LAB_001010fc:
  *param_4 = iVar26;
  return;
}


