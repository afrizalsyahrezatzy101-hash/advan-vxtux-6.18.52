// signature: undefined aud_smsg_dump_func(void)

void aud_smsg_dump_func(undefined4 param_1)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  long lVar4;
  code *pcVar5;
  bool bVar6;
  undefined2 *puVar7;
  undefined8 *in_x18;
  int iVar8;
  uint uVar9;
  ulong uVar10;
  uint uVar11;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = DAT_001009a0;
  uVar2 = **(uint **)(DAT_001009a0 + 0x40);
  iVar8 = *(int *)(DAT_001009a0 + 0x3dc);
  uVar11 = *(int *)(DAT_001009a0 + 0x38) - 1;
  if ((uVar11 & uVar2) != 0) {
    uVar11 = (uVar11 & uVar2) - 1;
  }
  printk(&DAT_001023c5,"aud_smsg_rx_dump",iVar8,*(undefined4 *)(DAT_001009a0 + 0x3e0),
         *(undefined4 *)(DAT_001009a0 + 0x3e4));
  printk(&DAT_0010151a,"aud_smsg_rx_dump",**(undefined4 **)(lVar4 + 0x48),
         **(undefined4 **)(lVar4 + 0x40),*(undefined4 *)(lVar4 + 0x38),iVar8 + uVar11 * 0x14,uVar2,
         uVar11);
  uVar9 = *(uint *)(lVar4 + 0x38);
  if (0x45 < uVar9) {
    uVar9 = 0x46;
  }
  printk(&DAT_0010157b,uVar9);
  printk(&DAT_00100b80,uVar11,*(undefined4 *)(lVar4 + 0x38));
  uVar1 = uVar9;
  if (uVar11 + 1 <= uVar9) {
    uVar1 = uVar11 + 1;
  }
  if (uVar9 != uVar1) {
    iVar8 = uVar1 - uVar9;
    do {
      uVar9 = uVar9 - 1;
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x30) + (ulong)(uint)(iVar8 + *(int *)(lVar4 + 0x38)) * 0x14);
      printk(&DAT_00101670,"aud_smsg_rx_dump",uVar9,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      bVar6 = iVar8 != -1;
      iVar8 = iVar8 + 1;
    } while (bVar6);
  }
  if (uVar1 != 0) {
    uVar11 = uVar11 - uVar1;
    uVar10 = (ulong)uVar1;
    do {
      uVar11 = uVar11 + 1;
      uVar10 = uVar10 - 1;
      puVar7 = (undefined2 *)(*(long *)(lVar4 + 0x30) + (ulong)uVar11 * 0x14);
      printk(&DAT_00101d95,"aud_smsg_rx_dump",uVar10 & 0xffffffff,*puVar7,puVar7[1],
             *(undefined4 *)(puVar7 + 2),*(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),
             *(undefined4 *)(puVar7 + 8));
    } while (uVar10 != 0);
  }
  printk(&DAT_00101f1b,**(int **)(lVar4 + 0x48) - **(int **)(lVar4 + 0x40));
  if (**(uint **)(lVar4 + 0x48) != uVar2) {
    iVar8 = 0;
    do {
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x30) +
               (ulong)(*(int *)(lVar4 + 0x38) - 1U & uVar2 + iVar8) * 0x14);
      printk(&DAT_00100ef9,"aud_smsg_rx_dump",iVar8,**(undefined4 **)(lVar4 + 0x48),
             **(undefined4 **)(lVar4 + 0x40),uVar2 + iVar8,puVar7);
      printk(&DAT_00100c91,"aud_smsg_rx_dump",iVar8,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      iVar8 = iVar8 + 1;
    } while (uVar2 + iVar8 != **(int **)(lVar4 + 0x48));
  }
  lVar4 = DAT_001009a0;
  uVar2 = **(uint **)(DAT_001009a0 + 0x20);
  uVar11 = *(int *)(DAT_001009a0 + 0x18) - 1U & uVar2;
  if (uVar11 == 0) {
    uVar11 = *(uint *)(DAT_001009a0 + 0x38);
  }
  iVar8 = *(int *)(DAT_001009a0 + 0x3d0);
  iVar3 = uVar11 - 1;
  printk(&DAT_00101204,"aud_smsg_tx_dump",iVar8,*(undefined4 *)(DAT_001009a0 + 0x3d4),
         *(undefined4 *)(DAT_001009a0 + 0x3d8));
  printk(&DAT_00100e93,"aud_smsg_tx_dump",**(undefined4 **)(lVar4 + 0x28),
         **(undefined4 **)(lVar4 + 0x20),*(undefined4 *)(lVar4 + 0x18),iVar8 + iVar3 * 0x14,uVar2,
         iVar3);
  uVar9 = *(uint *)(lVar4 + 0x18);
  if (0x45 < uVar9) {
    uVar9 = 0x46;
  }
  printk(&DAT_00100bdf,uVar9);
  printk(&DAT_00100b80,iVar3,*(undefined4 *)(lVar4 + 0x18));
  uVar1 = uVar9;
  if (uVar11 <= uVar9) {
    uVar1 = uVar11;
  }
  if (uVar9 != uVar1) {
    iVar8 = uVar1 - uVar9;
    do {
      uVar9 = uVar9 - 1;
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x10) + (ulong)(uint)(iVar8 + *(int *)(lVar4 + 0x18)) * 0x14);
      printk(&DAT_00101778,"aud_smsg_tx_dump",uVar9,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      bVar6 = iVar8 != -1;
      iVar8 = iVar8 + 1;
    } while (bVar6);
  }
  if (uVar1 != 0) {
    uVar10 = (ulong)uVar1;
    uVar11 = uVar11 - uVar1;
    do {
      uVar10 = uVar10 - 1;
      puVar7 = (undefined2 *)(*(long *)(lVar4 + 0x10) + (ulong)uVar11 * 0x14);
      printk(&DAT_00100f52,"aud_smsg_tx_dump",uVar10 & 0xffffffff,*puVar7,puVar7[1],
             *(undefined4 *)(puVar7 + 2),*(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),
             *(undefined4 *)(puVar7 + 8));
      uVar11 = uVar11 + 1;
    } while (uVar10 != 0);
  }
  printk(&DAT_00101f1b,**(int **)(lVar4 + 0x28) - **(int **)(lVar4 + 0x20));
  if (**(uint **)(lVar4 + 0x28) != uVar2) {
    iVar8 = 0;
    do {
      puVar7 = (undefined2 *)
               (*(long *)(lVar4 + 0x10) +
               (ulong)(*(int *)(lVar4 + 0x18) - 1U & uVar2 + iVar8) * 0x14);
      printk(&DAT_00100ef9,"aud_smsg_tx_dump",iVar8,**(undefined4 **)(lVar4 + 0x28),
             **(undefined4 **)(lVar4 + 0x20),uVar2 + iVar8,puVar7);
      printk(&DAT_00100c91,"aud_smsg_tx_dump",iVar8,*puVar7,puVar7[1],*(undefined4 *)(puVar7 + 2),
             *(undefined4 *)(puVar7 + 4),*(undefined4 *)(puVar7 + 6),*(undefined4 *)(puVar7 + 8));
      iVar8 = iVar8 + 1;
    } while (uVar2 + iVar8 != **(int **)(lVar4 + 0x28));
  }
  pcVar5 = dump_func;
  if (dump_func != (code *)0x0) {
    __cfi_slowpath(0xccf68899c64718a6,dump_func);
    (*pcVar5)(dump_func_priv,param_1);
  }
  return;
}



