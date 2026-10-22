// signature: undefined aud_ipc_dump(void)

uint aud_ipc_dump(undefined1 *param_1,uint param_2)

{
  uint uVar1;
  undefined1 *puVar2;
  undefined1 *puVar3;
  undefined1 *puVar4;
  ulong uVar5;
  undefined8 *in_x18;
  uint uVar6;
  long lVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar5 = (ulong)DAT_00100560;
  puVar3 = param_1;
  puVar4 = DAT_00100550;
  puVar2 = DAT_00100550;
  if ((((uint)DAT_00100550 ^ (uint)param_1) & 7) == 0) {
    while (((ulong)puVar2 & 7) != 0) {
      if (uVar5 == 0) {
        uVar5 = 0xffffffffffffffff;
        break;
      }
      uVar5 = uVar5 - 1;
      *puVar3 = *puVar4;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
      puVar2 = puVar2 + 1;
    }
    memcpy(puVar3,puVar4,uVar5);
  }
  else {
    puVar3 = DAT_00100550;
    puVar4 = param_1;
    if (DAT_00100560 != 0) {
      do {
        uVar5 = uVar5 - 1;
        *puVar4 = *puVar3;
        puVar3 = puVar3 + 1;
        puVar4 = puVar4 + 1;
      } while (uVar5 != 0);
    }
  }
  uVar6 = DAT_00100560;
  uVar5 = (ulong)DAT_00100560;
  *(undefined8 *)(param_1 + uVar5) = 0x5a5a5a5a5a5a5a5a;
  *(undefined8 *)((long)(param_1 + uVar5) + 8) = 0x5a5a5a5a5a5a5a5a;
  uVar5 = (ulong)DAT_00100448;
  puVar3 = param_1 + (uVar6 + 0x10);
  puVar4 = g_aud_ipc;
  puVar2 = g_aud_ipc;
  if ((((uint)g_aud_ipc ^ (uint)puVar3) & 7) == 0) {
    while (((ulong)puVar2 & 7) != 0) {
      if (uVar5 == 0) {
        uVar5 = 0xffffffffffffffff;
        break;
      }
      uVar5 = uVar5 - 1;
      *puVar3 = *puVar4;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
      puVar2 = puVar2 + 1;
    }
    memcpy(puVar3,puVar4,uVar5);
  }
  else if (DAT_00100448 != 0) {
    do {
      uVar5 = uVar5 - 1;
      *puVar3 = *puVar4;
      puVar3 = puVar3 + 1;
      puVar4 = puVar4 + 1;
    } while (uVar5 != 0);
  }
  uVar1 = DAT_00100448 + uVar6 + 0x10;
  lVar7 = 0;
  uVar6 = uVar1 + 0x10;
  *(undefined8 *)(param_1 + uVar1) = 0x5b5b5b5b5b5b5b5b;
  *(undefined8 *)((long)(param_1 + uVar1) + 8) = 0x5b5b5b5b5b5b5b5b;
  do {
    uVar1 = (&DAT_00100460)[lVar7 * 6];
    uVar5 = (ulong)uVar1;
    if ((uVar1 != 0) && (uVar1 + uVar6 < param_2)) {
      puVar4 = (undefined1 *)(&DAT_00100458)[lVar7 * 3];
      puVar3 = param_1 + uVar6;
      puVar2 = puVar4;
      if ((((uint)puVar4 ^ (uint)puVar3) & 7) == 0) {
        while (((ulong)puVar2 & 7) != 0) {
          if (uVar5 == 0) {
            uVar5 = 0xffffffffffffffff;
            break;
          }
          uVar5 = uVar5 - 1;
          *puVar3 = *puVar4;
          puVar4 = puVar4 + 1;
          puVar2 = puVar2 + 1;
          puVar3 = puVar3 + 1;
        }
        memcpy(puVar3,puVar4,uVar5);
      }
      else {
        do {
          uVar5 = uVar5 - 1;
          *puVar3 = *puVar4;
          puVar3 = puVar3 + 1;
          puVar4 = puVar4 + 1;
        } while (uVar5 != 0);
      }
      uVar6 = (&DAT_00100460)[lVar7 * 6] + uVar6;
    }
    lVar7 = lVar7 + 1;
    if (lVar7 == 8) {
      if (uVar6 + 0x10 < param_2) {
        *(undefined8 *)(param_1 + uVar6) = 0x5c5c5c5c5c5c5c5c;
        *(undefined8 *)((long)(param_1 + uVar6) + 8) = 0x5c5c5c5c5c5c5c5c;
        uVar6 = uVar6 + 0x10;
      }
      return uVar6;
    }
  } while( true );
}



