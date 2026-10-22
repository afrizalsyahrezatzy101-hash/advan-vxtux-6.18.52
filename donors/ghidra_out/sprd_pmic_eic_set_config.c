// sprd_pmic_eic_set_config @ 0010155c
// signature: undefined sprd_pmic_eic_set_config(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void sprd_pmic_eic_set_config(undefined8 param_1,uint param_2,ulong param_3)

{
  uint uVar1;
  long lVar2;
  undefined8 uVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  uint local_3c;
  long local_38;
  
  *in_x18 = unaff_x30;
  local_38 = ___stack_chk_guard;
  if ((param_3 & 0xff) == 0xb) {
    lVar2 = gpiochip_get_data();
    local_3c = 0;
    uVar1 = (param_2 & 0xf) << 2 | 0x40;
    uVar3 = regmap_read(*(undefined8 *)(lVar2 + 0x310),*(int *)(lVar2 + 0x318) + uVar1,&local_3c);
    if ((int)uVar3 == 0) {
      local_3c = ((uint)(param_3 >> 8) & 0xfffff8) / 1000 & 0xfff | local_3c & 0xfffff000;
      uVar3 = regmap_write(*(undefined8 *)(lVar2 + 0x310),*(int *)(lVar2 + 0x318) + uVar1);
    }
  }
  else {
    uVar3 = 0xfffffdf4;
  }
  if (___stack_chk_guard == local_38) {
    return;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail(uVar3);
}


