====================get_ip_dvfs_table.708@001102ac====================
// signature: undefined get_ip_dvfs_table.708(void)

undefined8 get_ip_dvfs_table_708(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  long lVar4;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    lVar4 = lVar3 * 0x38;
    puVar2 = (undefined4 *)(param_2 + lVar4);
    lVar1 = lVar3 + 1;
    *puVar2 = *(undefined4 *)(&isp_dvfs_config_table + lVar3 * 7);
    puVar2[5] = *(undefined4 *)((long)&DAT_00104048 + lVar4 + 4);
    puVar2[6] = (&DAT_00104050)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00104068)[lVar3 * 7];
    puVar2[4] = *(undefined4 *)(&DAT_00104048 + lVar3 * 7);
    puVar2[7] = *(undefined4 *)(&DAT_00104054 + lVar4);
    puVar2[8] = *(undefined4 *)(&DAT_00104058 + lVar3 * 7);
    puVar2[9] = *(undefined4 *)((long)&DAT_00104058 + lVar4 + 4);
    puVar2[10] = *(undefined4 *)(&DAT_00104060 + lVar3 * 7);
    *(undefined8 *)(puVar2 + 2) = (&DAT_00104040)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}


