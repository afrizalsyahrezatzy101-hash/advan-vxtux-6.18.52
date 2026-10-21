// vser_complete_in @ 00102424
// signature: undefined vser_complete_in(void)
// calling convention: unknown


void vser_complete_in(undefined8 param_1,long param_2)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  ulong uVar4;
  undefined8 *in_x18;
  long *plVar5;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = _vser_dev;
  if (*(int *)(param_2 + 0x54) != 0) {
    *(undefined4 *)(_vser_dev + 0x100) = 1;
  }
  uVar3 = _raw_spin_lock_irqsave(lVar2 + 0xe0);
  plVar5 = *(long **)(lVar2 + 0x120);
  lVar1 = param_2 + 0x40;
  uVar4 = __list_add_valid(lVar1,plVar5,lVar2 + 0x118);
  if ((uVar4 & 1) != 0) {
    *(long *)(lVar2 + 0x120) = lVar1;
    *(long *)(param_2 + 0x40) = lVar2 + 0x118;
    *(long **)(param_2 + 0x48) = plVar5;
    *plVar5 = lVar1;
  }
  tx_req_count = tx_req_count + 1;
  _raw_spin_unlock_irqrestore(lVar2 + 0xe0,uVar3);
  __wake_up(lVar2 + 0x150,3,1,0);
  return;
}


