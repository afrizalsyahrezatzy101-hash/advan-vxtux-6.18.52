// vser_open @ 00103c54
// signature: undefined vser_open(void)
// calling convention: unknown


undefined8 vser_open(undefined8 param_1,long param_2)

{
  int iVar1;
  long lVar2;
  int iVar3;
  long lVar4;
  undefined8 uVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar4 = _vser_dev;
  lVar2 = _vser_dev + 0xe0;
  uVar5 = _raw_spin_lock_irqsave(lVar2);
  *(long *)(param_2 + 200) = lVar4;
  iVar3 = *(int *)(lVar4 + 0x110);
  iVar1 = iVar3 + 1;
  *(int *)(lVar4 + 0x110) = iVar1;
  if (iVar1 < 3) {
    *(undefined8 *)(lVar4 + 0xfc) = 0;
    _raw_spin_unlock_irqrestore(lVar2,uVar5);
    uVar5 = 0;
  }
  else {
    *(int *)(lVar4 + 0x110) = iVar3;
    _raw_spin_unlock_irqrestore(lVar2,uVar5);
    uVar5 = 0xfffffff0;
  }
  return uVar5;
}


