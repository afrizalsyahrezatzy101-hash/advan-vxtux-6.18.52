// mmc_hsq_finalize_request @ 00101838
// signature: undefined mmc_hsq_finalize_request(void)
// calling convention: unknown


undefined8 mmc_hsq_finalize_request(long param_1,long param_2)

{
  long lVar1;
  long lVar2;
  int iVar3;
  undefined4 uVar4;
  char cVar5;
  code *pcVar6;
  undefined8 uVar7;
  undefined8 *in_x18;
  long lVar8;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar8 = *(long *)(param_1 + 0x658);
  lVar1 = lVar8 + 0x30;
  uVar7 = _raw_spin_lock_irqsave(lVar1);
  if (((*(char *)(lVar8 + 0x178) == '\0') || (*(long *)(lVar8 + 8) == 0)) ||
     (*(long *)(lVar8 + 8) != param_2)) {
    _raw_spin_unlock_irqrestore(lVar1,uVar7);
    uVar7 = 0;
  }
  else {
    *(undefined8 *)(*(long *)(lVar8 + 0x28) + (long)*(int *)(lVar8 + 0x68) * 8) = 0;
    _raw_spin_unlock_irqrestore(lVar1,uVar7);
    mmc_cqe_request_done(param_1,*(undefined8 *)(lVar8 + 8));
    uVar7 = _raw_spin_lock_irqsave(lVar1);
    iVar3 = *(int *)(lVar8 + 0x70);
    *(undefined8 *)(lVar8 + 8) = 0;
    if (iVar3 == 0) {
      *(undefined4 *)(lVar8 + 0x68) = 0x40;
      *(undefined4 *)(lVar8 + 0x74) = 0x40;
      if (*(char *)(lVar8 + 0x179) != '\0') {
        *(undefined1 *)(lVar8 + 0x179) = 0;
        __wake_up(lVar8 + 0x10,3,1,0);
      }
    }
    else {
      if (0x3f < *(uint *)(lVar8 + 0x68)) {
        __ubsan_handle_cfi_check_fail_abort(0x12);
                    /* WARNING: Does not return */
        pcVar6 = (code *)UndefinedInstructionException(0,0x101968);
        (*pcVar6)();
      }
      lVar2 = lVar8 + (ulong)*(uint *)(lVar8 + 0x68) * 4;
      uVar4 = *(undefined4 *)(lVar2 + 0x78);
      *(undefined4 *)(lVar2 + 0x78) = 0x40;
      *(undefined4 *)(lVar8 + 0x68) = uVar4;
    }
    cVar5 = *(char *)(lVar8 + 0x17a);
    _raw_spin_unlock_irqrestore(lVar1,uVar7);
    uVar7 = 1;
    if ((cVar5 == '\0') && (0 < iVar3)) {
      mmc_hsq_pump_requests(lVar8);
      uVar7 = 1;
    }
  }
  return uVar7;
}


