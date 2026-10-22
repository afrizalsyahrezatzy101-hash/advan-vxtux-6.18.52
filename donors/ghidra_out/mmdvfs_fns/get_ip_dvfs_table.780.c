====================get_ip_dvfs_table.780@001112c8====================
// signature: undefined get_ip_dvfs_table.780(void)

undefined8 get_ip_dvfs_table_780(undefined8 param_1,long param_2)

{
  long lVar1;
  undefined4 *puVar2;
  long lVar3;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  printk(&DAT_0010480c,"get_ip_dvfs_table");
  lVar3 = 0;
  do {
    puVar2 = (undefined4 *)(param_2 + lVar3);
    lVar1 = lVar3 + 0x38;
    *puVar2 = *(undefined4 *)((long)&dcamaxi_dvfs_config_table + lVar3);
    puVar2[5] = *(undefined4 *)((long)&DAT_00103b08 + lVar3 + 4);
    puVar2[6] = *(undefined4 *)((long)&DAT_00103b10 + lVar3);
    *(undefined8 *)(puVar2 + 0xc) = *(undefined8 *)((long)&PTR_DAT_00103b28 + lVar3);
    puVar2[4] = *(undefined4 *)((long)&DAT_00103b08 + lVar3);
    puVar2[7] = *(undefined4 *)(&DAT_00103b14 + lVar3);
    puVar2[8] = *(undefined4 *)((long)&DAT_00103b18 + lVar3);
    puVar2[9] = *(undefined4 *)((long)&DAT_00103b18 + lVar3 + 4);
    puVar2[10] = *(undefined4 *)((long)&DAT_00103b20 + lVar3);
    *(undefined8 *)(puVar2 + 2) = *(undefined8 *)((long)&DAT_00103b00 + lVar3);
    lVar3 = lVar1;
  } while (lVar1 != 0x1c0);
  return 0;
}


