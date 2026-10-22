// vser_alloc_all_request @ 001020fc
// signature: undefined vser_alloc_all_request(void)
// calling convention: unknown


undefined8 vser_alloc_all_request(long param_1)

{
  long lVar1;
  long *plVar2;
  int iVar3;
  long *plVar4;
  long lVar5;
  ulong uVar6;
  long lVar7;
  undefined8 *in_x18;
  undefined8 uVar8;
  long *plVar9;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  uVar8 = *(undefined8 *)(param_1 + 0xf0);
  iVar3 = *(int *)(param_1 + 0x17c);
  plVar4 = (long *)usb_ep_alloc_request(uVar8,0xcc0);
  if (plVar4 == (long *)0x0) {
    return 0xfffffff4;
  }
  lVar5 = __kmalloc((long)iVar3,0xcc0);
  *plVar4 = lVar5;
  if (lVar5 == 0) {
    usb_ep_free_request(uVar8,plVar4);
    return 0xfffffff4;
  }
  plVar4[6] = (long)&LAB_001053e4;
  uVar8 = *(undefined8 *)(param_1 + 0xe8);
  iVar3 = *(int *)(param_1 + 0x178);
  *(long **)(param_1 + 0x168) = plVar4;
  plVar4 = (long *)usb_ep_alloc_request(uVar8,0xcc0);
  if (plVar4 != (long *)0x0) {
    lVar5 = __kmalloc((long)iVar3,0xcc0);
    *plVar4 = lVar5;
    if (lVar5 != 0) {
      lVar5 = param_1 + 0xe0;
      lVar1 = param_1 + 0x118;
      plVar4[6] = (long)&LAB_001053e8;
      uVar8 = _raw_spin_lock_irqsave(lVar5);
      plVar9 = *(long **)(param_1 + 0x120);
      plVar2 = plVar4 + 8;
      uVar6 = __list_add_valid(plVar2,plVar9,lVar1);
      if ((uVar6 & 1) != 0) {
        *(long **)(param_1 + 0x120) = plVar2;
        plVar4[8] = lVar1;
        plVar4[9] = (long)plVar9;
        *plVar9 = (long)plVar2;
      }
      tx_req_count = tx_req_count + 1;
      _raw_spin_unlock_irqrestore(lVar5,uVar8);
      uVar8 = *(undefined8 *)(param_1 + 0xe8);
      iVar3 = *(int *)(param_1 + 0x178);
      plVar4 = (long *)usb_ep_alloc_request(uVar8,0xcc0);
      if (plVar4 == (long *)0x0) goto LAB_001023b0;
      lVar7 = __kmalloc((long)iVar3,0xcc0);
      *plVar4 = lVar7;
      if (lVar7 != 0) {
        plVar4[6] = (long)&LAB_001053e8;
        uVar8 = _raw_spin_lock_irqsave(lVar5);
        plVar9 = *(long **)(param_1 + 0x120);
        plVar2 = plVar4 + 8;
        uVar6 = __list_add_valid(plVar2,plVar9,lVar1);
        if ((uVar6 & 1) != 0) {
          *(long **)(param_1 + 0x120) = plVar2;
          plVar4[8] = lVar1;
          plVar4[9] = (long)plVar9;
          *plVar9 = (long)plVar2;
        }
        tx_req_count = tx_req_count + 1;
        _raw_spin_unlock_irqrestore(lVar5,uVar8);
        uVar8 = *(undefined8 *)(param_1 + 0xe8);
        iVar3 = *(int *)(param_1 + 0x178);
        plVar4 = (long *)usb_ep_alloc_request(uVar8,0xcc0);
        if (plVar4 == (long *)0x0) goto LAB_001023b0;
        lVar7 = __kmalloc((long)iVar3,0xcc0);
        *plVar4 = lVar7;
        if (lVar7 != 0) {
          plVar4[6] = (long)&LAB_001053e8;
          uVar8 = _raw_spin_lock_irqsave(lVar5);
          plVar9 = *(long **)(param_1 + 0x120);
          plVar2 = plVar4 + 8;
          uVar6 = __list_add_valid(plVar2,plVar9,lVar1);
          if ((uVar6 & 1) != 0) {
            *(long **)(param_1 + 0x120) = plVar2;
            plVar4[8] = lVar1;
            plVar4[9] = (long)plVar9;
            *plVar9 = (long)plVar2;
          }
          tx_req_count = tx_req_count + 1;
          _raw_spin_unlock_irqrestore(lVar5,uVar8);
          uVar8 = *(undefined8 *)(param_1 + 0xe8);
          iVar3 = *(int *)(param_1 + 0x178);
          plVar4 = (long *)usb_ep_alloc_request(uVar8,0xcc0);
          if (plVar4 == (long *)0x0) goto LAB_001023b0;
          lVar7 = __kmalloc((long)iVar3,0xcc0);
          *plVar4 = lVar7;
          if (lVar7 != 0) {
            plVar4[6] = (long)&LAB_001053e8;
            uVar8 = _raw_spin_lock_irqsave(lVar5);
            plVar9 = *(long **)(param_1 + 0x120);
            plVar2 = plVar4 + 8;
            uVar6 = __list_add_valid(plVar2,plVar9,lVar1);
            if ((uVar6 & 1) != 0) {
              *(long **)(param_1 + 0x120) = plVar2;
              plVar4[8] = lVar1;
              plVar4[9] = (long)plVar9;
              *plVar9 = (long)plVar2;
            }
            tx_req_count = tx_req_count + 1;
            _raw_spin_unlock_irqrestore(lVar5,uVar8);
            tx_req_count = 4;
            return 0;
          }
        }
      }
    }
    usb_ep_free_request(uVar8,plVar4);
  }
LAB_001023b0:
  vser_free_all_request(param_1);
  return 0xfffffff4;
}


