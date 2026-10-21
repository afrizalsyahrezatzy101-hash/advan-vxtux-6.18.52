====================GetFrontNodeFreq@0010c7c4====================
// signature: undefined GetFrontNodeFreq(void)

undefined8 GetFrontNodeFreq(long param_1)

{
  undefined8 uVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(long *)(param_1 + 0x18) == 0) {
    printk(&DAT_00105b07);
    uVar1 = 0;
  }
  else {
    uVar1 = *(undefined8 *)(*(long *)(param_1 + 0x18) + 8);
  }
  return uVar1;
}


