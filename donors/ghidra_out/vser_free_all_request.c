// vser_free_all_request @ 00101fc8
// signature: undefined vser_free_all_request(void)
// calling convention: unknown


void vser_free_all_request(long param_1)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  ulong uVar4;
  undefined8 *in_x18;
  undefined8 *puVar5;
  undefined8 uVar6;
  long *plVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar5 = *(undefined8 **)(param_1 + 0x168);
  if (puVar5 != (undefined8 *)0x0) {
    uVar6 = *(undefined8 *)(param_1 + 0xf0);
    kfree(*puVar5);
    usb_ep_free_request(uVar6,puVar5);
  }
  lVar1 = param_1 + 0xe0;
  uVar6 = _raw_spin_lock_irqsave(lVar1);
  plVar7 = *(long **)(param_1 + 0x118);
  if ((long *)(param_1 + 0x118) == plVar7) {
LAB_001020d4:
    _raw_spin_unlock_irqrestore(lVar1,uVar6);
  }
  else {
    uVar4 = __list_del_entry_valid(plVar7);
    if ((uVar4 & 1) != 0) {
      lVar2 = *plVar7;
      plVar3 = (long *)plVar7[1];
      *(long **)(lVar2 + 8) = plVar3;
      *plVar3 = lVar2;
    }
    *plVar7 = -0x2152ffffffffff00;
    plVar7[1] = -0x2152fffffffffede;
    while( true ) {
      plVar7 = plVar7 + -8;
      tx_req_count = tx_req_count + -1;
      _raw_spin_unlock_irqrestore(lVar1,uVar6);
      if (plVar7 == (long *)0x0) break;
      uVar6 = *(undefined8 *)(param_1 + 0xe8);
      kfree(*plVar7);
      usb_ep_free_request(uVar6,plVar7);
      uVar6 = _raw_spin_lock_irqsave(lVar1);
      plVar7 = *(long **)(param_1 + 0x118);
      if ((long *)(param_1 + 0x118) == plVar7) goto LAB_001020d4;
      uVar4 = __list_del_entry_valid(plVar7);
      if ((uVar4 & 1) != 0) {
        lVar2 = *plVar7;
        plVar3 = (long *)plVar7[1];
        *(long **)(lVar2 + 8) = plVar3;
        *plVar3 = lVar2;
      }
      *plVar7 = -0x2152ffffffffff00;
      plVar7[1] = -0x2152fffffffffede;
    }
  }
  return;
}


