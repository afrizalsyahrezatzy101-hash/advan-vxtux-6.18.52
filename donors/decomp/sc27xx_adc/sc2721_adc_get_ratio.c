// signature: undefined sc2721_adc_get_ratio(void)

undefined4 sc2721_adc_get_ratio(undefined4 param_1,int param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  switch(param_1) {
  case 1:
  case 2:
  case 3:
  case 4:
    uVar2 = 0x1900401;
    break;
  case 5:
    return 0x7001d;
  default:
    return 0x10001;
  case 7:
  case 9:
    uVar2 = 0x64007d;
    break;
  case 0xe:
    return 0x440384;
  case 0x10:
    return 0x300064;
  case 0x13:
    return 0x10003;
  }
  uVar1 = 0x10001;
  if (param_2 != 0) {
    uVar1 = uVar2;
  }
  return uVar1;
}



