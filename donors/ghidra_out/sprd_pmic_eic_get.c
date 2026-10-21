// sprd_pmic_eic_get @ 00101634
// signature: undefined sprd_pmic_eic_get(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sprd_pmic_eic_get(undefined8 param_1,uint param_2)

{
  long lVar1;
  ulong uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_2c;
  long local_28;
  
  *in_x18 = unaff_x30;
  local_28 = ___stack_chk_guard;
  lVar1 = gpiochip_get_data();
  local_2c = 0;
  uVar2 = regmap_read(*(undefined8 *)(lVar1 + 0x310),*(undefined4 *)(lVar1 + 0x318),&local_2c);
  if ((int)uVar2 == 0) {
    uVar2 = (ulong)(local_2c >> (param_2 & 0xf) & 1);
  }
  if (___stack_chk_guard == local_28) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar2);
}


