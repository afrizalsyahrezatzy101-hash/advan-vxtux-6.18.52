====================set_ip_dvfs_idle_index.1068@00115b90====================
// signature: undefined set_ip_dvfs_idle_index.1068(void)

void set_ip_dvfs_idle_index_1068(undefined8 param_1,uint param_2)

{
  uint *puVar1;
  uint *puVar2;
  uint *puVar3;
  long *plVar4;
  long lVar5;
  undefined8 *in_x18;
  undefined8 unaff_x30;
  
  *in_x18 = unaff_x30;
  lVar5 = 0;
  *(uint *)(g_mmreg_map + 0x1d4) = *(uint *)(g_mmreg_map + 0x1d4) & 0xfffffff8 | param_2;
  do {
    puVar1 = (uint *)((long)&DAT_00103948 + lVar5);
    puVar2 = (uint *)((long)&DAT_00103960 + lVar5);
    plVar4 = (long *)((long)&DAT_00103940 + lVar5);
    puVar3 = (uint *)((long)&DAT_00103950 + lVar5);
    lVar5 = lVar5 + 0x38;
    *(uint *)(*plVar4 + g_mmreg_map) = (*puVar1 & 7) << 5 | (*puVar2 & 7) << 2 | *puVar3 & 3;
  } while (lVar5 != 0x1c0);
  return;
}


