// vsp_dvfs_parse_dt.666 @ 00112034
// signature: undefined vsp_dvfs_parse_dt.666(void)
// calling convention: unknown


void vsp_dvfs_parse_dt_666(long param_1,undefined8 param_2)

{
  int iVar1;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,gfree-wait-delay",(undefined4 *)(param_1 + 0x460),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x460) = 0x100;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,freq-upd-hdsk-en",(undefined4 *)(param_1 + 0x464),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x464) = 1;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,freq-upd-delay-en",(undefined4 *)(param_1 + 0x468),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x468) = 1;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,freq-upd-en-byp",(undefined4 *)(param_1 + 0x46c),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x46c) = 0;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,sw-trig-en",(undefined4 *)(param_1 + 0x470),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x470) = 0;
  }
  return;
}


