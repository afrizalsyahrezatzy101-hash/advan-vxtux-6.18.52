====================get_ip_dvfs_table.915@00113420====================
// signature: undefined get_ip_dvfs_table.915(void)

undefined8 get_ip_dvfs_table_915(undefined8 param_1,long param_2)

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
    *puVar2 = *(undefined4 *)(&dcam_dvfs_config_table + lVar3 * 7);
    puVar2[5] = (&DAT_00103ccc)[lVar3 * 0xe];
    puVar2[6] = (&DAT_00103cd0)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00103ce8)[lVar3 * 7];
    puVar2[4] = (&DAT_00103cc8)[lVar3 * 0xe];
    puVar2[7] = (&DAT_00103cd4)[lVar3 * 0xe];
    puVar2[8] = (&DAT_00103cd8)[lVar3 * 0xe];
    puVar2[9] = (&DAT_00103cdc)[lVar3 * 0xe];
    puVar2[10] = *(undefined4 *)(&DAT_00103ce0 + lVar3 * 7);
    *(undefined8 *)(puVar2 + 2) = (&DAT_00103cc0)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 1;
}


