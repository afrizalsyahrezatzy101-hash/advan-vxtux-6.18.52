// get_dpu_dvfs_table.280 @ 0010d070
// signature: undefined get_dpu_dvfs_table.280(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined8 get_dpu_dvfs_table_280(undefined4 *param_1)

{
  undefined4 uVar1;
  
  *param_1 = map_table_289;
  param_1[1] = DAT_001014e4;
  param_1[2] = DAT_001014e8;
  uVar1 = _DAT_001014ec;
  *(undefined8 *)(param_1 + 4) = 0;
  param_1[3] = uVar1;
  param_1[6] = DAT_001014f8;
  param_1[7] = DAT_001014fc;
  param_1[8] = DAT_00101500;
  uVar1 = DAT_00101504;
  *(undefined8 *)(param_1 + 10) = 0;
  param_1[9] = uVar1;
  param_1[0xc] = DAT_00101510;
  param_1[0xd] = DAT_00101514;
  param_1[0xe] = DAT_00101518;
  uVar1 = DAT_0010151c;
  *(undefined8 *)(param_1 + 0x10) = 0;
  param_1[0xf] = uVar1;
  param_1[0x12] = DAT_00101528;
  param_1[0x13] = DAT_0010152c;
  param_1[0x14] = DAT_00101530;
  uVar1 = DAT_00101534;
  *(undefined8 *)(param_1 + 0x16) = 0;
  param_1[0x15] = uVar1;
  param_1[0x18] = DAT_00101540;
  param_1[0x19] = DAT_00101544;
  param_1[0x1a] = DAT_00101548;
  param_1[0x1b] = DAT_0010154c;
  *(undefined8 *)(param_1 + 0x1c) = 0;
  return 5;
}


