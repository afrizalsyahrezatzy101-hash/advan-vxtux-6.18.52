// signature: undefined sc2730_adc_get_ratio(void)

undefined4 sc2730_adc_get_ratio(undefined4 param_1,uint param_2)

{
  switch(param_1) {
  case 0xe:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100ca0 + (long)(int)param_2 * 4);
    }
    break;
  case 0xf:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100cb0 + (long)(int)param_2 * 4);
    }
    break;
  case 0x10:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c90 + (long)(int)param_2 * 4);
    }
    break;
  default:
    if (param_2 - 1 < 3) {
      return *(undefined4 *)(&DAT_00102080 + (long)(int)(param_2 - 1) * 4);
    }
    break;
  case 0x15:
  case 0x16:
  case 0x17:
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c80 + (long)(int)param_2 * 4);
    }
  }
  return 0x10001;
}



