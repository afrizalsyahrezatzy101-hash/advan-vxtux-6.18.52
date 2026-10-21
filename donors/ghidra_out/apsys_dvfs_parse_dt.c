// apsys_dvfs_parse_dt @ 0010e9c4
// signature: undefined apsys_dvfs_parse_dt(void)
// calling convention: unknown


uint apsys_dvfs_parse_dt(long param_1,undefined8 param_2)

{
  int iVar1;
  uint uVar2;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_001034f0,"apsys_dvfs_parse_dt");
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,ap-dvfs-hold",(undefined4 *)(param_1 + 0x3b4),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3b4) = 0;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,ap-dvfs-force-en",(undefined4 *)(param_1 + 0x3c0),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3c0) = 1;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,ap-dvfs-auto-gate",(undefined4 *)(param_1 + 0x3c4),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3c4) = 0;
  }
  iVar1 = of_property_read_variable_u32_array
                    (param_2,"sprd,ap-dvfs-wait-window",(undefined4 *)(param_1 + 0x3b8),1,0);
  if (iVar1 < 0) {
    *(undefined4 *)(param_1 + 0x3b8) = 0x10080;
  }
  uVar2 = of_property_read_variable_u32_array
                    (param_2,"sprd,ap-dvfs-min-volt",(undefined4 *)(param_1 + 0x3bc),1,0);
  if ((int)uVar2 < 0) {
    *(undefined4 *)(param_1 + 0x3bc) = 0;
  }
  return uVar2 & (int)uVar2 >> 0x1f;
}


