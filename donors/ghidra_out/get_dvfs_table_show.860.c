// get_dvfs_table_show.860 @ 00115ed0
// signature: undefined get_dvfs_table_show.860(void)
// calling convention: unknown


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

long get_dvfs_table_show_860(long param_1,undefined8 param_2,void *param_3)

{
  uint uVar1;
  int iVar2;
  undefined8 *in_x18;
  long lVar3;
  code *pcVar4;
  long lVar5;
  undefined8 unaff_x30;
  uint local_118 [48];
  long local_58;
  
  *in_x18 = unaff_x30;
  local_58 = ___stack_chk_guard;
  lVar3 = *(long *)(*(long *)(param_1 + 0x60) + 0x98);
  memset(local_118,0,0xc0);
  lVar3 = *(long *)(lVar3 + 0x480);
  if ((lVar3 == 0) || (pcVar4 = *(code **)(lVar3 + 0x58), pcVar4 == (code *)0x0)) {
    printk(&DAT_0010230e,"get_dvfs_table_show");
    memcpy(param_3,"map_index\tvolt_level\tclk_level\tclk_rate\n",0x29);
  }
  else {
    if (7 < ((ulong)(pcVar4 + -0x1181cc) >> 2 | (long)pcVar4 << 0x3e)) {
      __cfi_slowpath(0xb914d9c1747a0c1d,pcVar4);
    }
    uVar1 = (*pcVar4)(local_118);
    memcpy(param_3,"map_index\tvolt_level\tclk_level\tclk_rate\n",0x29);
    if (0 < (int)uVar1) {
      lVar5 = 0;
      lVar3 = 0x28;
      do {
        if (lVar5 == 0xc0) {
          __ubsan_handle_cfi_check_fail_abort(0x12);
          goto LAB_0011602c;
        }
        iVar2 = sprintf((char *)((long)param_3 + lVar3),"%d\t\t%d\t\t%d\t\t%d\t\t\n",
                        (ulong)*(uint *)((long)local_118 + lVar5),
                        (ulong)*(uint *)((long)local_118 + lVar5 + 4),
                        (ulong)*(uint *)((long)local_118 + lVar5 + 8),
                        (ulong)*(uint *)((long)local_118 + lVar5 + 0xc));
        lVar5 = lVar5 + 0x18;
        lVar3 = lVar3 + iVar2;
      } while ((ulong)uVar1 * 0x18 - lVar5 != 0);
      goto LAB_00115fec;
    }
  }
  lVar3 = 0x28;
LAB_00115fec:
  if (___stack_chk_guard == local_58) {
    return lVar3;
  }
LAB_0011602c:
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}


