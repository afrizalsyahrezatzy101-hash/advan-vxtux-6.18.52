// vser_release @ 00103d3c
// signature: undefined vser_release(void)
// calling convention: unknown


undefined8 vser_release(void)

{
  long lVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar2 = _vser_dev;
  lVar1 = _vser_dev + 0xe0;
  uVar3 = _raw_spin_lock_irqsave(lVar1);
  *(int *)(lVar2 + 0x110) = *(int *)(lVar2 + 0x110) + -1;
  _raw_spin_unlock_irqrestore(lVar1,uVar3);
  return 0;
}


