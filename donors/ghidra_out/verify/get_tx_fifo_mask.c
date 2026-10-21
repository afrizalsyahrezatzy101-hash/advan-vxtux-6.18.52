// get_tx_fifo_mask @ 00102030
// signature: undefined get_tx_fifo_mask(void)
// calling convention: unknown


uint get_tx_fifo_mask(uint param_1,undefined8 param_2,undefined8 param_3,ulong param_4,ulong param_5
                     ,ulong param_6,ulong param_7)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  ulong uVar6;
  ulong uVar7;
  int *piVar8;
  uint uVar9;
  ulong uVar10;
  long lVar11;
  undefined8 *in_x18;
  uint uVar12;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  _raw_spin_lock(&mbox_lock);
  uVar6 = (ulong)chan_num;
  if (chan_num == 0) {
    uVar12 = 0;
  }
  else {
    param_1 = param_1 & 0xff;
    if (chan_num == 1) {
      uVar7 = 0;
      uVar12 = 0;
    }
    else {
      uVar10 = 0;
      uVar9 = 0;
      uVar12 = 0;
      uVar7 = uVar6 & 0xfffffffe;
      lVar11 = tx_data;
      do {
        uVar4 = 1 << (ulong)((uint)uVar10 & 0x1f);
        uVar2 = uVar4 & param_1;
        if (uVar2 != 0) {
          param_4 = (ulong)*(uint *)(lVar11 + 0x200);
        }
        uVar5 = 1 << (ulong)((uint)uVar10 + 1 & 0x1f);
        uVar1 = uVar5 & param_1;
        if (uVar1 != 0) {
          param_5 = (ulong)*(uint *)(lVar11 + 0x408);
        }
        if (uVar2 != 0) {
          param_7 = (ulong)*(uint *)(lVar11 + 0x204);
        }
        if (uVar1 != 0) {
          param_6 = (ulong)*(uint *)(lVar11 + 0x40c);
        }
        uVar3 = 0;
        if ((int)param_4 != (int)param_7 && uVar2 != 0) {
          uVar3 = uVar4;
        }
        uVar10 = uVar10 + 2;
        uVar9 = uVar9 | uVar3;
        uVar2 = 0;
        if ((int)param_5 != (int)param_6 && uVar1 != 0) {
          uVar2 = uVar5;
        }
        uVar12 = uVar12 | uVar2;
        lVar11 = lVar11 + 0x410;
      } while (uVar7 != uVar10);
      uVar12 = uVar12 | uVar9;
      if (uVar7 == uVar6) goto LAB_00102114;
    }
    piVar8 = (int *)(tx_data + uVar7 * 0x208 + 0x204);
    do {
      uVar9 = 1 << (ulong)((uint)uVar7 & 0x1f);
      if (((uVar9 & param_1) != 0) && (piVar8[-1] != *piVar8)) {
        uVar12 = uVar12 | uVar9;
      }
      uVar7 = uVar7 + 1;
      piVar8 = piVar8 + 0x82;
    } while (uVar6 != uVar7);
  }
LAB_00102114:
  _raw_spin_unlock(&mbox_lock);
  return uVar12;
}


