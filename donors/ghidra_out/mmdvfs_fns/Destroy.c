====================Destroy@0010c7fc====================
// signature: undefined Destroy(void)

void Destroy(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar1 = *(long *)(param_1 + 0x18);
  while (lVar1 != 0) {
    lVar1 = *(long *)(lVar1 + 0x18);
    kfree();
  }
  return;
}


