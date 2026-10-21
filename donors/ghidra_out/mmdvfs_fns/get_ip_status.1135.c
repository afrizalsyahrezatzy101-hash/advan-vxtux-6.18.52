====================get_ip_status.1135@00116324====================
// signature: undefined get_ip_status.1135(void)

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void get_ip_status_1135(undefined8 param_1,long param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  undefined8 uVar7;
  long lVar8;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  local_3c = 0;
  printk(&DAT_00104eab,mm_power_flag);
  if (mm_power_flag != '\x01') {
    uVar7 = 0xffffffff;
    goto LAB_001170fc;
  }
  uVar1 = *(uint *)(g_mmreg_map + 0x38);
  uVar2 = *(uint *)(g_mmreg_map + 0x48);
  iVar6 = regmap_read(DAT_00100c20,0x20,&local_3c);
  uVar5 = (uint)DAT_00104048;
  if (iVar6 != 0) {
    printk(&DAT_00105e73,"get_ip_status");
    uVar7 = 0;
    goto LAB_001170fc;
  }
  uVar3 = local_3c >> 0x14 & 7;
  if (uVar3 == (uint)DAT_00104048) {
    lVar8 = 0;
LAB_00116490:
    *(undefined **)(param_2 + 0x58) = (&PTR_DAT_00104068)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00104080) {
      lVar8 = 1;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_001040b8) {
      lVar8 = 2;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_001040f0) {
      lVar8 = 3;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_00104128) {
      lVar8 = 4;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_00104160) {
      lVar8 = 5;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_00104198) {
      lVar8 = 6;
      goto LAB_00116490;
    }
    if (uVar3 == DAT_001041d0) {
      lVar8 = 7;
      goto LAB_00116490;
    }
  }
  uVar3 = uVar1 >> 0x18 & 7;
  if (uVar3 == uVar5) {
    lVar8 = 0;
LAB_00116558:
    *(undefined **)(param_2 + 0x50) = (&PTR_DAT_00104068)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00104080) {
      lVar8 = 1;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_001040b8) {
      lVar8 = 2;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_001040f0) {
      lVar8 = 3;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_00104128) {
      lVar8 = 4;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_00104160) {
      lVar8 = 5;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_00104198) {
      lVar8 = 6;
      goto LAB_00116558;
    }
    if (uVar3 == DAT_001041d0) {
      lVar8 = 7;
      goto LAB_00116558;
    }
  }
  uVar3 = uVar2 >> 3 & 7;
  uVar4 = uVar1 >> 6 & 7;
  if (uVar3 == DAT_00104050) {
    lVar8 = 0;
LAB_00116630:
    *(undefined4 *)(param_2 + 0x60) = *(undefined4 *)((long)&DAT_00104048 + lVar8 * 0x38 + 4);
  }
  else {
    if (uVar3 == DAT_00104088) {
      lVar8 = 1;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001040c0) {
      lVar8 = 2;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001040f8) {
      lVar8 = 3;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_00104130) {
      lVar8 = 4;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_00104168) {
      lVar8 = 5;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001041a0) {
      lVar8 = 6;
      goto LAB_00116630;
    }
    if (uVar3 == DAT_001041d8) {
      lVar8 = 7;
      goto LAB_00116630;
    }
  }
  if (uVar4 == uVar5) {
    lVar8 = 0;
LAB_001166f4:
    *(undefined **)(param_2 + 0x18) = (&PTR_DAT_00104068)[lVar8 * 7];
  }
  else {
    if (uVar4 == DAT_00104080) {
      lVar8 = 1;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_001040b8) {
      lVar8 = 2;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_001040f0) {
      lVar8 = 3;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_00104128) {
      lVar8 = 4;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_00104160) {
      lVar8 = 5;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_00104198) {
      lVar8 = 6;
      goto LAB_001166f4;
    }
    if (uVar4 == DAT_001041d0) {
      lVar8 = 7;
      goto LAB_001166f4;
    }
  }
  uVar5 = uVar2 >> 6 & 3;
  uVar3 = uVar1 >> 9 & 7;
  if (uVar5 == DAT_00103950) {
    lVar8 = 0;
LAB_001167d0:
    *(undefined4 *)(param_2 + 0x70) = (&DAT_0010394c)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00103988) {
      lVar8 = 1;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_001039c0) {
      lVar8 = 2;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_001039f8) {
      lVar8 = 3;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103a30) {
      lVar8 = 4;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103a68) {
      lVar8 = 5;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103aa0) {
      lVar8 = 6;
      goto LAB_001167d0;
    }
    if (uVar5 == DAT_00103ad8) {
      lVar8 = 7;
      goto LAB_001167d0;
    }
  }
  if (uVar3 == DAT_00103948) {
    lVar8 = 0;
LAB_0011689c:
    *(undefined **)(param_2 + 0x38) = (&PTR_DAT_00103968)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00103980) {
      lVar8 = 1;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_001039b8) {
      lVar8 = 2;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_001039f0) {
      lVar8 = 3;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103a28) {
      lVar8 = 4;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103a60) {
      lVar8 = 5;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103a98) {
      lVar8 = 6;
      goto LAB_0011689c;
    }
    if (uVar3 == DAT_00103ad0) {
      lVar8 = 7;
      goto LAB_0011689c;
    }
  }
  uVar5 = uVar2 >> 0x17 & 3;
  uVar3 = uVar1 >> 0x15 & 7;
  if (uVar5 == DAT_00103e90) {
    lVar8 = 0;
LAB_00116978:
    *(undefined4 *)(param_2 + 0x6c) = (&DAT_00103e8c)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00103ec8) {
      lVar8 = 1;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103f00) {
      lVar8 = 2;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103f38) {
      lVar8 = 3;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103f70) {
      lVar8 = 4;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103fa8) {
      lVar8 = 5;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00103fe0) {
      lVar8 = 6;
      goto LAB_00116978;
    }
    if (uVar5 == DAT_00104018) {
      lVar8 = 7;
      goto LAB_00116978;
    }
  }
  if (uVar3 == DAT_00103e88) {
    lVar8 = 0;
LAB_00116a44:
    *(undefined **)(param_2 + 0x30) = (&PTR_DAT_00103ea8)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00103ec0) {
      lVar8 = 1;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103ef8) {
      lVar8 = 2;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103f30) {
      lVar8 = 3;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103f68) {
      lVar8 = 4;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103fa0) {
      lVar8 = 5;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00103fd8) {
      lVar8 = 6;
      goto LAB_00116a44;
    }
    if (uVar3 == DAT_00104010) {
      lVar8 = 7;
      goto LAB_00116a44;
    }
  }
  uVar5 = uVar2 >> 8 & 3;
  uVar3 = uVar1 >> 0xc & 7;
  if (uVar5 == DAT_00104210) {
    lVar8 = 0;
LAB_00116b20:
    *(undefined4 *)(param_2 + 0x74) = (&DAT_0010420c)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00104248) {
      lVar8 = 1;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104280) {
      lVar8 = 2;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_001042b8) {
      lVar8 = 3;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_001042f0) {
      lVar8 = 4;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104328) {
      lVar8 = 5;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104360) {
      lVar8 = 6;
      goto LAB_00116b20;
    }
    if (uVar5 == DAT_00104398) {
      lVar8 = 7;
      goto LAB_00116b20;
    }
  }
  if (uVar3 == DAT_00104208) {
    lVar8 = 0;
LAB_00116bec:
    *(undefined **)(param_2 + 0x40) = (&PTR_DAT_00104228)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00104240) {
      lVar8 = 1;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104278) {
      lVar8 = 2;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_001042b0) {
      lVar8 = 3;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_001042e8) {
      lVar8 = 4;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104320) {
      lVar8 = 5;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104358) {
      lVar8 = 6;
      goto LAB_00116bec;
    }
    if (uVar3 == DAT_00104390) {
      lVar8 = 7;
      goto LAB_00116bec;
    }
  }
  uVar5 = uVar2 >> 10 & 3;
  local_3c = uVar1 >> 0xf & 7;
  if (uVar5 == DAT_00103b10) {
    lVar8 = 0;
LAB_00116ccc:
    *(undefined4 *)(param_2 + 0x68) = *(undefined4 *)((long)&DAT_00103b08 + lVar8 * 0x38 + 4);
  }
  else {
    if (uVar5 == DAT_00103b48) {
      lVar8 = 1;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103b80) {
      lVar8 = 2;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103bb8) {
      lVar8 = 3;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103bf0) {
      lVar8 = 4;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103c28) {
      lVar8 = 5;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103c60) {
      lVar8 = 6;
      goto LAB_00116ccc;
    }
    if (uVar5 == DAT_00103c98) {
      lVar8 = 7;
      goto LAB_00116ccc;
    }
  }
  if (local_3c == (uint)DAT_00103b08) {
    lVar8 = 0;
LAB_00116d98:
    *(undefined **)(param_2 + 0x28) = (&PTR_DAT_00103b28)[lVar8 * 7];
  }
  else {
    if (local_3c == DAT_00103b40) {
      lVar8 = 1;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103b78) {
      lVar8 = 2;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103bb0) {
      lVar8 = 3;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103be8) {
      lVar8 = 4;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103c20) {
      lVar8 = 5;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103c58) {
      lVar8 = 6;
      goto LAB_00116d98;
    }
    if (local_3c == DAT_00103c90) {
      lVar8 = 7;
      goto LAB_00116d98;
    }
  }
  uVar5 = uVar2 >> 0xc & 7;
  uVar3 = uVar1 >> 0x12 & 7;
  if (uVar5 == DAT_00103cd0) {
    lVar8 = 0;
LAB_00116e74:
    *(undefined4 *)(param_2 + 100) = (&DAT_00103ccc)[lVar8 * 0xe];
  }
  else {
    if (uVar5 == DAT_00103d08) {
      lVar8 = 1;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103d40) {
      lVar8 = 2;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103d78) {
      lVar8 = 3;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103db0) {
      lVar8 = 4;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103de8) {
      lVar8 = 5;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103e20) {
      lVar8 = 6;
      goto LAB_00116e74;
    }
    if (uVar5 == DAT_00103e58) {
      lVar8 = 7;
      goto LAB_00116e74;
    }
  }
  if (uVar3 == DAT_00103cc8) {
    lVar8 = 0;
LAB_00116f40:
    *(undefined **)(param_2 + 0x20) = (&PTR_DAT_00103ce8)[lVar8 * 7];
  }
  else {
    if (uVar3 == DAT_00103d00) {
      lVar8 = 1;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103d38) {
      lVar8 = 2;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103d70) {
      lVar8 = 3;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103da8) {
      lVar8 = 4;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103de0) {
      lVar8 = 5;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103e18) {
      lVar8 = 6;
      goto LAB_00116f40;
    }
    if (uVar3 == DAT_00103e50) {
      lVar8 = 7;
      goto LAB_00116f40;
    }
  }
  uVar2 = uVar2 & 7;
  uVar1 = uVar1 >> 3 & 7;
  if (uVar2 == DAT_001043d0) {
    lVar8 = 0;
LAB_0011701c:
    *(undefined4 *)(param_2 + 0x78) = (&DAT_001043cc)[lVar8 * 0xe];
  }
  else {
    if (uVar2 == DAT_00104408) {
      lVar8 = 1;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104440) {
      lVar8 = 2;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104478) {
      lVar8 = 3;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_001044b0) {
      lVar8 = 4;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_001044e8) {
      lVar8 = 5;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104520) {
      lVar8 = 6;
      goto LAB_0011701c;
    }
    if (uVar2 == DAT_00104558) {
      lVar8 = 7;
      goto LAB_0011701c;
    }
  }
  if (uVar1 == DAT_001043c8) {
    lVar8 = 0;
LAB_001170e8:
    *(undefined **)(param_2 + 0x48) = (&PTR_DAT_001043e8)[lVar8 * 7];
  }
  else {
    if (uVar1 == DAT_00104400) {
      lVar8 = 1;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104438) {
      lVar8 = 2;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104470) {
      lVar8 = 3;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_001044a8) {
      lVar8 = 4;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_001044e0) {
      lVar8 = 5;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104518) {
      lVar8 = 6;
      goto LAB_001170e8;
    }
    if (uVar1 == DAT_00104550) {
      lVar8 = 7;
      goto LAB_001170e8;
    }
  }
  uVar7 = 1;
LAB_001170fc:
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar7);
}


