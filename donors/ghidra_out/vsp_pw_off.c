// vsp_pw_off @ 00101434
// signature: undefined vsp_pw_off(void)
// calling convention: unknown


int vsp_pw_off(long param_1)

{
  int iVar1;
  int iVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  if (*(long *)(param_1 + 0x5f8) == 0) {
    printk(&DAT_0010030a);
    iVar2 = -0x16;
  }
  else {
    iVar1 = regmap_update_bits_base
                      (*(long *)(param_1 + 0x5f8),*(undefined4 *)(param_1 + 0x614),
                       *(undefined4 *)(param_1 + 0x624),*(undefined4 *)(param_1 + 0x624),0,0,0);
    iVar2 = 0;
    if (iVar1 != 0) {
      printk(&DAT_00100357,"vsp_pw_off",0xb5);
      iVar2 = iVar1;
    }
  }
  return iVar2;
}


