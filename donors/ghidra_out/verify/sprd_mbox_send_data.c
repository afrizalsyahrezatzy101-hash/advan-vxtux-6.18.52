// sprd_mbox_send_data @ 00102bac
// signature: undefined sprd_mbox_send_data(void)
// calling convention: unknown


undefined8 sprd_mbox_send_data(long *param_1,undefined8 *param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  undefined8 uVar4;
  code *pcVar5;
  long lVar6;
  ulong uVar7;
  undefined8 *in_x18;
  long lVar8;
  long lVar9;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar9 = *param_1;
  lVar8 = param_1[0x1e];
  uVar4 = _raw_spin_lock_irqsave(&mbox_lock);
  lVar6 = tx_data + lVar8 * 0x208;
  uVar1 = *(uint *)(lVar6 + 0x204);
  uVar2 = *(uint *)(lVar6 + 0x200);
  if (uVar1 == uVar2) {
    pcVar5 = *(code **)(*(long *)(lVar9 + 0xe0) + 0x18);
    if (1 < ((ulong)(pcVar5 + -0x10437c) >> 2 | (long)pcVar5 << 0x3e)) {
      __cfi_slowpath(0xc19c8378abe429fc,pcVar5);
    }
    iVar3 = (*pcVar5)(param_1,param_2);
    if (iVar3 != 0) {
      lVar6 = tx_data + lVar8 * 0x208;
      uVar7 = (ulong)*(uint *)(lVar6 + 0x200) & 0x3f;
      if ((*(uint *)(lVar6 + 0x204) == *(uint *)(lVar6 + 0x200)) ||
         ((*(uint *)(lVar6 + 0x204) & 0x3f) != (uint)uVar7)) {
        *(undefined8 *)(tx_data + lVar8 * 0x208 + uVar7 * 8) = *param_2;
        *(int *)(lVar6 + 0x200) = *(int *)(lVar6 + 0x200) + 1;
      }
      else {
        printk(&DAT_0010090e,lVar8);
      }
    }
  }
  else if ((uVar1 == uVar2) || ((uVar1 & 0x3f) != (uVar2 & 0x3f))) {
    lVar6 = tx_data + lVar8 * 0x208;
    *(undefined8 *)(lVar6 + (ulong)(uVar2 & 0x3f) * 8) = *param_2;
    *(int *)(lVar6 + 0x200) = *(int *)(lVar6 + 0x200) + 1;
  }
  else {
    printk(&DAT_0010090e,lVar8);
  }
  _raw_spin_unlock_irqrestore(&mbox_lock,uVar4);
  return 0;
}


