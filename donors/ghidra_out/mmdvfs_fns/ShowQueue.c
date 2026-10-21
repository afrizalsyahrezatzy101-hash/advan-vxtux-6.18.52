====================ShowQueue@0010c834====================
// signature: undefined ShowQueue(void)

void ShowQueue(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  for (puVar1 = *(undefined8 **)(param_1 + 0x18); puVar1 != (undefined8 *)0x0;
      puVar1 = (undefined8 *)puVar1[3]) {
    printk(&DAT_00105421,puVar1[1]);
    printk(&DAT_00106689,*puVar1);
    printk(&DAT_00105991,*(undefined4 *)(puVar1 + 2));
  }
  return;
}


