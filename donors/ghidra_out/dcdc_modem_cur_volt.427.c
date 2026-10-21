// dcdc_modem_cur_volt.427 @ 0010f2f8
// signature: undefined dcdc_modem_cur_volt.427(void)
// calling convention: unknown


uint dcdc_modem_cur_volt_427(long param_1)

{
  return *(uint *)(*(long *)(param_1 + 0x3e0) + 0x3f8) >> 0x14 & 7;
}


