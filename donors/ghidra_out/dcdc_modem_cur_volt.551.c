// dcdc_modem_cur_volt.551 @ 0010fdf0
// signature: undefined dcdc_modem_cur_volt.551(void)
// calling convention: unknown


uint dcdc_modem_cur_volt_551(long param_1)

{
  return *(uint *)(*(long *)(param_1 + 0x3e0) + 0x50) >> 0x14 & 7;
}


