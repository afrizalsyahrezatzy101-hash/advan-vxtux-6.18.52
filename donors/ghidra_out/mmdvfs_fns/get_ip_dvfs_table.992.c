====================get_ip_dvfs_table.992@00114494====================
// signature: undefined get_ip_dvfs_table.992(void)

undefined8 get_ip_dvfs_table_992(undefined8 param_1,long param_2)

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
    *puVar2 = *(undefined4 *)(&jpg_dvfs_config_table + lVar3 * 7);
    puVar2[5] = (&DAT_0010420c)[lVar3 * 0xe];
    puVar2[6] = (&DAT_00104210)[lVar3 * 0xe];
    *(undefined **)(puVar2 + 0xc) = (&PTR_DAT_00104228)[lVar3 * 7];
    puVar2[4] = (&DAT_00104208)[lVar3 * 0xe];
    puVar2[7] = *(undefined4 *)(&DAT_00104214 + lVar4);
    puVar2[8] = *(undefined4 *)(&DAT_00104218 + lVar3 * 7);
    puVar2[9] = *(undefined4 *)((long)&DAT_00104218 + lVar4 + 4);
    puVar2[10] = (&DAT_00104220)[lVar3 * 0xe];
    *(undefined8 *)(puVar2 + 2) = (&DAT_00104200)[lVar3 * 7];
    lVar3 = lVar1;
  } while (lVar1 != 8);
  return 0;
}


