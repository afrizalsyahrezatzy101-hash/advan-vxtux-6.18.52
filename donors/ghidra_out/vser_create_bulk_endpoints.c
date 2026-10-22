// vser_create_bulk_endpoints @ 001024e4
// signature: undefined vser_create_bulk_endpoints(void)
// calling convention: unknown


int vser_create_bulk_endpoints(long param_1)

{
  undefined8 *puVar1;
  int iVar2;
  long *plVar3;
  undefined8 uVar4;
  ulong uVar5;
  undefined8 *in_x18;
  undefined8 *puVar6;
  undefined8 *puVar7;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar6 = *(undefined8 **)(param_1 + 0xd8);
  plVar3 = (long *)usb_ep_autoconfig(*puVar6,&vser_fs_in_desc);
  if (plVar3 != (long *)0x0) {
    *plVar3 = param_1;
    *(long **)(param_1 + 0xe8) = plVar3;
    plVar3 = (long *)usb_ep_autoconfig(*puVar6,&vser_fs_out_desc);
    if (plVar3 != (long *)0x0) {
      *plVar3 = param_1;
      *(long **)(param_1 + 0xf0) = plVar3;
      iVar2 = vser_alloc_all_request(param_1);
      if (iVar2 != 0) {
        return iVar2;
      }
      iVar2 = 100;
      do {
        puVar6 = (undefined8 *)usb_ep_alloc_request(*(undefined8 *)(param_1 + 0xe8),0xcc0);
        if (puVar6 == (undefined8 *)0x0) {
          vser_free_all_request(param_1);
          return -0xc;
        }
        *puVar6 = 0;
        puVar6[6] = &LAB_001053ec;
        uVar4 = _raw_spin_lock_irqsave(param_1 + 0xe0);
        puVar7 = *(undefined8 **)(param_1 + 0x130);
        puVar1 = puVar6 + 8;
        uVar5 = __list_add_valid(puVar1,puVar7,param_1 + 0x128);
        if ((uVar5 & 1) != 0) {
          *(undefined8 **)(param_1 + 0x130) = puVar1;
          puVar6[8] = param_1 + 0x128;
          puVar6[9] = puVar7;
          *puVar7 = puVar1;
        }
        tx_req_count = tx_req_count + 1;
        _raw_spin_unlock_irqrestore(param_1 + 0xe0,uVar4);
        iVar2 = iVar2 + -1;
      } while (iVar2 != 0);
      return 0;
    }
  }
  return -0x13;
}


