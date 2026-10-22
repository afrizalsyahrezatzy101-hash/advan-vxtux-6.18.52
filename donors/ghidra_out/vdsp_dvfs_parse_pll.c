// vdsp_dvfs_parse_pll @ 001144d4
// signature: undefined vdsp_dvfs_parse_pll(void)
// calling convention: unknown


undefined4 vdsp_dvfs_parse_pll(undefined8 param_1,undefined8 param_2)

{
  undefined8 *in_x18;
  char *pcVar1;
  undefined8 *puVar2;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  pcVar1 = "vdsp_twpll";
  clk_pll = devm_clk_get(param_2,"vdsp_twpll");
  puVar2 = &clk_pll;
  if (clk_pll < 0xfffffffffffff001) {
    pcVar1 = "vdsp_lpll";
    DAT_00100858 = devm_clk_get(param_2,"vdsp_lpll");
    puVar2 = &DAT_00100858;
    if (DAT_00100858 < 0xfffffffffffff001) {
      pcVar1 = "vdsp_isppll";
      DAT_00100860 = devm_clk_get(param_2,"vdsp_isppll");
      puVar2 = &DAT_00100860;
      if (DAT_00100860 < 0xfffffffffffff001) {
        return 0;
      }
    }
  }
  _dev_err(param_2,"get clock %s failed\n",pcVar1);
  return *(undefined4 *)puVar2;
}


