// signature: undefined sc2720_adc_get_ratio(void)

undefined4 sc2720_adc_get_ratio(int param_1,uint param_2)

{
  if (param_1 - 0x15U < 3) {
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c80 + (long)(int)param_2 * 4);
    }
  }
  else if (param_1 == 0x10) {
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100c90 + (long)(int)param_2 * 4);
    }
  }
  else if (param_1 == 0xe) {
    if (param_2 < 4) {
      return *(undefined4 *)(&DAT_00100ca0 + (long)(int)param_2 * 4);
    }
  }
  else if (param_2 - 1 < 3) {
    return *(undefined4 *)(&DAT_00102074 + (long)(int)(param_2 - 1) * 4);
  }
  return 0x10001;
}



