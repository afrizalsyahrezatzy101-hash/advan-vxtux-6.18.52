// vser_alloc @ 00101a40
// signature: undefined vser_alloc(void)
// calling convention: unknown


undefined8 * vser_alloc(long param_1)

{
  undefined8 *puVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_00100714,"vser_alloc");
  puVar1 = *(undefined8 **)(param_1 + 0xc0);
  if (puVar1 == (undefined8 *)0x0) {
    printk(&DAT_00100859);
    puVar1 = (undefined8 *)0x0;
  }
  else {
    *puVar1 = &DAT_00100cb0;
    puVar1[2] = &vser_fs_function;
    puVar1[3] = &vser_hs_function;
    puVar1[9] = &LAB_001053cc;
    puVar1[10] = &LAB_001053d0;
    puVar1[0xf] = &LAB_001053dc;
    puVar1[0x10] = &LAB_001053d8;
    puVar1[4] = &vser_ss_function;
    puVar1[5] = &vser_ss_function;
    puVar1[0xd] = &LAB_001053d4;
    puVar1[0xb] = &LAB_001053e0;
  }
  return puVar1;
}


