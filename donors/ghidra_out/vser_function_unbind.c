// vser_function_unbind @ 00101bec
// signature: undefined vser_function_unbind(void)
// calling convention: unknown


void vser_function_unbind(undefined8 param_1,long param_2)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  undefined8 uVar4;
  ulong uVar5;
  undefined8 *in_x18;
  long *plVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  vser_free_all_request(param_2);
  lVar1 = param_2 + 0xe0;
  uVar4 = _raw_spin_lock_irqsave(lVar1);
  plVar6 = *(long **)(param_2 + 0x128);
  while (plVar6 != (long *)(param_2 + 0x128)) {
    uVar5 = __list_del_entry_valid(plVar6);
    if ((uVar5 & 1) != 0) {
      lVar2 = *plVar6;
      plVar3 = (long *)plVar6[1];
      *(long **)(lVar2 + 8) = plVar3;
      *plVar3 = lVar2;
    }
    *plVar6 = -0x2152ffffffffff00;
    plVar6[1] = -0x2152fffffffffede;
    tx_req_count = tx_req_count + -1;
    _raw_spin_unlock_irqrestore(lVar1,uVar4);
    if (plVar6 + -8 == (long *)0x0) goto LAB_00101cb0;
    usb_ep_free_request(*(undefined8 *)(param_2 + 0xe8),plVar6 + -8);
    uVar4 = _raw_spin_lock_irqsave(lVar1);
    plVar6 = *(long **)(param_2 + 0x128);
  }
  _raw_spin_unlock_irqrestore(lVar1,uVar4);
LAB_00101cb0:
  *(undefined8 *)(param_2 + 0xf8) = 0x100000000;
  *(undefined4 *)(param_2 + 0x100) = 1;
  return;
}


