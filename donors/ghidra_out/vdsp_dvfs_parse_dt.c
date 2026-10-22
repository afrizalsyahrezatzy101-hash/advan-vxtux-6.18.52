// vdsp_dvfs_parse_dt @ 001143d0
// signature: undefined vdsp_dvfs_parse_dt(void)
// calling convention: unknown


uint vdsp_dvfs_parse_dt(long param_1,undefined8 param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,gfree-wait-delay",(undefined4 *)(param_1 + 0x3ec),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3ec) = 0x100;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,freq-upd-hdsk-en",(undefined4 *)(param_1 + 0x3f0),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3f0) = 1;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,freq-upd-delay-en",(undefined4 *)(param_1 + 0x3f4),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3f4) = 0;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,freq-upd-en-byp",(undefined4 *)(param_1 + 0x3f8),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3f8) = 0;
  }
  uVar2 = of_property_read_variable_u32_array
                    (param_2,"sprd,sw-trig-en",(undefined4 *)(param_1 + 0x3fc),1,0);
  if ((int)uVar2 < 0) {
    *(undefined4 *)(param_1 + 0x3fc) = 0;
  }
  return uVar2 & (int)uVar2 >> 0x1f;
}


