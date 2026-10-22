// vser_pass_complete_in @ 00102730
// signature: undefined vser_pass_complete_in(void)
// calling convention: unknown


void vser_pass_complete_in(undefined8 param_1,undefined8 *param_2)

{
  undefined8 *puVar1;
  long lVar2;
  code *pcVar3;
  undefined8 uVar4;
  ulong uVar5;
  undefined8 *in_x18;
  long *plVar6;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = _vser_dev;
  if (*(int *)((long)param_2 + 0x54) != 0) {
    *(undefined4 *)(_vser_dev + 0x100) = 1;
  }
  uVar4 = _raw_spin_lock_irqsave(lVar2 + 0xe0);
  plVar6 = *(long **)(lVar2 + 0x130);
  puVar1 = param_2 + 8;
  uVar5 = __list_add_valid(puVar1,plVar6,lVar2 + 0x128);
  if ((uVar5 & 1) != 0) {
    *(undefined8 **)(lVar2 + 0x130) = puVar1;
    param_2[8] = lVar2 + 0x128;
    param_2[9] = plVar6;
    *plVar6 = (long)puVar1;
  }
  tx_req_count = tx_req_count + 1;
  _raw_spin_unlock_irqrestore(lVar2 + 0xe0,uVar4);
  pcVar3 = bulk_in_complete_function;
  if (bulk_in_complete_function != (code *)0x0) {
    __cfi_slowpath(0xb71f6e69fd76c127,bulk_in_complete_function);
    (*pcVar3)(*param_2,*(undefined4 *)(param_2 + 1),s_callback_data);
  }
  __wake_up(lVar2 + 0x150,3,1,0);
  return;
}


