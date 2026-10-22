// get_vdsp_dvfs_table @ 00114dd0
// signature: undefined get_vdsp_dvfs_table(void)
// calling convention: unknown


undefined8 get_vdsp_dvfs_table(undefined4 *param_1)

{
  undefined4 uVar1;
  
  *param_1 = map_table_801;
  param_1[1] = DAT_00101664;
  param_1[2] = DAT_00101668;
  uVar1 = DAT_0010166c;
  *(undefined8 *)(param_1 + 4) = 0;
  param_1[3] = uVar1;
  param_1[6] = DAT_00101674;
  param_1[7] = DAT_00101678;
  param_1[8] = DAT_0010167c;
  uVar1 = DAT_00101680;
  *(undefined8 *)(param_1 + 10) = 0;
  param_1[9] = uVar1;
  param_1[0xc] = DAT_00101688;
  param_1[0xd] = DAT_0010168c;
  param_1[0xe] = DAT_00101690;
  uVar1 = DAT_00101694;
  *(undefined8 *)(param_1 + 0x10) = 0;
  param_1[0xf] = uVar1;
  param_1[0x12] = DAT_0010169c;
  param_1[0x13] = DAT_001016a0;
  param_1[0x14] = DAT_001016a4;
  uVar1 = DAT_001016a8;
  *(undefined8 *)(param_1 + 0x16) = 0;
  param_1[0x15] = uVar1;
  param_1[0x18] = DAT_001016b0;
  param_1[0x19] = DAT_001016b4;
  param_1[0x1a] = DAT_001016b8;
  uVar1 = DAT_001016bc;
  *(undefined8 *)(param_1 + 0x1c) = 0;
  param_1[0x1b] = uVar1;
  param_1[0x1e] = DAT_001016c4;
  param_1[0x1f] = DAT_001016c8;
  param_1[0x20] = DAT_001016cc;
  param_1[0x21] = DAT_001016d0;
  *(undefined8 *)(param_1 + 0x22) = 0;
  return 6;
}


