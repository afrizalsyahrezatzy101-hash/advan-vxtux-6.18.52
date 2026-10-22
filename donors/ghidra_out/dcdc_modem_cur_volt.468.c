// dcdc_modem_cur_volt.468 @ 0010f670
// signature: undefined dcdc_modem_cur_volt.468(void)
// calling convention: unknown


uint dcdc_modem_cur_volt_468(long param_1)

{
  return *(uint *)(*(long *)(param_1 + 0x3e0) + 0x44) >> 0x14 & 7;
}


