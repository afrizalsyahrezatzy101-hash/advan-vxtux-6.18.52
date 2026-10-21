// tui_poll @ 0010127c
// signature: undefined tui_poll(void)
// calling convention: unknown


byte tui_poll(long param_1,undefined8 *param_2)

{
  undefined8 *in_x18;
  byte bVar1;
  code *pcVar2;
  int *piVar3;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  piVar3 = *(int **)(param_1 + 200);
  if (((param_2 != (undefined8 *)0x0) && (piVar3 + 4 != (int *)0x0)) &&
     (pcVar2 = (code *)*param_2, pcVar2 != (code *)0x0)) {
    __cfi_slowpath(0x8990d6be6cd7b51f,pcVar2);
    (*pcVar2)(param_1,piVar3 + 4,param_2);
  }
  bVar1 = 0 < *piVar3;
  if ((bool)bVar1) {
    printk(&DAT_001002a9);
  }
  if (0 < piVar3[2]) {
    bVar1 = bVar1 | 8;
    piVar3[2] = 0;
    printk(&DAT_00100230);
  }
  return bVar1;
}


