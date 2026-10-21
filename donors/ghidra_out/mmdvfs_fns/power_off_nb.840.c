====================power_off_nb.840@0011264c====================
// signature: undefined power_off_nb.840(void)

void power_off_nb_840(long param_1)

{
  undefined8 *in_x18;
  undefined8 *puVar1;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  puVar1 = *(undefined8 **)(*(long *)(param_1 + 0x90) + 0x98);
  if (puVar1 != (undefined8 *)0x0) {
    printk(&DAT_00106598);
    *puVar1 = 0;
  }
  return;
}


