====================get_ip_dvfs_table.836@001123a8====================
// signature: undefined get_ip_dvfs_table.836(void)

undefined8 get_ip_dvfs_table_836(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3 * 0x38);
    lVar1 = lVar3 + 1;
    *puVar2 = (&mtx_dvfs_config_table)[lVar3 * 0xe];
    puVar2[5] = (&DAT_001043cc)[lVar3 * 0xe];
    puVar2[6] = (&DAT_001043d0)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_001043e8)[lVar3 * 7];
    puVar2[4] = (&DAT_001043c8)[lVar3 * 0xe];
    puVar2[7] = (&DAT_001043d4)[lVar3 * 0xe];
    puVar2[8] = (&DAT_001043d8)[lVar3 * 0xe];
    puVar2[9] = (&DAT_001043dc)[lVar3 * 0xe];
    puVar2[10] = (&DAT_001043e0)[lVar3 * 0xe];
    *(undefined8 *)(puVar2 + 2) = (&DAT_001043c0)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}


