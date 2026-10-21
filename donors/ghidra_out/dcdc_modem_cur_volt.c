// dcdc_modem_cur_volt @ 0010ec44
// signature: undefined dcdc_modem_cur_volt(void)
// calling convention: unknown


uint dcdc_modem_cur_volt(long param_1)

{
  return *(uint *)(*(long *)(param_1 + 0x3e0) + 0x44) >> 0x14 & 7;
}


