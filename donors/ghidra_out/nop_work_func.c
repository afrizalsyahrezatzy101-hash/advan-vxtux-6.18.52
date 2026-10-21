// nop_work_func @ 001023fc
// signature: undefined nop_work_func(void)
// calling convention: unknown


void nop_work_func(long param_1)

{
  long lVar1;
  long lVar2;
  long *plVar3;
  bool bVar4;
  int iVar5;
  undefined8 uVar6;
  ulong uVar7;
  undefined8 *in_x18;
  long *plVar8;
  long *plVar9;
  long lVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  undefined4 uVar13;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar10 = *(long *)(param_1 + -8);
  lVar1 = lVar10 + 0x88;
  uVar6 = _raw_spin_lock_irqsave(lVar1);
  plVar8 = (long *)(lVar10 + 0x78);
  plVar9 = (long *)*plVar8;
  if (plVar8 == plVar9) {
    uVar11 = 0;
    uVar12 = 0;
    uVar13 = 0;
  }
  else {
    uVar7 = __list_del_entry_valid(plVar9);
    if ((uVar7 & 1) != 0) {
      lVar2 = *plVar9;
      plVar3 = (long *)plVar9[1];
      *(long **)(lVar2 + 8) = plVar3;
      *plVar3 = lVar2;
    }
    *plVar9 = (long)plVar9;
    uVar11 = (undefined4)plVar9[2];
    uVar12 = *(undefined4 *)((long)plVar9 + 0x14);
    uVar13 = (undefined4)plVar9[3];
    plVar9[1] = (long)plVar9;
  }
  _raw_spin_unlock_irqrestore(lVar1,uVar6);
code_r0x001024b4:
  do {
    iVar5 = trusty_std_call32(*(undefined8 *)(lVar10 + 0x60),0x3c000003,uVar11,uVar12,uVar13);
    uVar6 = _raw_spin_lock_irqsave(lVar1);
    plVar9 = *(long **)(lVar10 + 0x78);
    if (plVar8 == plVar9) {
      uVar11 = 0;
      uVar12 = 0;
      uVar13 = 0;
      bVar4 = false;
    }
    else {
      uVar7 = __list_del_entry_valid(plVar9);
      if ((uVar7 & 1) != 0) {
        lVar2 = *plVar9;
        plVar3 = (long *)plVar9[1];
        *(long **)(lVar2 + 8) = plVar3;
        *plVar3 = lVar2;
      }
      *plVar9 = (long)plVar9;
      uVar11 = (undefined4)plVar9[2];
      uVar12 = *(undefined4 *)((long)plVar9 + 0x14);
      uVar13 = (undefined4)plVar9[3];
      bVar4 = plVar9 != (long *)0x0;
      plVar9[1] = (long)plVar9;
    }
    _raw_spin_unlock_irqrestore(lVar1,uVar6);
    if (iVar5 != -0xf) {
      if (iVar5 == -0xe) goto code_r0x001024b4;
      _dev_err(*(undefined8 *)(lVar10 + 0x60),"%s: SMC_SC_NOP failed %d","nop_work_func",iVar5);
    }
    if (!bVar4) {
      return;
    }
  } while( true );
}


