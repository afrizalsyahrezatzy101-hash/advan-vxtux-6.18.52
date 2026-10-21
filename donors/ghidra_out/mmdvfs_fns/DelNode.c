====================DelNode@0010c9c4====================
// signature: undefined DelNode(void)

void DelNode(long param_1)

{
  long lVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  if (param_1 != 0) {
    *in_x18 = unaff_x30;
    lVar1 = *(long *)(param_1 + 0x18);
    *(undefined8 *)(param_1 + 0x18) = *(undefined8 *)(lVar1 + 0x18);
    kfree(lVar1);
  }
  return;
}


